/* 仅处理 ENTER_BOOT；APP 不执行擦写、下载或镜像提交。
 * 客户只调用下面一个函数。接收重组/CRC复用 APP_KB 的原 boot_core.c。
 * 正常轮询每次最多处理一帧或提交一帧，应答等待不阻塞客户调度。
 */
#include "uC_Inc.h"
#include "boot_core.h"
#include "kb_can_upgrade.h"
#include "kb_can_upgrade_config.h"

volatile unsigned int kb_upgrade_status;

void KB_CAN_UpgradeTask(void)
{
    static bl_context context;
    static bl_can_rx rx;
    static const bl_flash_ops no_flash = {0, 0, 0, 0, 0};
    static bl_octet response[BL_MAX_PACKET];
    static uint32_t previous = 0xFFFFFFFFUL, milliseconds, remainder, rx_last, tx_start;
    static uint16_t tx_length, tx_offset, tx_index, transfer;
    uint32_t current, delta, id, objects;
    uint16_t count, dlc, n, i;
    bl_octet frame[16]; /* 硬件原始DLC可能超过8，先容纳再交协议拒绝。 */
    CAN_MsgFrameType type;
    bool was_disabled, received;

    if (kb_upgrade_status == 3U) return;
    if (kb_upgrade_status == 0U)
    {
        /* 不调用 CAN_initModule、不修改位时序、不修改客户已有消息对象。 */
        was_disabled = Interrupt_disableMaster();
        objects = CAN_getValidMessageObjects(CANA_BASE);
        if (objects & ((1UL << (KB_UPGRADE_RX_OBJ - 1U)) |
                       (1UL << (KB_UPGRADE_TX_OBJ - 1U))))
        {
            kb_upgrade_status = 3U;
            if (!was_disabled) Interrupt_enableMaster();
            return;
        }
        CAN_setupMessageObject(CANA_BASE, KB_UPGRADE_RX_OBJ, KB_UPGRADE_RX_ID,
            CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_RX, 0x7FFU,
            CAN_MSG_OBJ_USE_ID_FILTER | CAN_MSG_OBJ_USE_EXT_FILTER |
            CAN_MSG_OBJ_USE_DIR_FILTER, 8U);
        CAN_setupMessageObject(CANA_BASE, KB_UPGRADE_TX_OBJ, KB_UPGRADE_TX_ID,
            CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8U);
        SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
        CPUTimer_stopTimer(CPUTIMER2_BASE);
        CPUTimer_disableInterrupt(CPUTIMER2_BASE);
        CPUTimer_setPreScaler(CPUTIMER2_BASE, KB_APP_SYSCLK_HZ / 1000000UL - 1UL);
        CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFFUL);
        CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);
        CPUTimer_startTimer(CPUTIMER2_BASE);
        if (!was_disabled) Interrupt_enableMaster();
        bl_init(&context, &no_flash);
        bl_can_reset(&rx);
        kb_upgrade_status = 1U;
        return;
    }

    /* Timer2独占、1MHz自由计数；无符号差值支持计数器自然回绕。 */
    current = CPUTimer_getTimerCount(CPUTIMER2_BASE);
    delta = previous - current;
    previous = current;
    milliseconds += delta / 1000UL;
    remainder += delta % 1000UL;
    if (remainder >= 1000UL) { ++milliseconds; remainder -= 1000UL; }

    if (tx_length)
    {
        if (milliseconds - tx_start > KB_UPGRADE_TX_TIMEOUT_MS)
        {
            /* 无ACK或断线：不设置RAM标记、不复位，继续客户后台与喂狗。 */
            tx_length = tx_offset = 0;
            kb_upgrade_status = 1U;
            bl_can_reset(&rx);
            return;
        }
        if (CAN_getTxRequests(CANA_BASE) & (1UL << (KB_UPGRADE_TX_OBJ - 1U))) return;
        if (tx_offset == tx_length)
        {
            /* 最后一帧应答已发送完成，先由客户完成设备停机。
             * 此处尚未额外关全局中断；客户函数必须在停机完成后返回。
             */
            Customer_ShutdownForUpgrade();
            Interrupt_disableMaster();
            ((volatile uint16_t *)KB_UPGRADE_REQUEST_ADDR)[0] = 0xB007U;
            ((volatile uint16_t *)KB_UPGRADE_REQUEST_ADDR)[1] = 0x4FF8U;
            ((volatile uint16_t *)KB_UPGRADE_REQUEST_ADDR)[2] = 0x5AA5U;
            ((volatile uint16_t *)KB_UPGRADE_REQUEST_ADDR)[3] = 0xA55AU;
            for (i = 0; i < 20U; ++i)
            {
                SysCtl_serviceWatchdog();
                SysCtl_delay(KB_APP_SYSCLK_HZ / 3000UL); /* 约1ms，累计20ms */
            }
            SysCtl_resetDevice();
            for (;;) { }
        }
        n = tx_length - tx_offset;
        if (n > 6U) n = 6U;
        frame[0] = transfer;
        frame[1] = tx_index | (tx_index ? 0U : 0x80U);
        for (i = 0; i < n; ++i) frame[i + 2U] = response[tx_offset + i];
        /* IF1保护只覆盖寄存器装载，不覆盖总线发送等待。保留调用前INTM状态。 */
        was_disabled = Interrupt_disableMaster();
        CAN_setupMessageObject(CANA_BASE, KB_UPGRADE_TX_OBJ, KB_UPGRADE_TX_ID,
            CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, n + 2U);
        CAN_sendMessage(CANA_BASE, KB_UPGRADE_TX_OBJ, n + 2U, frame);
        if (!was_disabled) Interrupt_enableMaster();
        tx_offset += n;
        ++tx_index;
        return;
    }

    if (milliseconds - rx_last > KB_UPGRADE_PARTIAL_MS) bl_can_reset(&rx);
    /* IF2读帧及DLC必须处于同一临界区，防客户ISR中途重用IF2。 */
    was_disabled = Interrupt_disableMaster();
    received = CAN_readMessageWithID(CANA_BASE, KB_UPGRADE_RX_OBJ, &type, &id, frame);
    dlc = HWREGH(CANA_BASE + CAN_O_IF2MCTL) & CAN_IF2MCTL_DLC_M;
    if (!was_disabled) Interrupt_enableMaster();
    if (!received || type != CAN_MSG_FRAME_STD || id != KB_UPGRADE_RX_ID) return;
    rx_last = milliseconds;
    count = bl_can_feed(&rx, frame, dlc);
    if (!count || rx.data[3] != BL_ENTER_BOOT) return;
    count = bl_handle(&context, rx.data, count, response);
    if (!count || response[14] != BL_OK) return;
    tx_length = count;
    tx_offset = tx_index = 0;
    transfer = (transfer + 1U) & 255U;
    tx_start = milliseconds;
    kb_upgrade_status = 2U;
}
