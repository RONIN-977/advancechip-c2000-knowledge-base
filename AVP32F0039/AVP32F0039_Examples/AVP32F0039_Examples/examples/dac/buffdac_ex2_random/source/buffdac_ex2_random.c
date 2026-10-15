/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   buffdac_ex2_random.c
*                         TITLE:   Buffered DAC Random Output Example.
*******************************************************************************************/
//! This example generates random voltages on the buffered DAC output,
//! DACOUTA/ADCINA0 and uses the default DAC reference setting of
//! VDAC.
//!
//! \b External \b Connections \n
//!  - When the DAC reference is set to VDAC, an external reference voltage
//!    must be applied to the VDAC pin. This can accomplished by connecting a
//!    jumper wire from 3.3V to ADCINB3.
//!
//! \b Watch \b Variables \n
//!  - None.
//!
//
//#############################################################################

//
// Included Files
//
#include <time.h>
#include <stdlib.h>
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Globals
//
uint16_t lowLimit = 410;
uint16_t highLimit = 3686;

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
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Initialize a random time seed
    //
    srand(time(NULL));

    //
    // Configure the DAC module
    // Configure DAC - Setup the reference voltage and output value for the DAC
    //
    Board_init();

    //
    // Continuously set the DAC output value based on random value
    //
    while(1)
    {
        DAC_setShadowValue(myDAC0_BASE, ((rand() % (highLimit - lowLimit)) +
                                       lowLimit));
        DEVICE_DELAY_US(1);
    }
}

//
// End of File
//
