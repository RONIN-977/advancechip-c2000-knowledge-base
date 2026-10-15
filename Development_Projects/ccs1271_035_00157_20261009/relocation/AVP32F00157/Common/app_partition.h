#ifndef APP_PARTITION_H
#define APP_PARTITION_H

/* USER SETTING: C28x 16-bit WORD address, sector alignment = 0x400.
 * Edit this ONE line, then rebuild BOTH BOOT and APP. */
#define APP_START_ADDRESS 0x08A000

/* Derived layout. Keep APP end compatible with the current host profile. */
#define APP_END_ADDRESS 0x09FFF0
#define FLASH_SECTOR_WORDS 0x400
#define BOOT_METADATA_ADDRESS (APP_START_ADDRESS - FLASH_SECTOR_WORDS)
#define BOOT_CODE_START 0x080002
#define BOOT_CODE_WORDS (BOOT_METADATA_ADDRESS - BOOT_CODE_START)
#if APP_START_ADDRESS < 0x080800 || APP_START_ADDRESS >= APP_END_ADDRESS
#error APP_START_ADDRESS is outside the supported flash layout
#endif
#if (APP_START_ADDRESS & 0x3FF)
#error APP_START_ADDRESS must align to a 0x400 WORD sector
#endif

/* A protects sectors 0..31 individually. B protects groups of 8 sectors
 * from 32..127. A group containing metadata is writable; software checks
 * still prohibit touching any BOOT sectors sharing that group. */
#define FIRST_WRITABLE_SECTOR ((BOOT_METADATA_ADDRESS - 0x080000) / FLASH_SECTOR_WORDS)
#if FIRST_WRITABLE_SECTOR < 32
#define BOOT_PROTECT_A ((1UL << FIRST_WRITABLE_SECTOR) - 1UL)
#define BOOT_PROTECT_B 0UL
#else
#define BOOT_PROTECT_A 0xFFFFFFFFUL
#define BOOT_PROTECT_B ((1UL << ((FIRST_WRITABLE_SECTOR - 32) / 8)) - 1UL)
#endif
#endif
