
#include "MotorInclude.h"
#include "Record.h"
#include "Main.h"
#include "FuncUser.h"
#include "Parameter.h"

/****************************************************************
	ADC的通用处理: ADC模块本身的漂移处理
*****************************************************************/
int EnableRecord = 0;
void ADCProcess(void)
{
	gADC.ResetTime++;
	gADC.ResetTime = (gADC.ResetTime>1000)?1000:gADC.ResetTime;
	
#if 0
	if(gADC.ZeroCnt >= 100)		return;			//ADC模块漂移检测完毕

	// Record 实际测试结果，m_Zero = 0. 
	// AdcRegs.ADCOFFTRIM.all = 0
	///////////////////////////////////
	m_Zero = ((int)ADC_GND)>>4;					//开始ADC模块漂移检测
	if(m_Zero == 0)
	{
		gADC.ZeroCnt = 0;
		gADC.Comp ++;
		if(gADC.Comp > 255)
		{
			gADC.Comp = 255;
			gADC.ZeroCnt = 100;
		}
		AdcRegs.ADCOFFTRIM.all = gADC.Comp;
	}
	else if(gADC.ZeroCnt < 32)
	{
		gADC.ZeroCnt ++;
		gADC.ZeroTotal += m_Zero;
	}
	else if(gADC.ZeroCnt == 32)
	{
		gADC.ZeroCnt = 100;
		gADC.Comp = gADC.Comp - (gADC.ZeroTotal>>5);
		gADC.Comp = (gADC.Comp < -255)?-255:gADC.Comp;
		AdcRegs.ADCOFFTRIM.all = gADC.Comp;
	}
#endif
}

/****************************************************************
	获取母线电压数据，输出gUDC
*****************************************************************/
void GetUDCInfo(void)
{
	Uint m_uDC;
    long ADCTemp = ADC_UDC;
   	m_uDC = ((Uint32)(ADCTemp) * gUDC.Coff)>>16;
   	gUDC.uDC = (gUDC.uDC + m_uDC)>>1;
#if 0
    gUDC.uDCFilter = gUDC.uDCFilter - (gUDC.uDCFilter>>3) + (gUDC.uDC>>3);
    gUDC.uDCBigFilter = Filter32(gUDC.uDC,gUDC.uDCBigFilter);
#else
    static long UdcAcc, UdcAcc2;
    UdcAcc = ((((long)gUDC.uDC << 16) - UdcAcc) >> 3) + UdcAcc;
    gUDC.uDCFilter  = (UdcAcc + 0x8000) >> 16;
    UdcAcc2 = ((((long)gUDC.uDC << 16) - UdcAcc2) >> 6) + UdcAcc2;
    gUDC.uDCBigFilter  = (UdcAcc2 + 0x8000) >> 16;
#endif
}

/****************************************************************
    获取母线电流数据
*****************************************************************/
// long GetIDData[6];
void GetIDCInfo(void)
{
    int m_Ibus,Result_Ibus;
    static long m_IbusFilter;
    m_Ibus = ADC_IBUS;
    m_IbusFilter = m_IbusFilter - (m_IbusFilter >> 1) + (m_Ibus >> 1);
    Result_Ibus = m_IbusFilter - gExcursionInfo.ErrIbus;
    IbusActual = Result_Ibus * MAX_PEAK_CUR_3_3V_2 >> 11;       //0.01A

    int Temp32;
    // Temp32 = (long)gUDC.uDCFilter * IbusActual / 100;
    Temp32 = (long)gUDC.uDCFilter * IbusActual >> 6;
    Temp32 = (long)Temp32 * 41 >> 6;
    ActualPower = Temp32;                                       //0.1W
}

/****************************************************************
	获取温度采样数据
*****************************************************************/
void GetTemperatureInfo(void)
{
	Uint m_Result;

	m_Result = (ADC_TEMP & 0xFFF0);

	if(gADC.ResetTime < 100)
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
    Uint m_Result = ADC_DSP_TEMP;

    // InternalTemperatureDegree = (ADC_DSP_TEMP - 1511) * 0.181;
    InternalTemperatureDegree = ((long)(ADC_DSP_TEMP - 1511) * 5931) >> 15;
}

