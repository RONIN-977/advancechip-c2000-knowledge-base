
#include "MotorInclude.h"
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
		data1 = (long)HIGH_SPEED_KI_GAIN - (long)ki;
		data1 = data1 * (freqs - (long)F_CHANGING_POINT_1_PI);
		data2 = (long)F_CHANGING_POINT_2_PI - (long)F_CHANGING_POINT_1_PI;
		data1 = data1 / data2;
		pm_ki_speed = (long)ki + data1;
		if(pm_ki_speed < 0)
		{
			pm_ki_speed = 0;
		}

		data1 = (long)HIGH_SPEED_KP_GAIN - (long)LOW_SPEED_KP;
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
	if (pm_forbid_revtorq == 0)
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
	pm_r  = (unsigned long)PMSM_RS;
	l_ld  = (unsigned long)PMSM_LD;
	data1 = (unsigned long)PMSM_LQ;
	l_lq  = (long)l_ld + (data1 - (long)l_ld) / 3;

#if 0//(LD_LQ_BASED_ON_CUR == 1)
	int LBasedCurTemp;
	int IndexBasedCur;
	IndexBasedCur = ((Uint32)gLineCur.CurPer * MOTOR_RATED_CUR) >> 12;
	IndexBasedCur /= 10;
	if (IndexBasedCur > 20) IndexBasedCur = 20;
	LBasedCurTemp = l_ld * MTPA_Ld_TableData_H[IndexBasedCur] >> 10;
	l_ld = LBasedCurTemp;
EnableRecord2[0]=LBasedCurTemp;
    LBasedCurTemp = l_lq * MTPA_Lq_TableData_H[IndexBasedCur] >> 10;
    l_lq = LBasedCurTemp;
EnableRecord2[1]=LBasedCurTemp;
#endif

#if 1
	pm_bem_kf = PMSM_KE;
#else
    int PMBemfKeUsed;
    if (gMainCmd.FreqReal < 3000) {
        PMBemfKeUsed = PMSM_KE << 1;
    } else if (gMainCmd.FreqReal > 6000) {
        PMBemfKeUsed = PMSM_KE;
    } else {
        PMBemfKeUsed = ((long)(6000 - gMainCmd.FreqReal) * PMSM_KE) / (6000 - 3000);
        PMBemfKeUsed += PMSM_KE;
        if (PMBemfKeUsed > (PMSM_KE << 1)) PMBemfKeUsed = (PMSM_KE << 1);
        if (PMBemfKeUsed < (PMSM_KE << 0)) PMBemfKeUsed = (PMSM_KE << 0);
    }
    pm_bem_kf = PMBemfKeUsed;
#endif

	// DQ 轴电流环PI 系数,KP KI
	//
	pm_ki_d = (long)D_CUR_KI * 10;
    pm_ki_q = (long)Q_CUR_KI * 10;
    kp = (long)D_CUR_KP * 10;
    pm_kp_d = kp;
    kp = (long)Q_CUR_KP * 10;
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
	Uint m_Index;
	for (m_Index = 0;m_Index < MOTOR_TO_FUNCTION_DATA_NUM;m_Index++)
	{
		gSendToFunctionDataBuff[m_Index] = *gSendToFuncTable[m_Index];
	}
}
