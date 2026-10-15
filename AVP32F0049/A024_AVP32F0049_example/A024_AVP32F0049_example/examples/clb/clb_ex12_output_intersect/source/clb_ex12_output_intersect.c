/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                          http://www.advancechip.com/
*
*                         FILE:   clb_ex12_output_intersect.c
*                         TITLE:  CLB Output Intersect.
*******************************************************************************************/


#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"

__interrupt void clb1ISR(void);


#define EPWM_TIMER_TBPRD    2000

//
// Function Prototypes
//
void initEPWM(uint32_t epwm_base);
void initOff180EPWM(uint32_t epwm_base);
uint16_t dutyValue = 0;

void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    Board_init();

    //
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    initEPWM(myEPWM2_BASE);
    initOff180EPWM(myEPWM3_BASE);
    EPWM_setupEPWMLinks(myEPWM3_BASE, EPWM_LINK_WITH_EPWM_2, EPWM_LINK_COMP_A);
    EPWM_setSyncOutPulseMode(myEPWM2_BASE, EPWM_SYNC_OUT_PULSE_ON_COUNTER_ZERO);
    EPWM_enablePhaseShiftLoad(myEPWM3_BASE);
    //
    // Enable sync and clock to PWM
    //

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    Interrupt_register(INT_CLB1, &clb1ISR);
    Interrupt_enable(INT_CLB1);

    //
    // Enabling CLB1
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);

    initTILE1(myTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    CLB_clearInterruptTag(myTILE1_BASE);

    //EPWM1A is EPWM2A ANDED with EPWM3A

    for(;;)
    {
        SysCtl_delay(100000);
        dutyValue ++;
        if (dutyValue >= EPWM_TIMER_TBPRD)
            dutyValue = 0;
        EPWM_setCounterCompareValue(myEPWM2_BASE, EPWM_COUNTER_COMPARE_A, dutyValue);
    }
}


__interrupt void clb1ISR(void)
{

    CLB_clearInterruptTag(myTILE1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

void initEPWM(uint32_t epwm_base)
{
    //
    // Set-up TBCLK
    //
    EPWM_setTimeBasePeriod(epwm_base, EPWM_TIMER_TBPRD);
    EPWM_setPhaseShift(epwm_base, 0U);
    EPWM_setTimeBaseCounter(epwm_base, 0U);

    //
    // Set Compare values
    //
    EPWM_setCounterCompareValue(epwm_base,
                                EPWM_COUNTER_COMPARE_A,
                                2*EPWM_TIMER_TBPRD/3);
    EPWM_setCounterCompareValue(epwm_base,
                                EPWM_COUNTER_COMPARE_B,
                                2*EPWM_TIMER_TBPRD/3);

    //
    // Set up counter mode
    //
    EPWM_setTimeBaseCounterMode(epwm_base, EPWM_COUNTER_MODE_UP);
    EPWM_disablePhaseShiftLoad(epwm_base);
    EPWM_setClockPrescaler(epwm_base,
                           EPWM_CLOCK_DIVIDER_8,
                           EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set up shadowing
    //
    EPWM_setCounterCompareShadowLoadMode(epwm_base,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(epwm_base,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //
    // Set actions
    //

    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);


    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);

    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

}


void initOff180EPWM(uint32_t epwm_base)
{
    //
    // Set-up TBCLK
    //
    EPWM_selectPeriodLoadEvent(epwm_base, EPWM_SHADOW_LOAD_MODE_SYNC);
    EPWM_setTimeBasePeriod(epwm_base, EPWM_TIMER_TBPRD);
    EPWM_setPhaseShift(epwm_base, EPWM_TIMER_TBPRD/2);
    EPWM_setTimeBaseCounter(epwm_base, EPWM_TIMER_TBPRD/2);

    //
    // Set Compare values
    //
    EPWM_setCounterCompareValue(epwm_base,
                                EPWM_COUNTER_COMPARE_A,
                                2*EPWM_TIMER_TBPRD/3);
    EPWM_setCounterCompareValue(epwm_base,
                                EPWM_COUNTER_COMPARE_B,
                                2*EPWM_TIMER_TBPRD/3);

    //
    // Set up counter mode
    //
    EPWM_setTimeBaseCounterMode(epwm_base, EPWM_COUNTER_MODE_UP);
    EPWM_disablePhaseShiftLoad(epwm_base);
    EPWM_setClockPrescaler(epwm_base,
                           EPWM_CLOCK_DIVIDER_8,
                           EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set up shadowing
    //
    EPWM_setCounterCompareShadowLoadMode(epwm_base,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(epwm_base,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //
    // Set actions
    //
    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(epwm_base,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);

}
