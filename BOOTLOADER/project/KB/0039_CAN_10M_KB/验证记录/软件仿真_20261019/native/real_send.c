#include "driverlib.h"
#include "board_config.h"
#include "boot_io.h"
static uint16_t can_transfer;
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

