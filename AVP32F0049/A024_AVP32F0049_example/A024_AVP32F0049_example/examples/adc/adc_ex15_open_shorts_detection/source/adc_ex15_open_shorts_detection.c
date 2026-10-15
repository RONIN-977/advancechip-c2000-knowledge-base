/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   adc_ex15_open_shorts_detection.c
*                         TITLE:  ADC input pin fault check through Open/Shorts Detection Circuit
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define EX_ENABLE_OS_DETECTION

//
// Globals
//
uint16_t i = 0U;
volatile uint16_t adcAResult0;
volatile uint16_t resultHi;
volatile uint16_t resultLo;
volatile uint16_t resultNormal;

#ifdef EX_ENABLE_OS_DETECTION
//
// Application specific limits from Vreflo to Vrefhi.
// This can change due to various factors like source
// voltage offset etc.
//
uint16_t osdHiLimit = 3900;    // upper limit
uint16_t osdLoLimit = 15;      // lower limit

//
// Macros for open/shorts detection circuit status. If the osDetect status is
// other than open, shorted to Vreflo and shorted to Vrefhi values (i.e > 4),
// it can be considered as a valid signal
//
#define ADC_OSDETECT_STATUS_OPEN                  1
#define ADC_OSDETECT_STATUS_SHORTED_TO_VREFLO     2
#define ADC_OSDETECT_STATUS_SHORTED_TO_VREFHI     4
#define ADC_OSDETECT_STATUS_GOOD                  8
#define ADC_OSDETECT_STATUS_UNDETERMINED          16

uint16_t osDetectStatusVal = 0;
#endif

//
// Function Prototypes
//
void configureADC(uint32_t adcBase);
void configureADCSOC(uint32_t adcBase, uint16_t channel);
#ifdef EX_ENABLE_OS_DETECTION
void configureADCTestMode(uint32_t adcBase, ADC_Channel channel);
uint16_t performOSDetection(uint32_t adcBase, uint32_t adcResultBase,
                            uint16_t channel);
#endif

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

#ifdef EX_ENABLE_OS_DETECTION
    //
    // Configure ADC in test mode for checking pin status
    //
    configureADCTestMode(ADCA_BASE, ADC_CH_ADCIN0);

    //
    // Perform open/shorts detection
    //
    for(i = 0U; i < 7U; i++)
    {
        osDetectStatusVal = osDetectStatusVal |
                        performOSDetection(ADCA_BASE, ADCARESULT_BASE, 0U);

    }

    if(osDetectStatusVal > ADC_OSDETECT_STATUS_SHORTED_TO_VREFHI)
    {
#endif
        //
        // Set up ADCs, initializing the SOCs to be triggered by software
        //
        configureADC(ADCA_BASE);
        configureADCSOC(ADCA_BASE, ADC_CH_ADCIN0);

        //
        // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
        //
        EINT;
        ERTM;

        //
        // Loop indefinitely
        //
        while(1)
        {

            //
            // Convert, wait for completion, and store results
            //
            ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);

            //
            // Wait for ADCA to complete, then acknowledge flag
            //
            while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == false)
            {
            }
            ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

            //
            // Store results
            //
            adcAResult0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

            //
            // Software breakpoint. At this point, conversion results are stored
            // in adcAResult0.
            //
            // Hit run again to get updated conversions.
            //
            ESTOP0;
        }
#ifdef EX_ENABLE_OS_DETECTION
    }
    else
    {
        //
        // Selected ADC pin is either open or shorted to Vrefhi or Vreflo.
        // Loop indefinitely.
        //
        while(1)
        {
        }
    }
#endif
}

