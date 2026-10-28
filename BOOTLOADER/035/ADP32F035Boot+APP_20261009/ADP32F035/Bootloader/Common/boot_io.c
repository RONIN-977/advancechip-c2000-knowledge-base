#include "DSP28x_Project.h"
#include "board_config.h"
#include "boot_io.h"
#include <string.h>
#if ((BOOT_SYSCLK_HZ / 2UL) % (BOOT_CAN_BAUD * 15UL)) != 0
#error "CAN bitrate must divide the 30 MHz eCAN clock into 15 TQ"
#endif
#if ((BOOT_SYSCLK_HZ / 2UL) / (BOOT_CAN_BAUD * 15UL)) > 256UL || ((BOOT_SYSCLK_HZ / 2UL) / (BOOT_CAN_BAUD * 15UL)) < 1UL
#error "CAN prescaler out of range"
#endif

extern Uint16 RamfuncsLoadStart, RamfuncsLoadSize, RamfuncsRunStart;
/* COFF startup does not clear uninitialized statics: initialize state explicitly. */
volatile uint32_t boot_debug_can_errors = 0;
static uint16_t can_ready = 0, can_transfer = 0;
#pragma CODE_SECTION(boot_reset, "ramfuncs")
#pragma CODE_SECTION(boot_trap, "ramfuncs")

void boot_reset(void)
{
    DINT;
    EALLOW;
    SysCtrlRegs.SCSR = 0; /* Watchdog reset, not watchdog interrupt. */
    SysCtrlRegs.WDCR = 0x0028;
    EDIS;
    for (;;) { }
}

__interrupt void boot_trap(void) { boot_reset(); }

void boot_device_init(void)
{
    DINT;
    /* TI's XtalOscSel uses DELAY_US in ramfuncs: copy BEFORE InitSysCtrl. */
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitSysCtrl(); /* TI F2803x example: XtalOscSel, InitPll(12,2). */
    InitFlash();
    InitPieCtrl();
    IER = 0; IFR = 0;
    InitPieVectTable();
    EALLOW;
    PieVectTable.NMI = &boot_trap;
    PieVectTable.ILLEGAL = &boot_trap;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
    EDIS;
    CpuTimer0Regs.TCR.all = 0x0010;
    CpuTimer0Regs.PRD.all = 0xFFFFFFFFUL;
    CpuTimer0Regs.TPR.all = BOOT_SYSCLK_HZ / 1000000UL - 1UL;
    CpuTimer0Regs.TPRH.all = 0;
    CpuTimer0Regs.TCR.all = 0x0020; /* reload, run, interrupt disabled */
}

uint32_t boot_millis(void)
{
    static uint32_t previous = 0xFFFFFFFFUL, remainder = 0, time = 0;
    uint32_t current = CpuTimer0Regs.TIM.all, delta = previous - current;
    previous = current;
    time += delta / 1000UL;
    remainder += delta % 1000UL;
    if (remainder >= 1000UL) { ++time; remainder -= 1000UL; }
    return time;
}

static uint16_t can_config_wait(uint16_t enable)
{
    uint32_t start = boot_millis();
    while (((ECanaRegs.CANES.all & 0x10UL) != 0) != (enable != 0))
        if (boot_millis() - start > 100UL) { ++boot_debug_can_errors; return 0; }
    return 1;
}

void boot_ports_init(void)
{
    struct ECAN_REGS shadow;
    uint16_t i;
    volatile struct MBOX *mbox = &ECanaMboxes.MBOX0;
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1; /* CANRXA */
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1; /* CANTXA */
    EDIS;
    boot_sci_init();

    /* eCAN control registers require 32-bit access: use shadow + .all. */
    EALLOW;
    ECanaRegs.CANME.all = 0;
    ECanaRegs.CANMIM.all = 0;
    ECanaRegs.CANGIM.all = 0;
    shadow.CANTIOC.all = 0; shadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = shadow.CANTIOC.all;
    shadow.CANRIOC.all = 0; shadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = shadow.CANRIOC.all;
    shadow.CANMC.all = 0;
    shadow.CANMC.bit.SCB = 1;
    shadow.CANMC.bit.ABO = 1;
    shadow.CANMC.bit.CCR = 1;
    /* DBO=0: byte 0 in MDL[31:24], byte 4 in MDH[31:24]. */
    ECanaRegs.CANMC.all = shadow.CANMC.all;
    EDIS;
    if (!can_config_wait(1)) return; /* SCI remains available. */
    EALLOW;
    shadow.CANBTC.all = 0;
    /* SYSCLK/2 = 30 MHz; /4 /15 TQ = 500 kbit/s, sample at 80%. */
    shadow.CANBTC.bit.BRPREG = (BOOT_SYSCLK_HZ / 2UL) / (BOOT_CAN_BAUD * 15UL) - 1UL;
    shadow.CANBTC.bit.TSEG1REG = 10;
    shadow.CANBTC.bit.TSEG2REG = 2;
    shadow.CANBTC.bit.SJWREG = 1;
    ECanaRegs.CANBTC.all = shadow.CANBTC.all;
    for (i = 0; i < 32; ++i) mbox[i].MSGCTRL.all = 0;
    ECanaRegs.CANTA.all = 0xFFFFFFFFUL;
    ECanaRegs.CANRMP.all = 0xFFFFFFFFUL;
    ECanaRegs.CANRML.all = 0xFFFFFFFFUL;
    ECanaRegs.CANGIF0.all = 0xFFFFFFFFUL;
    ECanaRegs.CANGIF1.all = 0xFFFFFFFFUL;
    ECanaMboxes.MBOX0.MSGID.all = BOOT_CAN_RX_ID << 18;
    ECanaMboxes.MBOX1.MSGID.all = BOOT_CAN_TX_ID << 18;
    ECanaRegs.CANMD.all = 1UL;
    ECanaRegs.CANOPC.all = 1UL; /* Protect unread RX mailbox from overwrite. */
    ECanaRegs.CANME.all = 3UL;
    shadow.CANMC.bit.CCR = 0;
    ECanaRegs.CANMC.all = shadow.CANMC.all;
    EDIS;
    can_ready = can_config_wait(0);
}

