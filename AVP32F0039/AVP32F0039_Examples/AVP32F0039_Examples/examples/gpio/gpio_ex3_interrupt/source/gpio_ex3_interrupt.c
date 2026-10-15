/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   gpio_ex3_interrupt.c
*                         TITLE:  Device GPIO Interrupt
*******************************************************************************************/
//! Configures the device GPIOs through the sysconfig file. One GPIO output
//! pin, and one GPIO input pin is configured. The example then configures the
//! GPIO input pin to be the source of an external interrupt which toggles
//! the GPIO output pin.
//
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Interrupt Handler
//
__interrupt void gpioInterruptHandler(void);

//
// Main
//
void main(void)
{
    //
    // Initializes system control, device clock, and peripherals
    //
    Device_init();

    //
    // Initializes PIE and clear PIE registers. Disables CPU interrupts.
    // and clear all CPU interrupt flags.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board Initialization
    //
    Board_init();

    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_FALLING_EDGE);
    GPIO_setInterruptPin(myGPIOInputInterrupt0, GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT1);

    Interrupt_register(INT_XINT1, &gpioInterruptHandler);
    Interrupt_enable(INT_XINT1);

    //
    // Enables CPU interrupts
    //
    Interrupt_enableGlobal();

    //
    // Loop.
    //
    for(;;)
    {

    }
}

__interrupt void gpioInterruptHandler(void)
{
    GPIO_togglePin(myGPIOOutput0);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


//
// End of File
//

