#include "f28004x_device.h"
// AVP32F0049 / TI TMS320F280049C；仅PWM时基参数
// SYSCLK=100 MHz; EPWMCLK=SYSCLK/1; TBCLK=25000000 Hz
// EPWMCLK分频须已在工程配置；不更改系统时钟、GPIO、动作限定、死区、中断。
// 实际频率=10000 Hz；TBPRD=2499。请在时基停止/初始化阶段调用。
// 依赖匹配型号的位域总头文件；示例实例EPwm1Regs。
void FAE_configPwmTimeBase(void)
{
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 2;
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    EPwm1Regs.TBPRD = 2499;
    EPwm1Regs.TBCTL.bit.CTRMODE = 0;
}