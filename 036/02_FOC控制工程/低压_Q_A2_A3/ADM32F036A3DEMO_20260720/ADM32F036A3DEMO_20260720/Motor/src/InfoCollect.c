/***************************************************************************
 * InfoCollect.c
 * ADM32F036 电机控制 — 信息采集模块
 * version : 1.0
 * Date    : 2025
 *
 * 功能说明：
 *   1. ADC 通用处理（漂移补偿）
 *   2. 母线电压/电流采集与滤波
 *   3. 温度传感器采集
 *   4. 相电流采集（支持 1-Shunt / 2-Shunt / 3-Shunt 三种模式）
 *   5. IPD 参数辨识电流获取
 *   6. SMO 反电动势角度计算
 ****************************************************************************/

#include "MotorInclude.h"
#include "Record.h"
#include "Main.h"
#include "FuncUser.h"
#include "Parameter.h"

/*=============================================================================
 *   本文件内部私有变量声明（static）
 *============================================================================*/
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
static int32 m_Iu, m_Iv, m_Iw;
static int16 Adc_Temp_IDC1, Adc_Temp_IDC2;
#endif

/***************************************************************************
  Private Functions
 ***************************************************************************/
// static void GetSMOAngle(void);


/****************************************************************
    ADC的通用处理: ADC模块本身的漂移处理
*****************************************************************/
void ADCProcess(void)
{
    gADC.ResetTime++;
    gADC.ResetTime = (gADC.ResetTime>1000)?1000:gADC.ResetTime;
}

/****************************************************************
    获取母线电压数据，输出gUDC
*****************************************************************/
void GetUDCInfo(void)
{
    Uint16 m_uDC;
    int32 ADCTemp = ADC_UDC;
    m_uDC = ((Uint32)(ADCTemp) * gUDC.Coff)>>16;
    gUDC.uDC = (gUDC.uDC + m_uDC)>>1;

    static int32 UdcAcc;
    UdcAcc = ((((int32)gUDC.uDC << 16) - UdcAcc) >> 3) + UdcAcc;
    gUDC.uDCFilter  = (UdcAcc + 0x8000) >> 16;
}

/****************************************************************
    获取母线电流数据
*****************************************************************/
// long GetIDData[6];
void GetIDCInfo(void)
{
    int16 m_Ibus,Result_Ibus;
    static int32 m_IbusFilter;
    m_Ibus = ADC_IBUS;
    m_IbusFilter = m_IbusFilter - (m_IbusFilter >> 5) + (m_Ibus >> 5);
    Result_Ibus = m_IbusFilter - gExcursionInfo.ErrIbus;
    IbusActual = (int32)Result_Ibus * MAX_PEAK_CUR_3_3V_2 >> 11;       //0.01A

    int16 Temp32;
    // Temp32 = (long)gUDC.uDCFilter * IbusActual / 100;
    Temp32 = (int32)gUDC.uDCFilter * IbusActual >> 6;
    Temp32 = (int32)Temp32 * 41 >> 6;
    ActualPower = Temp32;                                       //0.1W
#if 0
//if (gMainStatus.RunStep == OPEN_ACC && gMainCmd.Command.bit.StartDC == 0) {
static int16 EnableRecord;
if (gMainCmd.Command.bit.Start == 1 && IBUSEnable == 1) { // gRatio > RatioForLowNoisePWM) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,1);
IsRecordThisSample(
        ADC_IBUS,
        m_IbusFilter,
        gUDC.uDCFilter,                 // pm_ref_iq,
        Result_Ibus,          // pm_ref_id,
        IbusActual,
    gCurSamp.V,
    1,
    12345//
    );
}
#endif
}

/****************************************************************
    获取温度采样数据
*****************************************************************/
void GetTemperatureInfo(void)
{
    Uint16 m_Result;
    m_Result = (ADC_TEMP & 0xFFF0);
    if (gADC.ResetTime < 100)
    {
        gTemperature.TempAD = m_Result;
    }
    else
    {
        gTemperature.TempAD = Filter16(m_Result,gTemperature.TempAD);
    }
}

