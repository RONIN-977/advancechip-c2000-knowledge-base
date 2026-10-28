/**
 * @file boot_io.c
 * @brief CAN 专用 APP 的引脚、时基、应答发送及升级请求标记。
 * [移植必需] 与 boot_io.h、board_config.h 一起移入客户 APP。
 * [客户适配] 引脚/邮箱/时基应接到客户驱动；复用客户 CAN，升级计时单独使用 Timer2。
 * 修改板级参数请编辑 board_config.h；本文件不使用 device.h 的默认引脚宏。
 */
#include "driverlib.h"
#include "device.h"
#include "board_config.h"
#include "boot_io.h"
static uint16_t can_transfer;

/* [K05 已改] 独占 CPU Timer2，配置为 1MHz 向下计数，不使用中断。
 * 客户确认 Timer2 未被其他功能使用；DEVICE_SYSCLK_FREQ 必须等于真实 SYSCLK。
 * 客户系统初始化后调用一次，后续不可停止、重载或重新配置 Timer2。
 */
void boot_timer2_init(void)
{
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    CPUTimer_stopTimer(CPUTIMER2_BASE);
    CPUTimer_disableInterrupt(CPUTIMER2_BASE);
    CPUTimer_selectClockSource(CPUTIMER2_BASE, CPUTIMER_CLOCK_SOURCE_SYS,
                              CPUTIMER_CLOCK_PRESCALER_1);
    CPUTimer_setPreScaler(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ / 1000000UL - 1UL);
    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFFUL);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);
}

/* Timer2 每 1us 减一；保留原版差值与余数累计，换算为毫秒。
 * 仅在后台调用；两次读取间隔应小于一次硬件回绕周期（约 71.6 分钟）。
 */
uint32_t boot_millis(void)
{
    static uint32_t previous = 0xFFFFFFFFUL, remainder, time;
    uint32_t current = CPUTimer_getTimerCount(CPUTIMER2_BASE);
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

/* [K07 已改] 客户 CAN 初始化完成后，仅新增升级 RX/TX 消息对象。
 * 不复位 CAN 模块，不改引脚/波特率，不重新启动 CAN。
 * 必须位于 CAN_Config_Init() 后；对象号与业务接收调度须先确认。
 * 若客户改成统一接收队列，应同步改 upgrade_entry_poll 的读取路径。
 */
void boot_can_objects_init(void)
{
    CAN_setupMessageObject(
        BOOT_CAN_BASE, BOOT_CAN_RX_OBJ, BOOT_CAN_RX_ID,
        CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_RX, 0x7FF,
        CAN_MSG_OBJ_USE_ID_FILTER | CAN_MSG_OBJ_USE_EXT_FILTER |
        CAN_MSG_OBJ_USE_DIR_FILTER, 8);
    CAN_setupMessageObject(
        BOOT_CAN_BASE, BOOT_CAN_TX_OBJ, BOOT_CAN_TX_ID,
        CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0,
        CAN_MSG_OBJ_NO_FLAGS, 8);
}

static uint16_t can_wait_tx(void)
{
    uint32_t start = boot_millis();
    /* [K02 已改] TX 完成掩码随消息对象宏同步。 */
    while (CAN_getTxRequests(BOOT_CAN_BASE) & (1UL << (BOOT_CAN_TX_OBJ - 1U)))
    {
        if (boot_millis() - start > 500UL)
        {
            return 0;
        }
    }
    return 1;
}

/* [移植必需] transport 仅接受 2=CAN；返回非零表示整包发送完成。
 * [客户适配] 复用客户 TX 队列时，应等待最后一片真正发送完成后返回成功，
 * 不能把入队成功当作应答发完；发送中断必须能继续运行。
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
        CAN_sendMessage_updateDLC(BOOT_CAN_BASE, BOOT_CAN_TX_OBJ, n + 2U, frame);
        if (!can_wait_tx())
        {
            return 0;
        }
    }
    return 1;
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

/* [移植必需] 写入预留 M0 RAM，再通过 watchdog reset 返回 Bootloader。
 * 客户 .cmd 必须排除 0x3F8..0x3FB，启动/自检不得提前覆盖这 4 个字。
 * 不改 cookie 值，不直接跳 BOOT/main；发完 ACK 后调用本函数。
 * [K06] 使用原版复位流程，没有客户安全停机回调。
 */
void boot_request_reset(void)
{
    volatile uint16_t *cookie = (volatile uint16_t *)BOOT_REQUEST_ADDR;
    DINT;
    cookie[0] = 0xB007U;
    cookie[1] = 0x4FF8U;
    cookie[2] = 0x5AA5U;
    cookie[3] = 0xA55AU;
    /* [K03] 此延时依赖 device.h 的真实 CPU 时钟常量。 */
    DEVICE_DELAY_US(20000);
    SysCtl_resetDevice();
}
