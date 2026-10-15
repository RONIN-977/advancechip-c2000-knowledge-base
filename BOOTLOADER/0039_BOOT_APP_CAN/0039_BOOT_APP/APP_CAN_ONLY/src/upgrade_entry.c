/**
 * @file upgrade_entry.c
 * @brief 应用内的在线升级入口：仅接受 ENTER_BOOT，确认后复位进入 Bootloader。
 * 应用自身不擦写 Flash。移植时复用业务程序的接收调度，避免重复初始化外设。
 * [移植必需] 本文件与 include/upgrade_entry.h 放入客户 APP，保持协议校验流程。
 * [客户适配] 客户已有 CAN ISR 时，将下面的 CAN 收帧/重组接入后台通信任务；
 * 不允许 ISR 与本轮询同时读取同一个 RX 对象，不在控制 ISR 中等待发送或复位。
 */
#include "driverlib.h"
#include "board_config.h"
#include "boot_io.h"
#include "upgrade_entry.h"
static bl_can_rx can;
static bl_context context;
static const bl_flash_ops no_flash = {0, 0, 0, 0, 0};
static bl_octet response[BL_MAX_PACKET];
static uint32_t can_last;

/* 应答发完后再设置 RAM 请求标记并复位，避免主机误判进入升级失败。 */
static void dispatch(uint16_t transport, const bl_octet *packet, uint16_t size)
{
    uint16_t count;
    if (packet[3] != BL_ENTER_BOOT)
    {
        return;
    }
    count = bl_handle(&context, packet, size, response);
    /* [K06 已改] 按原版：请求有效且 ACK 发完后，直接写标记并复位。 */
    if (count && response[14] == BL_OK && boot_send_reply(transport, response, count))
    {
        boot_request_reset();
    }
}

void upgrade_entry_init(void)
{
    /* [K08 已改] 初始化独立 Timer2 和协议状态。
     * 客户系统初始化和 boot_can_objects_init 必须已完成。
     * 不再调用 boot_timers_init/boot_ports_init，避免覆盖客户资源。
     */
    boot_timer2_init();
    bl_init(&context, &no_flash);
    bl_can_reset(&can);
}

/* 主循环频繁调用；接收不完整的数据超时后清除，不使用长时间阻塞延时。 */
void upgrade_entry_poll(void)
{
    uint16_t count, dlc;
    uint32_t id, now = boot_millis();
    CAN_MsgFrameType type;
    bl_octet frame[16]; /* Accommodate raw invalid DLC safely. */
    if (now - can_last > BOOT_PARTIAL_MS)
    {
        bl_can_reset(&can);
    }
    /* [K09 客户确认] RX 对象统一使用 BOOT_CAN_RX_OBJ。
     * 客户已有收帧队列时，使用队列保存的 ID/帧型/DLC/数据，
     * 不在后台读取被 ISR 复用的 IF2 寄存器来推断 DLC。
     */
    if (CAN_readMessageWithID(BOOT_CAN_BASE, BOOT_CAN_RX_OBJ, &type, &id, frame) && type == CAN_MSG_FRAME_STD &&
        id == BOOT_CAN_RX_ID)
    {
        can_last = boot_millis();
        dlc = HWREGH(BOOT_CAN_BASE + CAN_O_IF2MCTL) & CAN_IF2MCTL_DLC_M;
        count = bl_can_feed(&can, frame, dlc);
        if (count)
        {
            dispatch(2, can.data, count);
        }
    }
}
