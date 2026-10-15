
#include "MotorInclude.h"
#include "param_lr.h"
#include "Main.h"
#include "Parameter.h"
#include "StaticVModelObs.h"

/*************************************************************
**************************************************************
	从功能模块获取需要的所有参数
*************************************************************/
void ParGetFromFunction(void)
{
	int m_Index;
	int data;
	long freqs,data1,data2,ki,kp;

	// 必须注意，这些参数有哪些是跟存储有关的...
	for(m_Index = 0;m_Index < FUNCTION_TO_MOTOR_DATA_NUM;m_Index++)
	{
		*gSendToMotorTable[m_Index] = gSendToMotorDataBuff[m_Index];
	}

	// 速度环PI 计算....
	// 额定频率，50Hz,>>6 = 0.78Hz...,Freqs with 0.01Hz units...
	// 目前低速/高速偏差设置为 1Hz 2Hz 是不是有点偏小............
	data1 = pm_ref_speed;
	freqs = labs(data1)>>4;
	ki = LOW_SPEED_KI* 3;

	if (freqs <= 100) {
		// 200Hz, then 3.12Hz below，低速KI 增强，KP不变。
		pm_ki_speed = LOW_SPEED_KI * 6;
		pm_kp_speed = LOW_SPEED_KP;

		pm_speed_lpf_k = LOW_SPEED_FILTER;
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
#ifdef ENABLE_NEW_STATIC_OBS
		gEst.FilterCnt = FilterCntLowForOBS;
#endif
	} else if(freqs < F_CHANGING_POINT_1_PI) {
		// 小于第一个切换点，KI 增大3倍，KP不变。
		//////////////////////////////////////
		pm_ki_speed = ki;
		pm_kp_speed = LOW_SPEED_KP;

		pm_speed_lpf_k = LOW_SPEED_FILTER;
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
#ifdef ENABLE_NEW_STATIC_OBS
		gEst.FilterCnt = FilterCntLowForOBS;
#endif
	} else if(freqs >= F_CHANGING_POINT_2_PI) {

		// 大于第二个切换点，直接用高速PI。。
		pm_ki_speed = HIGH_SPEED_KI_GAIN;
		pm_kp_speed = HIGH_SPEED_KP_GAIN;
		pm_speed_lpf_k = SPEED_FEEDBACK_FILTER;
		pm_speed_lpf_k = __IQsat(pm_speed_lpf_k,1024,4);
#ifdef ENABLE_NEW_STATIC_OBS
		gEst.FilterCnt = FilterCntHighForOBS;
#endif
	} else {
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
#ifdef ENABLE_NEW_STATIC_OBS
		gEst.FilterCnt = FilterCntLowForOBS;
#endif
	}
#ifdef ENABLE_NEW_STATIC_OBS
    gEst.Lambda = LamdaValueForOBS;
    gEst.Alpha0 = AlfaValueForOBS;  // Alpha0 = 0.1*wbase(额定转速）. 200HZ, 2PI*200*0.1 = 125
#endif

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


	l_ld = (unsigned long)PMSM_LD;
	data1 = (unsigned long)PMSM_LQ;
	l_lq = (long)l_ld + (data1 - (long)l_ld) / 3;
    pm_r = (unsigned long)PMSM_RS;

#if 1
    pm_bem_kf = PMSM_KE;
#else
    int PMBemfKeUsed;
    int PMBemfKeUsed1;
    int PMBemfKeUsed2;
    static int PMBemfKeUsedOld = 102;
//////////////////////////////////////////////////////
    if ((labs(pm_est_omg)>>4) < 2000) {
        PMBemfKeUsed = (PMSM_KE*5) >> 2;//1.25
    } else if ((labs(pm_est_omg)>>4) > 5000) {
        PMBemfKeUsed = PMSM_KE;
    } else {
        PMBemfKeUsed = ((long)(5000 - (labs(pm_est_omg)>>4)) * PMSM_KE) / ((5000 - 2000)*2);
        PMBemfKeUsed += PMSM_KE;
        if (PMBemfKeUsed > ((PMSM_KE*5) >> 2)) PMBemfKeUsed = (PMSM_KE*5) >> 2;
        if (PMBemfKeUsed < PMSM_KE) PMBemfKeUsed = PMSM_KE;
    }
    static long PMBemfKeUsedAcc = 102;
    PMBemfKeUsedAcc = ((((int32)PMBemfKeUsed << 16) - PMBemfKeUsedAcc) >> 8) + PMBemfKeUsedAcc;
    PMBemfKeUsed1  = (PMBemfKeUsedAcc + 0x8000) >> 16;

    static long PMBemfKeUsedLp2Acc = 102;
    PMBemfKeUsedLp2Acc = ((((int32)PMBemfKeUsed1 << 16) - PMBemfKeUsedLp2Acc) >> 6) + PMBemfKeUsedLp2Acc;
    PMBemfKeUsed2 = (PMBemfKeUsedLp2Acc + 0x8000) >> 16;
    pm_bem_kf = PMBemfKeUsed2;

    // 最终上下限限制
    pm_bem_kf = (pm_bem_kf > ((PMSM_KE*5)>>2)) ? ((PMSM_KE*5)>>2) : pm_bem_kf;
    pm_bem_kf = (pm_bem_kf < PMSM_KE) ? PMSM_KE : pm_bem_kf;

    // 更新旧值
    PMBemfKeUsedOld = pm_bem_kf;
#endif

	// DQ 轴电流环PI 系数,pm_especial_pi 对于小电感电机，放大电流环参数
	// 此处不涉及....
	///////////////////////////////////////////////////////////////////
	pm_ki_d = (unsigned long)D_CUR_KI * 10;
    pm_ki_q = (unsigned long)Q_CUR_KI * 10;
    if(gBasePar.FcSetApply <= 40)
    {
        kp = (long)D_CUR_KP * gBasePar.FcSetApply>>2;
        kp = (long)Q_CUR_KP * gBasePar.FcSetApply>>2;
    } else {
        kp = (long)D_CUR_KP * 10;
        pm_kp_d = kp;
        kp = (long)Q_CUR_KP * 10;
        pm_kp_q = kp;
    }

    if (gMainCmd.Command.bit.StartDC == 1)
    {
        pm_ki_d = pm_ki_d >> 2;
        pm_ki_q = pm_ki_q >> 2;
        pm_kp_d = pm_kp_d >> 1;
        pm_kp_q = pm_kp_q >> 1;
    }

    if (gMainStatus.RunStep == OPEN_ACC || gMainCmd.Command.bit.StartDC == 1)
    {
        pm_ki_d >>= 2;
        pm_ki_q >>= 2;
        pm_kp_d >>= 2;
        pm_kp_q >>= 2;
    }
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
