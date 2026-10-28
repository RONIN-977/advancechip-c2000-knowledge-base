#include "driverlib.h"
// AVP32F0049 / TI TMS320F280049C；经典CAN，仅时序配置
// XTAL=20 MHz; SYSCLK=100 MHz; CANCLK=100 MHz
// CANCLK须已在工程中正确选择；不修改PLL、GPIO、邮箱、中断或外设时钟使能。
// 实际=125 kbit/s; 采样点=80.000%; SJW=1 TQ
// 以下字段/API参数均为编码值；物理分频/时间段数减1。运行中修改将中断CAN通信。
// 依赖匹配型号 C2000Ware driverlib.h、driverlib.lib。
// CAN_setBitTiming内部设置INIT/CCE并恢复原INIT状态；不启动/初始化其余外设。
void FAE_configCanBitTiming(void)
{
    CAN_setBitTiming(CANA_BASE, 39U, 0U, 14U, 3U, 0U);
}