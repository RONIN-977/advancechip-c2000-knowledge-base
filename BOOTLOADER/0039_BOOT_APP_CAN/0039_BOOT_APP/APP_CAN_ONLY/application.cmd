/* [K10 客户整合] 本链接文件仍为升级示例分区，不直接覆盖客户 .cmd。
 * 合并 APP 入口 0x090000、Flash 上界 0x0AF000 和 RAM 0x3F8..0x3FB 预留；
 * 客户 CLA/DMA/控制变量等段按真实客户工程保留，BOOT 必须配套。
 */
/* [移植必需 合并到客户 .cmd，不盲目覆盖整个客户链接文件]
 * 1. code_start/codestart 固定在 0x090000，Flash LOAD 段在 [0x090000,0x0AF000)。
 * 2. 排除 M0 的 0x3F8..0x3FB 请求标记，客户栈/变量/启动清零不得覆盖。
 * 3. 保留客户 CLA/DMA/RAM 函数 LOAD/RUN 布局；全工程仅一份启动汇编。
 */
/* App must be RE-LINKED here before conversion. A file built for 0x080000 is rejected. */
MEMORY
{
    BEGIN : origin = 0x090000, length = 0x000002
    APP_FLASH : origin = 0x090002, length = 0x01EFFE
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
    .reset : > RESET, TYPE = DSECT
    .TI.ramfunc : LOAD = APP_FLASH, RUN = RAMLS,
      LOAD_START(RamfuncsLoadStart), LOAD_SIZE(RamfuncsLoadSize),
      RUN_START(RamfuncsRunStart), ALIGN(8)
}
