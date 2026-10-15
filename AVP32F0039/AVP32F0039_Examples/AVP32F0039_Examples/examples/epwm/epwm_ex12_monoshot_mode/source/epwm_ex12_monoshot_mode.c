/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   epwm_ex12_monoshot_mode.c
*                         TITLE:  Realization of Monoshot mode.
*******************************************************************************************/
//! This example showcases how to generate monoshot PWM output based on external
//! trigger i.e. generating just a single pulse output on receipt of an external
//! trigger. And the next pulse will be generated only when the next trigger
//! comes. The example utilizes external synchronization and T1 action qualifier
//! event features to achieve the desired output.
//!
//! ePWM1 is used to generate the monoshot output and ePWM2 is used an external
//! trigger for that. No external connections are required as ePWM2A is fed
//! as the trigger using Input X-BAR automatically.
//!
//! ePWM1 is configured to generated a single pulse of 0.5us when received
//! an external trigger. This is achieved by enabling the phase synchronization
//! feature and configuring EPWMxSYNCI as EXTSYNCIN1. And this EPWMxSYNCI
//! is also configured as T1 event of action qualifier to set output HIGH while
//! "CTR = PRD" action is used to set output LOW.
//!
//! ePWM2 is configured to generate a 100 KHz signal with a duty of 1% (to
//! simulate a rising edge trigger) which is routed to EXTSYNCIN1 using Input XBAR.
//!
//! Observe GPIO0 (EPWM1A : Monoshot Output) and GPIO2(EPWM2 : External Trigger)
//! on oscilloscope.
//!
//!
//! \b NOTE : In the following example, the ePWM timer is still running in a
//!        continuous mode rather than a one-shot mode thus for more reliable
//!        implementation, refer to CLB based one shot PWM implementation
//!        demonstrated in "clb_ex17_one_shot_pwm" example
//
//
//#############################################################################
//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"


void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();

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
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Configure ePWM1, ePWM2 GPIOs and Modules
    //
    Board_init();

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // IDLE loop. Just sit and loop forever (optional):
    //
    for(;;)
    {
        NOP;

    }
}

