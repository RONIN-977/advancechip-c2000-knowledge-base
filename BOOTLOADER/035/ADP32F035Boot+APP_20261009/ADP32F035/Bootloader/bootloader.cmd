#include "partition_config.h"
MEMORY
{
PAGE 0:
    BOOTFLASH : origin = PART_BOOT_START, length = (0x3F7F80 - PART_BOOT_START)
    BEGIN     : origin = 0x3F7FF6, length = 0x000002
    RAMCODE   : origin = 0x008000, length = 0x001000
    RESET     : origin = 0x3FFFC0, length = 0x000002
PAGE 1:
    RAMM0     : origin = 0x000050, length = 0x0003A8
    REQUEST   : origin = 0x0003F8, length = 0x000004
    RAMM1     : origin = 0x000400, length = 0x000400
    RAMDATA   : origin = 0x009000, length = 0x001000
}
SECTIONS
{
    codestart : > BEGIN, PAGE = 0
    /* Flash API functions are absolute chip-ROM symbols. No load image. */
    ramfuncs : LOAD = BOOTFLASH, RUN = RAMCODE, PAGE = 0,
      LOAD_START(_RamfuncsLoadStart), LOAD_SIZE(_RamfuncsLoadSize), RUN_START(_RamfuncsRunStart)
    .text    : > BOOTFLASH, PAGE = 0
    .cinit   : > BOOTFLASH, PAGE = 0
    .pinit   : > BOOTFLASH, PAGE = 0
    .econst  : > BOOTFLASH, PAGE = 0
    .switch  : > BOOTFLASH, PAGE = 0
    .stack   : > RAMM1, PAGE = 1
    .ebss    : > RAMDATA, PAGE = 1
    .esysmem : > RAMM0, PAGE = 1
    .reset   : > RESET, PAGE = 0, TYPE = DSECT
}
