#include "boot_io.h"
#include "flash_port.h"
#include "update_service.h"
#include "board_config.h"
extern volatile uint16_t boot_debug_stage;

void main(void)
{
    uint16_t requested = boot_take_request(), valid;
    uint32_t startup;
    boot_debug_stage = 1;
    boot_device_init();
    boot_debug_stage = 2;
    if (!boot_flash_init()) { boot_debug_stage = 0xE1; for (;;) { } }
    boot_debug_stage = 3;
    valid = boot_image_valid();
    update_service_init(&boot_flash_ops, valid);
    boot_debug_stage = 4;
    boot_ports_init();
    startup = boot_millis();
    boot_debug_stage = 5;
    for (;;)
    {
        update_service_poll();
#ifndef BOOT_SCI_ECHO_TEST
        if (!requested && !update_service_keep_boot() &&
            update_service_image_valid() && boot_millis() - startup >= BOOT_LISTEN_MS)
        { boot_debug_stage = 6; boot_jump_app(); }
#else
        (void)requested; (void)startup;
#endif
    }
}
