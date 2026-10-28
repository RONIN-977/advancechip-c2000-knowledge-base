#include "app_partition.h"
/* App must be RE-LINKED here before conversion. A file built for 0x080000 is rejected. */
MEMORY
{
    BEGIN : origin = APP_START_ADDRESS, length = 0x000002
    APP_FLASH : origin = (APP_START_ADDRESS + 2), length = (APP_END_ADDRESS - APP_START_ADDRESS - 2)
    RAMM1 : origin = 0x000400, length = 0x0003F8
    RAMLS : origin = 0x008000, length = 0x004000
    RAMGS : origin = 0x00C000, length = 0x003FF8
    RESET : origin = 0x3FFFC0, length = 0x000002
}
SECTIONS
{
    codestart : > BEGIN
    .text : > APP_FLASH, ALIGN(8)
    .cinit : > APP_FLASH, ALIGN(8)
    .const : > APP_FLASH, ALIGN(8)
    .switch : > APP_FLASH, ALIGN(8)
    .init_array : > APP_FLASH, ALIGN(8)
    .stack : > RAMM1
    .bss : > RAMGS
    .bss:output : > RAMGS
    .bss:cio : > RAMGS
    .data : > RAMGS
    .sysmem : > RAMGS
    IQmath : > RAMLS
    IQmathTables : > RAMLS
    ramgs0 : > RAMGS
    .reset : > RESET, TYPE = DSECT
    /* Customer NVM Flash API also executes from RAM when later restored. */
    GROUP {
        .TI.ramfunc { -l FAPI_F28003x_EABI_v1.58.10.lib }
    } LOAD = APP_FLASH, RUN = RAMLS,
      LOAD_START(RamfuncsLoadStart), LOAD_SIZE(RamfuncsLoadSize),
      RUN_START(RamfuncsRunStart), RUN_END(RamfuncsRunEnd),
      LOAD_END(RamfuncsLoadEnd), RUN_SIZE(RamfuncsRunSize), ALIGN(8)
}
