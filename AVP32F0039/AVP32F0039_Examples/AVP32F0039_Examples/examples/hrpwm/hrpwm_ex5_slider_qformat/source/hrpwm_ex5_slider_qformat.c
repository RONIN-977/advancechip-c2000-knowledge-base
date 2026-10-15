/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hrpwm_ex5_slider_qformat.c
*                         TITLE:  HRPWM MEP Slider example
*******************************************************************************************/
//! This example modifies the MEP control registers to show edge displacement
//! due to HRPWM. Control blocks of the respective ePWM module channel A and B
//! will have fine edge movement due to HRPWM logic. Load the
//! hrpwm_slider.gel file. Select the HRPWM_eval from the GEL menu.
//! A FineDuty slider graphics will show up in CCS. Load the program and run.
//! Use the Slider to and observe the EPWM edge displacement for each slider
//! step change. This explains the MEP control on the EPwmxA channels.
//!
//! Monitor ePWM1 & ePWM2 A/B pins on an oscilloscope.
//
//
//
//#############################################################################

//
// Included Files
//
#include "board.h"

//
// Globals
//
uint32_t  dutySliderVal, update, i, j, n;

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
    // user specific code
    //
    update = 1;
    dutySliderVal = 0;

    //
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Configure ePWM module/s and GPIOs
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

    while(update == 1U)
    {
        //
        // Write to HRPWM extension of CMPA/ CMPB
        //
        HWREGH(myEPWM1_BASE + HRPWM_O_CMPA) =
                (HWREGH(myEPWM1_BASE + HRPWM_O_CMPA) & ~(0xFF00)) |
                (dutySliderVal << 8U);
        HWREGH(myEPWM1_BASE + HRPWM_O_CMPB) =
                (HWREGH(myEPWM1_BASE + HRPWM_O_CMPB) & ~(0xFF00)) |
                (dutySliderVal << 8U);

        //
        // Write to HRPWM extension of CMPA/ CMPB
        //
        HWREGH(myEPWM2_BASE + HRPWM_O_CMPA) =
                (HWREGH(EPWM2_BASE + HRPWM_O_CMPA) & ~(0xFF00)) |
                (dutySliderVal << 8U);
        HWREGH(myEPWM2_BASE + HRPWM_O_CMPB) =
                (HWREGH(myEPWM2_BASE + HRPWM_O_CMPB) & ~(0xFF00)) |
                (dutySliderVal << 8U);
    }

    while(1)
    {
    }
}

