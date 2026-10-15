
/****************************************************************
文件功能：变频冰箱驱动参数辨识函数
文件版本：V1.0
作者          ：徐中领
最新更新：2015-11-10

****************************************************************/

#include "MainInclude.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"
#include "param_lr.h"


unsigned long	l_ld_est;
unsigned long	l_lq_est;
unsigned long	l_ld;
unsigned long	l_lq;

unsigned int	l_start_cal_flag;
unsigned int    l_disable_dbc;


unsigned long 	calc_lbc();
int  			orient_random_angel(unsigned int angel);
int  			get_lbc();


void reset_peripherals()
{
	EPwm2Regs.AQCSFRC.all = 0;
	EPwm3Regs.AQCSFRC.all = 0;
	EPwm4Regs.AQCSFRC.all = 0;
	
	InitSetPWM();
	InitSetAdc();

	AdcRegs.ADCOFFTRIM.all = gADC.Comp;

	return;
}
