#include "f28003x_device.h"
// AVP32F0039 / TI TMS320F280039C；仅PWM时基参数
// SYSCLK=120 MHz; EPWMCLK=SYSCLK/1; TBCLK=30000000 Hz
// EPWMCLK分频须已在工程配置；不更改系统时钟、GPIO、动作限定、死区、中断。
// 实际频率=5000 Hz；TBPRD=3000。请在时基停止/初始化阶段调用。
// 依赖匹配型号的位域总头文件；示例实例EPwm1Regs。
void FAE_configPwmTimeBase(void)
{
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 2;
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    EPwm1Regs.TBPRD = 3000;
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
}