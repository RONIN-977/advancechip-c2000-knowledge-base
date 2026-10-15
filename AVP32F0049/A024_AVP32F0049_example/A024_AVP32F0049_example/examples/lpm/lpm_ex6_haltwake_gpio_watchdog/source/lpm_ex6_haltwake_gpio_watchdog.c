/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  lpm_ex6_haltwake_gpio_watchdog.c
*                         TITLE: Halt entry and Exit example.
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Function Prototypes
//
void enterLPM();
__interrupt void wakeISR(void);

//
// To avoid flash accesses, the function enterLPM and the ISR wakeISR should
// be run from RAM.
// Note that the function SysCtl_enterHaltMode is defined inline. But this
// example is built with opt_level=off which does not inline functions. To avoid
// flash accesses, include SysCtl_enterHaltMode in .TI.ramfunc section (This is
// not required if optimization level 0 or higher is used)
//
#pragma CODE_SECTION(enterLPM, ".TI.ramfunc");
#pragma CODE_SECTION(wakeISR, ".TI.ramfunc");
#pragma CODE_SECTION(SysCtl_enterHaltMode, ".TI.ramfunc");

//
// Main
//
void main(void)
{
    //
    // When the example is used with the watchdog Timer, the watchdog
    // timer will reset the device when it is LPM. If the example is run
    // from flash and the device is in a boot to flash configuration,
    // then it will restart the application and will enter the condition
    // below and ESTOP0 if the debugger is connected.
    //
    // Check whether this was a normal startup or a watchdog reset.
    //
    if( SysCtl_getResetCause() == SYSCTL_CAUSE_WDRS)
    {
        SysCtl_clearResetCause(SYSCTL_CAUSE_WDRS);
        ESTOP0;
    }

    //
    // Configure PLL, disable WD, enable peripheral clocks.
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // GPIO0 is the external wake-up source
    //
    GPIO_setPinConfig(GPIO_0_GPIO0);
    GPIO_setDirectionMode(0, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(0, GPIO_QUAL_ASYNC);

    //
    // GPIO1 is an output
    //
    GPIO_setPinConfig(GPIO_1_GPIO1);
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);

    //
    // LED pin configuration
    //
    GPIO_setPinConfig(DEVICE_GPIO_CFG_LED1);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);

    //
    // Use GPIO0 to wake the CPU from halt.
    //
    SysCtl_enableLPMWakeupPin(0);

    //
    // Disable global interrupts.
    //
    DINT;

    //
    // Initialize interrupt controller and vector table.
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Map the ISR to the wake interrupt.
    //
    Interrupt_register(INT_WAKE, wakeISR);

    //
    // Enable the wake interrupt in the PIE: Group 1 interrupt 8.
    //
    Interrupt_enable(INT_WAKE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

    //
    // Enable global interrupts.
    //
    EINT;

    //
    // Set GPIO1 high.
    //
    GPIO_writePin(1, 1);

    //
    // Enable watchdog in halt.
    //
    SysCtl_setWatchdogMode(SYSCTL_WD_MODE_INTERRUPT);
    SysCtl_enableWatchdogInHalt();
    SysCtl_serviceWatchdog();
    SysCtl_enableWatchdog();

    //
    // Power down the flash and enter LPM
    //
    enterLPM();

    while(1)
    {
        //
        // Toggle LED1
        //
        GPIO_togglePin(DEVICE_GPIO_PIN_LED1);

        //
        // 500ms delay
        //
        DEVICE_DELAY_US(500000);
    }
}

//
// wakeISR() - this will be triggered from a watchdog timeout
//             or when GPIO1 is set low.
//
__interrupt void
wakeISR(void)
{
    //
    // Wake the flash from low power mode.
    //
    Flash_wakeFromLPM(FLASH0CTRL_BASE);

    //
    // Reconfigure PLL after waking from halt.
    //
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);

    //
    // Set GPIO1 low.
    //
    GPIO_writePin(1, 0);

    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// enterLPM() - This will power down the flash and enter LPM.
//
void enterLPM()
{
    //
    // Power down the flash bank and pump. Ensure there are no subsequent
    // flash accesses.
    //
    Flash_powerDown(FLASH0CTRL_BASE);

    //
    // Enter halt mode.
    //
    SysCtl_enterHaltMode();
}

//
// End of File
//

