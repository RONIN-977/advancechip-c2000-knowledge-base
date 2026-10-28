#include "DSP28x_Project.h"
#include "Flash2803x_API_Library.h"
#include "board_config.h"
#include "flash_port.h"
#include "flash_layout.h"

extern Uint16 RamfuncsLoadStart, RamfuncsLoadSize, RamfuncsRunStart;
volatile uint16_t boot_debug_flash_status = 0xFFFFU;
volatile FLASH_ST boot_debug_flash_detail = {0, 0, 0};
volatile uint16_t boot_debug_flash_stage = 0;
volatile uint16_t boot_debug_flash_ram_ok = 0;
volatile uint16_t boot_debug_flash_csm_ok = 0;
volatile uint16_t boot_debug_flash_api_version = 0;
volatile uint16_t boot_debug_flash_erase_mask = 0;

/* Match the user's working AVP CAN project: API executes in chip ROM;
 * our active erase/program callers execute in RAM. */
#pragma CODE_SECTION(api_erase, "ramfuncs")
#pragma CODE_SECTION(api_program, "ramfuncs")
#pragma CODE_SECTION(begin_image, "ramfuncs")
#pragma CODE_SECTION(program_image, "ramfuncs")
#pragma CODE_SECTION(commit_image, "ramfuncs")

static uint32_t words_crc(const uint16_t *p, uint32_t count)
{
    uint32_t crc = 0xFFFFFFFFUL, i;
    uint16_t byte, bit;
    for (i = 0; i < count; ++i)
        for (byte = 0; byte < 2; ++byte)
        {
            crc ^= (p[i] >> (8U * byte)) & 255U;
            for (bit = 0; bit < 8; ++bit)
                crc = (crc >> 1) ^ ((crc & 1UL) ? 0xEDB88320UL : 0UL);
        }
    return ~crc;
}
static uint32_t get32(const uint16_t *p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 16); }
static void put32(uint16_t *p, uint32_t value) { p[0] = (uint16_t)value; p[1] = (uint16_t)(value >> 16); }

static uint16_t ram_matches(const volatile Uint16 *load,
                            const volatile Uint16 *run, Uint32 words)
{
    while (words--)
        if (*load++ != *run++) return 0;
    return 1;
}

uint16_t boot_flash_init(void)
{
    /* Exact ROM symbols verified against ADP32F035_BOOT_CAN.map.
     * Reject accidental linkage of the external TI algorithm. */
    if ((Uint32)Flash2803x_Program != 0x3FF8D2UL ||
        (Uint32)Flash2803x_Erase != 0x3FF9B3UL ||
        (Uint32)Flash2803x_Verify != 0x3FFDD5UL ||
        (Uint32)Flash2803x_APIVersionHex != 0x3FFEC1UL)
    {
        boot_debug_flash_ram_ok = 0;
        boot_debug_flash_status = 0xFFFCU;
        return 0;
    }
    DINT;
    boot_debug_flash_csm_ok = CsmUnlock();
    if (!boot_debug_flash_csm_ok)
    {
        boot_debug_flash_status = 0xFFFBU;
        return 0;
    }
    /* ROM has no API image to copy; ramfuncs was copied in device init. */
    boot_debug_flash_ram_ok =
        (Uint32)&RamfuncsLoadSize != 0 &&
        ram_matches(&RamfuncsLoadStart, &RamfuncsRunStart, (Uint32)&RamfuncsLoadSize);
    if (!boot_debug_flash_ram_ok)
    {
        boot_debug_flash_status = 0xFFFDU;
        return 0;
    }
    EALLOW;
    Flash_CPUScaleFactor = (Uint32)SCALE_FACTOR;
    Flash_CallbackPtr = 0;
    EDIS;
    boot_debug_flash_api_version = Flash_APIVersionHex();
    boot_debug_flash_status = STATUS_SUCCESS;
    return 1;
}

