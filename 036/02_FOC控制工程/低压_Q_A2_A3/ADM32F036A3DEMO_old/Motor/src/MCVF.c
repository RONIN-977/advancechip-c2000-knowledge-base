
#include "MotorInclude.h"
#include "Parameter.h"

void CalOutVotInVFStatus(void);
void VFFreqCal(void);

int RecordEnable = 0;
void CalOutVoltInDCBrakeStatus(void)
{
    short m_BrakeCur;
    static int KPBasedONDeta;

    if (gMainCmd.Command.bit.LastDCPhase == 1) {
        m_BrakeCur = 0;
    } else {
        if (RsSecond == 1) {
            m_BrakeCur = DCBrakeCur >> 1;
        } else {
            m_BrakeCur = DCBrakeCur;
        }
    }

    gDCBrake.Time++;
    if(gDCBrake.Time < 5)
    {
        gOutVolt.Volt = 0;
        gDCBrake.PID.Total = ((long)gOutVolt.Volt<<16);
    } else {
        gDCBrake.Time = 10;

        // gDCBrake.PID.Deta = m_BrakeCur - gCurSamp.U;
        gDCBrake.PID.Deta = m_BrakeCur - gLineCur.CurPer;

        KPBasedONDeta = gDCBrake.PID.Deta >> 4;
        KPBasedONDeta = KPBasedONDeta > 20 ? 20 : KPBasedONDeta;
        KPBasedONDeta = KPBasedONDeta <  5 ?  5 : KPBasedONDeta;

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
if (gMainCmd.Command.bit.Start == 1 && gMainCmd.Command.bit.StartDC == 1) {
    RecordEnable = 1;
}
if (RecordEnable == 1) {// && gMainCmd.Command.bit.Start == 1) {
ControlAllowRecord(1,40);    // 1ms * 128 = 128ms
IsRecordThisSample(
        gCurSamp.U,
        gCurSamp.V,
        gCurSamp.W,
        gRatio,
        gLineCur.CurPer,//
        gDCBrake.PID.Deta,
        gDCBrake.PID.Total >> 16//
        );
}
#endif
}
/*************************************************************
	计算VF曲线：从给定频率计算输出电压，区别不同VF曲线
*************************************************************/
void CalOutVotInVFStatus(void)
{
	int  m_AbsFreq;
	
	gOutVolt.VoltPhase = 0;	
	m_AbsFreq = abs(gMainCmd.FreqSetApply);

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
	gMainCmd.FreqPreWs = gMainCmd.FreqSet;
	return;
}
