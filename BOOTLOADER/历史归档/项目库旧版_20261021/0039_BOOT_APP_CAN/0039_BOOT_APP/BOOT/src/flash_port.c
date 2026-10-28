/**
 * @file flash_port.c
 * @brief F280039C Flash 适配层：擦除、4 字编程、CRC/ECC 校验和有效标记提交。
 * 地址均为 C28x 字地址。擦写期间的执行路径必须留在 RAM 中。
 */
#include "driverlib.h"
#include "device.h"
#include "F021_F28003x_C28x.h"
#include "board_config.h"
#include "flash_port.h"

/* Every function in this file executes from RAM. Firmware is linked with the
 * complete Flash API in the same RAM group. CPU interrupts remain disabled. */
#pragma CODE_SECTION(wait_ready, ".TI.ramfunc")
#pragma CODE_SECTION(boot_flash_init, ".TI.ramfunc")
#pragma CODE_SECTION(erase_sector, ".TI.ramfunc")
#pragma CODE_SECTION(program_four, ".TI.ramfunc")
#pragma CODE_SECTION(begin_image, ".TI.ramfunc")
#pragma CODE_SECTION(program_image, ".TI.ramfunc")
#pragma CODE_SECTION(verify_image, ".TI.ramfunc")
#pragma CODE_SECTION(commit_image, ".TI.ramfunc")
#pragma CODE_SECTION(boot_image_valid, ".TI.ramfunc")
#pragma CODE_SECTION(word_crc, ".TI.ramfunc")
#pragma CODE_SECTION(ecc_valid, ".TI.ramfunc")
#pragma CODE_SECTION(put_long, ".TI.ramfunc")
#pragma CODE_SECTION(get_long, ".TI.ramfunc")
#pragma CODE_SECTION(boot_flash_nmi, ".TI.ramfunc")

__interrupt void boot_flash_nmi(void)
{
    /* NMI is not masked by DINT. Keep the emergency path in RAM, including
     * during metadata-sector erase in the bootloader's own bank. */
    SysCtl_resetDevice();
    for (;;)
    {
    }
}

static uint16_t wait_ready(void)
{
    uint32_t start = CPUTimer_getTimerCount(CPUTIMER2_BASE);
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady)
    {
        if ((uint32_t)(start - CPUTimer_getTimerCount(CPUTIMER2_BASE)) > DEVICE_SYSCLK_FREQ * 5UL)
        {
            SysCtl_resetDevice();
            for (;;)
            {
            } /* Never return into flash while the FSM is busy. */
        }
    }
    return Fapi_getFsmStatus() == 0;
}

uint16_t boot_flash_init(void)
{
    Fapi_StatusType result;
    EALLOW;
    result = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, DEVICE_SYSCLK_FREQ / 1000000UL);
    Flash_disablePrefetch(FLASH0CTRL_BASE);
    FLASH_DELAY_CONFIG;
    if (result == Fapi_Status_Success)
    {
        result = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    }
    Flash_enablePrefetch(FLASH0CTRL_BASE);
    FLASH_DELAY_CONFIG;
    EDIS;
    return result == Fapi_Status_Success;
}

/* 只允许元数据扇区和应用扇区，禁止擦除 Bootloader、OTP 或 DCSM。 */
static uint16_t erase_sector(uint32_t address)
{
    Fapi_StatusType result;
    Fapi_FlashStatusWordType check;
    uint16_t ready;
    if (address != BOOT_METADATA && (address < BL_APP_START || address >= BL_APP_END))
    {
        return 0;
    }
    if (address & 0xFFFUL)
    {
        return 0;
    }
    EALLOW;
    result = Fapi_issueAsyncCommand(Fapi_ClearMore);
    ready = wait_ready();
    if (result == Fapi_Status_Success && ready)
    {
        result = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)address);
        ready = wait_ready();
    }
    if (result == Fapi_Status_Success && ready)
    {
        result = Fapi_doBlankCheck((uint32 *)address, 0x800UL, &check);
    }
    Fapi_flushPipeline();
    EDIS;
    return result == Fapi_Status_Success && ready;
}

/* 每次编程 4 个 16 位字（一个 64 位 ECC 单元），随后立即读回验证。
 * 同一 ECC 单元在一次擦除后只能编程一次，协议重试不能重新调用此函数。
 */
static uint16_t program_four(uint32_t address, const uint16_t *words)
{
    Fapi_StatusType result;
    Fapi_FlashStatusWordType check;
    uint16_t ready, buffer[4];
    uint32_t expected[2];
    uint16_t i;
    if ((address & 3UL) || !((address >= BL_APP_START && address <= BL_APP_END - 4UL) ||
                             (address >= BOOT_METADATA && address <= BOOT_METADATA + 20UL)))
    {
        return 0;
    }
    for (i = 0; i < 4; ++i)
    {
        buffer[i] = words[i];
    }
    expected[0] = (uint32_t)words[0] | ((uint32_t)words[1] << 16);
    expected[1] = (uint32_t)words[2] | ((uint32_t)words[3] << 16);
    EALLOW;
    result =
        Fapi_issueProgrammingCommand((uint32 *)address, buffer, 4, 0, 0, Fapi_AutoEccGeneration);
    ready = wait_ready();
    if (result == Fapi_Status_Success && ready)
    {
        result = Fapi_doVerify((uint32 *)address, 2, expected, &check);
    }
    Fapi_flushPipeline();
    EDIS;
    return result == Fapi_Status_Success && ready;
}

static uint16_t begin_image(void *user, const bl_manifest *image)
{
    uint32_t address, end = image->start + image->count;
    (void)user;
    /* Durable invalidation must finish and blank-check before app destruction. */
    if (!erase_sector(BOOT_METADATA))
    {
        return 0;
    }
    for (address = BL_APP_START; address < end; address += 0x1000UL)
    {
        if (!erase_sector(address))
        {
            return 0;
        }
    }
    return 1;
}

