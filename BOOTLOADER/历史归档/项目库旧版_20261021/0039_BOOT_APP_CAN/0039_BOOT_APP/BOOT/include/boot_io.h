#ifndef BOOT_IO_H
#define BOOT_IO_H
#include "boot_core.h"
uint32_t boot_millis(void);
void boot_timers_init(void);
void boot_ports_init(void);
uint16_t boot_send_reply(uint16_t transport, const bl_octet *data, uint16_t count);
/* Reserve M0 word addresses 0x3F8..0x3FB in the application's linker file.
 * Watchdog reset keeps this cookie; power-cycle is not required. */
#define BOOT_REQUEST_ADDR 0x0003F8UL
uint16_t boot_take_request(void);
void boot_request_reset(void);
#endif
