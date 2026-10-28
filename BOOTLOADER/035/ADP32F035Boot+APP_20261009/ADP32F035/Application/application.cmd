#include "partition_config.h"
MEMORY
{
PAGE 0:
    BEGIN     : origin = PART_APP_START, length = 0x000002
    APPFLASH  : origin = (PART_APP_START + 2), length = (PART_APP_END - PART_APP_START - 2)
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
    ramfuncs : LOAD = APPFLASH, RUN = RAMCODE, PAGE = 0,
      LOAD_START(_RamfuncsLoadStart), LOAD_SIZE(_RamfuncsLoadSize), RUN_START(_RamfuncsRunStart)
    .text    : > APPFLASH, PAGE = 0
    .cinit   : > APPFLASH, PAGE = 0
    .pinit   : > APPFLASH, PAGE = 0
    .econst  : > APPFLASH, PAGE = 0
    .switch  : > APPFLASH, PAGE = 0
    .stack   : > RAMM1, PAGE = 1
    .ebss    : > RAMDATA, PAGE = 1
    .esysmem : > RAMM0, PAGE = 1
    .reset   : > RESET, PAGE = 0, TYPE = DSECT
}
