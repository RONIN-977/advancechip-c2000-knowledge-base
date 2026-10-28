/**
 * @file main.c
 * @brief CAN 在线升级 APP；GPIO17 闪灯源码保留但已屏蔽。
 *
 * Bootloader 跳转到应用 codestart，C 运行库初始化后进入本函数。
 * LED 每 500 ms 翻转一次，即完整亮灭周期为 1 秒。
 */
#include "driverlib.h"
#include "device.h"
#include "board_config.h"
#include "boot_io.h"
#include "upgrade_entry.h"

/* 保留调试心跳：每 100 ms 加一，可在 CCS Expressions 中观察。 */
volatile uint32_t application_heartbeat = 0;

#if 0 /* KB：保留 LED 初始化源码，禁止配置和驱动 LED GPIO。 */
static void application_led_init(void)
{
    GPIO_setPinConfig(APP_LED_CONFIG);
    GPIO_setPadConfig(APP_LED_GPIO, GPIO_PIN_TYPE_STD);

    /* 先设置输出锁存值，再打开输出，避免使用未知的初始电平。
     * LED 高/低有效只影响初始亮灭，不影响翻转周期。
     */
    GPIO_writePin(APP_LED_GPIO, 0U);
    GPIO_setDirectionMode(APP_LED_GPIO, GPIO_DIR_MODE_OUT);
}

#endif

void main(void)
{
    uint32_t now;
    uint32_t last_heartbeat;
#if 0 /* KB：LED 已屏蔽，保留原变量声明。 */
    uint32_t last_led_toggle;
#endif

    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();

    /* 初始化毫秒时基及 CAN；LED 初始化保持屏蔽。 */
    upgrade_entry_init();
#if 0 /* KB：保留 LED 初始化调用，不执行。 */
    application_led_init();
#endif
    last_heartbeat = boot_millis();
#if 0
    last_led_toggle = last_heartbeat;
#endif

    for (;;)
    {
        /* 非阻塞轮询：不使用 500 ms 延时，以免阻塞升级请求接收。 */
        upgrade_entry_poll();
        now = boot_millis();

#if 0 /* KB：保留 LED 翻转代码，禁止闪灯。 */
        /* 无符号差值允许毫秒计数器自然回绕。 */
        if (now - last_led_toggle >= APP_LED_TOGGLE_MS)
        {
            last_led_toggle = now;
            GPIO_togglePin(APP_LED_GPIO);
        }

#endif

        if (now - last_heartbeat >= 100UL)
        {
            last_heartbeat = now;
            ++application_heartbeat;
        }
    }
}
