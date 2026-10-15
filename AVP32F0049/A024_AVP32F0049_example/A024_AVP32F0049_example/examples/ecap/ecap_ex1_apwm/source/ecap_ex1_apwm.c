/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   ecap_ex1_apwm.c
*                         TITLE:  eCAP APWM Example.
*******************************************************************************************/
//! This program sets up the eCAP module in APWM mode. The PWM waveform will
//! come out on GPIO5. The frequency of PWM is configured to vary between 5Hz
//! and 10Hz using the shadow registers to load the next period/compare values.
//!

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Defines
//
#define DECREASE_FREQUENCY  0
#define INCREASE_FREQUENCY  1

//
// Globals
//
uint16_t direction;
volatile uint32_t cap1Count;

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
    // Board initialization
    // Select eCAP1OUT on MUX 0. Make GPIO5 eCAP1OUT for PWM output
    // Configure eCAP in APWM mode
    //
    Board_init();

    for(;;)
    {
        //
        // Set the duty cycle to 50%
        //
        ECAP_setAPWMShadowCompare(myECAP0_BASE,
                      (ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_1) >> 1U));

        cap1Count = ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_1);

        //
        // Vary frequency
        //
        if(cap1Count >= 20000000U)
        {
           direction = INCREASE_FREQUENCY;
        }
        else if (cap1Count <= 10000000U)
        {
           direction = DECREASE_FREQUENCY;
        }

        if(direction == INCREASE_FREQUENCY)
        {
           ECAP_setAPWMShadowPeriod(myECAP0_BASE, (cap1Count - 500000U));
        }
        else
        {
           ECAP_setAPWMShadowPeriod(myECAP0_BASE, (cap1Count + 500000U));
        }
    }
}

