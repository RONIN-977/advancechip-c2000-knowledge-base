#include "driverlib.h"
// AVP32F0039 / TI TMS320F280039C；仅PWM时基参数
// SYSCLK=120 MHz; EPWMCLK=SYSCLK/1; TBCLK=30000000 Hz
// EPWMCLK分频须已在工程配置；不更改系统时钟、GPIO、动作限定、死区、中断。
// 实际频率=5000 Hz；TBPRD=5999。请在时基停止/初始化阶段调用。
// 依赖匹配型号的driverlib.h和driverlib.lib；示例实例EPWM1。
void FAE_configPwmTimeBase(void)
{
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_4);
    EPWM_setTimeBasePeriod(EPWM1_BASE, 5999U);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP);
}