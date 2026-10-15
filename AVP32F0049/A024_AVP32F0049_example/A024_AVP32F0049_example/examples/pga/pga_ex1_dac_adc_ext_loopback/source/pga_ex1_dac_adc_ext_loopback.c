/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  pga_ex1_dac_adc_ext_loopback.c
*                         TITLE: PGA DAC-ADC External Loopback Example.
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
volatile uint16_t dacResult;
volatile uint16_t pgaResult;
float pgaGain;

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

	Board_init();

	//
	// Enable Global Interrupt (INTM) and real time interrupt (DBGM)
	//
	EINT;
	ERTM;

	//
	// Force ADC conversion
	//
    ADC_forceSOC(myADC0_BASE, ADC_SOC_NUMBER0);
    ADC_forceSOC(myADC0_BASE, ADC_SOC_NUMBER1);

    //
    // Loop indefinitely
    //
    while(1)
    {
        //
        // Check if conversion is complete
	    //
	    if(ADC_getInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1))
	    {
	        //
	        // Acknowledge flag
	        //
	        ADC_clearInterruptStatus(myADC0_BASE, ADC_INT_NUMBER1);

	        dacResult = ADC_readResult(myADC0_RESULT_BASE, ADC_SOC_NUMBER0);
	        pgaResult = ADC_readResult(myADC0_RESULT_BASE, ADC_SOC_NUMBER1);
	        pgaGain   = (float)pgaResult / (float)dacResult;

	        //
	        // Force ADC conversion
	        //
	        ADC_forceSOC(myADC0_BASE, ADC_SOC_NUMBER0);
	        ADC_forceSOC(myADC0_BASE, ADC_SOC_NUMBER1);
	    }
	}
}

//
// End of File
//