/****************************************************************
    获取电流采样数据，输出gCurSamp
*****************************************************************/
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
long m_Iu,m_Iv,m_Iw;
int Adc_Temp_IDC1,Adc_Temp_IDC2;
#pragma CODE_SECTION(SVPWM_1ShuntGetPhaseCurrent, "ramfuncs");
void SVPWM_1ShuntGetPhaseCurrent(void)
{
    static int LastCurrSampleNumAct;
    int CurrSampleNumActUsed;
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
                m_Iu = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IU_INV_EST)
            {
                m_Iv = gCurSamp.V;
            }
            else if(CurrSampleNumActUsed == IU_IV)
            {
                m_Iv = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iv = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
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
                m_Iu = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IU_INW_EST)
            {
                m_Iw = gCurSamp.W;
            }
            else if(CurrSampleNumActUsed == IU_IW)
            {
                m_Iw = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iw = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
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
                m_Iv = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IV_INU_EST)
            {
                m_Iu = gCurSamp.U;
            }
            else if(CurrSampleNumActUsed == IV_IU)
            {
                m_Iu = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iu = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
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
                m_Iv = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IV_INW_EST)
            {
                m_Iw = gCurSamp.W;
            }
            else if(CurrSampleNumActUsed == IV_IW)
            {
                m_Iw = (int)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iw = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
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
                m_Iw = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IW_INU_EST)
            {
                m_Iu = gCurSamp.U;
            }
            else if(CurrSampleNumActUsed == IW_IU)
            {
                m_Iu = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iu = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
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
                m_Iw = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
                m_Iw = (m_Iw * gCurSamp.Coff)>>11;
            }

            if(CurrSampleNumActUsed == IW_INV_EST)
            {
                m_Iv = gCurSamp.V;
            }
            else if(CurrSampleNumActUsed == IW_IV)
            {
                m_Iv = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
                m_Iv = (m_Iv * gCurSamp.Coff)>>11;
            }
            else
            {
                m_Iv = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
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
        m_Iu = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
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

if (EnableRecord == 1) {
ControlAllowRecord(1,30);
IsRecordThisSample(
        gCurSamp.U,
        gCurSamp.V,
        gCurSamp.W,
        ADC_IV,//gPWM.U,//ADC_UDC >> 4,
        0,//gPWM.U,
        1,//gPWM.V,
        2,//gPWM.W,
        ((long)gLineCur.CurPer * 500) >> 12
        );//pm_ref_iq);
}

}
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT) || (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT))
#pragma CODE_SECTION(SVPWM_3ShuntGetPhaseCurrent, "ramfuncs");
void SVPWM_3ShuntGetPhaseCurrent(void)
{
    long m_Iu,m_Iv,m_Iw;
    int IuCur;
    int IvCur;
    int IwCur;

    int CurSelection;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    CurSelection = V_W_CUR_SEN;
#else
    CurSelection = GetPWMSector();
#endif
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

#if 0
{
static int EnableRecord = 0;
if (GetCur == 1) {// && gMainCmd.Command.bit.Start == TRUE) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,3);
IsRecordThisSample(
        ((long)((int)(ADC_IU - (Uint)32768) -(long)gExcursionInfo.ErrIu)* gCurSamp.Coff)>>15,//gCurSamp.U,
        ((long)((int)(ADC_IV - (Uint)32768) -(long)gExcursionInfo.ErrIv)* gCurSamp.Coff)>>15,//gCurSamp.V,
        ((long)((int)(ADC_IW - (Uint)32768) -(long)gExcursionInfo.ErrIw)* gCurSamp.Coff)>>15,//gCurSamp.W,
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
#endif
#endif

long GetCurrentIPD(int SectionData)
{
    long ReturnValue = 1;

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    switch (SectionData) {
        case 0:
        case 2:
        case 4:
            ReturnValue = (long)ADC_IV;
        break;
        case 1:
        case 3:
        case 5:
            ReturnValue = -(long)ADC_IV;
        break;
        default:
        break;
    }
#else
    switch (SectionData)
    {
        case 0:
            ReturnValue  = (-(long)ADC_IV - (long)ADC_IW)>>4;
        break;
        case 1:
            ReturnValue  = (long)ADC_IU>>4;
        break;
        case 2:
            ReturnValue  = (-(long)ADC_IU - (long)ADC_IW)>>4;
        break;
        case 3:
            ReturnValue  = (long)ADC_IV>>4;
        break;
        case 4:
            ReturnValue  = (-(long)ADC_IU - (long)ADC_IV)>>4;
        break;
        case 5:
            ReturnValue  = (long)ADC_IW>>4;
        break;
        default:
        break;
    }
#endif
    return ReturnValue;
}
