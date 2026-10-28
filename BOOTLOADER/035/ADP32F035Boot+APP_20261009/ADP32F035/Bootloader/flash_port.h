#ifndef FLASH_PORT_H
#define FLASH_PORT_H
#include "boot_core.h"
enum boot_flash_stage
{
    BOOT_FLASH_INVALIDATE = 1,
    BOOT_FLASH_ERASE_METADATA,
    BOOT_FLASH_ERASE_APP,
    BOOT_FLASH_PROGRAM_APP,
    BOOT_FLASH_PROGRAM_HEADER,
    BOOT_FLASH_COMMIT
};
extern const bl_flash_ops boot_flash_ops;
uint16_t boot_flash_init(void);
uint16_t boot_image_valid(void);
#endif
