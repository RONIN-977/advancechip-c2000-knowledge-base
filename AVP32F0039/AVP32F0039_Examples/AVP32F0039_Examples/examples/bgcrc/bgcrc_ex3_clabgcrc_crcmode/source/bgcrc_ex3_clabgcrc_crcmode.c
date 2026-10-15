/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   bgcrc_ex3_clabgcrc_crcmode.c
*                         TITLE:  Background CRC computation with interrupt.
*******************************************************************************************/
//! This example demonstrates how to configure and trigger CLABGCRC from the CPU.
//! It also showcases how to configure the CRC watchdog and lock the registers
//! after configuring the module. The watchdog is used as a diagnostic to check
//! memory test completion within the expected time window. An error signal is
//! generated if the test does not complete in the specified time window.
//!
//! The module is configured for 1kB of CLA ROM  memory. The golden CRC value
//! for comparison is computed using software method. Interrupt is generated 
//! once the computation is done and checks if no error flags are raised. The
//! NMI is enabled and is triggered if an error is detected.
//!
//! \b External \b Connections \n
//!  - None.
//!
//! \b Watch \b Variables \n
//!  - pass
//!  - bgcrcDone
//!
//
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include <stdlib.h>
#include <time.h>

//
// Defines and globals
//
#define DATA_SIZE               256         // Total size  = 256 * 4 = 1kB
#define CLA_ROM_ADDR            0x1001080   // CPU view
#define CLA_ROM_ADDR_BGCRC_VIEW 0xB080      // BGCRC_CLA view

uint32_t POLYNOMIAL = (uint32_t)0x04C11DB7;
uint32_t crc32;
uint32_t seed;
uint32_t byteSwappedData;

volatile bool bgcrcDone = false;
volatile bool bgcrcError = false;
volatile uint32_t BgcrcNmiStatus = 0;
volatile uint32_t intStatus =0;
volatile bool Bgcrc_fail = false;
volatile bool Bgcrc_uncorr_err = false;
volatile bool Bgcrc_corr_err = false;
volatile bool Bgcrc_Wd_underflow = false;
volatile bool Bgcrc_Wd_overflow = false;
volatile bool error_status_pin_diagnostic_failed = false;

bool pass = false;

//
// Function Prototypes
//
void computeCRC_software(void);
void bgcrcInit(void);
void delay(uint32_t i);
__interrupt void CLAbgcrcIsr(void);
__interrupt void NMIbgcrcIsr(void);

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    //
    // Disable global interrupts.
    //
    DINT;

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Map the ISRs of the BGCRC interrupt and NMI.
    //
    Interrupt_register(INT_BGCRC_CLA1, CLAbgcrcIsr);
    Interrupt_register(INT_NMI, NMIbgcrcIsr);

    //
    // Enable the CRC interrupt in the PIE
    //
    Interrupt_enable(INT_BGCRC_CLA1);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);

    //
    // Enable NMI
    //
    Interrupt_enable(INT_NMI);
    SysCtl_clearAllNMIFlags();
    SysCtl_enableNMIGlobalInterrupt();
    SysCtl_setNMIWatchdogPeriod(0xFFFFU);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Initialize the data with random numbers
    //
    computeCRC_software();

    //
    // Initialize the BGCRC module
    //
    bgcrcInit();

    delay(2000); // Expecting the BGCRC completion in this time

    if(error_status_pin_diagnostic_failed == true )
    {
        //
        // Something is wrong with the error status pin
        // Need to reset the system
        //
    }

    if((bgcrcDone == true) && (bgcrcError == false))
    {
        if((intStatus == BGCRC_TEST_DONE))
        {
            pass = true;
        }
    }

    while(1);
}

