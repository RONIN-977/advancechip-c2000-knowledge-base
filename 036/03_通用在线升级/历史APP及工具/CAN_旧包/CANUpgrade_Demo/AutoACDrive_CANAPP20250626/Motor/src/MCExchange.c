
#include "MotorInclude.h"
#include "param_lr.h"
#include "Main.h"
#include "Parameter.h"

/*************************************************************
**************************************************************
	从功能模块获取需要的所有参数
*************************************************************/
void ParGetFromFunction(void)
{
	int m_Index;
	int data;
	long freqs,data1,data2,ki,kp,udc;

	gDeadBand.CompMode = 1;
	pm_control_mode = PMSM_CONTROL_MODE;

	// 必须注意，这些参数有哪些是跟存储有关的...
	for(m_Index = 0;m_Index < FUNCTION_TO_MOTOR_DATA_NUM;m_Index++)
	{
		*gSendToMotorTable[m_Index] = gSendToMotorDataBuff[m_Index];
	}

	if(gMainCmd.Command.bit.TunePart != 0)
	{
		return;
	}

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
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
	} else if(freqs < F_CHANGING_POINT_1_PI) {
		// 小于第一个切换点，KI 增大3倍，KP不变。
		//////////////////////////////////////
		pm_ki_speed = ki;
		pm_kp_speed = LOW_SPEED_KP;

		pm_speed_lpf_k = LOW_SPEED_FILTER;
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
	} else if(freqs >= F_CHANGING_POINT_2_PI) {

		// 大于第二个切换点，直接用高速PI。。
		pm_ki_speed = HIGH_SPEED_KI_GAIN;
		pm_kp_speed = HIGH_SPEED_KP_GAIN;
		pm_speed_lpf_k = SPEED_FEEDBACK_FILTER;
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
	} else {

		// 一二频率切换点之间，差值计算....
		// 分析：
		// 之前用 0 ~ 4Hz: P = 20, I = 300
		// 4 ~ 10Hz:      P = 15, I = 150
		// > 10Hz:        P = 10, I = 100
		// 现在考虑。
		// FE.30 = 4HZ. FE.33 = 40HZ
		// FE.04 = 10
		// FE.05 = 100
		// FE.31 = 30
		// FE.32 = 100
		/////////////////////////////////
		pm_speed_lpf_k = LOW_SPEED_FILTER;
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
		data1 = (long)HIGH_SPEED_KI_GAIN - (long)ki;
		data1 = data1 * (freqs - (long)F_CHANGING_POINT_1_PI);
		data2 = (long)F_CHANGING_POINT_2_PI - (long)F_CHANGING_POINT_1_PI;
		data1 = data1 / data2;
		pm_ki_speed = (long)ki + data1;
		if(pm_ki_speed < 0)
		{
			pm_ki_speed = 0;
		}

		data1 = (long)HIGH_SPEED_KI_GAIN - (long)LOW_SPEED_KP;
		data1 = data1 * (freqs - (long)F_CHANGING_POINT_1_PI);
		data2 = (long)F_CHANGING_POINT_2_PI - (long)F_CHANGING_POINT_1_PI;
		data1 = data1 / data2;
		pm_kp_speed = (long)LOW_SPEED_KP + data1;
		if(pm_kp_speed < 0)
		{
			pm_kp_speed = 0;
		}
	}

	// 转矩限制方向？
	if(pm_forbid_revtorq == 0)
	{  
		data = abs(gMainCmd.VCTorqueLim);
		pm_curr_limitH = (long)data * 4096/1000;
		pm_curr_limitL = -pm_curr_limitH;	
	}
	else
	{
		data = abs(gMainCmd.VCTorqueLim);
		if(gMainCmd.FreqSetApply >= 0)
		{
			pm_curr_limitH = (long)data * 4096/1000;
			pm_curr_limitL = 0;
		}
		else
		{
			pm_curr_limitH = 0;
			pm_curr_limitL = -(long)data * 4096/1000;
		}
	}

	// LD,LQ,RS 单位问题...
	data = PMS_L_R_UNIT%10;
	if(data == 0)
	{
		l_ld = (unsigned long)PMSM_LD;
		data1 = (unsigned long)PMSM_LQ;
	}
	if(data == 1)
	{
		l_ld = (unsigned long)PMSM_LD * 10;
		data1 = (unsigned long)PMSM_LQ * 10;
	}
	if( data == 2)
	{
		l_ld = (unsigned long)PMSM_LD * 100;
		data1 = (unsigned long)PMSM_LQ * 100;
	}
	l_lq =  (long)l_ld + (data1 - (long)l_ld)/3;


    data = PMS_L_R_UNIT/10;
    if(data == 0)
    {
        pm_r = (unsigned long)PMSM_RS;
    }
    else
    {
        pm_r = (unsigned long)PMSM_RS * 10;
    }


	pm_bem_kf = PMSM_KE;

	// DQ 轴电流环PI 系数,pm_especial_pi 对于小电感电机，放大电流环参数
	// 此处不涉及....
	///////////////////////////////////////////////////////////////////
	pm_ki_d = (unsigned long)D_CUR_KI * 10;
    pm_ki_q = (unsigned long)Q_CUR_KI * 10;
	if(pm_especial_pi == 0)
	{
		data2 = 64;
	} else {
		if(gLineCur.CurBaseInv >= 4096) {
			data2 = 64;
		} else {
			udc = gUDC.uDCFilter;
			if(udc > 5600) {
				udc = 5600;
			}
			data1 = l_ld * (gInvInfo.InvCurrent+gMotorInfo.Current)>>7;
			data1 = 4000L * 250L/data1;
			data1 = data1 * udc>>13;
			data1 = __IQsat(data1,350,66);
			data1 = __IQsat(data1,pm_especial_pi*7+64,66);
			data2 = gLineCur.CurBaseInvFilter>>9;
			if(data2 > 4096) {
				data2 = 64;
			} else {
				data2 = data1 + ((64-data1)*data2>>12);
			}
			data2 = __IQsat(data2,data1,64);
		}
		data2 = __IQsat(data2,data1,64);
	}
    if(gBasePar.FcSetApply <= 40)
    {
        kp = (long)D_CUR_KP * gBasePar.FcSetApply>>2;
		pm_kp_d = kp * data2>>6;
        kp = (long)Q_CUR_KP * gBasePar.FcSetApply>>2;
		pm_kp_q = kp * data2>>6;
    } else {
        kp = (long)D_CUR_KP * 10;
		pm_kp_d = kp * data2>>6;
        kp = (long)Q_CUR_KP * 10;
		pm_kp_q = kp * data2>>6;
    }

    if (gMainCmd.Command.bit.StartDC == 1)
    {
        pm_ki_d = pm_ki_d >> 2;
        pm_ki_q = pm_ki_q >> 2;
        pm_kp_d = pm_kp_d >> 1;
        pm_kp_q = pm_kp_q >> 1;
    }
pm_ki_d >>= 2;
pm_ki_q >>= 2;
pm_kp_d >>= 2;
pm_kp_q >>= 2;

    if (gMainStatus.RunStep == OPEN_ACC || gMainCmd.Command.bit.StartDC == 1)
    {
        pm_ki_d >>= 2;
        pm_ki_q >>= 2;
        pm_kp_d >>= 2;
        pm_kp_q >>= 2;
    }
}
	
void CalParToFunction(void)
{

}

/*************************************************************
	把电机运行的实时参数传送给功能模块
*************************************************************/
void ParSendToFunction(void)		
{
	Uint m_Index;

	for (m_Index = 0;m_Index < MOTOR_TO_FUNCTION_DATA_NUM;m_Index++)
	{
		gSendToFunctionDataBuff[m_Index] = *gSendToFuncTable[m_Index];
	}
}

