#ifndef FLASH_LAYOUT_H
#define FLASH_LAYOUT_H
#include <stdint.h>
#include "partition_config.h"

/* These helpers are also compiled by native regression tests. No flash table
 * is needed: H starts at 0x3E8000 and uses bit 7; A uses bit 0. */
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION(flash_range_mask, "ramfuncs")
#pragma CODE_SECTION(flash_write_allowed, "ramfuncs")
#pragma CODE_SECTION(flash_begin_plan, "ramfuncs")
#endif
static uint16_t flash_range_mask(uint32_t start, uint32_t count)
{
    uint16_t index, mask = 0;
    uint32_t sector_start, end;
    if (!count || start < PART_FLASH_START || start >= PART_FLASH_END ||
        count > PART_FLASH_END - start) return 0;
    end = start + count;
    for (index = 0; index < 8; ++index)
    {
        sector_start = PART_FLASH_START + (uint32_t)index * PART_SECTOR_WORDS;
        if (start < sector_start + PART_SECTOR_WORDS && end > sector_start)
            mask |= (uint16_t)(0x0080U >> index);
    }
    return mask;
}

static uint16_t flash_write_allowed(uint32_t address, uint32_t count)
{
    if (!count) return 0;
    /* Deliberately exclude any unused gap between APP and metadata. */
    return (address >= PART_APP_START && address < PART_APP_END &&
            count <= PART_APP_END - address) ||
           (address >= PART_METADATA && address < PART_METADATA + PART_METADATA_WORDS &&
            count <= PART_METADATA + PART_METADATA_WORDS - address);
}

static uint16_t flash_begin_plan(uint32_t start, uint32_t count, uint32_t entry,
                                uint16_t *app_mask, uint16_t *metadata_mask)
{
    *app_mask = *metadata_mask = 0;
    if (start != PART_APP_START || entry != PART_APP_START || !count ||
        (count & 3UL) || count > PART_APP_END - PART_APP_START) return 0;
    *app_mask = flash_range_mask(start, count);
    *metadata_mask = flash_range_mask(PART_METADATA, PART_SECTOR_WORDS);
    return *app_mask != 0 && *metadata_mask != 0 && !(*app_mask & *metadata_mask);
}
#endif
