/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   dcdc_ex1_enable.c
*                         TITLE:  DCDC Enable Example
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Main
//
void main(void)
{
    //
    // Disable the watchdog timer
    //
    SysCtl_disableWatchdog();

    //
    // DC-DC initialization
    //
    DC_DC_init(); 

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure GPIO pins for CANTX/CANRX
    //
    Device_initGPIO();

    //
    // Board initialization
    //
    Board_init();

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
    // Loop Forever 
    //
     while(1)
    {
        // DC-DC is enabled by sysconfig
        /* First Breakpoint is provided for the user to measure I/O current while DC-DC is enabled. */ 
        ESTOP0; 
        ASysCtl_disableDCDC();
        /* Second breakpoint is provided for the user to measure I/o current while DC-DC is disabled. */
        ESTOP0;
    }
}

//
// End of File
//
