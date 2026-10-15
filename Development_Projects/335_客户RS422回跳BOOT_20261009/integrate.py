from pathlib import Path

root = Path(__file__).parent
src = (root / 'original/rs422.c').read_bytes().decode('gbk')
hdr = (root / 'original/rs422.h').read_bytes().decode('gbk')
added = r'''
/* Added SCI-A BOOT monitor. Customer baud/8E1/FIFO/GPIO stay unchanged.
 * V201 0x72 request, 11 bytes. No reply: business TX ownership is unchanged.
 */
#define RS422_BOOT_ENTRY_ADDRESS 0x33FFF6UL
#define RS422_BOOT_REQUEST_SIZE  11U
static volatile Uint16 rs422_boot_pending = 0U;
static Uint16 rs422_boot_window[RS422_BOOT_REQUEST_SIZE];
static Uint16 rs422_boot_count = 0U;

static Uint16 RS422_BootCRC8(const Uint16 *data, Uint16 size)
{
    Uint16 crc = 0U, i;
    while (size--)
    {
        crc ^= *data++ & 0x00FFU;
        for (i = 0U; i < 8U; i++)
            crc = (crc >> 1) ^ ((crc & 1U) ? 0x008CU : 0U);
    }
    return crc;
}

static Uint16 RS422_BootCRC16(const Uint16 *data, Uint16 size)
{
    Uint16 crc = 0U, i;
    while (size--)
    {
        crc ^= *data++ & 0x00FFU;
        for (i = 0U; i < 8U; i++)
            crc = (crc >> 1) ^ ((crc & 1U) ? 0xA001U : 0U);
    }
    return crc;
}

/* ISR-only fixed sliding window; business bytes are not consumed.
 * Reserve the complete request: it must not occur inside business payloads.
 */
static void RS422_BootFeed(Uint16 raw)
{
    Uint16 i, check;
    if (rs422_boot_pending)
        return;
    if ((raw & 0xC000U) || SciaRegs.SCIFFRX.bit.RXFFOVF ||
        SciaRegs.SCIRXST.bit.RXERROR)
    {
        rs422_boot_count = 0U;
        return;
    }
    if (rs422_boot_count == RS422_BOOT_REQUEST_SIZE)
    {
        for (i = 1U; i < RS422_BOOT_REQUEST_SIZE; i++)
            rs422_boot_window[i - 1U] = rs422_boot_window[i];
        rs422_boot_count--;
    }
    rs422_boot_window[rs422_boot_count++] = raw & 0x00FFU;
    if (rs422_boot_count != RS422_BOOT_REQUEST_SIZE)
        return;
    if (rs422_boot_window[0] != 0xCCU ||
        rs422_boot_window[1] != 0x58U ||
        rs422_boot_window[2] != 0U ||
        rs422_boot_window[3] != RS422_BootCRC8(rs422_boot_window, 3U) ||
        (rs422_boot_window[4] & 0x80U) ||
        rs422_boot_window[5] != 0x72U ||
        rs422_boot_window[6] != 0U)
        return;
    check = rs422_boot_window[9] | (rs422_boot_window[10] << 8);
    if (check == RS422_BootCRC16(rs422_boot_window, 9U))
        rs422_boot_pending = 1U;
}

Uint16 RS422_BootPending(void)
{
    return rs422_boot_pending;
}

/* Main/task ONLY, after safe shutdown and stopping new TX jobs.
 * Poll until both channels finish TX. No blocking wait in RX interrupt.
 * Direct jump, not reset. Existing BOOT's listening window still applies.
 */
void RS422_BootTask(void)
{
    if (!rs422_boot_pending ||
        SciaRegs.SCIFFTX.bit.TXFFST || !SciaRegs.SCICTL2.bit.TXEMPTY ||
        ScibRegs.SCIFFTX.bit.TXFFST || !ScibRegs.SCICTL2.bit.TXEMPTY)
        return;
    DINT;
    IER = 0U;
    IFR = 0U;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0U;
    SciaRegs.SCICTL1.bit.SWRESET = 0U;
    ScibRegs.SCICTL1.bit.SWRESET = 0U;
    EALLOW;
    SysCtrlRegs.WDCR = 0x0068U;
    EDIS;
    ((void (*)(void))RS422_BOOT_ENTRY_ADDRESS)();
    for (;;) {}
}
'''
anchor = 'StSCI g_StSCI = {0};'
assert src.count(anchor) == 1
src = src.replace(anchor, anchor + '\r\n' + added.replace('\n', '\r\n'))
anchor = '    Uint16 scibaud=0U;\r\n'
assert src.count(anchor) == 2
src = src.replace(anchor, anchor + '\r\n    /* Reset added monitor only; preserve customer SCI settings. */\r\n    rs422_boot_pending = 0U;\r\n    rs422_boot_count = 0U;\r\n', 1)
old = '    VMC_RX_BUF[VMC_RX_CNT] = SciaRegs.SCIRXBUF.all;'
assert src.count(old) == 1
src = src.replace(old, '    Uint16 raw;\r\n\r\n    /* Read once and preserve the original raw business RX value. */\r\n    raw = SciaRegs.SCIRXBUF.all;\r\n    VMC_RX_BUF[VMC_RX_CNT] = raw;\r\n    RS422_BootFeed(raw);')
anchor = 'void RS422_Init(Uint32 baud);'
assert hdr.count(anchor) == 1
hdr = hdr.replace(anchor, anchor + '\r\n\r\n/* Added SCI-A BOOT entry interfaces. */\r\nUint16 RS422_BootPending(void);\r\nvoid RS422_BootTask(void);')
(root / 'rs422.c').write_bytes(src.encode('gbk'))
(root / 'rs422.h').write_bytes(hdr.encode('gbk'))
print('Created rs422.c / rs422.h; original GBK encoding preserved.')
