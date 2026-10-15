/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   adc_ex7_ppb_offset.c
*                         TITLE:  ADC offset adjust using post-processing block
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
uint16_t myADC0Result;
uint16_t myADC0PPBResult;
uint16_t myADC1Result;
uint16_t myADC1PPBResult;

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
    // - Configure the ADCs and power them up
    // - Setup the ADCs for software conversions
    // - Setup PPB offset correction.
    //      conversion on channel A will subtract 100.
    //      conversion on channel C will add 100.
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
        // Convert, wait for completion, and store results.
        // Start conversions immediately via software, ADCA
        //
        ADC_forceSOC(myADC0_BASE, ADC_SOC_NUMBER0);
        ADC_forceSOC(myADC0_BASE, ADC_SOC_NUMBER1);

        //
        // Wait for ADCA to complete, then acknowledge flag
        //
        while(ADC_getInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1) == false);
        ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);

        //
        // Start conversions immediately via software, ADCC
        //
        ADC_forceSOC(myADC1_BASE, ADC_SOC_NUMBER0);
        ADC_forceSOC(myADC1_BASE, ADC_SOC_NUMBER1);

        //
        // Wait for ADCC to complete, then acknowledge flag
        //
        while(ADC_getInterruptStatus(myADC1_BASE, ADC_INT_NUMBER1) == false);
        ADC_clearInterruptStatus(myADC1_BASE, ADC_INT_NUMBER1);

        //
        // Store results
        //
        myADC0Result = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
        myADC0PPBResult = ADC_readPPBResult(ADCARESULT_BASE, ADC_PPB_NUMBER1);
        myADC1Result = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0);
        myADC1PPBResult = ADC_readPPBResult(ADCCRESULT_BASE, ADC_PPB_NUMBER1);

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
