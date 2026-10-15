
#include "MotorInclude.h"
#include "Main.h"
#include "Parameter.h"

/*************************************************************
**************************************************************
	从功能模块获取需要的所有参数
*************************************************************/
#ifdef SWITCH_FRE_EMC
unsigned int PWMFreSetFlag = 0;
unsigned int RandomNum = 10000;
unsigned int RandomNum1 = 0;
unsigned int RandomNum2 = 10000;
unsigned int PWMFreSet = 160;
#endif
//int EnableRecord2[5];
void ParGetFromFunction(void)
{
	int m_Index;
	int data;
	long freqs,data1,data2,ki,kp;

	pm_control_mode = PMSM_CONTROL_MODE_VF;//PMSM_CONTROL_MODE_FOC;

#if SWITCH_FRE_EMC
    RandomNum = PWMFreSet * 229;

    if(RandomNum1 <= RandomNum)
        RandomNum1++;
    else
        RandomNum1 = 0;

    if(RandomNum2 >= 10)
        RandomNum2--;
    else
        RandomNum2 = RandomNum;

    PWMFreSet = (RandomNum1*RandomNum2)%190;
    EnableRecord2[1] = PWMFreSet;
    if(PWMFreSet <= 165)
    {
        if(PWMFreSet >=80)
            PWMFreSet = 165 + (PWMFreSet >>3);
        else
            PWMFreSet = 165 + (PWMFreSet >>3);
    }
    else if(PWMFreSet >= 185)
    {
        PWMFreSet = 185;
    }

    gBasePar.FcSetApply = PWMFreSet;
    gFcCal.FcBak  = PWMFreSet;
#endif

#if 0
//if (gMainStatus.RunStep == OPEN_ACC && gMainCmd.Command.bit.StartDC == 0) {
if (EnableRecord2[0] == 1) {
ControlAllowRecord(1,2);
IsRecordThisSample(
        RandomNum1,
        RandomNum2,
        RandomNum,
        EnableRecord2[1],
        PWMFreSet,
        gBasePar.FcSetApply,
        pm_est_omg >> 4
        );
}
#endif

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

    data = PMS_L_R_UNIT/10;
    if(data == 0)
    {
        pm_r = (unsigned long)PMSM_RS;
    }
    else
    {
        pm_r = (unsigned long)PMSM_RS * 10;
    }

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

    if ((gMainCmd.Command.bit.StartDC   == 1                 ) ||
        (gMainStatus.RunStep            == OPEN_ACC          ) ||
        (gMainStatus.RunStep            == HALF_OPEN_ACC     ))
    {
        pm_kp_d = pm_kp_d >> 2;
        pm_kp_q = pm_kp_q >> 2;
        pm_ki_d = pm_ki_d >> 1;
        pm_ki_q = pm_ki_q >> 1;
    } else if (gMainStatus.RunStep      == STATUS_SPEED_CHECK) {
        pm_kp_d = pm_kp_d << 1;
        pm_kp_q = pm_kp_q << 1;
        pm_ki_d = pm_ki_d << 1;
        pm_ki_q = pm_ki_q << 1;
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
