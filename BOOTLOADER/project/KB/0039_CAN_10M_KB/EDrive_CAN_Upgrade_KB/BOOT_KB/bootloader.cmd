#include "app_partition.h"
/* F280039C, all addresses/lengths are C28x 16-bit words. */
MEMORY
{
    BEGIN : origin = 0x080000, length = 0x000002
    BOOT_FLASH : origin = BOOT_CODE_START, length = BOOT_CODE_WORDS
    /* BOOT ends before explicit metadata at 0x084000; customer data follows. */
    RAMM1 : origin = 0x000400, length = 0x0003F8
    RAMLS : origin = 0x008000, length = 0x004000
    RAMGS : origin = 0x00C000, length = 0x003FF8
    RESET : origin = 0x3FFFC0, length = 0x000002
}
SECTIONS
{
    codestart : > BEGIN
    .text : > BOOT_FLASH, ALIGN(8)
    .cinit : > BOOT_FLASH, ALIGN(8)
    .const : > BOOT_FLASH, ALIGN(8)
    .switch : > BOOT_FLASH, ALIGN(8)
    .init_array : > BOOT_FLASH, ALIGN(8)
    .stack : > RAMM1
    .bss : > RAMGS
    .bss:output : > RAMGS
    .bss:cio : > RAMGS
    .data : > RAMGS
    .sysmem : > RAMGS
    .reset : > RESET, TYPE = DSECT
    GROUP {
        .TI.ramfunc { -l FAPI_F28003x_EABI_v1.58.10.lib }
    } LOAD = BOOT_FLASH, RUN = RAMLS,
      LOAD_START(RamfuncsLoadStart), LOAD_SIZE(RamfuncsLoadSize),
      RUN_START(RamfuncsRunStart), ALIGN(8)
}
