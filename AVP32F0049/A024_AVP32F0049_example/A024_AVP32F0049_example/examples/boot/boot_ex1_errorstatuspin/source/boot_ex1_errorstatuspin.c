/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   boot_ex1_errorstatuspin.c
*                         TITLE:  Boot Error Status Pin Example with DCSM OTP
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

#ifdef _FLASH
// These are defined by the linker (see device linker command file)
extern uint16_t RamfuncsLoadStart;
extern uint16_t RamfuncsLoadSize;
extern uint16_t RamfuncsRunStart;
#endif

//
// Globals
//
volatile uint32_t counter = 0;

//
// Function Prototypes
//
__interrupt void nmiISR(void);

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
    // Map the ISR to the NMI interrupt.
    //
    Interrupt_register(INT_NMI, nmiISR);

    //
    // Clear all NMI flags and enable the nmi interrupt.
    //
    SysCtl_clearAllNMIFlags();
    Interrupt_enable(INT_NMI);
    

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop Forever
    //
    for(;;)
    {
        //
        // Force the NMI by software which causes the error status
        // pin low.
        //
        SysCtl_forceNMIFlags(SYSCTL_NMI_SWERR);

        //
        // Delay for a 500000 microseconds.
        //
        DEVICE_DELAY_US(500000);
    }
}

//
// nmiISR - Non-Maskable Interrupt which should have been triggered by software
//          in the main loop.
//
__interrupt void
nmiISR(void)
{
    //
    // Increment the counter each time the ISR is entered.
    //
    counter++;

    //
    // Delay for a 500 microseconds.
    //
    DEVICE_DELAY_US(500);

    //
    // Clear the NMI flag and NMIINT to cause the error status
    // pin to go high.
    //
    SysCtl_clearNMIStatus(SYSCTL_NMI_SWERR);
}

//
// End of File
//
