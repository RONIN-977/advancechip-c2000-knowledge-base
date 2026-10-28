#ifndef UPDATE_SERVICE_H
#define UPDATE_SERVICE_H
#include "boot_core.h"
/* NULL ops selects APP mode: ENTER_BOOT -> ACK -> jump to BOOT startup. */
void update_service_init(const bl_flash_ops *ops, uint16_t valid_image);
void update_service_poll(void);
uint16_t update_service_keep_boot(void);
uint16_t update_service_image_valid(void);
#endif