static uint16_t api_erase(uint16_t mask)
{
    uint16_t allowed = flash_range_mask(BL_APP_START, BL_APP_END - BL_APP_START) |
                       flash_range_mask(BOOT_METADATA, PART_SECTOR_WORDS);
    /* Whitelist only configured APP/metadata sectors, never BOOT or gaps. */
    if (!mask || (mask & (uint16_t)~allowed)) return 0;
    boot_debug_flash_erase_mask = mask;
    DINT;
    boot_debug_flash_status = Flash_Erase(mask, (FLASH_ST *)&boot_debug_flash_detail);
    return boot_debug_flash_status == STATUS_SUCCESS;
}
static uint16_t api_program(uint32_t address, const uint16_t *data, uint16_t count)
{
    if (!flash_write_allowed(address, count)) return 0;
    DINT;
    boot_debug_flash_status = Flash_Program((Uint16 *)address, (Uint16 *)data,
                                           count, (FLASH_ST *)&boot_debug_flash_detail);
    if (boot_debug_flash_status != STATUS_SUCCESS) return 0;
    boot_debug_flash_status = Flash_Verify((Uint16 *)address, (Uint16 *)data,
                                          count, (FLASH_ST *)&boot_debug_flash_detail);
    return boot_debug_flash_status == STATUS_SUCCESS;
}
static uint16_t begin_image(void *user, const bl_manifest *m)
{
    uint16_t invalid[2] = {0, 0}, mask, metadata_mask;
    const volatile uint16_t *metadata = (const volatile uint16_t *)BOOT_METADATA;
    (void)user;
    /* Validate and plan the whole operation BEFORE changing the old image. */
    if (!flash_begin_plan(m->start, m->count, m->entry, &mask, &metadata_mask)) return 0;
    /* Only a committed image needs invalidation. On first download, erase metadata
     * before programming it; an erased/uncommitted marker is already invalid. */
    boot_debug_flash_stage = BOOT_FLASH_INVALIDATE;
    if (metadata[18] == 0xA55AU && metadata[19] == 0x5AA5U)
        if (!api_program(BOOT_METADATA + 18UL, invalid, 2)) return 0;
    boot_debug_flash_stage = BOOT_FLASH_ERASE_METADATA;
    if (!api_erase(metadata_mask)) return 0;
    boot_debug_flash_stage = BOOT_FLASH_ERASE_APP;
    return api_erase(mask);
}
static uint16_t program_image(void *user, uint32_t address, const uint16_t *data, uint16_t count)
{
    (void)user;
    if (address < BL_APP_START || address >= BL_APP_END || !count ||
        count > BL_APP_END - address || (address & 3UL) || (count & 3U)) return 0;
    boot_debug_flash_stage = BOOT_FLASH_PROGRAM_APP;
    return api_program(address, data, count);
}
static uint16_t verify_image(void *user, const bl_manifest *m)
{
    (void)user;
    return m->start == BL_APP_START && m->count && m->count <= BL_APP_END - BL_APP_START &&
           words_crc((const uint16_t *)m->start, m->count) == m->crc;
}
static uint16_t commit_image(void *user, const bl_manifest *m)
{
    uint16_t header[20];
    (void)user;
    header[0] = 0xB035U; header[1] = 0x4FCAU;
    put32(header + 2, BL_DEVICE_ID); put32(header + 4, BL_BOARD_ID);
    put32(header + 6, m->start); put32(header + 8, m->count);
    put32(header + 10, m->entry); put32(header + 12, m->crc);
    put32(header + 14, m->version); put32(header + 16, words_crc(header, 16));
    header[18] = 0xA55AU; header[19] = 0x5AA5U;
    boot_debug_flash_stage = BOOT_FLASH_PROGRAM_HEADER;
    if (!api_program(BOOT_METADATA, header, 18)) return 0;
    boot_debug_flash_stage = BOOT_FLASH_COMMIT;
    return api_program(BOOT_METADATA + 18UL, header + 18, 2);
}
uint16_t boot_image_valid(void)
{
    const uint16_t *p = (const uint16_t *)BOOT_METADATA;
    uint32_t count;
    if (p[18] != 0xA55AU || p[19] != 0x5AA5U || p[0] != 0xB035U || p[1] != 0x4FCAU ||
        get32(p + 2) != BL_DEVICE_ID || get32(p + 4) != BL_BOARD_ID ||
        get32(p + 6) != BL_APP_START || get32(p + 10) != BL_APP_START ||
        get32(p + 16) != words_crc(p, 16)) return 0;
    count = get32(p + 8);
    return count && !(count & 3UL) && count <= BL_APP_END - BL_APP_START &&
           words_crc((const uint16_t *)BL_APP_START, count) == get32(p + 12);
}
const bl_flash_ops boot_flash_ops = {0, begin_image, program_image, verify_image, commit_image};
