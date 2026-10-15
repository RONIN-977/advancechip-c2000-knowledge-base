
#include "MotorInclude.h"
#include "Record.h"

void	TemperatureCheck(void);					//温度检查
void	OutputPhaseLoseDetect(void);			//输出缺相检测
void 	OutputLoseAdd(void);
void 	OutputLoseReset(void);		
void    OutputPhaseLoseDetectNew(void);

/************************************************************
	变频器保护处理
************************************************************/
void InvDeviceKontrol(void)			
{
	if (gADC.ResetTime < 500)	return;	//AD采样稳定后开始

	//TemperatureCheck();				//温度检查
	OutputPhaseLoseDetect();			//输出缺相检测
	OutputPhaseLoseDetectNew();
}

long MadddData[9];
void OutputPhaseLoseDetectNew(void)
{
    static int m_U,m_V,m_W;
    static long IuAcc,IvAcc,IwAcc;
    static Uint IuAccCounter;

    // ADM32F036, Positive Logic, OutPhase = 0;  U:2000, VW -1000
    // 512 IS 1.0S....
    if ((gMainCmd.Command.bit.StartDC != 1) ||
        ((gMainStatus.RunStep != STATUS_RUN         ) &&
         (gMainStatus.RunStep != HALF_OPEN_ACC      ) &&
         (gMainStatus.RunStep != OPEN_ACC           ) &&
         (gMainStatus.RunStep != STATUS_SPEED_CHECK )))
    {
MadddData[5]++;
        IuAcc = IvAcc = IwAcc = 0;
        IuAccCounter = 0;
        return;
    }
    if (IuAccCounter == 256) { //
        m_U = IuAcc >> 8; if (m_U < 0) m_U = -m_U;
        m_V = IvAcc >> 8; if (m_V < 0) m_V = -m_V;
        m_W = IwAcc >> 8; if (m_W < 0) m_W = -m_W;
MadddData[0]++;
MadddData[1]=m_U;
MadddData[2]=m_V;
MadddData[3]=m_W;
        if ((m_U < 350) || (m_V < 350) || (m_W < 350)) {
MadddData[6]=gMainCmd.Command.bit.StartDC;
MadddData[7]=gMainStatus.RunStep;
MadddData[8]=gPhase.OutPhase;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_LOSE_PHASE_OUTPUT;
        }
    } else if (IuAccCounter < 256) {
        IuAccCounter++;
MadddData[4]++;
        IuAcc += gCurSamp.U;
        IvAcc += gCurSamp.V;
        IwAcc += gCurSamp.W;
    }
#if 0
if (gMainCmd.Command.bit.StartDC == 1) {
//if (0) {
ControlAllowRecord(1, 5);   // 2ms, 2*128*10=2s
IsRecordThisSample(
        gCurSamp.U,//IuAcc,//gIMT.M,//pm_ref_id,//ADC_IV,//gPWM.U,//gPWM.U,//AdcResult.ADCRESULT3,   // IU
        gCurSamp.V,
        gCurSamp.W,//IuAccCounter,
        m_U,    //gIMT.T,//pm_ref_iq,//ADC_IV1,//gPWM.V,//gPWM.V,//SampPoint1Time,//EPwm1Regs.CMPA.half.CMPA,//AdcResult.ADCRESULT4,   // IV
        m_V,
        m_W,
        IuAccCounter//gIAmpTheta.Theta//gPWM_CMP.CurrSampleNumAct//ADCData[8]//((long)gLineCur.CurPer * 500) >> 12//gPWM_CMP.CurrSampleNumAct//ADC_IV1//EPwm1Regs.CMPA.half.CMPA//gPWM_CMP.T2CMP//ADC_IV1//ADCtest[1]
        );
}
#endif
}

void TemperatureCheck(void)
{

}

/************************************************************
	输出缺相检测
************************************************************/
void OutputPhaseLoseDetect(void)
{
    Uint m_U = 0,m_V,m_W = 0;
    Ulong m_Max = 0,m_Min = 0;

    if ((gMainCmd.FreqReal      < 500       ) ||
        (gMainStatus.ErrorCode != ERROR_NONE) ||
        (gMainCmd.Command.bit.StartDC == 1  ) ||
        (gMainStatus.RunStep != STATUS_RUN  ))
    {
        OutputLoseReset();
        return;
    }

    // 2ms, then 2s here....
    // 20ksf, gPWM.gPWMPrdApply = 3750, then 800 -- gPhaseLose.Time, 1.6S
    // 2ms tick, then 1000 is 2s
    if (gPhaseLose.Cnt < 1024)
    {
        return;
    }

    m_U = gPhaseLose.TotalU >> 10;
    m_V = gPhaseLose.TotalV >> 10;
    m_W = gPhaseLose.TotalW >> 10;

    m_Max = (m_U   > m_V) ? m_U  : m_V;
    m_Max = (m_Max > m_W) ? m_Max: m_W;
    m_Min = (m_U   < m_V) ? m_U  : m_V;
    m_Min = (m_Min < m_W) ? m_Min: m_W;
//MadddData[10]++;
//MadddData[11]=m_U;
//MadddData[12]=m_V;
//MadddData[13]=m_W;
//MadddData[14]=m_Max;
//MadddData[15]=m_Min;
//MadddData[16]=(m_Min << 3);
//MadddData[17]=gPhaseLose.TotalU;
//MadddData[18]=gPhaseLose.TotalV;
//MadddData[19]=gPhaseLose.TotalW;
    if ((m_Max > 500) && (m_Max > (m_Min << 3)))
    {
//MadddData[9]++;
        DisableDrive();
        gMainStatus.ErrorCode = ERROR_LOSE_PHASE_OUTPUT;
    }
    OutputLoseReset();
}

void OutputLoseAdd(void)		//输出缺相检测累加电流处理
{
	gPhaseLose.TotalU += abs(gIUVW.U);
	gPhaseLose.TotalV += abs(gIUVW.V);
	gPhaseLose.TotalW += abs(gIUVW.W);
	gPhaseLose.Time   += gPWM.gPWMPrdApply;
	gPhaseLose.Cnt++;
}

void OutputLoseReset(void)		//输出缺相检测复位寄存器处理
{
	gPhaseLose.Cnt = 0;
	gPhaseLose.TotalU = 0;
	gPhaseLose.TotalV = 0;
	gPhaseLose.TotalW = 0;
	gPhaseLose.Time   = 0;
}
