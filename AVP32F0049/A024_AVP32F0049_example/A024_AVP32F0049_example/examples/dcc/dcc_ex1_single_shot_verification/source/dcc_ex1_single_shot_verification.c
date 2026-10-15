/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   dcc_ex1_single_shot_verification.c
*                         TITLE:  DCC Single shot Clock verification
*******************************************************************************************/
//! This program uses the XTAL clock as a reference clock to verify
//! the frequency of the PLLRAW clock.
//!
//! The Dual-Clock Comparator Module 0 is used for the clock verification.
//! The clocksource0 is the reference clock (Fclk0 = 20Mhz) and the
//! clocksource1 is the clock that needs to be verified (Fclk1 = 200Mhz).
//! Seed is the value that gets loaded into the Counter.
//!
//! Please refer to the TRM for details on counter seed values to be set.
//!
//! \b External \b Connections \n
//!   - None
//!
//! \b Watch \b Variables \n
//!  - \b status/result - Status of the PLLRAW clock verification
//!
//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define PASS 0
#define FAIL 1

//
// Globals
//
uint32_t result = FAIL;

//
// Main
//
void main(void)
{
    bool status=0;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Verify the frequency of PLL clock using the XTAL as reference clock
    // FClk1 = PLLRAW frequency = 200MHz
    // FClk0 = XTAL frequency   = 20MHz
    // Tolerance = 1%
    // Allowable Frequency Tolerance = 0% (update as per the error in the XTAL frequency)
    // SysClk Freq = 100MHz
    //
    // Note: Update the parameters in case you are using different PLL or XTAL
    // frequencies,
    //
    status = DCC_verifyClockFrequency(DCC0_BASE,
                                      DCC_COUNT1SRC_PLL, 200.0F,
                                      DCC_COUNT0SRC_XTAL, 20.0F,
                                      1.0F, 0.0F, 100.0F);

    //
    // Status of the PLLRAW clock verification
    //
    if (!status)
    {
        result = FAIL;
    }
    else
    {
        result = PASS;
    }

    ESTOP0;
}

