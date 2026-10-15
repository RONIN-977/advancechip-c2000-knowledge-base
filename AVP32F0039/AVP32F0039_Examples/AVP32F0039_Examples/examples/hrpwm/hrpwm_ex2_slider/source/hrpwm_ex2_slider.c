/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hrpwm_ex2_slider.c
*                         TITLE:  HRPWM Slider.
*******************************************************************************************/

//
// Included Files
//
#include "board.h"

//
// Defines
//

//
// Globals
//
#define EPWM_TIMER_TBPRD 100

uint16_t dutyFine = 1;
uint16_t previousDutyFine = 1;
uint16_t status;

//
// Function Prototypes
//
void error(void);

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
    // Disable pin locks and enable internal pull ups.
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
    // Initialize the EPWM GPIOs and CHANGE XBAR inputs from using GPIO0
    //
    Board_init();

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);


    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    for(;;)
    {
        if (dutyFine != previousDutyFine)
        {
            HRPWM_setCounterCompareValue(myEPWM1_BASE, HRPWM_COUNTER_COMPARE_A,
                                         (EPWM_TIMER_TBPRD/2 << 8) | dutyFine);
            HRPWM_setCounterCompareValue(myEPWM1_BASE, HRPWM_COUNTER_COMPARE_B,
                                         (EPWM_TIMER_TBPRD/2 << 8) | dutyFine);

            previousDutyFine = dutyFine;
        }
     }
}

//
// error - Halt debugger when called
//
void error (void)
{
    ESTOP0;         // Stop here and handle error
}