uint16_t boot_can_read(bl_octet *frame, uint16_t *dlc)
{
    uint32_t low, high, ctrl, id;
    uint16_t i, lost;
    if (!can_ready || !(ECanaRegs.CANRMP.all & 1UL)) return 0;
    ctrl = ECanaMboxes.MBOX0.MSGCTRL.all;
    id = ECanaMboxes.MBOX0.MSGID.all;
    low = ECanaMboxes.MBOX0.MDL.all; high = ECanaMboxes.MBOX0.MDH.all;
    lost = (ECanaRegs.CANRML.all & 1UL) != 0;
    ECanaRegs.CANRML.all = 1UL;
    ECanaRegs.CANRMP.all = 1UL;
    if (lost) { ++boot_debug_can_errors; return 0; }
    *dlc = ctrl & 15U;
    if ((ctrl & 0x10UL) || id != (BOOT_CAN_RX_ID << 18) || *dlc > 8U) return 0;
    for (i = 0; i < 4; ++i)
    {
        frame[i] = (low >> (24U - i * 8U)) & 255U;
        frame[i + 4] = (high >> (24U - i * 8U)) & 255U;
    }
    return 1;
}

static uint16_t can_send(const bl_octet *frame, uint16_t count)
{
    uint32_t low = 0, high = 0, start = boot_millis();
    uint16_t i;
    if (!can_ready || (ECanaRegs.CANTRS.all & 2UL)) return 0;
    for (i = 0; i < count; ++i)
        if (i < 4) low |= (uint32_t)frame[i] << (24U - i * 8U);
        else high |= (uint32_t)frame[i] << (24U - (i - 4U) * 8U);
    EALLOW;
    ECanaRegs.CANME.all = 1UL; /* Disable TX mailbox while changing its DLC. */
    ECanaMboxes.MBOX1.MDL.all = low;
    ECanaMboxes.MBOX1.MDH.all = high;
    ECanaMboxes.MBOX1.MSGCTRL.all = count;
    ECanaRegs.CANME.all = 3UL;
    EDIS;
    ECanaRegs.CANTA.all = 2UL;
    ECanaRegs.CANTRS.all = 2UL;
    while (!(ECanaRegs.CANTA.all & 2UL))
        if (boot_millis() - start > 500UL)
        { ECanaRegs.CANTRR.all = 2UL; ++boot_debug_can_errors; return 0; }
    ECanaRegs.CANTA.all = 2UL;
    return 1;
}

uint16_t boot_send_reply(uint16_t transport, const bl_octet *data, uint16_t count)
{
    uint16_t i, n, offset, index = 0;
    bl_octet frame[8];
    if (transport == 1) return boot_sci_send(data, count);
    can_transfer = (can_transfer + 1U) & 255U;
    for (offset = 0; offset < count; offset += n)
    {
        n = count - offset > 6U ? 6U : count - offset;
        frame[0] = can_transfer; frame[1] = index | (index ? 0U : 0x80U); ++index;
        for (i = 0; i < n; ++i) frame[i + 2] = data[offset + i];
        if (!can_send(frame, n + 2U)) return 0;
    }
    return 1;
}

uint16_t boot_take_request(void)
{
    volatile uint16_t *p = (volatile uint16_t *)BOOT_REQUEST_ADDR;
    uint16_t valid = p[0] == 0xB007U && p[1] == 0x4FF8U && p[2] == 0x5AA5U && p[3] == 0xA55AU;
    p[0] = p[1] = p[2] = p[3] = 0;
    return valid;
}
/* Both images enter through codestart/_c_int00, never through main directly.
 * Stop active peripherals before the destination copies/reinitializes RAM. */
static void jump_image(uint32_t entry)
{
    DINT; IER = 0; IFR = 0;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;
    SciaRegs.SCICTL1.all = 0;
    EALLOW;
    ECanaRegs.CANME.all = 0;
    SysCtrlRegs.WDCR = 0x0068;
    EDIS;
    CpuTimer0Regs.TCR.bit.TSS = 1;
    ((void (*)(void))entry)();
    for (;;) { }
}
void boot_enter_boot(void)
{
    volatile uint16_t *p = (volatile uint16_t *)BOOT_REQUEST_ADDR;
    p[0] = 0xB007U; p[1] = 0x4FF8U; p[2] = 0x5AA5U; p[3] = 0xA55AU;
    DELAY_US(20000L);
    /* No chip reset / ROM startup: preserve the reserved request words. */
    jump_image(0x3F7FF6UL);
}
void boot_jump_app(void)
{
    jump_image(BL_APP_START);
}
