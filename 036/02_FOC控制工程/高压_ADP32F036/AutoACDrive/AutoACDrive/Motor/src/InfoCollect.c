
#include "MotorInclude.h"
#include "param_lr.h"
#include "Record.h"
#include "Parameter.h"


/****************************************************************
	ADC的通用处理: ADC模块本身的漂移处理
*****************************************************************/
void ADCProcess(void)
{
	gADC.ResetTime++;
	gADC.ResetTime = (gADC.ResetTime>1000)?1000:gADC.ResetTime;
	if(gADC.ZeroCnt >= 100)		return;			//ADC模块漂移检测完毕
}

/****************************************************************
	获取母线电压数据，输出gUDC
*****************************************************************/
void GetUDCInfo(void)
{
	Uint m_uDC;

   	m_uDC = ((Uint32)ADC_UDC * gUDC.Coff)>>16;//9
   	gUDC.uDC = (gUDC.uDC + m_uDC)>>1;
   	gUDC.uDCFilter = gUDC.uDCFilter - (gUDC.uDCFilter>>3) + (gUDC.uDC>>3);
}

/****************************************************************
	获取温度采样数据
*****************************************************************/
void GetTemperatureInfo(void)
{
	Uint m_Result;
	m_Result = (ADC_TEMP & 0xFFF0)>>4;
	if (gADC.ResetTime < 100)
	{
		gTemperature.TempAD = m_Result;
	}
	else
	{
		gTemperature.TempAD = Filter16(m_Result,gTemperature.TempAD);
	}
}

#ifdef NEW_CUR_SAMPLE_POINT_MODE
int GetCur;
/* -----------------------------------
         ------------------
    ----|                  |-------
            -----------  T3
        ----|          |-------
                ---- T2
            ----|  |-------
                 T1
  -----------------------------------*/
int T1;
int T2;
int T3;
enum {
    CUR_U_V,
    CUR_U_W,
    CUR_V_W,
    CUR_U_V_W_LAST,     // UV
    CUR_V_U_W_LAST,     // VW
    CUR_W_U_V_LAST      // UW
};
int CurrentSel;
int MinPWMForSen = 200;
long OutPutPWMData[12];
int shift;
void OutPutPWMPhaseShift(void)
{
    if (gPWM.U > gPWM.V)
    {
        if (gPWM.V > gPWM.W)            // U > V > W
        {
            T1 = gPWM.W;
            T2 = gPWM.V - gPWM.W;
            T3 = gPWM.U - gPWM.V;

            
/*            
            if (T1 > MinPWMForSen) {                // T1 > MIN
                CurrentSel = CUR_U_V;
            } else {                                // T1 < MIN
                if (T2 > MinPWMForSen) {            // T1 < MIN & T2 > MIN
                    CurrentSel = CUR_U_V;
                } else {                            // T1 < MIN & T2 < MIN
                    CurrentSel = CUR_U_V_W_LAST;
                }
            }
*/            
            
            if (T2 < MinPWMForSen && T1 < MinPWMForSen) {
                CurrentSel = CUR_U_V_W_LAST;
            } else {
                CurrentSel = CUR_U_V;
            }
        } else {
            if (gPWM.U > gPWM.W) {      // U > W > V
                T1 = gPWM.V;
                T2 = gPWM.W - gPWM.V;
                T3 = gPWM.U - gPWM.W;

                if (T2 < MinPWMForSen && T1 < MinPWMForSen) {
                    CurrentSel = CUR_U_V_W_LAST;
                } else {
                    CurrentSel = CUR_U_W;
                }
            } else {                    // W > U > V
                T1 = gPWM.V;
                T2 = gPWM.U - gPWM.V;
                T3 = gPWM.W - gPWM.U;

                if (T2 < MinPWMForSen && T1 < MinPWMForSen) {
                    CurrentSel = CUR_W_U_V_LAST;
                } else {
                    CurrentSel = CUR_U_W;
                }
            }
        }
    } else {
        if (gPWM.U > gPWM.W)            // V > U > W
        {
            T1 = gPWM.W;
            T2 = gPWM.U - gPWM.W;
            T3 = gPWM.V - gPWM.U;

            if (T2 < MinPWMForSen && T1 < MinPWMForSen) {
                CurrentSel = CUR_V_U_W_LAST;
            } else {
                CurrentSel = CUR_U_V;
            }
        } else {
            if (gPWM.V > gPWM.W) {      // V > W > U
                T1 = gPWM.U;
                T2 = gPWM.W - gPWM.U;
                T3 = gPWM.V - gPWM.W;

                if (T2 < MinPWMForSen && T1 < MinPWMForSen) {
                    CurrentSel = CUR_V_U_W_LAST;
                } else {
                    CurrentSel = CUR_V_W;
                }
            } else {                    // W > V > U
                T1 = gPWM.U;
                T2 = gPWM.V - gPWM.U;
                T3 = gPWM.W - gPWM.V;

                if (T2 < MinPWMForSen && T1 < MinPWMForSen) {
                    CurrentSel = CUR_W_U_V_LAST;
                } else {
                    CurrentSel = CUR_V_W;
                }
            }
        }
    }

    if (T1 > MinPWMForSen) {
        EPwm1Regs.CMPB  = 1;OutPutPWMData[0]++;
    } else {
        if (T2 > MinPWMForSen) {
            EPwm1Regs.CMPB  = (T2 >> 1) + T1 + shift;   OutPutPWMData[1]++;
            if (EPwm1Regs.CMPB < 1) EPwm1Regs.CMPB = 1;
        } else {
            EPwm1Regs.CMPB  = (T2 + T3) >> 1 + T1;      OutPutPWMData[2]++;
        }
    }
    EPwm1Regs.CMPB  = 1;                                OutPutPWMData[0]++;
#if 0
{
static int EnableRecord = 0;
if (GetCur == 1){// && gMainCmd.Command.bit.Start == TRUE) {
    EnableRecord = 1;
}//T1T2定义是否有问题？？？!!!!!
if (EnableRecord == 1) {
ControlAllowRecord(1,2);
IsRecordThisSample(
        T1,//gPWM.U,
        T2,//gPWM.V,
        gPWM.U,//((long)((int)(ADC_IU - (Uint)32768) -(long)gExcursionInfo.ErrIu)* gCurSamp.Coff)>>15,//gCurSamp.U,
        ((long)((int)(ADC_IV - (Uint)32768) -(long)gExcursionInfo.ErrIv)* gCurSamp.Coff)>>15,//gCurSamp.V,
        ((long)((int)(ADC_IW - (Uint)32768) -(long)gExcursionInfo.ErrIw)* gCurSamp.Coff)>>15,//gCurSamp.W,
        CurrentSel,//gPWM.U,//ADC_UDC >> 4,
        EPwm1Regs.CMPB,//gCurSamp.U,//((long)gLineCur.CurPer * 500) >> 12,
        gCurSamp.U,//gPWM.W,
        gCurSamp.V,//gPWM.V,//ADC_IV >> 4,
        gCurSamp.W);//pm_ref_iq);
}
}
#endif
}
#endif