#ifdef EX_ENABLE_OS_DETECTION
//
// performOSDetection - Function to perform open/shorts detection
// for selected ADC channel
//
uint16_t performOSDetection(uint32_t adcBase, uint32_t adcResultBase,
                                      uint16_t channel)
{
    uint16_t i;
    uint16_t status = ADC_OSDETECT_STATUS_UNDETERMINED;
    resultHi = 0, resultLo = 0, resultNormal = 0;

    //
    // Step 1: Configure full scale OSDETECT mode and capture
    // ADC results
    //

    //
    // Enable pull-up mode
    //
    ADC_configOSDetectMode(adcBase, ADC_OSDETECT_MODE_VDDA);

    //
    // Force SOC0-SOC15
    //
    ADC_forceMultipleSOC(adcBase, 0xFFFF);

    //
    // Wait for ADC to complete, then acknowledge flag
    //
    while(ADC_getInterruptStatus(adcBase, ADC_INT_NUMBER1) == false)
    {
    }
    ADC_clearInterruptStatus(adcBase, ADC_INT_NUMBER1);

    //
    // Read ADC results
    //
    for(i = 0U; i < 16U; i++)
    {
        resultHi = resultHi + ADC_readResult(adcResultBase,
                                                 (ADC_SOCNumber)i);
    }

    resultHi = resultHi >> 4U;

    //
    // Step 2: Configure zero scale OSDETECT mode and capture
    // ADC results
    //

    //
    // Enable pull-down mode
    //
    ADC_configOSDetectMode(adcBase, ADC_OSDETECT_MODE_VSSA);

    //
    // Force SOC0-SOC15
    //
    ADC_forceMultipleSOC(adcBase, 0xFFFFU);

    //
    // Wait for ADC to complete, then acknowledge flag
    //
    while(ADC_getInterruptStatus(adcBase, ADC_INT_NUMBER1) == false)
    {
    }
    ADC_clearInterruptStatus(adcBase, ADC_INT_NUMBER1);

    //
    // Read ADC results
    //
    for(i = 0U; i < 16U; i++)
    {
        resultLo = resultLo + ADC_readResult(adcResultBase, (ADC_SOCNumber)i);
    }

    resultLo = resultLo >> 4U;

    //
    // Step 3: Disable OSDETECT mode and capture
    // ADC results
    //

    //
    // Disable OSDETECT mode
    //
    ADC_configOSDetectMode(adcBase, ADC_OSDETECT_MODE_DISABLED);

    //
    // Force SOC0-SOC15
    //
    ADC_forceMultipleSOC(adcBase, 0xFFFFU);

    //
    // Wait for ADC to complete, then acknowledge flag
    //
    while(ADC_getInterruptStatus(adcBase, ADC_INT_NUMBER1) == false)
    {
    }
    ADC_clearInterruptStatus(adcBase, ADC_INT_NUMBER1);

    //
    // Read ADC results
    //
    for(i = 0U; i < 16U; i++)
    {
        resultNormal = resultNormal + ADC_readResult(adcResultBase,
                                                     (ADC_SOCNumber)i);
    }

    resultNormal = resultNormal >> 4U;

    //
    // Step 4: Determine the state of the ADC pin
    //

    //
    // If the pin is open, resultLo would be equal to Vreflo and
    // resultHi would be equal to Vrefhi
    //
    if((resultLo <= osdLoLimit) && (resultHi >= osdHiLimit))
    {
        status = ADC_OSDETECT_STATUS_OPEN;
    }

    //
    // If the pin is shorted to Vrefhi, resultLo should be approximately
    // equal to Vrefhi and resultHi should be equal to Vrefhi
    //
    else if((resultLo > osdLoLimit) && (resultHi >= osdHiLimit) &&
            (resultNormal >= osdHiLimit))
    {
        status = ADC_OSDETECT_STATUS_SHORTED_TO_VREFHI;
    }

    //
    // If the pin is shorted to Vreflo, resultLo should be equal to Vreflo
    // and resultHi should be approximately equal to Vreflo
    //
    else if((resultLo <= osdLoLimit) && (resultHi < osdHiLimit ) &&
            (resultNormal <= osdLoLimit ))
    {
        status = ADC_OSDETECT_STATUS_SHORTED_TO_VREFLO;
    }

    //
    // If the pin is connected to a valid signal, resultLo should be greater
    // than osdLoLimit but less than resultNormal while resultHi should be
    // less than osdHiLimit but greater than resultNormal
    //
    else if((osdLoLimit < resultLo < resultNormal) &&
            (osdHiLimit < resultHi < resultNormal ))
    {
        status = ADC_OSDETECT_STATUS_GOOD;
    }

    return(status);
}


