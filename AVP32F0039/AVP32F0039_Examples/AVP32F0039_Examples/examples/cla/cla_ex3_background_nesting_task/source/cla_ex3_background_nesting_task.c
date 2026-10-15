/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  cla_ex3_background_nesting_task.c
*                         TITLE: CLA background nesting task
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "cla_ex3_background_nesting_task_shared.h"
#include "board.h"

//
// Defines
//
#define EPWM_CLKDIV        1792UL
#define EPWM1_FREQ         2UL
#define EPWM1_PERIOD  (uint16_t)(DEVICE_SYSCLK_FREQ /(EPWM_CLKDIV * EPWM1_FREQ))

//
// Defines
//
#define WAITSTEP    __asm(" RPT #255||NOP")
#define EPSILON     1e-1

//
// Globals
//

//
// Function Prototypes
//
void initEPWM(void);

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
    // Configure the LEDs
    //
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED2, GPIO_DIR_MODE_OUT);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_LED1, GPIO_QUAL_SYNC);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_LED2, GPIO_QUAL_SYNC);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_LED1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_LED2);

    GPIO_setControllerCore(DEVICE_GPIO_PIN_LED1, GPIO_CORE_CPU1_CLA1);
    GPIO_setControllerCore(DEVICE_GPIO_PIN_LED2, GPIO_CORE_CPU1_CLA1);

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    // initialize CPU timers

    // Initialize timer period : 2 seconds

    CPUTimer_setPeriod(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ * 2);

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    // Make sure timer is stopped
    CPUTimer_stopTimer(CPUTIMER1_BASE);
    CPUTimer_setEmulationMode(CPUTIMER1_BASE,
                              CPUTIMER_EMULATIONMODE_RUNFREE);
    // Enables CPU timer interrupt.
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    // Starts(restarts) CPU timer.
    CPUTimer_startTimer(CPUTIMER1_BASE);

    //
    // Initialize resources
    //
    Board_init();

    //
    // Disable sync(Freeze clock to PWM as well)
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    initEPWM();

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    for(;;)
    {
    }
}

//
// EPWM Initialization
//
// Description: EPWM1A will run at EPWM1_FREQ Hz and trigger CLA Task1
// The default time base for the EPWM module is half the system clock i.e.
// TBCLK = SYSCLKOUT
// EPWM1A will be setup in count-up mode and an event generated every period
//
void initEPWM(void)
{
    //
    // Set up EPWM1 to
    // - run on a base clock of SYSCLK/128 / 14
    // - have a period of EPWM1_PERIOD
    // - run in count up mode
    //
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_128,
                           EPWM_HSCLOCK_DIVIDER_14);
    EPWM_setTimeBasePeriod(EPWM1_BASE, EPWM1_PERIOD);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
    EPWM_setTimeBaseCounter(EPWM1_BASE, 0U);

    //
    // Enable INT, generate INT on 1st event
    //
    EPWM_setInterruptSource(EPWM1_BASE, EPWM_INT_TBCTR_ZERO);
    EPWM_enableInterrupt(EPWM1_BASE);
    EPWM_setInterruptEventCount(EPWM1_BASE, 1U);

    //
    // EPWM 1 should Stop when counter completes whole cycle in emulation mode
    //
    EPWM_setEmulationMode(EPWM1_BASE, EPWM_EMULATION_STOP_AFTER_FULL_CYCLE);
}


//
// End of File
//
