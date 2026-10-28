//#############################################################################
// FILE: sci_ex3_echoback.c
// TITLE: AVP32F0039 SCI runtime autobaud, circular RX and continuous hello.
// Based on the customer's V12 sci_ex3_echoback example.
//#############################################################################
// $Copyright:
// Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Defines
//
#define RX_BUFFER_SIZE          128U
#define AUTOBAUD_TIMEOUT_TICKS  DEVICE_SYSCLK_FREQ
#define AUTOBAUD_IDLE           0U
#define AUTOBAUD_DRAIN_TX       1U
#define AUTOBAUD_DETECT         2U

//
// Globals (add these variables to the CCS Expressions window)
// C28x uint16_t stores one received 8-bit UART byte per array element.
//
volatile uint16_t rxBuffer[RX_BUFFER_SIZE];
volatile uint16_t rxWriteIndex = 0U;
volatile uint32_t rxByteCount = 0UL;
volatile uint32_t rxErrorCount = 0UL;
volatile uint16_t autoBaudRequest = 0U;
volatile uint16_t autoBaudState = AUTOBAUD_IDLE;
volatile uint32_t autoBaudSuccessCount = 0UL;
volatile uint32_t autoBaudTimeoutCount = 0UL;
volatile uint16_t autoBaudLastResult = 0U; // 0:none, 1:success, 2:timeout
volatile uint16_t autoBaudDivider = 0U;

static const uint16_t helloData[5] = {'h', 'e', 'l', 'l', 'o'};
static uint16_t helloIndex = 0U;
static uint16_t savedBaudHigh;
static uint16_t savedBaudLow;
static uint32_t autoBaudStart;

//
// Function Prototypes
//
bool SCI_autoBaudCondition(void);
static void SCI_autoBaudStart(void);
static void SCI_autoBaudService(void);
static void SCI_receiveService(void);
static void SCI_transmitService(void);

//
// Main
//
void main(void)
{
    bool condition;
    bool previousCondition = false;

    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();

    //
    // SysConfig: SCI-A, GPIO28 RX, GPIO29 TX, 9600 baud, 8N1, FIFO.
    //
    Board_init();
    autoBaudDivider = (HWREGH(mySCI0_BASE + SCI_O_HBAUD) << 8U) |
                     HWREGH(mySCI0_BASE + SCI_O_LBAUD);

    //
    // Timer2 is reserved for the nonblocking one-second timeout.
    // CPU clock, no prescaling, free-running 32-bit down counter.
    //
    CPUTimer_selectClockSource(CPUTIMER2_BASE, CPUTIMER_CLOCK_SOURCE_SYS,
                              CPUTIMER_CLOCK_PRESCALER_1);
    CPUTimer_stopTimer(CPUTIMER2_BASE);
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0U);
    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFFUL);
    CPUTimer_disableInterrupt(CPUTIMER2_BASE);
    CPUTimer_setEmulationMode(CPUTIMER2_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);

    for(;;)
    {
        condition = SCI_autoBaudCondition();
        if(condition && !previousCondition && (autoBaudState == AUTOBAUD_IDLE))
        {
            SCI_autoBaudStart();
        }
        previousCondition = condition;

        SCI_autoBaudService();
        if(autoBaudState == AUTOBAUD_IDLE)
        {
            SCI_receiveService();
            SCI_transmitService();
        }
    }
}

//
// Customer hook: return true only in an agreed synchronization window.
// The peer must stop business data and send A/a AFTER state becomes DETECT.
// A rising edge starts one attempt; return false before requesting again.
// For debugging, set autoBaudRequest from 0 to 1, then send ASCII A or a.
//
bool SCI_autoBaudCondition(void)
{
    return (autoBaudRequest != 0U);
}

//
// Stop queuing hello, then wait for pending TX to physically finish.
//
static void SCI_autoBaudStart(void)
{
    savedBaudHigh = HWREGH(mySCI0_BASE + SCI_O_HBAUD);
    savedBaudLow = HWREGH(mySCI0_BASE + SCI_O_LBAUD);
    autoBaudStart = CPUTimer_getTimerCount(CPUTIMER2_BASE);
    autoBaudLastResult = 0U;
    autoBaudState = AUTOBAUD_DRAIN_TX;
}

