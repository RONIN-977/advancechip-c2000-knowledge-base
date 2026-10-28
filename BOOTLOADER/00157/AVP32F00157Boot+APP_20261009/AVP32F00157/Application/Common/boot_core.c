/**
 * @file boot_core.c
 * @brief 与外设无关的升级协议：帧校验、会话状态机及 SCI/CAN 重组。
 * 线上的字节使用 bl_octet 保存；Flash 地址/镜像长度以 16 位字为单位。
 */
#include "boot_core.h"

/* 显式按小端装拆包，不把 PC 结构体直接映射到 C28x 内存。 */
static uint16_t get16(const bl_octet *p)
{
    return (p[0] & 255U) | ((p[1] & 255U) << 8);
}

static uint32_t get32(const bl_octet *p)
{
    return (uint32_t)get16(p) | ((uint32_t)get16(p + 2) << 16);
}

static void put16(bl_octet *p, uint16_t v)
{
    p[0] = v & 255U;
    p[1] = (v >> 8) & 255U;
}

static void put32(bl_octet *p, uint32_t v)
{
    put16(p, (uint16_t)v);
    put16(p + 2, (uint16_t)(v >> 16));
}

static void copy(bl_octet *dst, const bl_octet *src, uint16_t n)
{
    uint16_t i;
    for (i = 0; i < n; ++i)
    {
        dst[i] = src[i];
    }
}

static uint16_t equal(const bl_octet *a, const bl_octet *b, uint16_t n)
{
    uint16_t i;
    for (i = 0; i < n; ++i)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
    }
    return 1;
}

/* CRC-32/ISO-HDLC：初值、多项式及最终取反与上位机一致。 */
uint32_t bl_crc32(const bl_octet *data, uint16_t length)
{
    uint32_t crc = 0xFFFFFFFFUL;
    uint16_t i, j;
    for (i = 0; i < length; ++i)
    {
        crc ^= data[i] & 255U;
        for (j = 0; j < 8; ++j)
        {
            crc = (crc >> 1) ^ ((crc & 1UL) ? 0xEDB88320UL : 0UL);
        }
    }
    return ~crc;
}

/* 先检查长度，再访问字段；错误帧静默丢弃，交由上位机超时重试。 */
static uint16_t valid_packet(const bl_octet *p, uint16_t n)
{
    uint16_t i, payload;
    if (n < 18 || n > BL_MAX_PACKET || p[0] != 0xA5 || p[1] != 0x5A || p[2] != 1)
    {
        return 0;
    }
    for (i = 0; i < n; ++i)
    {
        if (p[i] > 255U)
        {
            return 0;
        }
    }
    payload = get16(p + 12);
    return payload <= BL_MAX_PAYLOAD && n == payload + 18U &&
           bl_crc32(p + 2, n - 6U) == get32(p + n - 4U);
}

void bl_init(bl_context *ctx, const bl_flash_ops *ops)
{
    ctx->flash = *ops;
    ctx->session = ctx->sequence = 0;
    ctx->next = BL_APP_START;
    ctx->state = BL_IDLE;
    ctx->cached_len = ctx->reply_len = 0;
    ctx->image.start = ctx->image.count = ctx->image.entry = ctx->image.crc = ctx->image.version =
        0;
}

/* 应答保留请求的会话/序号；INFO 额外返回身份和应用分区范围。 */
static uint16_t reply(bl_context *ctx, const bl_octet *in, bl_octet *out, uint16_t status,
                      uint16_t info)
{
    uint16_t size = info ? 24U : 6U;
    copy(out, in, 14);
    out[3] |= 0x80U;
    put16(out + 12, size);
    out[14] = status;
    out[15] = ctx->state;
    put32(out + 16, ctx->next);
    if (info)
    {
        put32(out + 20, BL_DEVICE_ID);
        put32(out + 24, BL_BOARD_ID);
        put16(out + 28, BL_CPU);
        put32(out + 30, BL_APP_START);
        put32(out + 34, BL_APP_END);
    }
    put32(out + 14 + size, bl_crc32(out + 2, size + 12U));
    return size + 18U;
}

