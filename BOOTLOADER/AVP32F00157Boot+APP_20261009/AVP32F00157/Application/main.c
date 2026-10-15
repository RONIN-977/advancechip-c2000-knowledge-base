#include "device.h"
#include "boot_io.h"
#include "update_service.h"
#include "board_config.h"
void main(void)
{
    uint32_t last;
    (void)boot_take_request();
    boot_device_init();
    GPIO_setPinConfig(GPIO_35_GPIO35);
    GPIO_writePin(APP_LED_PIN, 0);
    GPIO_setDirectionMode(APP_LED_PIN, GPIO_DIR_MODE_OUT);
    update_service_init(0, 0);
    boot_ports_init();
    last = boot_millis();
    for (;;)
    {
        update_service_poll(); /* ENTER_BOOT: ACK, then enter BOOT codestart. */
        if (boot_millis() - last >= APP_LED_TOGGLE_MS)
        { last = boot_millis(); GPIO_togglePin(APP_LED_PIN); }
        /* Put nonblocking application work here. */
    }
}