//
// configureADCTestMode - Function to configure selected ADC to enable testing
// of Open/Shorts Detection
//
void configureADCTestMode(uint32_t adcBase, ADC_Channel channel)
{
    uint16_t i;

    //
    // Setup VREF as internal
    //
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    //
    // Set ADCCLK divider to /4
    //
    ADC_setPrescaler(adcBase, ADC_CLK_DIV_4_0);

    //
    // Set pulse positions to late
    //
    ADC_setInterruptPulseMode(adcBase, ADC_PULSE_END_OF_CONV);

    //
    // Power up the ADCs and then delay for 1 ms
    //
    ADC_enableConverter(adcBase);

    DEVICE_DELAY_US(1000);

    //
    // Configure ADC SOCs to sample selected channel in test mode
    //
    for(i = 0; i < 16; i++)
    {
        ADC_setupSOC(adcBase, (ADC_SOCNumber)i, ADC_TRIGGER_SW_ONLY,
                     channel, 64U);
    }

    //
    // Set SOC15 to set the interrupt 1 flag. Enable the interrupt and make
    // sure its flag is cleared.
    //
    ADC_setInterruptSource(adcBase, ADC_INT_NUMBER1, ADC_SOC_NUMBER15);
    ADC_enableInterrupt(adcBase, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(adcBase, ADC_INT_NUMBER1);
}

#endif

//
// configureADC - Write ADC configurations and power up the desired ADC instance
//
void configureADC(uint32_t adcBase)
{
    //
    // Setup VREF as internal
    //
    ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);

    //
    // Set ADCCLK divider to /4
    //
    ADC_setPrescaler(adcBase, ADC_CLK_DIV_4_0);

    //
    // Set pulse positions to late
    //
    ADC_setInterruptPulseMode(adcBase, ADC_PULSE_END_OF_CONV);

    //
    // Power up the ADCs and then delay for 1 ms
    //
    ADC_enableConverter(adcBase);

    //
    // Delay for 1ms to allow ADC time to power up
    //
    DEVICE_DELAY_US(1000);
}

//
// configureADCSOC - Setup ADC EPWM channel and trigger settings
//
void configureADCSOC(uint32_t adcBase, uint16_t channel)
{
    //
    // Determine minimum acquisition window (in SYSCLKS) based on resolution
    // - NOTE: A longer sampling window will be required if the ADC driving
    //   source is less than ideal (an ideal source would be a high bandwidth
    //   op-amp with a small series resistance). See TI application report
    //   SPRACT6 for guidance on ADC driver design.
    //
    uint16_t acqps = 8;

    //
    // Select the channels to convert and end of conversion flag
    // ADCA
    //
    ADC_setupSOC(adcBase, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
                 (ADC_Channel)channel, acqps);

    //
    // Configure ADCINT1 as SOC0 trigger
    //
    ADC_setInterruptSOCTrigger(adcBase, ADC_SOC_NUMBER0,
                               ADC_INT_SOC_TRIGGER_ADCINT1);

    //
    // Enable continuous mode
    //
    ADC_enableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);

    //
    // Configure source as EOC1, clear & enable the interrupt
    //
    ADC_setInterruptSource(adcBase, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_clearInterruptStatus(adcBase, ADC_INT_NUMBER1);
    ADC_enableInterrupt(adcBase, ADC_INT_NUMBER1);
}

//
// End of File
//

