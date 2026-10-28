#include "device.h"
#include "board_config.h"
#include "boot_io.h"

volatile uint16_t boot_debug_rx_error_status=0;
volatile uint32_t boot_debug_sci_errors=0, boot_debug_sci_overflows=0;

void boot_sci_init(void)
{
    GPIO_setAnalogMode(28, GPIO_ANALOG_DISABLED); /* AGPIO on F280015x. */
    GPIO_setPinConfig(GPIO_28_SCIA_RX);
    GPIO_setPinConfig(GPIO_29_SCIA_TX);
    GPIO_setPadConfig(28, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(28, GPIO_QUAL_ASYNC);
    SCI_disableModule(SCIA_BASE);
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, BOOT_SCI_BAUD,
                  SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE);
    SCI_disableInterrupt(SCIA_BASE, SCI_INT_RXFF|SCI_INT_TXFF|SCI_INT_RXERR);
    SCI_resetChannels(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE); SCI_resetTxFIFO(SCIA_BASE);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);
    SCI_clearOverflowStatus(SCIA_BASE);
}
uint16_t boot_sci_read(bl_octet *value)
{
    uint16_t status=SCI_getRxStatus(SCIA_BASE), word;
    bool overflow=SCI_getOverflowStatus(SCIA_BASE);
    if ((status & SCI_RXSTATUS_ERROR) || overflow)
    {
        boot_debug_rx_error_status=status; ++boot_debug_sci_errors;
        if(overflow)++boot_debug_sci_overflows;
        SCI_performSoftwareReset(SCIA_BASE);
        SCI_resetRxFIFO(SCIA_BASE); SCI_clearOverflowStatus(SCIA_BASE);
        return 2;
    }
    if(SCI_getRxFIFOStatus(SCIA_BASE)==SCI_FIFO_RX0)return 0;
    word=HWREGH(SCIA_BASE+SCI_O_RXBUF);
    if(word&0xC000U)
    {
        boot_debug_rx_error_status=word; ++boot_debug_sci_errors;
        SCI_performSoftwareReset(SCIA_BASE); SCI_resetRxFIFO(SCIA_BASE);
        return 2;
    }
    *value=word&255U;return 1;
}
uint16_t boot_sci_send(const bl_octet *data,uint16_t count)
{
    uint16_t i; uint32_t start;
    for(i=0;i<count;++i)
    {
        start=boot_millis();
        /* TI echo-style conservative pacing: independent of FIFO depth. */
        while(SCI_getTxFIFOStatus(SCIA_BASE)!=SCI_FIFO_TX0)
            if(boot_millis()-start>500UL)return 0;
        SCI_writeCharNonBlocking(SCIA_BASE,data[i]&255U);
    }
    start=boot_millis();
    while(SCI_getTxFIFOStatus(SCIA_BASE)!=SCI_FIFO_TX0 || SCI_isTransmitterBusy(SCIA_BASE))
        if(boot_millis()-start>500UL)return 0;
    return 1;
}
