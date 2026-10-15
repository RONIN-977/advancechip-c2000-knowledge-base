/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   gpio_ex5_interrupt_sysconfig.c
*                         TITLE:  XINT/XBAR Example
*******************************************************************************************/
// This example demonstrates the XINT feature in SysConfig by using it in 
// conjunction with the input and output XBARs. An input is received through a 
// GPIO and connected to the input XBAR, while simultaneously triggering an 
// external interrupt. The interrupt increments a counter which can be observed 
// in the watch window. In addition to triggering an interrupt, the input 
// signal is routed from the input XBAR to the output XBAR so that the output 
// can be observed via oscilloscope or logic analyzer on a separate pin.

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
//Global Variables
//
int counter = 0;

//
// Main
//
void main(void)
{
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
    //SysConfig settings
    //
    Board_init();
    EINT;
    while(1);
}

__interrupt void INT_myGPIO_INPUT_XINT_ISR(void){
    counter++;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// End of File
//