/****************************************************************
    获取电流采样数据，输出gCurSamp
*****************************************************************/
#pragma CODE_SECTION(GetCurrentInfo, "ramfuncs");
int EnableRecord = 0;
void GetCurrentInfo(void)
{
    long m_Iu,m_Iv,m_Iw;
    int IuCur;
    int IvCur;
    int IwCur;
#ifndef NEW_CUR_SAMPLE_POINT_MODE
    int CurSelection;
    CurSelection = GetPWMSector();
#endif

#ifdef NEW_CUR_SAMPLE_POINT_MODE
    switch (CurrentSel) {
    case CUR_U_V:
        IuCur = (int)(ADC_IU - (Uint)32768);
        IvCur = (int)(ADC_IV - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (long)IuCur - (long)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iv = (long)IvCur - (long)gExcursionInfo.ErrIv;    //去除零漂
        m_Iv = __IQsat(m_Iv,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff)>>15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        m_Iw = - m_Iu - m_Iv;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    break;

    case CUR_U_W:
        IuCur = (int)(ADC_IU - (Uint)32768);
        IwCur = (int)(ADC_IW - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (long)IuCur - (long)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iw = (long)IwCur - (long)gExcursionInfo.ErrIw;    //去除零漂
        m_Iw = __IQsat(m_Iw,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff)>>15;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        m_Iv = - m_Iu - m_Iw;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    break;

    case CUR_V_W:
        IvCur = (int)(ADC_IV - (Uint)32768);
        IwCur = (int)(ADC_IW - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iv = (long)IvCur - (long)gExcursionInfo.ErrIv;    //去除零漂
        m_Iv = __IQsat(m_Iv,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff)>>15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        m_Iw = (long)IwCur - (long)gExcursionInfo.ErrIw;    //去除零漂
        m_Iw = __IQsat(m_Iw,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff)>>15;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        m_Iu = - m_Iv - m_Iw;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    break;

    case CUR_U_V_W_LAST:
        IuCur = (int)(ADC_IU - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (long)IuCur - (long)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = gCurSamp.VLast;
        gCurSamp.W = gCurSamp.WLast;
    break;

    case CUR_V_U_W_LAST:
        IvCur = (int)(ADC_IV - (Uint)32768);

        m_Iv = (long)IvCur - (long)gExcursionInfo.ErrIv;    //去除零漂
        m_Iv = __IQsat(m_Iv,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff)>>15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        gCurSamp.V = m_Iv;
        gCurSamp.U = gCurSamp.ULast;
        gCurSamp.W = gCurSamp.WLast;
    break;

    case CUR_W_U_V_LAST:
        IwCur = (int)(ADC_IW - (Uint)32768);

        m_Iw = (long)IwCur - (long)gExcursionInfo.ErrIw;    //去除零漂
        m_Iw = __IQsat(m_Iw,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff)>>15;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        gCurSamp.W = m_Iw;
        gCurSamp.U = gCurSamp.ULast;
        gCurSamp.V = gCurSamp.VLast;
    break;

    default:
    break;
    }


    ///////////////////////////////////////////////////////////////////////////////
    //For HVM isssue
    ///////////////////////////////////////////////////////////////////////////////
    //    gIavg.value[0] += (gAdcData.I.value[0] - gIavg.value[0])>>gIavg_shift;
    //    gIavg.value[1] += (gAdcData.I.value[1] - gIavg.value[1])>>gIavg_shift;
    //    gIavg.value[2] += (gAdcData.I.value[2] - gIavg.value[2])>>gIavg_shift;
    //    if(svgencurrent.IgnoreShunt == ignore_ab)
    //    {
    //        gAdcData.I.value[0] = gIavg.value[0];
    //        gAdcData.I.value[1] = gIavg.value[1];
    //    }
    gCurSamp.ULast += (gCurSamp.U - gCurSamp.ULast) >> 1;
    gCurSamp.VLast += (gCurSamp.V - gCurSamp.VLast) >> 1;
    gCurSamp.WLast += (gCurSamp.W - gCurSamp.WLast) >> 1;
#else
    switch (CurSelection) {
    case U_V_CUR_SEN:
        IuCur = (int)(ADC_IU - (Uint)32768);
        IvCur = (int)(ADC_IV - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (long)IuCur - (long)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iv = (long)IvCur - (long)gExcursionInfo.ErrIv;    //去除零漂
        m_Iv = __IQsat(m_Iv,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff)>>15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        m_Iw = - m_Iu - m_Iv;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    break;

    case U_W_CUR_SEN:
        IuCur = (int)(ADC_IU - (Uint)32768);
        IwCur = (int)(ADC_IW - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (long)IuCur - (long)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iw = (long)IwCur - (long)gExcursionInfo.ErrIw;    //去除零漂
        m_Iw = __IQsat(m_Iw,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff)>>15;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        m_Iv = - m_Iu - m_Iw;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    break;

    case V_W_CUR_SEN:
        IvCur = (int)(ADC_IV - (Uint)32768);
        IwCur = (int)(ADC_IW - (Uint)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iv = (long)IvCur - (long)gExcursionInfo.ErrIv;    //去除零漂
        m_Iv = __IQsat(m_Iv,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff)>>15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        m_Iw = (long)IwCur - (long)gExcursionInfo.ErrIw;    //去除零漂
        m_Iw = __IQsat(m_Iw,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff)>>15;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        m_Iu = - m_Iv - m_Iw;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        // 给外部接口模块，瞬时值...
        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    break;

    default:
    break;
    }

    //运放反向输出，所以取反操作
    gCurSamp.U = m_Iu;
    gCurSamp.V = m_Iv;
    gCurSamp.W = m_Iw;

#endif
#if 1
{
if (EnableRecord == 1) {
ControlAllowRecord(1,3);
IsRecordThisSample(
        gCurSamp.U,
        gCurSamp.V,//((long)((int)(ADC_IV - (Uint)32768) -(long)gExcursionInfo.ErrIv)* gCurSamp.Coff)>>15,//gCurSamp.V,
        gCurSamp.W,//((long)((int)(ADC_IW - (Uint)32768) -(long)gExcursionInfo.ErrIw)* gCurSamp.Coff)>>15,//gCurSamp.W,
        0,//CurSelection,//gPWM.U,//ADC_UDC >> 4,
        0,
        0,//pm_ud,
        0,//pm_uq,
        0,//gCurSamp.U,//((long)gLineCur.CurPer * 500) >> 12,
        0,//gPWM.V,//ADC_IV >> 4,
        0);//pm_ref_iq);
}
}
#endif
    if (pm_check_ip_flag   == 1)
    {
        int IuTemp;
        int IvTemp;
        int IwTemp;
        IuTemp = (int)(ADC_IU - (Uint)32768);
        IvTemp = (int)(ADC_IV - (Uint)32768);
        IwTemp = (int)(ADC_IW - (Uint)32768);

        IuTemp = (long)IuTemp - (long)gExcursionInfo.ErrIu;
        gExcursionInfo.Iu = IuTemp;
        m_Iu = __IQsat(IuTemp,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff) >> 15;
        m_Iu = __IQsat(m_Iu,32767,-32767);
        gCurSamp.U = m_Iu;

        IvTemp = (long)IvTemp - (long)gExcursionInfo.ErrIv;
        gExcursionInfo.Iv = IvTemp;
        m_Iv = __IQsat(IvTemp,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff) >> 15;
        m_Iv = __IQsat(m_Iv,32767,-32767);
        gCurSamp.V = m_Iv;

        IwTemp = (long)IwTemp - (long)gExcursionInfo.ErrIw;
        gExcursionInfo.Iw = IwTemp;
        m_Iw = __IQsat(IwTemp,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff) >> 15;
        m_Iw = __IQsat(m_Iw,32767,-32767);
        gCurSamp.W = m_Iw;
    } else {
        gExcursionInfo.Iu = m_Iu;
        gExcursionInfo.Iv = m_Iv;
        gExcursionInfo.Iw = m_Iw;
    }
}