uint16_t bl_handle(bl_context *ctx, const bl_octet *in, uint16_t length, bl_octet *out)
{
    uint16_t cmd, payload, i, count, status = BL_OK, accepted = 0, outlen;
    uint16_t words[60];
    uint32_t sid, seq;
    const bl_octet *p;
    bl_manifest candidate;
    if (!valid_packet(in, length))
    {
        return 0;
    }
    /* ACK 丢失后的原包重试直接返回缓存，禁止重复擦除或重复编程。 */
    if (length == ctx->cached_len && equal(in, ctx->cached, length))
    {
        copy(out, ctx->reply, ctx->reply_len);
        return ctx->reply_len;
    }
    cmd = in[3];
    sid = get32(in + 4);
    seq = get32(in + 8);
    payload = get16(in + 12);
    p = in + 14;
    if (cmd == BL_INFO)
    {
        return reply(ctx, in, out, (payload || sid || seq) ? BL_BAD_LENGTH : BL_OK,
                     !(payload || sid || seq));
    }
    if (cmd == BL_RESET)
    {
        return reply(ctx, in, out, (payload || ctx->state != BL_VALID) ? BL_BAD_STATE : BL_OK, 0);
    }
    if (cmd == BL_ENTER_BOOT)
    {
        return reply(ctx, in, out, (payload || sid || seq) ? BL_BAD_LENGTH : BL_OK, 0);
    }
    /* BEGIN：完整验证镜像清单后，才使旧镜像失效并擦除应用分区。 */
    if (cmd == BL_BEGIN)
    {
        if (payload != 32)
        {
            status = BL_BAD_LENGTH;
        }
        else if (ctx->state == BL_RECEIVING)
        {
            status = BL_BUSY;
        }
        else if (!sid || sid == ctx->session || seq != 1)
        {
            status = BL_BAD_SEQUENCE;
        }
        else
        {
            candidate.start = get32(p + 12);
            candidate.count = get32(p + 16);
            candidate.entry = get32(p + 20);
            candidate.crc = get32(p + 24);
            candidate.version = get32(p + 28);
            if (get32(p) != BL_DEVICE_ID || get32(p + 4) != BL_BOARD_ID || get32(p + 8) != BL_CPU ||
                candidate.start != BL_APP_START || candidate.entry != BL_APP_START ||
                !candidate.count || (candidate.count & 3UL) ||
                candidate.count > BL_APP_END - BL_APP_START)
            {
                status = BL_BAD_IMAGE;
            }
            else
            {
                ctx->session = sid;
                ctx->sequence = seq;
                ctx->image = candidate;
                ctx->next = BL_APP_START;
                ctx->state = BL_FAILED;
                accepted = 1;
                if (!ctx->flash.begin || !ctx->flash.program || !ctx->flash.verify ||
                    !ctx->flash.commit || !ctx->flash.begin(ctx->flash.user, &ctx->image))
                {
                    status = BL_FLASH_ERROR;
                }
                else
                {
                    ctx->state = BL_RECEIVING;
                }
            }
        }
    }
    else if (cmd == BL_DATA || cmd == BL_FINISH || cmd == BL_ABORT)
    {
        if (ctx->state != BL_RECEIVING || sid != ctx->session)
        {
            status = BL_BAD_STATE;
        }
        /* ABORT 可跳过应答不确定的序号；DATA/FINISH 必须严格连续。 */
        else if ((cmd == BL_ABORT && seq <= ctx->sequence) ||
                 (cmd != BL_ABORT && seq != ctx->sequence + 1UL))
        {
            status = BL_BAD_SEQUENCE;
        }
        else if (cmd == BL_DATA)
        {
            if (payload < 12 || ((payload - 4U) % 8U))
            {
                status = BL_BAD_LENGTH;
            }
            else
            {
                /* 前 4 字节为目标字地址；按现有上位机协议保留 4 字对齐。 */
                count = (payload - 4U) / 2U;
                if (get32(p) != ctx->next ||
                    count > ctx->image.start + ctx->image.count - ctx->next)
                {
                    status = BL_BAD_IMAGE;
                }
                else
                {
                    for (i = 0; i < count; ++i)
                    {
                        words[i] = get16(p + 4 + 2 * i);
                    }
                    accepted = 1;
                    if (!ctx->flash.program(ctx->flash.user, ctx->next, words, count))
                    {
                        ctx->state = BL_FAILED;
                        status = BL_FLASH_ERROR;
                    }
                    else
                    {
                        ctx->next += count;
                    }
                }
            }
        }
        else if (payload)
        {
            status = BL_BAD_LENGTH;
        }
        else if (cmd == BL_ABORT)
        {
            ctx->state = BL_IDLE;
            accepted = 1;
        }
        else if (ctx->next != ctx->image.start + ctx->image.count)
        {
            status = BL_BAD_STATE;
        }
        else
        {
            /* FINISH：读回校验成功后才提交有效标记，失败时不可启动。 */
            accepted = 1;
            ctx->state = BL_FAILED;
            if (!ctx->flash.verify(ctx->flash.user, &ctx->image))
            {
                status = BL_VERIFY_ERROR;
            }
            else if (!ctx->flash.commit(ctx->flash.user, &ctx->image))
            {
                status = BL_FLASH_ERROR;
            }
            else
            {
                ctx->state = BL_VALID;
            }
        }
    }
    else
    {
        status = BL_BAD_COMMAND;
    }
    outlen = reply(ctx, in, out, status, 0);
    if (accepted)
    {
        ctx->sequence = seq;
        ctx->cached_len = length;
        ctx->reply_len = outlen;
        copy(ctx->cached, in, length);
        copy(ctx->reply, out, outlen);
    }
    return outlen;
}

