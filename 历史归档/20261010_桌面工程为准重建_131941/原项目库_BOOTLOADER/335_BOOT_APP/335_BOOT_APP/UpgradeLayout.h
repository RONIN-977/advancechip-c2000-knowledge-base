#ifndef UPGRADE_LAYOUT_H
#define UPGRADE_LAYOUT_H

/* Select exactly ONE address: uncomment it and comment the previous choice.
 * C28x WORD addresses. Rebuild BOTH BOOT and APP after changing this choice. */
//#define UPGRADE_APP_AT_300000     /* Default: sector H */
 #define UPGRADE_APP_AT_308000  /* Sector G */
// #define UPGRADE_APP_AT_310000  /* Sector F */
// #define UPGRADE_APP_AT_318000  /* Sector E */
// #define UPGRADE_APP_AT_320000  /* Sector D */
// #define UPGRADE_APP_AT_328000  /* Sector C */
// #define UPGRADE_APP_AT_330000  /* Sector B (also contains the fixed flag) */

#if (defined(UPGRADE_APP_AT_300000) + defined(UPGRADE_APP_AT_308000) + \
     defined(UPGRADE_APP_AT_310000) + defined(UPGRADE_APP_AT_318000) + \
     defined(UPGRADE_APP_AT_320000) + defined(UPGRADE_APP_AT_328000) + \
     defined(UPGRADE_APP_AT_330000)) != 1
#error Enable exactly one UPGRADE_APP_AT_xxxxxx option
#endif

/* Derived configuration below: do not edit these addresses directly. */
#if defined(UPGRADE_APP_AT_300000)
#define UPGRADE_APP_START 0x300000
#elif defined(UPGRADE_APP_AT_308000)
#define UPGRADE_APP_START 0x308000
#elif defined(UPGRADE_APP_AT_310000)
#define UPGRADE_APP_START 0x310000
#elif defined(UPGRADE_APP_AT_318000)
#define UPGRADE_APP_START 0x318000
#elif defined(UPGRADE_APP_AT_320000)
#define UPGRADE_APP_START 0x320000
#elif defined(UPGRADE_APP_AT_328000)
#define UPGRADE_APP_START 0x328000
#elif defined(UPGRADE_APP_AT_330000)
#define UPGRADE_APP_START 0x330000
#endif

/* Fixed V201 flag and BOOT partition; do not move with the APP start. */
#define UPGRADE_APP_END 0x337FFE
#define UPGRADE_FLAG_ADDR 0x337FFE
#if UPGRADE_APP_START < 0x300000 || UPGRADE_APP_START > 0x330000
#error APP start must be within sectors H through B
#endif
#if (UPGRADE_APP_START & 0x7FFF) != 0
#error APP start must be aligned to a 0x8000 WORD erase sector
#endif

#define UPGRADE_APP_FLASH_START (UPGRADE_APP_START + 2)
#define UPGRADE_APP_FLASH_LENGTH (UPGRADE_APP_END - UPGRADE_APP_FLASH_START)
#define UPGRADE_MAX_BYTES ((UPGRADE_APP_END - UPGRADE_APP_START) * 2UL)
/* B is always erased separately for durable flag invalidation. A is BOOT. */
#define UPGRADE_APP_ERASE_MASK ((0x00FFU >> ((UPGRADE_APP_START - 0x300000) / 0x8000)) & 0x00FCU)

#endif
