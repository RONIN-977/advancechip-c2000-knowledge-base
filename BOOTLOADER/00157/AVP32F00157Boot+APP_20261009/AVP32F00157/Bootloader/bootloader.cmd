#include "app_partition.h"
MEMORY
{
    BEGIN     : origin = 0x080000, length = 0x000002
    BOOTFLASH : origin = BOOT_CODE_START, length = BOOT_CODE_WORDS
    RAMM0     : origin = 0x000128, length = 0x0002D0
    REQUEST   : origin = 0x0003F8, length = 0x000004
    RAMM1     : origin = 0x000400, length = 0x0003F8
    RAMCODE   : origin = 0x008000, length = 0x002000
    RAMDATA   : origin = 0x00A000, length = 0x001FF8
    RESET     : origin = 0x3FFFC0, length = 0x000002
}
SECTIONS
{
    codestart : > BEGIN, ALIGN(4)
    GROUP
    {
        .TI.ramfunc
        { -l FAPI_F280015x_EABI_v2.00.10.lib }
    } LOAD = BOOTFLASH, RUN = RAMCODE,
      LOAD_START(RamfuncsLoadStart), LOAD_SIZE(RamfuncsLoadSize),
      RUN_START(RamfuncsRunStart), ALIGN(8)
    .text       : > BOOTFLASH, ALIGN(8)
    .cinit      : > BOOTFLASH, ALIGN(8)
    .const      : > BOOTFLASH, ALIGN(8)
    .switch     : > BOOTFLASH, ALIGN(8)
    .init_array : > BOOTFLASH, ALIGN(8)
    .bss        : > RAMDATA
    .bss:output : > RAMDATA
    .bss:cio    : > RAMDATA
    .data       : > RAMDATA
    .sysmem     : > RAMM0
    .stack      : > RAMM1
    .reset      : > RESET, TYPE=DSECT
}
