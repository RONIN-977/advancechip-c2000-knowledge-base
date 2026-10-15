/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   dcc_ex2_single_shot measurement.c
*                         TITLE:  DCC Single shot Clock measurement.
*******************************************************************************************/
//! This program demonstrates Single Shot measurement of the INTOSC2
//! clock post trim using XTAL as the reference clock.
//!
//! The Dual-Clock Comparator Module 0 is used for the clock measurement.
//! The clocksource0 is the reference clock (Fclk0 = 20Mhz) and the
//! clocksource1 is the clock that needs to be measured (Fclk1 = 10Mhz).
//! Since the frequency of the clock1 needs to be measured an initial seed is
//! set to the max value of the counter.
//!
//! Please refer to the TRM for details on counter seed values to be set.
//!
//! \b External \b Connections \n
//!   - None
//!
//! \b Watch \b Variables \n
//!  - \b result - Status if the INTOSC2 clock measurement
//!       completed successfully.
//!  - \b meas_freq1 - measured clock frequency, in this case for INTOSC2.
//!
//
//#############################################################################
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
uint32_t  result     = FAIL;
float32_t meas_freq1 = 0.0F;

void main(void)
{
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
    // Measure the frequency of INTOSC2 using XTAL as reference clock
    // Clk1 = INTOSC2
    // CLk2 = XTAL (Frequency = 20MHz)
    // Tolerance = 1%
    // Allowable Frequency Tolerance = 0% (update as per the error in the XTAL frequency)
    // SysClk Freq = 120MHz
    //
    meas_freq1 = DCC_measureClockFrequency(DCC0_BASE,
                                          DCC_COUNT1SRC_INTOSC2,
                                          DCC_COUNT0SRC_XTAL, 10.0F,
                                          1.0F, 0.0F, 120.0F);

    //
    // Measured clock frequency of INTOSC2.
    // Value of INTOSC2 freq can be checked in the device datasheet
    //
    if ((meas_freq1 >= 9.7F) && (meas_freq1 <= 10.3F))
    {
        result = PASS;
    }
    else
    {
        result = FAIL;
    }

    ESTOP0;
}