//
// SW method for computing CRC of CLA ROM memory data
//
void computeCRC_software()
{
    uint32_t tseed;
    uint32_t i,j;

    //
    // Initialize the random number generator
    //
    tseed = time(NULL);
    srand(tseed);
    srand(rand());

    seed  =  rand();
    seed = (uint32_t)(seed << 16) | rand();       // Initialize with seed
    crc32 = seed;

    //
    // Compute the Software CRC for validation
    //
    uint32_t current_data;
    for(i = 0; i < DATA_SIZE; i++)
    {
        current_data = HWREG(CLA_ROM_ADDR + 2 * i);

        byteSwappedData = (((current_data & (uint32_t)0x000000FFU) << 24) |
                           ((current_data & (uint32_t)0x0000FF00U) <<  8) |
                           ((current_data & (uint32_t)0x00FF0000U) >>  8) |
                           ((current_data & (uint32_t)0xFF000000U) >> 24)
                          );

        crc32 = byteSwappedData ^ crc32;

        for(j = 0; j < 32; j++)
        {
            if(crc32 & (uint32_t)0x80000000U)
            {
                crc32 = ((crc32 << 1) ^ POLYNOMIAL);
            }
            else
            {
                crc32 = (crc32 << 1);
            }
            crc32 = crc32 & (uint32_t)0xFFFFFFFFU;
        }
    }
}

//
// Init routine for BGCRC module
//
void bgcrcInit()
{
    //
    // Unlock the BGCRC configuration in case it is locked CFG-1
    //
    BGCRC_unlockRegister(BGCRC_CLA1_BASE, BGCRC_REG_CTRL1  |
                                          BGCRC_REG_WD_CFG |
                                          BGCRC_REG_INTEN  |
                                          BGCRC_REG_SEED);

    //
    // Unlock the BGCRC configuration in case it is locked CFG-2
    //
    BGCRC_unlockRegister(BGCRC_CLA1_BASE, BGCRC_REG_EN         |
                                          BGCRC_REG_CTRL2      |
                                          BGCRC_REG_START_ADDR |
                                          BGCRC_REG_GOLDEN     |
                                          BGCRC_REG_WD_MIN     |
                                          BGCRC_REG_WD_MAX);
    //
    // Enable NMI and set the emulation mode as soft.
    // The CRC module and the watchdog stops immediately on debug suspend
    //
    BGCRC_setConfig(BGCRC_CLA1_BASE, BGCRC_NMI_ENABLE, BGCRC_EMUCTRL_SOFT);

    //
    // Configure the CRC Watchdog
    // Watchdog window - Min : DATA_SIZE, Max : DATA_SIZE + 100
    //
    BGCRC_enableWatchdog(BGCRC_CLA1_BASE);
    BGCRC_setWatchdogWindow(BGCRC_CLA1_BASE, DATA_SIZE, DATA_SIZE + 100);

    //
    // Configure the region
    //
    BGCRC_setRegion(BGCRC_CLA1_BASE, CLA_ROM_ADDR_BGCRC_VIEW,
                    BGCRC_SIZE_KBYTES(1), BGCRC_CRC_MODE);

    //
    // Set the seed value same as used in SW computation
    //
    BGCRC_setSeedValue(BGCRC_CLA1_BASE, seed);

    //
    // Initialize the golden value with previously computed CRC using software
    //
    BGCRC_setGoldenCRCValue(BGCRC_CLA1_BASE, crc32);

    //
    // Enable interrupts
    //
    BGCRC_enableInterrupt(BGCRC_CLA1_BASE, BGCRC_TEST_DONE    |
                                           BGCRC_CRC_FAIL     |
                                           BGCRC_UNCORR_ERR   |
                                           BGCRC_CORR_ERR     |
                                           BGCRC_WD_UNDERFLOW |
                                           BGCRC_WD_OVERFLOW);

    //
    // Lock Register configuration for CFG1
    //
    BGCRC_lockRegister(BGCRC_CLA1_BASE, BGCRC_REG_CTRL1  |
                                        BGCRC_REG_WD_CFG |
                                        BGCRC_REG_INTEN  |
                                        BGCRC_REG_SEED);


    //
    // Commit Register Lock for CFG1
    //
    BGCRC_commitRegisterLock(BGCRC_CLA1_BASE, BGCRC_REG_CTRL1  |
                                              BGCRC_REG_WD_CFG |
                                              BGCRC_REG_INTEN  |
                                              BGCRC_REG_SEED);

    BGCRC_start(BGCRC_CLA1_BASE);

    //
    // Lock Register configuration for CFG2
    //
    BGCRC_lockRegister(BGCRC_CLA1_BASE, BGCRC_REG_EN         |
                                        BGCRC_REG_CTRL2      |
                                        BGCRC_REG_START_ADDR |
                                        BGCRC_REG_GOLDEN     |
                                        BGCRC_REG_WD_MIN     |
                                        BGCRC_REG_WD_MAX);
}

