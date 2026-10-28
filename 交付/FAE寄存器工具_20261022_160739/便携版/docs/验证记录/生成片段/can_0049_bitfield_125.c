#include "f28004x_device.h"
// AVP32F0049 / TI TMS320F280049C；经典CAN，仅时序配置
// XTAL=20 MHz; SYSCLK=100 MHz; CANCLK=100 MHz
// CANCLK须已在工程中正确选择；不修改PLL、GPIO、邮箱、中断或外设时钟使能。
// 实际=125 kbit/s; 采样点=80.000%; SJW=1 TQ
// 以下字段/API参数均为编码值；物理分频/时间段数减1。运行中修改将中断CAN通信。
// 依赖匹配型号位域总头文件，CanaRegs；保留原控制寄存器与INIT状态。
void FAE_configCanBitTiming(void)
{
    unsigned long savedCtl;
    union CAN_BTR_REG btr;
    savedCtl = CanaRegs.CAN_CTL.all;
    CanaRegs.CAN_CTL.all = savedCtl | 0x41UL; /* INIT=1, CCE=1 */
    btr.all = 0UL;
    btr.bit.BRP = 39;
    btr.bit.BRPE = 0;
    btr.bit.TSEG1 = 14;
    btr.bit.TSEG2 = 3;
    btr.bit.SJW = 0;
    CanaRegs.CAN_BTR.all = btr.all;
    CanaRegs.CAN_CTL.all = savedCtl & ~0x40UL; /* 清CCE并恢复INIT */
}