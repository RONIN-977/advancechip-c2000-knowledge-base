#ifndef PARTITION_CONFIG_H
#define PARTITION_CONFIG_H
#include "app_partition.h"

/* C28x 16-bit WORD addresses. End addresses are exclusive.
 * Keep Bootloader/bootloader.cmd and Application/application.cmd in sync.
 * Flash sectors are H..A in ascending address order, 0x2000 words each. */
#ifndef PART_APP_START
#define PART_APP_START      APP_START_ADDRESS
#endif
#ifndef PART_APP_END
#define PART_APP_END        0x3F2000
#endif
#ifndef PART_METADATA
#define PART_METADATA       0x3F2000
#endif
#ifndef PART_BOOT_START
#define PART_BOOT_START     0x3F4000
#endif
#define PART_FLASH_START    0x3E8000
#define PART_FLASH_END      0x3F8000
#define PART_SECTOR_WORDS   0x2000
#define PART_METADATA_WORDS 20

#if PART_APP_START < PART_FLASH_START || PART_APP_START >= PART_APP_END
#error Invalid APP start/end
#endif
#if ((PART_APP_START | PART_APP_END | PART_METADATA | PART_BOOT_START) & 0x1FFF)
#error Partition boundaries must align to 0x2000 WORD flash sectors
#endif
#if PART_APP_END > PART_METADATA || PART_METADATA < PART_FLASH_START
#error APP overlaps metadata or metadata is outside flash
#endif
#if PART_BOOT_START < PART_FLASH_START || PART_BOOT_START > 0x3F6000
#error BOOT must reserve at least sector A including codestart and CSM
#endif
#if PART_METADATA >= PART_BOOT_START || PART_BOOT_START - PART_METADATA < PART_SECTOR_WORDS
#error Metadata sector overlaps BOOT
#endif
#endif
