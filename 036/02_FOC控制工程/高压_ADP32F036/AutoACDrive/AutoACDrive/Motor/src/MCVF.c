
#include "MotorInclude.h"
#include "Parameter.h"
#include "Record.h"

void CalOutVotInVFStatus(void);
void VFFreqCal(void);

void CalOutVoltInDCBrakeStatus(void)
{
    static int m_BrakeCur;
    gDCBrake.Time++;        // 10KSF, 200MS SLOPE, 2048
    if (gDCBrake.Time < 5) {
        m_BrakeCur = 0;
        gOutVolt.Volt = 0;
        gDCBrake.PID.Total = ((long)gOutVolt.Volt<<16);
    } else {
        if (gDCBrake.Time < 2048) {
            m_BrakeCur = ((long)DCBrakeCur * gDCBrake.Time) >> 11;
        } else {
            m_BrakeCur = DCBrakeCur;
            gDCBrake.Time = 2048;
        }

        gDCBrake.PID.Deta = m_BrakeCur - gLineCur.CurPer;
        gDCBrake.PID.KP   = DC_CONTROL_KP;
        gDCBrake.PID.KI   = DC_CONTROL_KI;
        gDCBrake.PID.KD   = 0;
        gDCBrake.PID.Max  = 4096;
        gDCBrake.PID.Min  = 0;
        PID((PID_STRUCT *)&gDCBrake.PID);
        gOutVolt.Volt = gDCBrake.PID.Out>>16;
    }
    gOutVolt.VoltApply = gOutVolt.Volt;

#if 0
{
static int EnableRecord = 0;
if (1) {// && gMainCmd.Command.bit.Start == TRUE) {
    EnableRecord = 1;
}
// 100us*10 = 1ms, 1*10*128 = 1.28s
if (EnableRecord == 1) {
ControlAllowRecord(1,150);
IsRecordThisSample(
        gDCBrake.Time,//((long)((int)(ADC_IU - (Uint)32
        m_BrakeCur,//((long)((int)(ADC_IV - (
        gLineCur.CurPer,//((long)((int)(ADC_IW - (Ui
        gRatio,//((long)gLineCur.CurPer * 500) >> 12,
        gCurSamp.U,//ADC_UDC >> 4,
        gCurSamp.W);//pm_ref_iq);
}
}
#endif

/*
    short m_BrakeCur;

    if (gMainCmd.Command.bit.LastDCPhase == 1) {
        m_BrakeCur = 0;
    } else {
        m_BrakeCur = DCBrakeCur;
    }

    gDCBrake.Time++;
    if (gDCBrake.Time < 5)
    {
        gOutVolt.Volt = 0;
        gDCBrake.PID.Total = ((long)gOutVolt.Volt<<16);
    } else {
        gDCBrake.Time = 10;
        gDCBrake.PID.Deta = m_BrakeCur - gLineCur.CurPer;
        gDCBrake.PID.KP   = DC_CONTROL_KP;
        gDCBrake.PID.KI   = DC_CONTROL_KI;
        gDCBrake.PID.KD   = 0;
        gDCBrake.PID.Max  = 4096;
        gDCBrake.PID.Min  = 0;
        PID((PID_STRUCT *)&gDCBrake.PID);
        gOutVolt.Volt = gDCBrake.PID.Out>>16;
    }
    gOutVolt.VoltApply = gOutVolt.Volt;*/
}
/*************************************************************
	计算VF曲线：从给定频率计算输出电压，区别不同VF曲线
*************************************************************/
long Volt = 200;
void CalOutVotInVFStatus(void)
{
	int  m_AbsFreq;
	
	gOutVolt.VoltPhase = 0;	
	m_AbsFreq = abs(gMainCmd.FreqSetApply);
//	gTorqueUp.CompPhase = 0;

	//...恒功率区域
	if(m_AbsFreq >= gMotorInfo.FreqPer)	
	{
		gOutVolt.Volt = 4096;
		return;		
	}
	if(m_AbsFreq == 0)						//0频率电压为0
	{
		gOutVolt.Volt = 0;
		return;
	}

	//线性VF曲线
	gOutVolt.Volt = ((Ulong)m_AbsFreq * 4096L)/gMotorInfo.FreqPer;
}

/************************************************************
	VF频率生成函数
************************************************************/
void VFFreqCal(void)
{
	gMainCmd.FreqPreWs = gMainCmd.FreqSet;return;
}
