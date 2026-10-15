/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                            湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  eqep_ex2_pos_speed.c
*                         TITLE: Position and Speed Measurement Using eQEP
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "IQmathLib.h"
#include "eqep_ex2_calculation.h"
#include "board.h"


//
// Defines
//
#define TB_CLK    DEVICE_SYSCLK_FREQ        // Time base clock is SYSCLK
#define PWM_CLK   5000                      // We want to output at 5 kHz
                                            // (300 rpm)
#define PRD_VAL   (TB_CLK / (PWM_CLK * 2))  // Calculate value period value
                                            // for up-down count mode

// .9999 / 4000 converted to IQ26 fixed point format
#define MECH_SCALER     16776
// 2 pole pairs in this example
#define POLE_PAIRS      2
// Angular offset between encoder and Phase A
#define CAL_ANGLE       0
// See Equation 5 in eqep_ex2_calculation.c
#define SPEED_SCALER    ((((uint64_t)32 * DEVICE_SYSCLK_FREQ / 64) * 60) / (24000000))
// Base/max rpm is 6000rpm
#define BASE_RPM        6000


//
// Function Prototypes
//
void initEPWM(void);
__interrupt void epwmISR(void);

//
// Globals
//
PosSpeed_Object posSpeed =
{
    0, 0, 0, 0,     // Initialize outputs to zero
    MECH_SCALER,    // mechScaler
    POLE_PAIRS,     // polePairs
    CAL_ANGLE,      // calAngle
    SPEED_SCALER,   // speedScaler
    0,              // Initialize output to zero
    BASE_RPM,       // baseRPM
    0, 0, 0, 0      // Initialize outputs to zero
};

uint16_t interruptCount = 0;
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

    //
    // Board Initialization
    // Setup eQEP1, configuring the unit timer and quadrature capture units
    // Initialize GPIOs for use as EQEP1A, EQEP1B, and EQEP1I
    //
    Board_init();

    //
    // On controlCARD, Initialize GPIO0 to ePWM1A, GPIO1 to ePWM1B, and GPIO4 
    // as an output. (On LaunchPad, use GPIO10 on ePWM6A, GPIO11 on ePWM6B, and 
    // GPIO8 as an output.)
    // They will be used to simulate incoming eQEP Phase A, Phase B, and Index
    // signals respectively.
    //
    GPIO_setPinConfig(DEVICE_GPIO_CFG_EPWMxA);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_EPWMxA, GPIO_PIN_TYPE_STD);

    GPIO_setPinConfig(DEVICE_GPIO_CFG_EPWMxB);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_EPWMxB, GPIO_PIN_TYPE_STD);
    
    //
    // Interrupts that are used in this example are re-mapped to ISR functions
    // found within this file.
    //
    Interrupt_register(DEVICE_PERIPHERAL_INT_EPWM, &epwmISR);

    //
    // Setup ePWM1 to generate a 5 kHz signal to be an input to the eQEP
    //
    initEPWM();

    //
    // Enable interrupts required for this example
    //
    Interrupt_enable(DEVICE_PERIPHERAL_INT_EPWM);

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
        ;
    }
}

//
// Function to configure ePWM1 to generate a 5 kHz signal.
//
void initEPWM(void)
{
    //
    // Disable the ePWM time base clock before configuring the module
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Set phase shift to 0 and clear the time base counter
    //
    EPWM_setPhaseShift(DEVICE_PERIPHERAL_BASE_EPWM, 0);
    EPWM_setTimeBaseCounter(DEVICE_PERIPHERAL_BASE_EPWM, 0);

    //
    // Disable the shadow load; the load will be immediate instead
    //
    EPWM_disableCounterCompareShadowLoadMode(DEVICE_PERIPHERAL_BASE_EPWM,
                                             EPWM_COUNTER_COMPARE_A);
    EPWM_disableCounterCompareShadowLoadMode(DEVICE_PERIPHERAL_BASE_EPWM,
                                             EPWM_COUNTER_COMPARE_B);

    //
    // Set the compare A value to half the period value, compare B to 0
    //
    EPWM_setCounterCompareValue(DEVICE_PERIPHERAL_BASE_EPWM, 
                                EPWM_COUNTER_COMPARE_A, PRD_VAL/2);
    EPWM_setCounterCompareValue(DEVICE_PERIPHERAL_BASE_EPWM, 
                                EPWM_COUNTER_COMPARE_B, 0);

    //
    // Set action qualifier behavior on compare A events
    // - EPWM1A --> 1 when CTR = CMPA and increasing
    // - EPWM1A --> 0 when CTR = CMPA and decreasing
    //
    EPWM_setActionQualifierAction(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

    //
    // Set action qualifier behavior on compare B events
    // - EPWM1B --> 1 when CTR = PRD and increasing
    // - EPWM1B --> 0 when CTR = 0 and decreasing
    //
    EPWM_setActionQualifierAction(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);

    //
    // Enable interrupt when the counter is equal to PRD
    //
    EPWM_setInterruptSource(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_INT_TBCTR_PERIOD);
    EPWM_enableInterrupt(DEVICE_PERIPHERAL_BASE_EPWM);

    //
    // Interrupt on first event
    //
    EPWM_setInterruptEventCount(DEVICE_PERIPHERAL_BASE_EPWM, 1);

    //
    // Set the time base clock prescaler to /1
    //
    EPWM_setClockPrescaler(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_CLOCK_DIVIDER_1,
                           EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set the period value; don't shadow the register
    //
    EPWM_setPeriodLoadMode(DEVICE_PERIPHERAL_BASE_EPWM, EPWM_PERIOD_DIRECT_LOAD);
    EPWM_setTimeBasePeriod(DEVICE_PERIPHERAL_BASE_EPWM, PRD_VAL);

    //
    // Put the time base counter into up-down count mode
    //
    EPWM_setTimeBaseCounterMode(DEVICE_PERIPHERAL_BASE_EPWM,  
                                EPWM_COUNTER_MODE_UP_DOWN);


    //
    // Sync the ePWM time base clock
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

//
// ePWM1 ISR--interrupts once every 4 QCLK counts (one period)
//
 __interrupt void epwmISR(void)
{
    uint16_t i;

    //
    // Position speed and measurement
    //
    PosSpeed_calculate(&posSpeed);


    //
    // Control loop for position control and speed control
    //
    interruptCount++;
    if(interruptCount == 1000)
    {
        //
        // Pulse index signal (1 pulse/rev)
        //
        GPIO_writePin(DEVICE_GPIO_PIN_IOINDEX, 1);
        for(i = 0; i < 700; i++)
        {
            ;
        }
        GPIO_writePin(DEVICE_GPIO_PIN_IOINDEX, 0);
    }

    //
    // Clear interrupt flag and issue ACK
    //
    EPWM_clearEventTriggerInterruptFlag(DEVICE_PERIPHERAL_BASE_EPWM);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}
