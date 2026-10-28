/**
 * @file boot_io.c
 * @brief Bootloader/示例应用共享的引脚、时基、应答发送及升级请求标记。
 * 修改板级参数请编辑 board_config.h；本文件不使用 device.h 的默认引脚宏。
 */
#include "driverlib.h"
#include "device.h"
#include "board_config.h"
#include "boot_io.h"
static uint16_t can_transfer;

/* Timer0 为 1 MHz 向下计数器。累计差值和余数，避免频繁调用丢失微秒。
 * 需在主循环持续调用；单次间隔必须小于硬件计数器完整回绕周期。
 */
uint32_t boot_millis(void)
{
    static uint32_t previous = 0xFFFFFFFFUL, remainder, time;
    uint32_t current = CPUTimer_getTimerCount(CPUTIMER0_BASE);
    uint32_t delta = previous - current;
    previous = current;
    time += delta / 1000UL;
    remainder += delta % 1000UL;
    if (remainder >= 1000UL)
    {
        ++time;
        remainder -= 1000UL;
    }
    return time;
}

/* 仅监听 CAN；CAN RX 使用异步采样，报文对象 1=RX、2=TX。 */
void boot_ports_init(void)
{
    GPIO_setPinConfig(BOOT_CAN_RX_CONFIG);
    GPIO_setPinConfig(BOOT_CAN_TX_CONFIG);
    GPIO_setQualificationMode(BOOT_CAN_RX_GPIO, GPIO_QUAL_ASYNC);
    CAN_initModule(BOOT_CAN_BASE);
    CAN_setBitRate(BOOT_CAN_BASE, DEVICE_SYSCLK_FREQ, BOOT_CAN_BAUD,
                   BOOT_CAN_BAUD == 800000UL ? 15U : 20U);
    CAN_setupMessageObject(
        BOOT_CAN_BASE, 1, BOOT_CAN_RX_ID, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_RX, 0x7FF,
        CAN_MSG_OBJ_USE_ID_FILTER | CAN_MSG_OBJ_USE_EXT_FILTER | CAN_MSG_OBJ_USE_DIR_FILTER, 8);
    CAN_setupMessageObject(BOOT_CAN_BASE, 2, BOOT_CAN_TX_ID, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX,
                           0, CAN_MSG_OBJ_NO_FLAGS, 8);
    CAN_startModule(BOOT_CAN_BASE);
    if (BOOT_FORCE_GPIO != 0xFFFFU)
    {
        GPIO_setDirectionMode(BOOT_FORCE_GPIO, GPIO_DIR_MODE_IN);
        GPIO_setPadConfig(BOOT_FORCE_GPIO, GPIO_PIN_TYPE_PULLUP);
    }
}

static uint16_t can_wait_tx(void)
{
    uint32_t start = boot_millis();
    while (CAN_getTxRequests(BOOT_CAN_BASE) & 2UL)
    {
        if (boot_millis() - start > 500UL)
        {
            return 0;
        }
    }
    return 1;
}

/* transport: 2=CAN；返回非零表示整包发送完成。
 * 所有发送等待均有超时，CAN 分片逐帧等待，避免覆盖尚未发送的邮箱。
 */
uint16_t boot_send_reply(uint16_t transport, const bl_octet *data, uint16_t count)
{
    uint16_t offset, i, n, index = 0;
    bl_octet frame[8];
    if (transport != 2U)
    {
        return 0;
    }
    can_transfer = (can_transfer + 1U) & 255U;
    for (offset = 0; offset < count; offset += n)
    {
        n = (count - offset > 6U) ? 6U : count - offset;
        frame[0] = can_transfer;
        frame[1] = index | (index ? 0U : 0x80U);
        ++index;
        for (i = 0; i < n; ++i)
        {
            frame[i + 2] = data[offset + i];
        }
        if (!can_wait_tx())
        {
            return 0;
        }
        CAN_sendMessage_updateDLC(BOOT_CAN_BASE, 2, n + 2U, frame);
        if (!can_wait_tx())
        {
            return 0;
        }
    }
    return 1;
}

/* Timer0 提供毫秒时基；Timer2 全速运行，供 RAM 中的 Flash 超时保护使用。 */
void boot_timers_init(void)
{
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_setPreScaler(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ / 1000000UL - 1UL);
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFFUL);
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER2_BASE);
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);
    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFFUL);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);
}

/* 读取后清除 RAM 标记，避免普通复位时反复停留在升级模式。 */
uint16_t boot_take_request(void)
{
    volatile uint16_t *cookie = (volatile uint16_t *)BOOT_REQUEST_ADDR;
    uint16_t valid = cookie[0] == 0xB007U && cookie[1] == 0x4FF8U && cookie[2] == 0x5AA5U &&
                     cookie[3] == 0xA55AU;
    cookie[0] = cookie[1] = cookie[2] = cookie[3] = 0;
    return valid;
}

/* 写入预留 M0 RAM，再通过 watchdog reset 返回 Bootloader。
 * 业务应用应在调用前完成控制输出停机；链接文件必须保留此 RAM 区域。
 */
void boot_request_reset(void)
{
    volatile uint16_t *cookie = (volatile uint16_t *)BOOT_REQUEST_ADDR;
    DINT;
    cookie[0] = 0xB007U;
    cookie[1] = 0x4FF8U;
    cookie[2] = 0x5AA5U;
    cookie[3] = 0xA55AU;
    DEVICE_DELAY_US(20000);
    SysCtl_resetDevice();
}