//
// CLAbgcrcIsr - DONE interrupt from BGCRC.
//
__interrupt void CLAbgcrcIsr(void)
{
    bgcrcDone = true;

    //
    // Read the interrupt status
    //
    intStatus= BGCRC_getInterruptStatus(BGCRC_CLA1_BASE);
    intStatus &= ~BGCRC_GLOBAL_INT;

    //
    // Clear interrupt status
    //
    BGCRC_clearInterruptStatus(BGCRC_CLA1_BASE, BGCRC_GLOBAL_INT |
                                                BGCRC_TEST_DONE  |
                                                BGCRC_ALL_ERROR_FLAGS);

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);
}

//
// NMIbgcrcIsr - NMI ISR
//
__interrupt void NMIbgcrcIsr(void)
{
    uint32_t timeout = 20;
    BgcrcNmiStatus = BGCRC_getNMIStatus(BGCRC_CLA1_BASE);

    //
    // Safety diagnostic check
    //
    if(!SysCtl_isErrorTriggered())
    {
        //
        // If due to some fault the ERROR STS couldn't be triggered, force
        // the error status pin so that external host get to know the error
        //
        while((SysCtl_getErrorPinStatus() == 0) && (timeout > 0))
        {
            SysCtl_forceError();
            timeout--;
        }

        if (timeout == 0)
        {
            //
            // Something is wrong with the error status pin
            //
            error_status_pin_diagnostic_failed = true;
        }
    }

    if((BgcrcNmiStatus & BGCRC_CRC_FAIL) == BGCRC_CRC_FAIL)
    {
        //
        // CRC Fail NMI
        //
        Bgcrc_fail = true;
        BGCRC_clearNMIStatus(BGCRC_CLA1_BASE, BGCRC_CRC_FAIL);
    }

    if((BgcrcNmiStatus & BGCRC_UNCORR_ERR) == BGCRC_UNCORR_ERR )
    {
         //
         // Uncorrectable Error NMI
         //
         Bgcrc_uncorr_err = true;
         BGCRC_clearNMIStatus(BGCRC_CLA1_BASE, BGCRC_UNCORR_ERR);
    }

    if((BgcrcNmiStatus & BGCRC_CORR_ERR) == BGCRC_CORR_ERR)
    {
          //
          // Correctable Error NMI
          //
          Bgcrc_corr_err = true;
          BGCRC_clearNMIStatus(BGCRC_CLA1_BASE, BGCRC_CORR_ERR);
    }

    if((BgcrcNmiStatus & BGCRC_WD_UNDERFLOW) == BGCRC_WD_UNDERFLOW)
    {
          //
          // Watchdog Underflow Error NMI
          //
          Bgcrc_Wd_underflow = true;
          BGCRC_clearNMIStatus(BGCRC_CLA1_BASE, BGCRC_WD_UNDERFLOW);
    }

    if((BgcrcNmiStatus & BGCRC_WD_OVERFLOW) == BGCRC_WD_OVERFLOW)
    {
          //
          // Watchdog Overflow Error NMI
          //
          Bgcrc_Wd_overflow = true;
          BGCRC_clearNMIStatus(BGCRC_CLA1_BASE, BGCRC_WD_OVERFLOW);
    }

    SysCtl_clearAllNMIFlags();
    bgcrcError = true;
}

//
// Delay function
//
void delay(uint32_t i)
{
    uint32_t j =i;
    while(j > 0)
    {
        j--;
    }
}

//
// End of File
//
