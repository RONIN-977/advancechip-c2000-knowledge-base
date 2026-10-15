#include <assert.h>
#include <stdio.h>
#include "DSP28x_Project.h"
#include "board_config.h"
#include "boot_io.h"
volatile struct SCI_REGS SciaRegs;
volatile struct SYS_CTRL_REGS SysCtrlRegs;
volatile struct GPIO_CTRL_REGS GpioCtrlRegs;
static uint32_t ticks;
uint32_t boot_millis(void) { return ticks++; }
int main(void)
{
    bl_octet value = 0xCC, tx[] = {0xA5, 0x5A};
    boot_sci_init();
    assert(SysCtrlRegs.LOSPCP.all == 2 && SciaRegs.SCILBAUD == 15);
    assert(SciaRegs.SCICCR.all == 7 && SciaRegs.SCICTL1.all == 0x23);
    assert(GpioCtrlRegs.GPAMUX2.bit.GPIO28 == 1 && GpioCtrlRegs.GPAMUX2.bit.GPIO29 == 1);
    assert(boot_sci_read(&value) == 0 && value == 0xCC);
#if BOOT_SCI_USE_FIFO
    SciaRegs.SCIFFRX.bit.RXFFST = 1;
#else
    SciaRegs.SCIRXST.bit.RXRDY = 1;
#endif
    SciaRegs.SCIRXBUF.all = 0x55;
    assert(boot_sci_read(&value) == 1 && value == 0x55);
    SciaRegs.SCIRXBUF.all = 0xC055;
    assert(boot_sci_read(&value) == 2 && boot_debug_sci_errors == 1);
    SciaRegs.SCIRXBUF.all = 0;
    SciaRegs.SCIRXST.all = 0x90;
    assert(boot_sci_read(&value) == 2 && boot_debug_sci_errors == 2);
    assert(SciaRegs.SCICTL1.bit.SWRESET == 1);
    SciaRegs.SCIRXST.all = 0;
#if BOOT_SCI_USE_FIFO
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 0;
    SciaRegs.SCIFFRX.bit.RXFFOVF = 1;
    assert(boot_sci_read(&value) == 2 && boot_debug_sci_overflows == 1);
    /* Regresses the old 0x8040 write: overflow must be cleared via bit14. */
    assert(SciaRegs.SCIFFRX.bit.RXFFOVRCLR == 1);
    assert(SciaRegs.SCIFFRX.bit.RXFIFORESET == 1);
    SciaRegs.SCIFFRX.all = 0;
    /* A busy FIFO must not accept another write. The previous ==16 check
     * overwrote SCITXBUF before timing out in the final drain loop. */
    SciaRegs.SCITXBUF = 0xCC;
    SciaRegs.SCIFFTX.bit.TXFFST = 4;
    assert(boot_sci_send(tx, 2) == 0 && SciaRegs.SCITXBUF == 0xCC);
    SciaRegs.SCIFFTX.bit.TXFFST = 1;
    assert(boot_sci_send(tx, 2) == 0 && SciaRegs.SCITXBUF == 0xCC);
#else
    SciaRegs.SCICTL2.bit.TXRDY = 0;
#endif
    assert(boot_sci_send(tx, 2) == 0); /* Hardware not ready must time out. */
    SciaRegs.SCIFFTX.bit.TXFFST = 0;
    SciaRegs.SCICTL2.bit.TXRDY = 1;
    SciaRegs.SCICTL2.bit.TXEMPTY = 0;
    assert(boot_sci_send(tx, 2) == 0); /* Do not ACK reset while bits remain. */
    SciaRegs.SCICTL2.bit.TXEMPTY = 1;
    assert(boot_sci_send(tx, 2) == 1 && SciaRegs.SCITXBUF == 0x5A);
    puts("PASS SCI: init, receive, error/overflow recovery, TX timeout and drain");
    return 0;
}
