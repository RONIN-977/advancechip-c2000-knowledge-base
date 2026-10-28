#ifndef BOOT_IO_H
#define BOOT_IO_H
#include "boot_core.h"
void boot_device_init(void);
void boot_ports_init(void);
void boot_sci_init(void);
uint16_t boot_sci_send(const bl_octet *data, uint16_t count);
uint32_t boot_millis(void);
/* SCI: 0=empty, 1=octet, 2=receiver error (FIFO discarded). */
uint16_t boot_sci_read(bl_octet *value);
uint16_t boot_can_read(bl_octet *frame, uint16_t *dlc);
uint16_t boot_send_reply(uint16_t transport, const bl_octet *data, uint16_t count);
uint16_t boot_take_request(void);
void boot_enter_boot(void);
void boot_reset(void);
void boot_jump_app(void);
extern volatile uint16_t boot_debug_rx_error_status;
extern volatile uint32_t boot_debug_sci_errors, boot_debug_sci_overflows;
extern volatile uint32_t boot_debug_can_errors;
#endif
