// TI File $Revision: /main/1 $
// Checkin $Date: July 31, 2009   17:58:47 $
//###########################################################################
//
// FILE:    Example_281xHaltWake.c
//
// TITLE:   Device Halt Mode and Wakeup Program.
//
// ASSUMPTIONS:
//
//    This program requires the DSP281x header files.
//
//    XNMI pin is configured as the LPM wakeup pin to trigger a
//    XNMI interrupt upon detection of a negative edge.
//    To wake device from halt mode, pull GPE2/ XNMI from high to low.
//
//    To observe when device wakes from HALT mode, monitor
//    GPIO1 with an oscilloscope (set to 1 in WAKEINT ISR)
//
//    As supplied, this project is configured for "boot to H0"
//    operation.
//
//
// DESCRIPTION:
//
//    This example puts the device into HALT mode. If the lowest
//    possible current consumption in HALT mode is desired, the
//    JTAG connector must be removed from the device board while
//    the device is in HALT mode.
//
//    The example then wakes up the device from HALT using the XNMI pin.
//    XNMI wakes the device from HALT mode when a falling edge
//    (signal goes high->low)is detected on the pin.
//    This pin must be driven by an external agent for wakeup.
//
//    As soon as XNMI pin goes low, the device
//    should wake up, and GPIOA1 can be observed to go high.
//
//
//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################

#include "DSP281x_Device.h"
#include "DSP281x_Examples.h"

// Prototype statements for functions found within this file.

interrupt void XNMI_ISR(void);      // ISR for WAKEINT

void main()

{

asm("  EALLOW");

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
//  Enable all pull-ups

    EALLOW;
    /* Configure XNMI pin   */
    XIntruptRegs.XNMICR.bit.ENABLE = 1;            // Enable NMI pin
    XIntruptRegs.XNMICR.bit.POLARITY = 0;          // 0 -ve edge, 1 +ve edge triggered
    GpioMuxRegs.GPEMUX.bit.XNMI_XINT13_GPIOE2 = 1; // GPIOE2 is XNMI pin

    GpioMuxRegs.GPADIR.bit.GPIOA1 = 1;             // GPIOA1 set in the ISR to indicate device woken up.
    GpioDataRegs.GPACLEAR.bit.GPIOA1 = 1;
    EDIS;

/// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP281x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();


// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.XNMI = &XNMI_ISR;
   EDIS;

// Step 4. Initialize all the Device Peripherals:
// Not applicable for this example.

// Step 5. User specific code, enable interrupts:

// Enable global Interrupts:
   EINT;   // Enable Global interrupt INTM

// Write the LPM code value
    EALLOW;
    SysCtrlRegs.LPMCR0.bit.LPM = 0x0002;   // LPM mode = Halt
    EDIS;
// Force device into HALT

    asm(" IDLE");                           // Device waits in IDLE until falling edge on XNMI pin
                                            // wakes device from halt mode.
    while(1){}                              // Loop here after wake-up.

}

/* ----------------------------------------------- */
/* ISR for WAKEINT - Will be executed when         */
/* low pulse triggered on GPIO0 pin                */
/* ------------------------------------------------*/
interrupt void XNMI_ISR(void)
{
   GpioDataRegs.GPASET.bit.GPIOA1 = 1;  // Set GPIO1 in the ISR - monitored with oscilloscope
   // CPU acknowledges interrupt (IFR)
   return;
}
