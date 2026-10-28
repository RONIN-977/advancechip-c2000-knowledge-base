/**
 * @file main.c
 * @brief 升级示例应用：GPIO17 闪灯，同时保持 CAN/SCI 在线升级入口可响应。
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

void main(void)
{
    uint32_t now;
    uint32_t last_heartbeat;
    uint32_t last_led_toggle;

    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();

    /* 初始化共享毫秒时基及 CAN/SCI，再配置应用专用 LED 引脚。 */
    upgrade_entry_init();
    application_led_init();
    last_heartbeat = boot_millis();
    last_led_toggle = last_heartbeat;

    for (;;)
    {
        /* 非阻塞轮询：不使用 500 ms 延时，以免阻塞升级请求接收。 */
        upgrade_entry_poll();
        now = boot_millis();

        /* 无符号差值允许毫秒计数器自然回绕。 */
        if (now - last_led_toggle >= APP_LED_TOGGLE_MS)
        {
            last_led_toggle = now;
            GPIO_togglePin(APP_LED_GPIO);
        }

        if (now - last_heartbeat >= 100UL)
        {
            last_heartbeat = now;
            ++application_heartbeat;
        }
    }
}