void bl_sci_reset(bl_sci_rx *rx)
{
    rx->length = 0;
}

static void drop_first(bl_sci_rx *rx)
{
    uint16_t i;
    --rx->length;
    for (i = 0; i < rx->length; ++i)
    {
        rx->data[i] = rx->data[i + 1];
    }
}

/* SCI 按字节输入；遇到噪声或坏帧时逐字节重新寻找 A5 5A 帧头。 */
uint16_t bl_sci_feed(bl_sci_rx *rx, bl_octet value)
{
    uint16_t payload, total;
    if (value > 255U)
    {
        bl_sci_reset(rx);
        return 0;
    }
    if (rx->length >= BL_MAX_PACKET)
    {
        bl_sci_reset(rx);
    }
    rx->data[rx->length++] = value;
    while (rx->length >= 2)
    {
        if (rx->data[0] != 0xA5 || rx->data[1] != 0x5A)
        {
            drop_first(rx);
            continue;
        }
        if (rx->length < 14)
        {
            return 0;
        }
        payload = get16(rx->data + 12);
        if (rx->data[2] != 1 || payload > BL_MAX_PAYLOAD)
        {
            drop_first(rx);
            continue;
        }
        total = payload + 18U;
        if (rx->length < total)
        {
            return 0;
        }
        if (!valid_packet(rx->data, total))
        {
            drop_first(rx);
            continue;
        }
        /* No trailing bytes: feed consumes one octet at a time. */
        rx->length = 0;
        return total;
    }
    return 0;
}

void bl_can_reset(bl_can_rx *rx)
{
    rx->length = rx->active = rx->transfer = rx->next = 0;
}

/* CAN 每帧前 2 字节为传输号/分片索引，最多携带 6 字节协议数据。
 * 缺片、乱序或溢出会丢弃本次重组；只返回完整且 CRC 正确的协议包。
 */
uint16_t bl_can_feed(bl_can_rx *rx, const bl_octet *frame, uint16_t dlc)
{
    uint16_t i, index, payload, total;
    if (dlc < 3 || dlc > 8)
    {
        bl_can_reset(rx);
        return 0;
    }
    for (i = 0; i < dlc; ++i)
    {
        if (frame[i] > 255U)
        {
            bl_can_reset(rx);
            return 0;
        }
    }
    index = frame[1] & 0x7FU;
    if (frame[1] & 0x80U)
    {
        bl_can_reset(rx);
        if (index)
        {
            return 0;
        }
        rx->active = 1;
        rx->transfer = frame[0];
    }
    if (!rx->active || frame[0] != rx->transfer || index != rx->next ||
        rx->length + dlc - 2U > BL_MAX_PACKET)
    {
        bl_can_reset(rx);
        return 0;
    }
    ++rx->next;
    for (i = 2; i < dlc; ++i)
    {
        rx->data[rx->length++] = frame[i];
    }
    if (rx->length < 14)
    {
        return 0;
    }
    payload = get16(rx->data + 12);
    if (payload > BL_MAX_PAYLOAD)
    {
        bl_can_reset(rx);
        return 0;
    }
    total = payload + 18U;
    if (rx->length < total)
    {
        return 0;
    }
    i = rx->length == total && valid_packet(rx->data, total);
    bl_can_reset(rx);
    return i ? total : 0;
}
