/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  interrupt_ex4_epwm_realtime_interrupt.c
*                         TITLE: EPWM Real-Time Interrupt
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define EPWM1_TIMER_TBPRD   0x2000


//
// Globals
//
uint32_t EPwm1TimerIntCount;
uint16_t LEDcount;

//
// Function Prototypes
//
extern void SetDBGIER(uint16_t dbgier); // Defined in <device>_dbgier.asm file
void initEPWM(void);
__interrupt void epwm1ISR(void);

//
// Main
//
void main(void)
{
    //
    // Initializes device clock and peripherals
    //
    Device_init();

    //
    // Configures the GPIO pin as a push-pull output
    //
    Device_initGPIO();
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED2, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED2, GPIO_DIR_MODE_OUT);

    //
    // Initializes PIE and clears PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initializes the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    Interrupt_register(INT_EPWM1, &epwm1ISR);

    //
    // Initialize EPWM1
    //
    initEPWM();

    //
    // Initialize counters
    //
    EPwm1TimerIntCount = 0;
    LEDcount = 0;

    //
    // Enable EPWM1 Interrupt
    //
    Interrupt_enable(INT_EPWM1);

    //
    // Initially disable time-critical interrupts.
    // The parameter to SetDBGIER() can be Ored value of INTERRUPT_CPU_INTx.
    // Eg : SetDBGIER(INTERRUPT_CPU_INT3);
    //
    SetDBGIER(0x0000);   // PIE groups time-critical designation

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop forever
    //
    while(1)
    {
        //
        // Toggle LED1
        //
        GPIO_togglePin(DEVICE_GPIO_PIN_LED1);

        //
        // 200ms delay
        //
        DEVICE_DELAY_US(200000);
    }
}

void initEPWM()
{
    //
    // Disable sync
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Initially disable Free/Soft Bits
    //
    EPWM_setEmulationMode(EPWM1_BASE, EPWM_EMULATION_STOP_AFTER_NEXT_TB);

    //
    // Setup TBCLK
    //
    EPWM_setTimeBasePeriod(EPWM1_BASE, EPWM1_TIMER_TBPRD);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setTimeBaseCounter(EPWM1_BASE, 0U);

    //
    // CompareA event at half of period
    //
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A,
                                EPWM1_TIMER_TBPRD/2);

    //
    // Action Qualifiers : Set on CMPA, Clear on PRD
    //
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

    //
    // Configure interrupt on event CTR=ZERO
    //
    EPWM_setInterruptSource(EPWM1_BASE, EPWM_INT_TBCTR_ZERO);
    EPWM_enableInterrupt(EPWM1_BASE);
    EPWM_setInterruptEventCount(EPWM1_BASE, 1);

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
}

__interrupt void
epwm1ISR(void)
{
    EPwm1TimerIntCount++;
    LEDcount++;

    //
    // Toggle LED2 when the count reaches 500
    //
    if (LEDcount == 500)
    {
        GPIO_togglePin(DEVICE_GPIO_PIN_LED2);
        LEDcount=0;
    }

    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(EPWM1_BASE);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}

//
// End of File
//
