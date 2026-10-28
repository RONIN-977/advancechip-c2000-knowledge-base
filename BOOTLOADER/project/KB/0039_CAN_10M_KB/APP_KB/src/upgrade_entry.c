/**
 * @file upgrade_entry.c
 * @brief 应用内的在线升级入口：仅接受 ENTER_BOOT，确认后复位进入 Bootloader。
 * 应用自身不擦写 Flash。移植时复用业务程序的接收调度，避免重复初始化外设。
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
    if (count && response[14] == BL_OK && boot_send_reply(transport, response, count))
    {
        boot_request_reset();
    }
}

void upgrade_entry_init(void)
{
    boot_timers_init();
    boot_ports_init();
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
    if (CAN_readMessageWithID(BOOT_CAN_BASE, 1, &type, &id, frame) && type == CAN_MSG_FRAME_STD &&
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
