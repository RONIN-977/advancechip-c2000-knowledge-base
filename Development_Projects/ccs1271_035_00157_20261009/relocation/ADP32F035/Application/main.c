#include "DSP28x_Project.h"
#include "board_config.h"
#include "boot_io.h"
#include "update_service.h"

void main(void)
{
    uint32_t last_toggle;
    (void)boot_take_request();
    boot_device_init(); /* TI clock, Flash wait states, PIE initialization. */
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
    GpioDataRegs.GPACLEAR.all = 1UL << 17;
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;
    EDIS;
    update_service_init(0, 0); /* APP listens for SCI/CAN ENTER_BOOT. */
    boot_ports_init();
    last_toggle = boot_millis();
    for (;;)
    {
        update_service_poll(); /* ACK drained -> cookie -> BOOT codestart. */
        if (boot_millis() - last_toggle >= APP_LED_TOGGLE_MS)
        {
            last_toggle = boot_millis();
            GpioDataRegs.GPATOGGLE.all = 1UL << 17;
        }
        /* Add nonblocking application work here. */
    }
}
