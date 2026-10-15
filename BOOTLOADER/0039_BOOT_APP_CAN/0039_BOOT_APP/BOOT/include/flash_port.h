#ifndef FLASH_PORT_H
#define FLASH_PORT_H
#include "boot_core.h"
extern const bl_flash_ops boot_flash_ops;
uint16_t boot_flash_init(void);
uint16_t boot_image_valid(void);
__interrupt void boot_flash_nmi(void);
#endif
