#include "DSP2803x_Device.h"
// ADP32F035 / TI TMS320F28035；经典CAN，仅时序配置
// XTAL=20 MHz; SYSCLK=60 MHz; CANCLK=30 MHz
// CANCLK须已在工程中正确选择；不修改PLL、GPIO、邮箱、中断或外设时钟使能。
// 实际=125 kbit/s; 采样点=80.000%; SJW=1 TQ
// 以下字段/API参数均为编码值；物理分频/时间段数减1。运行中修改将中断CAN通信。
// 依赖匹配型号的位域总头文件；ECanaRegs；CANMC/CANBTC的32位shadow写入。
void FAE_configCanBitTiming(void)
{
    union CANMC_REG mc;
    union CANBTC_REG btc;
    unsigned long savedCcr;
    EALLOW;
    mc.all = ECanaRegs.CANMC.all;
    savedCcr = mc.bit.CCR;
    mc.bit.CCR = 1;
    ECanaRegs.CANMC.all = mc.all;
    while (ECanaRegs.CANES.bit.CCE != 1) { }
    btc.all = 0UL; /* CANBTC保留位按TRM写0 */
    btc.bit.BRPREG = 11;
    btc.bit.TSEG1REG = 14;
    btc.bit.TSEG2REG = 3;
    btc.bit.SJWREG = 0;
    btc.bit.SAM = 0; /* 单次采样 */
    ECanaRegs.CANBTC.all = btc.all;
    mc.all = ECanaRegs.CANMC.all;
    mc.bit.CCR = savedCcr;
    ECanaRegs.CANMC.all = mc.all;
    if (savedCcr == 0) { while (ECanaRegs.CANES.bit.CCE != 0) { } }
    EDIS;
}