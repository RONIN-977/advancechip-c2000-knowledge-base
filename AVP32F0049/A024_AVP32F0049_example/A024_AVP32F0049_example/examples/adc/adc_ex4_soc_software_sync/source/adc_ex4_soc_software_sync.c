/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   adc_ex4_soc_software_sync.c
*                         TITLE:  ADC synchronous software triggering
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Globals
//
uint16_t myADC0Result0;
uint16_t myADC0Result1;
uint16_t myADC1Result0;
uint16_t myADC1Result1;

void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
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
    // Board Initialization
    // - Write ADC configurations and power up the ADC for both ADC A and ADC C
    // - Setup GPIO 0 to trigger input XBAR line 5.  GPIO0 is used
    //   as an example, but any spare GPIO could be used. The
    //   physical GPIO pin should be allowed to float, since the
    //   code configures it as an output and changes the value.
    // - Setup ADC acquisition window and compare values
    //
    Board_init();

    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //
    // Take conversions indefinitely in loop
    //
    do
    {
        //
        // Convert, wait for completion, and store results
        //

        //
        // Toggle GPIO0 in software.  This will cause a trigger to
        // both ADCs via input XBAR, line 5.
        //
        GPIO_writePin(0U, 1U); // Set pin
        GPIO_writePin(0U, 0U); // Clear pin

        //
        // Wait for ADCA to complete, then acknowledge the flag.
        // Since both ADCs are running synchronously, it isn't necessary
        // to wait for completion notification from both ADCs
        //
        while(ADC_getInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1) == 0U);
        ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);

        //
        // Store results
        //
        myADC0Result0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
        myADC0Result1 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);
        myADC1Result0 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0);
        myADC1Result1 = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER1);

        //
        // At this point, conversion results are stored in
        // myADC0Result0, myADC0Result1, myADC1Result0, and myADC1Result1
        //

        //
        // Software breakpoint, hit run again to get updated conversions
        //
        asm("   ESTOP0");

    }
    while(1);
}

//
// End of file
//
