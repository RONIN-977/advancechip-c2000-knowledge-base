/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   adc_ex8_ppb_limits.c
*                         TITLE:  ADC limits check using post-processing block for f2838x.
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
uint32_t intStatus;
//
// Functional Prototypes
//
void configureEPWM(uint32_t epwmBase);

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
    // - Configure the ADC and power it up
    // - Setup the ADC for ePWM triggered conversions on channel 0
    // - Configure ADC post-processing limits
    //      SOC0 will generate an interrupt if conversion is above or below limits
    // 
    Board_init();

    //
    // Configure the ePWM
    //
    configureEPWM(EPWM1_BASE);

    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //
    // Start ePWM:
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Enable SOCA trigger
    //
    EPWM_enableADCTrigger(EPWM1_BASE, EPWM_SOC_A);

    //
    // Unfreeze epwm counter to count-up
    //
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);

    //
    // Take conversions indefinitely in loop
    //
    do
    {
        //
        // Wait while ePWM causes ADC conversions.
        // If the ADC conversions exceed the PPB limits, then an interrupt
        // will be generated. User can read ADC results either in idle loop
        // or ADCINT ISR as per application requirement.
        //
    }
    while(1);
}

//
// configureEPWM - Setup SOC and compare values for EPWM
//
void configureEPWM(uint32_t epwmBase)
{
    //
    // Disable SOCA trigger
    //
    EPWM_disableADCTrigger(epwmBase, EPWM_SOC_A);

    //
    // Trigger SOCA on CMPA up-count
    //
    EPWM_setADCTriggerSource(epwmBase, EPWM_SOC_A, EPWM_SOC_TBCTR_U_CMPA);

    //
    // Generate pulse on 1st event
    //
    EPWM_setADCTriggerEventPrescale(epwmBase, EPWM_SOC_A, 1U);

    //
    // Set compare A value to 2048 counts
    //
    EPWM_setCounterCompareValue(epwmBase, EPWM_COUNTER_COMPARE_A, 2048U);

    //
    // Set period to 4096 counts
    //
    EPWM_setTimeBasePeriod(epwmBase, 4096U);

    //
    // Freeze counter
    //
    EPWM_setTimeBaseCounterMode(epwmBase, EPWM_COUNTER_MODE_STOP_FREEZE);
}

//
// adcAEvtISR - ISR for ADCA PPB
//
interrupt void adcAEvtISR(void)
{
    //
    // Warning, you are outside of PPB limits
    //
    intStatus = ADC_getPPBEventStatus(myADC0_BASE, ADC_PPB_NUMBER1);
    if((intStatus & ADC_EVT_TRIPHI) != 0U)
    {
        //
        // Voltage exceeded high limit
        //
        asm("   ESTOP0");

        //
        // Clear the trip flag and continue
        //
        ADC_clearPPBEventStatus(myADC0_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPHI);
    }
    if((intStatus & ADC_EVT_TRIPLO) != 0U)
    {
        //
        // Voltage exceeded low limit
        //
        asm("   ESTOP0");

        //
        // Clear the trip flag and continue
        //
        ADC_clearPPBEventStatus(myADC0_BASE, ADC_PPB_NUMBER1, ADC_EVT_TRIPLO);
    }
    Interrupt_clearACKGroup(INT_myADC0_EVT_INTERRUPT_ACK_GROUP);
}

//
// End of file
//
