
/****************************************************************
文件功能：变频驱动参数辨识函数
文件版本：V1.0
作者        ：徐中领
最新更新：2015-11-10

****************************************************************/

#include "MainInclude.h"
#include "MotorInclude.h"



unsigned long	l_ld_est;
unsigned long	l_lq_est;
unsigned long	l_ld;
unsigned long	l_lq;



void reset_peripherals()
{
	EPwm2Regs.AQCSFRC.all = 0;
	EPwm3Regs.AQCSFRC.all = 0;
	EPwm4Regs.AQCSFRC.all = 0;
	
	InitSetPWM();
	InitSetAdc();

	return;
}
