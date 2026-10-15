/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   dcc_ex3_continuous_monitoring_of_clock_syscfg.c
*                         TITLE:  DCC Continuous clock monitoring with sysconfig.
*******************************************************************************************/
//! This program demonstrates continuous monitoring of PLL Clock in the
//! system using INTOSC2 as the reference clock. This would trigger an
//! interrupt on any error, causing the decrement/ reload of counters to
//! stop.
//! The Dual-Clock Comparator Module 0 is used for the clock monitoring.
//! The clocksource0 is the reference clock (Fclk0 = 10Mhz) and the
//! clocksource1 is the clock that needs to be monitored (Fclk1 = 120Mhz).
//! The clock0 and clock1 seed are set automatically by the error tolerances
//! defined in the sysconfig file included this project.
//! For the sake of demo an un-realistic tolerance is assumed
//! to generate an error on continuous monitoring.
//!
//! Please refer to the TRM for details on counter seed values to be set.
//! Note : When running in flash configuration it is good to do a reset
//! & restart after loading the example to remove any stale flags/states.
//!
//! \b External \b Connections \n
//!  - None
//!
//! \b Watch \b Variables \n
//!  - \b status/result - Status of the PLLRAW clock monitoring
//!  - \b cnt0 - Counter0 Value measure when error is generated
//!  - \b cnt1 - Counter1 Value measure when error is generated
//!  - \b valid - Valid0 Value measure when error is generated
//!
//
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
#define PASS 0
#define FAIL 1

//
//   SysConfig Contains the assumptions for the DCC error tolerances
//   This example uses the internal oscillator2 clock source(Fclk0) to measure the accuracy of the PLL(Fclk1)
//   In this scenario SysClk is the same as the Fclk1
//   DCC Error Tolerance is set to 1%
//   Frequency Error Tolerance is set to 3% due to the spec of the internal oscillator
//   These assumptions are slightly optimistic in order for the example to trigger a DCC error
//   Error Signal is enabled to trigger the DCC ISR


//
// Globals
//
uint32_t result = FAIL, pass_base = myDCC0_BASE , isr_enter = 0;
uint32_t cnt0 = 0,cnt1 = 0,valid = 0;



void main(void)
{

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    //Initialize PIE and clear PIE registers. Disables CPU interrupts
    //
    Interrupt_initModule();

    //
    //Initialize the PIE vector table with pointers to the shell Interrupt
    //Service Routines (ISR)
    //
    Interrupt_initVectorTable();

    //
    //Call Board_init() to invoke the Sysconfig controlled settings
    //
    Board_init();

    //
    // Enable Global Interrupts
    //
    EINT;
    ERTM;


    //
    //Start the DCC timers
    //
    DCC_enableModule(myDCC0_BASE);

    //
    //Allow time for the dcc module to complete
    //
    DEVICE_DELAY_US(1000);

    //
    // Status of the PLL clock measurement
    //
    if (isr_enter)
    {
        result = FAIL;
    }
    else
    {
        result = PASS;
    }

    ESTOP0;
}



//
// DCC ISR triggered on an error
//
__interrupt void INT_myDCC0_ISR()
{
    uint32_t base;

    //
    // Pass the DCC base address to the ISR
    //
    base = pass_base;

    //
    // Shows that the ISR was called
    //
    isr_enter = 1;

    if (DCC_getErrorStatus(base) == 1U)
    {
        //
        // Dump the DCC counters into RAM
        //
        cnt0 = DCC_getCounter0Value(base);
        cnt1 = DCC_getCounter1Value(base);
        valid = DCC_getValidCounter0Value(base);

        //
        // Clear the Error flag
        //
        DCC_clearErrorFlag(base);
    }

    //
    // Clear the interrupt at PIE level
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
}