static uint16_t program_image(void *user, uint32_t address, const uint16_t *words, uint16_t count)
{
    uint16_t i;
    (void)user;
    if (!count || (count & 3U) || address < BL_APP_START || address >= BL_APP_END ||
        count > BL_APP_END - address)
    {
        return 0;
    }
    for (i = 0; i < count; i += 4)
    {
        if (!program_four(address + i, words + i))
        {
            return 0;
        }
    }
    return 1;
}

/* 每个 Flash 字按低字节、高字节顺序计算 CRC，与上位机镜像字节流一致。 */
static uint32_t word_crc(const volatile uint16_t *words, uint32_t count)
{
    uint32_t crc = 0xFFFFFFFFUL, i;
    uint16_t word, j, k;
    for (i = 0; i < count; ++i)
    {
        word = words[i];
        for (k = 0; k < 2; ++k)
        {
            crc ^= word & 255U;
            word >>= 8;
            for (j = 0; j < 8; ++j)
            {
                crc = (crc >> 1) ^ ((crc & 1UL) ? 0xEDB88320UL : 0UL);
            }
        }
    }
    return ~crc;
}

/* Read with hardware ECC checking off so a torn write cannot cause an NMI.
 * Compare the stored ECC to the API-generated ECC before trusting the image. */
static uint16_t ecc_valid(uint32_t address, uint32_t count)
{
    uint32_t pos;
    uint16_t stored, expected;
    uint64_t value;
    for (pos = address; pos < address + count; pos += 4)
    {
        value = *(volatile uint64_t *)pos;
        expected = Fapi_calculateEcc(pos, value);
        if (expected > 255U)
        {
            return 0;
        }
        stored = *(volatile uint16_t *)(0x01080000UL + ((pos - 0x080000UL) >> 3));
        stored = (pos & 4UL) ? (stored >> 8) : (stored & 255U);
        if (stored != expected)
        {
            return 0;
        }
    }
    return 1;
}

static uint16_t verify_image(void *user, const bl_manifest *image)
{
    uint16_t valid;
    (void)user;
    EALLOW;
    Flash_disableECC(FLASH0ECC_BASE);
    Fapi_flushPipeline();
    valid = word_crc((volatile uint16_t *)image->start, image->count) == image->crc &&
            ecc_valid(image->start, image->count);
    Flash_enableECC(FLASH0ECC_BASE);
    EDIS;
    return valid;
}

static void put_long(uint16_t *p, uint32_t value)
{
    p[0] = (uint16_t)value;
    p[1] = (uint16_t)(value >> 16);
}

static uint32_t get_long(const uint16_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 16);
}

/* 元数据头共 20 字；最后 4 字为独立有效标记，必须最后写入。
 * 任何中途掉电都不能把未完成的应用识别为有效镜像。
 */
static uint16_t commit_image(void *user, const bl_manifest *image)
{
    uint16_t record[24], i;
    (void)user;
    put_long(record, 0x424C3031UL);
    put_long(record + 2, BL_DEVICE_ID);
    put_long(record + 4, BL_BOARD_ID);
    put_long(record + 6, BL_CPU);
    put_long(record + 8, image->start);
    put_long(record + 10, image->count);
    put_long(record + 12, image->entry);
    put_long(record + 14, image->crc);
    put_long(record + 16, image->version);
    put_long(record + 18, word_crc(record, 18));
    record[20] = 0xA55A;
    record[21] = 0x5AA5;
    record[22] = 0xC33C;
    record[23] = 0x3CC3;
    for (i = 0; i < 20; i += 4)
    {
        if (!program_four(BOOT_METADATA + i, record + i))
        {
            return 0;
        }
    }
    /* Independent final ECC granule: programmed only after full verification. */
    if (!program_four(BOOT_METADATA + 20UL, record + 20))
    {
        return 0;
    }
    return boot_image_valid();
}

/* 临时关闭硬件 ECC，用软件检查撕裂写入，避免检查损坏镜像时触发 NMI。
 * 返回前恢复 ECC；身份、边界、头 CRC、数据 CRC、ECC 全部通过才返回成功。
 */
uint16_t boot_image_valid(void)
{
    uint16_t record[24], i, valid = 0;
    bl_manifest image;
    EALLOW;
    Flash_disableECC(FLASH0ECC_BASE);
    Fapi_flushPipeline();
    for (i = 0; i < 24; ++i)
    {
        record[i] = *(volatile uint16_t *)(BOOT_METADATA + i);
    }
    if (record[20] == 0xA55A && record[21] == 0x5AA5 && record[22] == 0xC33C &&
        record[23] == 0x3CC3 && get_long(record) == 0x424C3031UL &&
        get_long(record + 2) == BL_DEVICE_ID && get_long(record + 4) == BL_BOARD_ID &&
        get_long(record + 6) == BL_CPU && word_crc(record, 18) == get_long(record + 18) &&
        ecc_valid(BOOT_METADATA, 24))
    {
        image.start = get_long(record + 8);
        image.count = get_long(record + 10);
        image.entry = get_long(record + 12);
        image.crc = get_long(record + 14);
        if (image.start == BL_APP_START && image.entry == BL_APP_START && image.count &&
            !(image.count & 3UL) && image.count <= BL_APP_END - BL_APP_START)
        {
            valid = word_crc((volatile uint16_t *)image.start, image.count) == image.crc &&
                    ecc_valid(image.start, image.count);
        }
    }
    Flash_enableECC(FLASH0ECC_BASE);
    EDIS;
    return valid;
}

const bl_flash_ops boot_flash_ops = {0, begin_image, program_image, verify_image, commit_image};
