#include "DSP28x_Project.h"
#include "board_config.h"
#include "boot_io.h"

volatile uint16_t boot_debug_rx_error_status = 0;
volatile uint16_t boot_debug_sci_rx_word = 0, boot_debug_sci_brr = 0;
volatile uint32_t boot_debug_sci_errors = 0, boot_debug_sci_overflows = 0;

/* TI Example_2803xSci_Echoback.c sequence; interrupts disabled for polling.
 * Select GPIO28/29 explicitly: the local TI Sci.c actually selects 19/18. */
void boot_sci_init(void)
{
    uint32_t brr = (BOOT_LSPCLK_HZ + 4UL * BOOT_SCI_BAUD) /
                   (8UL * BOOT_SCI_BAUD) - 1UL;
    EALLOW;
    SysCtrlRegs.LOSPCP.all = 2; /* TI default: SYSCLK /4. */
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;
    EDIS;
    SciaRegs.SCICTL1.all = 0x0003;
#if BOOT_SCI_USE_FIFO
    SciaRegs.SCIFFTX.all = 0xC040; /* TX FIFO held in reset. */
    SciaRegs.SCIFFRX.all = 0x4044; /* RX reset; overflow clear is bit 14. */
#else
    SciaRegs.SCIFFTX.all = 0x8000;
    SciaRegs.SCIFFRX.all = 0;
#endif
    SciaRegs.SCIFFCT.all = 0;
    SciaRegs.SCICCR.all = 0x0007;
    SciaRegs.SCICTL2.all = 0;
    SciaRegs.SCIHBAUD = (brr >> 8) & 255U;
    SciaRegs.SCILBAUD = brr & 255U;
    boot_debug_sci_brr = (uint16_t)brr;
#if BOOT_SCI_USE_FIFO
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
#endif
    SciaRegs.SCICTL1.all = 0x0023;
}

static void sci_recover(void)
{
    SciaRegs.SCICTL1.bit.SWRESET = 0;
#if BOOT_SCI_USE_FIFO
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
#endif
    SciaRegs.SCICTL1.bit.SWRESET = 1;
}

uint16_t boot_sci_read(bl_octet *value)
{
    uint16_t status = SciaRegs.SCIRXST.all, word, overflow = 0;
#if BOOT_SCI_USE_FIFO
    overflow = SciaRegs.SCIFFRX.bit.RXFFOVF;
#endif
    if ((status & 0x80U) || overflow)
    {
        boot_debug_rx_error_status = status;
        if (status & 0x80U) ++boot_debug_sci_errors;
        if (overflow) ++boot_debug_sci_overflows;
        sci_recover();
        return 2;
    }
#if BOOT_SCI_USE_FIFO
    if (!SciaRegs.SCIFFRX.bit.RXFFST) return 0;
#else
    if (!SciaRegs.SCIRXST.bit.RXRDY) return 0;
#endif
    word = SciaRegs.SCIRXBUF.all;
    boot_debug_sci_rx_word = word;
    if (word & 0xC000U)
    {
        boot_debug_rx_error_status = word;
        ++boot_debug_sci_errors;
        sci_recover();
        return 2;
    }
    *value = word & 255U;
    return 1;
}

uint16_t boot_sci_send(const bl_octet *data, uint16_t count)
{
    uint16_t i;
    uint32_t start;
    for (i = 0; i < count; ++i)
    {
        start = boot_millis();
#if BOOT_SCI_USE_FIFO
        /* TI F2803x sci_echoback scia_xmit: wait until FIFO is empty.
         * F28035 has a 4-entry FIFO, not 16. Do not assume clone depth. */
        while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
#else
        while (!SciaRegs.SCICTL2.bit.TXRDY)
#endif
            if (boot_millis() - start > 500UL) return 0;
        SciaRegs.SCITXBUF = data[i] & 255U;
    }
    start = boot_millis();
#if BOOT_SCI_USE_FIFO
    while (SciaRegs.SCIFFTX.bit.TXFFST)
        if (boot_millis() - start > 500UL) return 0;
#endif
    /* Wait through the final stop bit before APP transfers control to BOOT. */
    while (!SciaRegs.SCICTL2.bit.TXEMPTY)
        if (boot_millis() - start > 500UL) return 0;
    return 1;
}
