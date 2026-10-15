
#include "MotorInclude.h"
#include "Main.h"
#include "Runsrc.h"
#include "Parameter.h"

/*************************************************************
**************************************************************
	从功能模块获取需要的所有参数
*************************************************************/
void ParGetFromFunction(void)
{
	int16 data;
	int32 freqs,data1,data2,ki,kp;

	// 速度环PI 计算....
	// 额定频率，50Hz,>>6 = 0.78Hz...,Freqs with 0.01Hz units...
	// 目前低速/高速偏差设置为 1Hz 2Hz 是不是有点偏小............
	data1 = pm_ref_speed;
	freqs = labs(data1)>>4;
	ki = LOW_SPEED_KI * 3;

	if (freqs <= 100) {
		// 200Hz, then 3.12Hz below，低速KI 增强，KP不变。
		pm_ki_speed = LOW_SPEED_KI * 6;
		pm_kp_speed = LOW_SPEED_KP;

		pm_speed_lpf_k = LOW_SPEED_FILTER;
	} else if(freqs < F_CHANGING_POINT_1_PI) {
		// 小于第一个切换点，KI 增大3倍，KP不变。
		//////////////////////////////////////
		pm_ki_speed = ki;
		pm_kp_speed = LOW_SPEED_KP;

		pm_speed_lpf_k = LOW_SPEED_FILTER;
	} else if(freqs >= F_CHANGING_POINT_2_PI) {

		// 大于第二个切换点，直接用高速PI。。
		pm_ki_speed = HIGH_SPEED_KI_GAIN;
		pm_kp_speed = HIGH_SPEED_KP_GAIN;
		pm_speed_lpf_k = SPEED_FEEDBACK_FILTER;
	} else {
		pm_speed_lpf_k = LOW_SPEED_FILTER;
		data1 = (int32)HIGH_SPEED_KI_GAIN - (int32)ki;
		data1 = data1 * (freqs - (int32)F_CHANGING_POINT_1_PI);
		data2 = (int32)F_CHANGING_POINT_2_PI - (int32)F_CHANGING_POINT_1_PI;
		data1 = data1 / data2;
		pm_ki_speed = (int32)ki + data1;
		if(pm_ki_speed < 0)
		{
			pm_ki_speed = 0;
		}

		data1 = (int32)HIGH_SPEED_KP_GAIN - (int32)LOW_SPEED_KP;
		data1 = data1 * (freqs - (int32)F_CHANGING_POINT_1_PI);
		data2 = (int32)F_CHANGING_POINT_2_PI - (int32)F_CHANGING_POINT_1_PI;
		data1 = data1 / data2;
		pm_kp_speed = (int32)LOW_SPEED_KP + data1;
		if(pm_kp_speed < 0)
		{
			pm_kp_speed = 0;
		}
	}

	// 转矩限制方向？
	if (pm_forbid_revtorq == 0)
	{  
		data = abs(gMainCmd.VCTorqueLim);
		pm_curr_limitH = (int32)data * 4096/1000;
		pm_curr_limitL = -pm_curr_limitH;	
	}
	else
	{
		data = abs(gMainCmd.VCTorqueLim);
		if(gMainCmd.FreqSetApply >= 0)
		{
			pm_curr_limitH = (int32)data * 4096/1000;
			pm_curr_limitL = 0;
		}
		else
		{
			pm_curr_limitH = 0;
			pm_curr_limitL = -(int32)data * 4096/1000;
		}
	}

	// LD,LQ,RS 单位问题...
	pm_r  = (Uint32)PMSM_RS;
	l_ld  = (Uint32)PMSM_LD;
	data1 = (Uint32)PMSM_LQ;
	l_lq  = (int32)l_ld + (data1 - (int32)l_ld) / 3;
	pm_bem_kf = PMSM_KE;

	// DQ 轴电流环PI 系数,KP KI
	pm_ki_d = (int32)D_CUR_KI * 10;
    pm_ki_q = (int32)Q_CUR_KI * 10;
    kp = (int32)D_CUR_KP * 10;
    pm_kp_d = kp;
    kp = (int32)Q_CUR_KP * 10;
    pm_kp_q = kp;

    if ((gMainCmd.Command.bit.StartDC   == 1                 ) ||
        (gMainStatus.RunStep            == OPEN_ACC          ) ||
        (gMainStatus.RunStep            == HALF_OPEN_ACC     ))
    {
        pm_kp_d = pm_kp_d >> 1;
        pm_kp_q = pm_kp_q >> 1;
        pm_ki_d = pm_ki_d >> 1;
        pm_ki_q = pm_ki_q >> 1;
    } else if (gMainStatus.RunStep      == STATUS_SPEED_CHECK) {
        pm_kp_d = pm_kp_d << 1;
        pm_kp_q = pm_kp_q << 1;
        pm_ki_d = pm_ki_d << 1;
        pm_ki_q = pm_ki_q << 1;
    }
}

/*************************************************************
    把电机运行的实时参数传送给功能模块
*************************************************************/
void ParSendToFunction(void)
{
    dspStatus.all       = gMainStatus.StatusWord.all;
    errorCodeFromCore   = gMainStatus.ErrorCode;
}