/****************************************************************
    获取外部模拟量采样数据
    实际没用，可以删除！
*****************************************************************/
void GetAIInfo(void)
{
    // A5. AdcRegs.ADCCTL1.bit.TEMPCONV    = 1;
    // Temperature = (sensor - Offset) * Slope
    // TSLOPE = 0.181
    // OFFSET_0C = 1511
    Uint16 m_Result = ADC_DSP_TEMP;

    // InternalTemperatureDegree = (ADC_DSP_TEMP - 1511) * 0.181;
    InternalTemperatureDegree = ((int32)(ADC_DSP_TEMP - 1511) * 5931) >> 15;
}

/****************************************************************
    获取电流采样数据，输出gCurSamp
*****************************************************************/
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#pragma CODE_SECTION(SVPWM_1ShuntGetPhaseCurrent, "ramfuncs");
void SVPWM_1ShuntGetPhaseCurrent(void)
{
    static int16 LastCurrSampleNumAct;
    int16 CurrSampleNumActUsed;
    if (LastLowNoseElmPWMMode != LowNoseElmPWMMode) {
        CurrSampleNumActUsed = 50;//gPWM_CMP.CurrSampleNumAct;
    } else {
        CurrSampleNumActUsed = LastCurrSampleNumAct;
    }

    Adc_Temp_IDC1 = ADC_IV;
    Adc_Temp_IDC2 = ADC_IV1;

    switch(CurrSampleNumActUsed)
    {
        case IU_INV:
        case IU_INV_EST:
        case IUEST_INV:
        case IU_IV:
        {
            if(CurrSampleNumActUsed == IUEST_INV)
            {
                m_Iu = gCurSamp.U;
            }
            else
            {
                m_Iu = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IU_INV_EST)
            {
                m_Iv = gCurSamp.V;
            }
            else if(CurrSampleNumActUsed == IU_IV)
            {
                m_Iv = (int32)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iv = gExcursionInfo.ErrIv - (int32)Adc_Temp_IDC2;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }

            m_Iw = - m_Iu - m_Iv;
            break;
        }
        case IU_INW:
        case IU_INW_EST:
        case IUEST_INW:
        case IU_IW:
        {
            if(CurrSampleNumActUsed == IUEST_INW)
            {
                m_Iu = gCurSamp.U;
            }
            else
            {
                m_Iu = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IU_INW_EST)
            {
                m_Iw = gCurSamp.W;
            }
            else if(CurrSampleNumActUsed == IU_IW)
            {
                m_Iw = (int32)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iw = gExcursionInfo.ErrIv - (int32)Adc_Temp_IDC2;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }

            m_Iv = - m_Iu - m_Iw;
            break;
        }
        case IV_INU:
        case IV_INU_EST:
        case IVEST_INU:
        case IV_IU:
        {
            if(CurrSampleNumActUsed == IVEST_INU)
            {
                m_Iv = gCurSamp.V;
            }
            else
            {
                m_Iv = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IV_INU_EST)
            {
                m_Iu = gCurSamp.U;
            }
            else if(CurrSampleNumActUsed == IV_IU)
            {
                m_Iu = (int32)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iu = gExcursionInfo.ErrIv - (int32)Adc_Temp_IDC2;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }

            m_Iw = - m_Iv - m_Iu;
            break;
        }
        case IV_INW:
        case IV_INW_EST:
        case IVEST_INW:
        case IV_IW:
        {
            if(CurrSampleNumActUsed == IVEST_INW)
            {
                m_Iv = gCurSamp.V;
            }
            else
            {
                m_Iv = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IV_INW_EST)
            {
                m_Iw = gCurSamp.W;
            }
            else if(CurrSampleNumActUsed == IV_IW)
            {
                m_Iw = (int16)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iw = gExcursionInfo.ErrIv - (int32)Adc_Temp_IDC2;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }

            m_Iu = - m_Iv - m_Iw;
            break;
        }
        case IW_INU:
        case IW_INU_EST:
        case IWEST_INU:
        case IW_IU:
        {
            if(CurrSampleNumActUsed == IWEST_INU)
            {
                m_Iw = gCurSamp.W;
            }
            else
            {
                m_Iw = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IW_INU_EST)
            {
                m_Iu = gCurSamp.U;
            }
            else if(CurrSampleNumActUsed == IW_IU)
            {
                m_Iu = (int32)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iu = gExcursionInfo.ErrIv - (int32)Adc_Temp_IDC2;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }

            m_Iv = - m_Iu - m_Iw;
            break;
        }
        case IW_INV:
        case IW_INV_EST:
        case IWEST_INV:
        case IW_IV:
        {
            if(CurrSampleNumActUsed == IWEST_INV)
            {
                m_Iw = gCurSamp.W;
            }
            else
            {
                m_Iw = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IW_INV_EST)
            {
                m_Iv = gCurSamp.V;
            }
            else if(CurrSampleNumActUsed == IW_IV)
            {
                m_Iv = (int32)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iv = gExcursionInfo.ErrIv - (int32)Adc_Temp_IDC2;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }

            m_Iu = - m_Iv - m_Iw;
            break;
        }
        default:
            break;
    }

    // gPWM_CMP.T1CMP = 10;
    if (gMainStatus.RunStep == STATUS_SHORT_GND)
    {
        m_Iu = (int32)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
        m_Iu = (m_Iu * gCurSamp.Coff)>>11;
        gShortGnd.ShortCur = Filter32(m_Iu,gShortGnd.ShortCur);
    }

    gCurSamp.U = m_Iu;
    gCurSamp.V = m_Iv;
    gCurSamp.W = m_Iw;

    gExcursionInfo.Iu = m_Iu;
    gExcursionInfo.Iv = m_Iv;
    gExcursionInfo.Iw = m_Iw;

    LastCurrSampleNumAct = gPWM_CMP.CurrSampleNumAct;
}
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT) || (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT))
#pragma CODE_SECTION(SVPWM_3ShuntGetPhaseCurrent, "ramfuncs");
void SVPWM_3ShuntGetPhaseCurrent(void)
{
    int32 m_Iu,m_Iv,m_Iw;
    int16 IuCur;
    int16 IvCur;
    int16 IwCur;

    int16 CurSelection;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    CurSelection = U_V_CUR_SEN;//V_W_CUR_SEN;
#else
    CurSelection = GetPWMSector();
#endif
    switch (CurSelection) {
    case U_V_CUR_SEN:
        IuCur = (int16)(ADC_IU - (Uint16)32768);
        IvCur = (int16)(ADC_IV - (Uint16)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (int32)IuCur - (int32)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iv = (int32)IvCur - (int32)gExcursionInfo.ErrIv;    //去除零漂
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
        IuCur = (int16)(ADC_IU - (Uint16)32768);
        IwCur = (int16)(ADC_IW - (Uint16)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iu = (int32)IuCur - (int32)gExcursionInfo.ErrIu;    //去除零漂
        m_Iu = __IQsat(m_Iu,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff)>>15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iw = (int32)IwCur - (int32)gExcursionInfo.ErrIw;    //去除零漂
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
        IvCur = (int16)(ADC_IV - (Uint16)32768);
        IwCur = (int16)(ADC_IW - (Uint16)32768);

        // eg,751, then 751 >> 4 = 46. ~ 37mV.
        m_Iv = (int32)IvCur - (int32)gExcursionInfo.ErrIv;    //去除零漂
        m_Iv = __IQsat(m_Iv,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff)>>15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        m_Iw = (int32)IwCur - (int32)gExcursionInfo.ErrIw;    //去除零漂
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
    ////////////////////////////
    // 运放反向输出，所以取反操作
    // gCurSamp.U = m_Iu;
    // gCurSamp.V = m_Iv;
    // gCurSamp.W = m_Iw;
    ////////////////////////////
#if 0
{
static int16 EnableRecord = 0;
if (GetCur == 1) {// && gMainCmd.Command.bit.Start == TRUE) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,3);
IsRecordThisSample(
        ((int32)((int16)(ADC_IU - (Uint16)32768) -(int32)gExcursionInfo.ErrIu)* gCurSamp.Coff)>>15,//gCurSamp.U,
        ((int32)((int16)(ADC_IV - (Uint16)32768) -(int32)gExcursionInfo.ErrIv)* gCurSamp.Coff)>>15,//gCurSamp.V,
        ((int32)((int16)(ADC_IW - (Uint16)32768) -(int32)gExcursionInfo.ErrIw)* gCurSamp.Coff)>>15,//gCurSamp.W,
        CurSelection,//gPWM.U,//ADC_UDC >> 4,
        gPWM.U,
        gPWM.V,
        gPWM.W,
        gCurSamp.U,//((long)gLineCur.CurPer * 500) >> 12,
        gCurSamp.V,//gPWM.V,//ADC_IV >> 4,
        gCurSamp.W);//pm_ref_iq);
}
}
#endif
    if ((pm_check_ip_flag   == 1                ) ||
       (gMainStatus.RunStep == STATUS_GET_PAR   ))
    {
        int32 IuTemp = (int32)((int16)(ADC_IU - (Uint16)32768)) - (int32)gExcursionInfo.ErrIu;
        int32 IvTemp = (int32)((int16)(ADC_IV - (Uint16)32768)) - (int32)gExcursionInfo.ErrIv;
        int32 IwTemp = (int32)((int16)(ADC_IW - (Uint16)32768)) - (int32)gExcursionInfo.ErrIw;

        gExcursionInfo.Iu = IuTemp;
        gExcursionInfo.Iv = IvTemp;
        gExcursionInfo.Iw = IwTemp;

        m_Iu = __IQsat(IuTemp,32767,-32767);
        m_Iu = (m_Iu * gCurSamp.Coff) >> 15;
        m_Iu = __IQsat(m_Iu,32767,-32767);

        m_Iv = __IQsat(IvTemp,32767,-32767);
        m_Iv = (m_Iv * gCurSamp.Coff) >> 15;
        m_Iv = __IQsat(m_Iv,32767,-32767);

        m_Iw = __IQsat(IwTemp,32767,-32767);
        m_Iw = (m_Iw * gCurSamp.Coff) >> 15;
        m_Iw = __IQsat(m_Iw,32767,-32767);

        gCurSamp.U = m_Iu;
        gCurSamp.V = m_Iv;
        gCurSamp.W = m_Iw;
    } else {
        gExcursionInfo.Iu = m_Iu;
        gExcursionInfo.Iv = m_Iv;
        gExcursionInfo.Iw = m_Iw;
    }
}
#endif
#endif

int32 GetCurrentIPD(int16 SectionData)
{
    int32 ReturnValue = 1;

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    switch (SectionData) {
        case 0:
        case 2:
        case 4:
            ReturnValue = (int32)ADC_IV;
        break;
        case 1:
        case 3:
        case 5:
            ReturnValue = -(int32)ADC_IV;
        break;
        default:
        break;
    }
#else
    switch (SectionData)
    {
        case 0:
            ReturnValue  = (-(int32)ADC_IV - (int32)ADC_IW)>>4;
        break;
        case 1:
            ReturnValue  = (int32)ADC_IU>>4;
        break;
        case 2:
            ReturnValue  = (-(int32)ADC_IU - (int32)ADC_IW)>>4;
        break;
        case 3:
            ReturnValue  = (int32)ADC_IV>>4;
        break;
        case 4:
            ReturnValue  = (-(int32)ADC_IU - (int32)ADC_IV)>>4;
        break;
        case 5:
            ReturnValue  = (int32)ADC_IW>>4;
        break;
        default:
        break;
    }
#endif
    return ReturnValue;
}

#if 0
int16 SMOAngle;
static void GetSMOAngle(void)
{
    gBemf.Ua = ADCRESULT_UU;
    gBemf.Ub = ADCRESULT_UV;
    gBemf.Uc = ADCRESULT_UW;

    gBemf.BEMF_Alpha = (((int32)gBemf.Ua * 2 - (int32)gBemf.Ub - (int32)gBemf.Uc) * 10923L ) >> 15;
    gBemf.BEMF_Beta  = (((int32)gBemf.Ub - (int32)gBemf.Uc) * 18919L)>>15;
    if (gBemf.DIR == 1)
    {
        gBemf.Spd_Hz = -gBemf.Spd_Hz;
        gBemf.BEMF_Alpha = -gBemf.BEMF_Alpha;
        gBemf.BEMF_Beta  = -gBemf.BEMF_Beta;
    }
    SMOAngle = -atan((int16)gBemf.BEMF_Beta, (int16)gBemf.BEMF_Alpha);
}
#endif
