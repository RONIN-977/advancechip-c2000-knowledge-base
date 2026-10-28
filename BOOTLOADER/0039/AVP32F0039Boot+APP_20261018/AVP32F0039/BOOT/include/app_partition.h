#ifndef APP_PARTITION_H
#define APP_PARTITION_H

/* USER SETTING: C28x 16-bit WORD address, sector alignment = 0x1000.
 * This is the BOOT project LOCAL copy; no files are shared.
 * Match APP_START_ADDRESS in BOTH projects, then rebuild BOTH. */
#define APP_START_ADDRESS 0x090000

#define APP_END_ADDRESS 0x0AF000
#define FLASH_SECTOR_WORDS 0x1000
#define BOOT_METADATA_ADDRESS (APP_START_ADDRESS - FLASH_SECTOR_WORDS)
#define BOOT_CODE_START 0x080002
#define BOOT_CODE_WORDS (BOOT_METADATA_ADDRESS - BOOT_CODE_START)
#if APP_START_ADDRESS < 0x082000 || APP_START_ADDRESS >= APP_END_ADDRESS
#error APP_START_ADDRESS is outside the supported flash layout
#endif
#if (APP_START_ADDRESS & 0xFFF)
#error APP_START_ADDRESS must align to a 0x1000 WORD sector
#endif
#endif
