/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex29_timestamp.c
*                         TITLE: CLB Trip Zone Timestamp
*******************************************************************************************/



//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"
#include "board.h"

//
// Globals
//
uint32_t epwm1TZIntCount;
#define EPWM_TZ1_INT_TAG    12U
#define EPWM_TZ2_INT_TAG    13U

uint32_t initialTripZone;
uint64_t tz1Counter64bit;
uint64_t tz2Counter64bit;

//
// Function Prototypes
//
void initEPWM1(void);
__interrupt void epwm1TZISR(void);
__interrupt void clb1ISR(void);

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
    // Disable pin locks and enable internal pull ups.
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
    // Interrupts that are used in this example are re-mapped to ISR functions
    // found within this file.
    //
    Interrupt_register(INT_EPWM1_TZ, &epwm1TZISR);
    Interrupt_register(INT_CLB1, &clb1ISR);

    //
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Initialize ePWM1
    //
    initEPWM1();

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Enabling CLB1
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);

    // Board Initialization
    Board_init();

    initTILE1(myTILE1_BASE);

    //
    // Enable interrupts required for this example
    //
    Interrupt_enable(INT_EPWM1_TZ);
    Interrupt_enable(INT_CLB1);

    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;

    CLB_clearInterruptTag(myTILE1_BASE);

    // Enable synchronized CLB counters
    HWREG(myTILE1_BASE + CLB_LOGICCTL + CLB_O_GP_REG) = 1 << 7;

    //
    // IDLE loop. Just sit and loop forever (optional):
    //
    for(;;)
    {
        NOP;
    }
}

__interrupt void clb1ISR(void)
{
    // Get counter value for TZ1
    uint32_t baseTZ1 = CLB_getRegister(myTILE1_BASE, CLB_REG_HLC_R0);
    uint32_t overflowTZ1 = CLB_getRegister(myTILE1_BASE, CLB_REG_HLC_R2);
    tz1Counter64bit = (uint64_t) overflowTZ1 << 32 | baseTZ1;

    // Get counter value for TZ2
    uint32_t baseTZ2 = CLB_getRegister(myTILE1_BASE, CLB_REG_HLC_R1);
    uint32_t overflowTZ2 = CLB_getRegister(myTILE1_BASE, CLB_REG_HLC_R3);
    tz2Counter64bit = (uint64_t) overflowTZ2 << 32 | baseTZ2;

    // Determine initial source of interrupt, TZ1 or TZ2
    if ( (tz1Counter64bit != 0) && ((tz1Counter64bit < tz2Counter64bit) || (tz2Counter64bit == 0)) )
        initialTripZone = 1;
    else if ( (tz2Counter64bit != 0) && ((tz2Counter64bit < tz1Counter64bit) || (tz1Counter64bit == 0)) )
        initialTripZone = 2;
    else
        initialTripZone = 0;

    CLB_clearInterruptTag(myTILE1_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP5);
}

//
// epwm1TZISR - ePWM1 TZ ISR
//
__interrupt void epwm1TZISR(void)
{
    epwm1TZIntCount++;

    //
    // To re-enable the OST Interrupt, uncomment the below code:
    //
    // EPWM_clearTripZoneFlag(myEPWM1_BASE,
    //                        (EPWM_TZ_INTERRUPT | EPWM_TZ_FLAG_CBC));

    //
    // Acknowledge this interrupt to receive more interrupts from group 2
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP2);
}

//
// initEPWM1 - Configure ePWM1
//
void initEPWM1()
{
    //
    // Enable TZ1 and TZ2 as one shot trip sources
    //
    EPWM_enableTripZoneSignals(myEPWM1_BASE, EPWM_TZ_SIGNAL_OSHT1);
    EPWM_enableTripZoneSignals(myEPWM1_BASE, EPWM_TZ_SIGNAL_OSHT2);

    //
    // Action on TZ1
    //
    EPWM_setTripZoneAction(myEPWM1_BASE,
                           EPWM_TZ_ACTION_EVENT_TZA,
                           EPWM_TZ_ACTION_HIGH);

    //
    // Enable TZ interrupt
    //
    EPWM_enableTripZoneInterrupt(myEPWM1_BASE, EPWM_TZ_INTERRUPT_OST);

    //
    // Set-up TBCLK
    //
    EPWM_setTimeBasePeriod(myEPWM1_BASE, 12000U);
    EPWM_setPhaseShift(myEPWM1_BASE, 0U);
    EPWM_setTimeBaseCounter(myEPWM1_BASE, 0U);
    EPWM_setTimeBaseCounterMode(myEPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_disablePhaseShiftLoad(myEPWM1_BASE);

    //
    // Set ePWM clock pre-scaler
    //
    EPWM_setClockPrescaler(myEPWM1_BASE,
                           EPWM_CLOCK_DIVIDER_4,
                           EPWM_HSCLOCK_DIVIDER_4);

    //
    // Set up shadowing
    //
    EPWM_setCounterCompareShadowLoadMode(myEPWM1_BASE,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);

    //
    // Set-up compare
    //
    EPWM_setCounterCompareValue(myEPWM1_BASE, EPWM_COUNTER_COMPARE_A, 6000U);

    //
    // Set actions
    //
    EPWM_setActionQualifierAction(myEPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(myEPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
}