//
// Same hardware sequence as SCI_lockAutobaud(), without its infinite wait.
// No business RX/TX is allowed while waiting for synchronization.
// Timeout covers both draining TX and detecting A/a; restore prior divider.
//
static void SCI_autoBaudService(void)
{
    if(autoBaudState == AUTOBAUD_IDLE)
    {
        return;
    }

    if((autoBaudStart - CPUTimer_getTimerCount(CPUTIMER2_BASE)) >=
       AUTOBAUD_TIMEOUT_TICKS)
    {
        HWREGH(mySCI0_BASE + SCI_O_FFCT) &= ~SCI_FFCT_CDC;
        HWREGH(mySCI0_BASE + SCI_O_FFCT) |= SCI_FFCT_ABDCLR;
        HWREGH(mySCI0_BASE + SCI_O_HBAUD) = savedBaudHigh;
        HWREGH(mySCI0_BASE + SCI_O_LBAUD) = savedBaudLow;
        SCI_resetChannels(mySCI0_BASE);
        SCI_performSoftwareReset(mySCI0_BASE);
        SCI_resetRxFIFO(mySCI0_BASE);
        SCI_resetTxFIFO(mySCI0_BASE);
        SCI_clearOverflowStatus(mySCI0_BASE);
        helloIndex = 0U;
        autoBaudLastResult = 2U;
        autoBaudTimeoutCount++;
        autoBaudState = AUTOBAUD_IDLE;
        return;
    }

    if(autoBaudState == AUTOBAUD_DRAIN_TX)
    {
        if((SCI_getTxFIFOStatus(mySCI0_BASE) != SCI_FIFO_TX0) ||
           ((HWREGH(mySCI0_BASE + SCI_O_CTL2) & SCI_CTL2_TXEMPTY) == 0U))
        {
            return;
        }
        SCI_resetChannels(mySCI0_BASE);
        SCI_performSoftwareReset(mySCI0_BASE);
        SCI_resetRxFIFO(mySCI0_BASE);
        SCI_clearOverflowStatus(mySCI0_BASE);
        HWREGH(mySCI0_BASE + SCI_O_HBAUD) = 0U;
        HWREGH(mySCI0_BASE + SCI_O_LBAUD) = 1U;
        HWREGH(mySCI0_BASE + SCI_O_FFCT) |= SCI_FFCT_CDC;
        HWREGH(mySCI0_BASE + SCI_O_FFCT) |= SCI_FFCT_ABDCLR;
        autoBaudState = AUTOBAUD_DETECT;
    }

    if((HWREGH(mySCI0_BASE + SCI_O_FFCT) & SCI_FFCT_ABD) != 0U)
    {
        HWREGH(mySCI0_BASE + SCI_O_FFCT) |= SCI_FFCT_ABDCLR;
        HWREGH(mySCI0_BASE + SCI_O_FFCT) &= ~SCI_FFCT_CDC;
        autoBaudDivider = (HWREGH(mySCI0_BASE + SCI_O_HBAUD) << 8U) |
                         HWREGH(mySCI0_BASE + SCI_O_LBAUD);
        //
        // Discard the synchronization character, not part of business data.
        // Peer must wait for hello to resume before sending business data.
        //
        SCI_resetRxFIFO(mySCI0_BASE);
        SCI_clearOverflowStatus(mySCI0_BASE);
        helloIndex = 0U;
        autoBaudLastResult = 1U;
        autoBaudSuccessCount++;
        autoBaudState = AUTOBAUD_IDLE;
    }
}

//
// Poll at most one hardware FIFO per pass; overwrite oldest array elements.
//
static void SCI_receiveService(void)
{
    uint16_t count;

    if(((SCI_getRxStatus(mySCI0_BASE) & SCI_RXSTATUS_ERROR) != 0U) ||
       SCI_getOverflowStatus(mySCI0_BASE))
    {
        rxErrorCount++;
        SCI_resetChannels(mySCI0_BASE);
        SCI_performSoftwareReset(mySCI0_BASE);
        SCI_resetRxFIFO(mySCI0_BASE);
        SCI_clearOverflowStatus(mySCI0_BASE);
        return;
    }

    for(count = 0U; count < 16U; count++)
    {
        if(SCI_getRxFIFOStatus(mySCI0_BASE) == SCI_FIFO_RX0)
        {
            break;
        }
        rxBuffer[rxWriteIndex] = SCI_readCharNonBlocking(mySCI0_BASE) & 0x00FFU;
        rxWriteIndex++;
        if(rxWriteIndex >= RX_BUFFER_SIZE)
        {
            rxWriteIndex = 0U;
        }
        rxByteCount++;
    }
}

//
// Send hello repeatedly, without waiting for RX or blocking on a full FIFO.
// No newline or terminating NUL is sent.
//
static void SCI_transmitService(void)
{
    if(SCI_getTxFIFOStatus(mySCI0_BASE) != SCI_FIFO_TX16)
    {
        SCI_writeCharNonBlocking(mySCI0_BASE, helloData[helloIndex]);
        helloIndex++;
        if(helloIndex >= 5U)
        {
            helloIndex = 0U;
        }
    }
}

//
// End of File
//
