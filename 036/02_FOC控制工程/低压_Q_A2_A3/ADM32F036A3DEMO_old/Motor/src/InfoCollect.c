
#include "MotorInclude.h"
#include "Record.h"
#include "Main.h"
#include "FuncUser.h"

/****************************************************************
	ADC的通用处理: ADC模块本身的漂移处理
*****************************************************************/
void ADCProcess(void)
{
	gADC.ResetTime++;
	gADC.ResetTime = (gADC.ResetTime>1000)?1000:gADC.ResetTime;
	
	if(gADC.ZeroCnt >= 100)		return;			//ADC模块漂移检测完毕

#if 0
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
unsigned long IbusCoff = 20000;
unsigned long m_Ibus,m_IbusFilter,Result_Ibus,IbusActual;
void GetIDCInfo(void)
{
    m_Ibus = ADC_IBUS;
    m_IbusFilter = m_IbusFilter - (m_IbusFilter>>3) + (m_Ibus>>3);
    Result_Ibus = m_IbusFilter - gExcursionInfo.ErrIbus;
    IbusActual = Result_Ibus * IbusCoff>>12;//0.01A
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
long m_Iu,m_Iv,m_Iw;
int Adc_Temp_IDC1,Adc_Temp_IDC2;
int IuLast;
int IuErrorCnt;
void GetCurrentInfo(void)
{
    static int LastCurrSampleNumAct;
    int CurrSampleNumActUsed;
    if (LastCurrSampleNumAct != gPWM_CMP.CurrSampleNumAct) {
        CurrSampleNumActUsed = LastCurrSampleNumAct;
    } else {
        CurrSampleNumActUsed = gPWM_CMP.CurrSampleNumAct;
    }

    Adc_Temp_IDC1 = ADC_IV;
    Adc_Temp_IDC2 = ADC_IV1;

#if 1
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
#else
    switch(CurrSampleNumActUsed)
    {
        case IU_INV:
        case IU_INV_EST:
        {
            m_Iu = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIu;
            m_Iu = (m_Iu * gCurSamp.Coff)>>11;

            if(CurrSampleNumActUsed == IU_INV)
            {
                m_Iv = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
            }
            else
            {
                m_Iv = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
            }
            m_Iv = (m_Iv * gCurSamp.Coff)>>11;

            m_Iw = - m_Iu - m_Iv;
            break;
        }
        case IU_INW:
        case IU_INW_EST:
        {
            m_Iu = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIu;
            m_Iu = (m_Iu * gCurSamp.Coff)>>11;

            if(CurrSampleNumActUsed == IU_INW)
            {
                m_Iw = gExcursionInfo.ErrIw - (long)Adc_Temp_IDC2;
            }
            else
            {
                m_Iw = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIw;
            }
            m_Iw = (m_Iw * gCurSamp.Coff)>>11;

            m_Iv = - m_Iu - m_Iw;
            break;
        }
        case IV_INU:
        case IV_INU_EST:
        {
            m_Iv = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
            m_Iv = (m_Iv * gCurSamp.Coff)>>11;

            if(CurrSampleNumActUsed == IV_INU)
            {
                m_Iu = gExcursionInfo.ErrIu - (long)Adc_Temp_IDC2;
            }
            else
            {
                m_Iu = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIu;
            }
            m_Iu = (m_Iu * gCurSamp.Coff)>>11;

            m_Iw = - m_Iv - m_Iu;
            break;
        }
        case IV_INW:
        case IV_INW_EST:
        {
            m_Iv = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIv;
            m_Iv = (m_Iv * gCurSamp.Coff)>>11;

            if(CurrSampleNumActUsed == IV_INW)
            {
                m_Iw = gExcursionInfo.ErrIw - (long)Adc_Temp_IDC2;
            }
            else
            {
                m_Iw = (int)Adc_Temp_IDC2 - gExcursionInfo.ErrIw;
            }
            m_Iw = (m_Iw * gCurSamp.Coff)>>11;

            m_Iu = - m_Iv - m_Iw;
            break;
        }
        case IW_INU:
        case IW_INU_EST:
        {
            m_Iw = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIw;
            m_Iw = (m_Iw * gCurSamp.Coff)>>11;

            if(CurrSampleNumActUsed == IW_INU)
            {
                m_Iu = gExcursionInfo.ErrIu - (long)Adc_Temp_IDC2;
            }
            else
            {
                m_Iu = (int)Adc_Temp_IDC2 - gExcursionInfo.ErrIu;
            }
            m_Iu = (m_Iu * gCurSamp.Coff)>>11;
            m_Iv = - m_Iu - m_Iw;
            break;
        }
        case IW_INV:
        case IW_INV_EST:
        {
            m_Iw = (long)Adc_Temp_IDC1 - gExcursionInfo.ErrIw;
            m_Iw = (m_Iw * gCurSamp.Coff)>>11;

            if(CurrSampleNumActUsed == IW_INV)
            {
                m_Iv = gExcursionInfo.ErrIv - (long)Adc_Temp_IDC2;
            }
            else
            {
                m_Iv = (long)Adc_Temp_IDC2 - gExcursionInfo.ErrIv;
            }
            m_Iv = (m_Iv * gCurSamp.Coff)>>11;

            m_Iu = - m_Iv - m_Iw;
            break;
        }
        default:
            break;
    }
#endif

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
}

long GetCurrentIPD(int SectionData)
{
    long ReturnValue;

    switch (SectionData) {
        case 0:
        case 2:
        case 4:
            ReturnValue = ADC_IV1;
        break;
        case 1:
        case 3:
        case 5:
            ReturnValue = -ADC_IV1;
        break;
        default:
        break;
    }

    return ReturnValue;
}

int BemfA_Adc,BemfB_Adc,BemfC_Adc;
int LastBemfA_Adc;
long CounterInOneFreqCyle;
long CounterInOneFreqCyleOmg;
long LastCounterInOneFreqCyleOmg;
long LastLastCounterInOneFreqCyleOmg;
long StartReadingSpeedBasedBemfCnt;
int SectorOFBemf = 1;   // must be not 0
int LastSectorOFBemf;
int ReadSpeedBasedonFinished = 0;
unsigned int PostiveCnt, NegativeCnt;
int VolOffset;
int g_InitPos2;
unsigned int g_InitPos4;
ALPHABETA_STRUCT gEmfAlphBeta;
//long BemfData[6];
void GetBemf_DirSpeed(void)
{
    static unsigned int NegativeVolCnt = 0,PostiveVolCnt = 0;

    BemfA_Adc = ADCRESULT_UU;
    BemfB_Adc = ADCRESULT_UV;
    BemfC_Adc = ADCRESULT_UW;

    VolOffset = MAX( BemfA_Adc, BemfB_Adc );
    VolOffset = MAX( VolOffset, BemfC_Adc );

    if (gMainStatus.RunStep == STATUS_SPEED_CHECK)
    {
        // 判断方向
        if (BemfA_Adc > BemfB_Adc) {
            if (BemfB_Adc > BemfC_Adc) {
                SectorOFBemf = 6;                   // UA > UB > UC
            } else {
                if (BemfC_Adc > BemfA_Adc) {
                    SectorOFBemf = 5;               // UA > UC > UB
                } else {
                    SectorOFBemf = 4;               // UC > UA > UB
                }
            }
        } else {
            if (BemfB_Adc < BemfC_Adc) {
                SectorOFBemf = 3;                   // UC > UB > UA
            } else {
                if (BemfA_Adc > BemfC_Adc) {
                    SectorOFBemf = 1;               // UB > UA > UC
                } else {
                    SectorOFBemf = 2;               // UB > UC > UA
                }
            }
        }

        /// 5->3->2->1->6->4 - 正转
        if (LastSectorOFBemf == 5)
        {
            if (SectorOFBemf == 3) PostiveCnt++;
            if (SectorOFBemf == 4) NegativeCnt++;
        }
        else if(LastSectorOFBemf == 3)
        {
            if (SectorOFBemf == 2) PostiveCnt++;
            if (SectorOFBemf == 5) NegativeCnt++;
        }
        else if(LastSectorOFBemf == 2)
        {
            if (SectorOFBemf == 1) PostiveCnt++;
            if (SectorOFBemf == 3) NegativeCnt++;
        }
        else if(LastSectorOFBemf == 1)
        {
            if (SectorOFBemf == 6) PostiveCnt++;
            if (SectorOFBemf == 2) NegativeCnt++;
        }
        else if(LastSectorOFBemf == 6)
        {
            if (SectorOFBemf == 4) PostiveCnt++;
            if (SectorOFBemf == 1) NegativeCnt++;
        }
        else if(LastSectorOFBemf == 4)
        {
            if (SectorOFBemf == 5) PostiveCnt++;
            if (SectorOFBemf == 6) NegativeCnt++;
        }
        LastSectorOFBemf = SectorOFBemf;

        // 计算转速
        CounterInOneFreqCyle++;
        if (BemfA_Adc == 1)
        {
            PostiveVolCnt = 0;
            NegativeVolCnt++;
            if (NegativeVolCnt >= (2)) {
                LastBemfA_Adc = 1;
                NegativeVolCnt = 0;
            }
        } else {
            NegativeVolCnt = 0;
        }
        if (LastBemfA_Adc == 1)
        {
            if (BemfA_Adc >= 2) {
                PostiveVolCnt++;
                if (PostiveVolCnt <= (2)) {
                    BemfA_Adc = 1;
                }
            } else {
                PostiveVolCnt = 0;
            }
        }

        if (BemfA_Adc != 1 && LastBemfA_Adc == 1)
        {
            StartReadingSpeedBasedBemfCnt++;
            if (CounterInOneFreqCyle >= 2000)
            {
                CounterInOneFreqCyleOmg = 2000;
                CounterInOneFreqCyle = 2000;
            } else {
                if(StartReadingSpeedBasedBemfCnt >= 2)            //丢掉第一次周期值
                {
//GpioDataRegs.GPATOGGLE.bit.GPIO16 = 1;
//hhldfdfd[3]++;
                    if (PostiveCnt > NegativeCnt) {
                        LastCounterInOneFreqCyleOmg =  CounterInOneFreqCyle;
                    } else {
                        LastCounterInOneFreqCyleOmg = -CounterInOneFreqCyle;
                    }
                }
                CounterInOneFreqCyle = 0;
            }
            if (gFeisu.VoltCNT > 100)
            {
                ReadSpeedBasedonFinished = 1;
//hhldfdfd[0] = 20000 / LastLastCounterInOneFreqCyleOmg;
//hhldfdfd[1] = 20000 / LastCounterInOneFreqCyleOmg;
//hhldfdfd[2] = 20000 / CounterInOneFreqCyleOmg;
            }

            CounterInOneFreqCyleOmg = (LastLastCounterInOneFreqCyleOmg + LastCounterInOneFreqCyleOmg) >> 1;
            LastLastCounterInOneFreqCyleOmg = LastCounterInOneFreqCyleOmg;
        }
        if (BemfA_Adc != 1) LastBemfA_Adc = BemfA_Adc;

        // 计算角度
        // clark
        long temp1,temp2;
        temp1 = ( (long)(BemfA_Adc * 2 - BemfB_Adc - BemfC_Adc) * 10923 ) >> 15;
        temp2 = (((long)BemfB_Adc - (long)BemfC_Adc) * 18919L)>>15;

        gEmfAlphBeta.Alph = Filter2(temp1,gEmfAlphBeta.Alph);
        gEmfAlphBeta.Beta = Filter2(temp2,gEmfAlphBeta.Beta);

        if (LastCounterInOneFreqCyleOmg > 0)
        {
            //theta = atan（alpha/beta）
            g_InitPos2 = atan((int)gEmfAlphBeta.Beta, (int)gEmfAlphBeta.Alph);
        }
        else
        {
            //theta = -atan（beta/alpha）
            //g_InitPos2 = atan((int)gEmfAlphBeta.Beta, (int)gEmfAlphBeta.Alph);
            g_InitPos2 = -atan((int)gEmfAlphBeta.Alph, (int)gEmfAlphBeta.Beta);
//BemfData[0] = g_InitPos2;
//BemfData[1] = atan((int)gEmfAlphBeta.Beta, (int)gEmfAlphBeta.Alph);    // 5461 (30度)
//BemfData[2] = -atan((int)gEmfAlphBeta.Beta, (int)gEmfAlphBeta.Alph);   // -5461
//BemfData[3] = atan((int)gEmfAlphBeta.Alph, (int)gEmfAlphBeta.Beta);    // 10923(60度)
//BemfData[4] = -atan((int)gEmfAlphBeta.Alph, (int)gEmfAlphBeta.Beta);   // -10923
        }

        if (g_InitPos2 < 0) g_InitPos2 += 65535; //0-65535   0-360
        g_InitPos4 = g_InitPos2;

        pm_est_angel = ((long)g_InitPos4 * 3217UL) >> 8;

        pm_integral_d = 0;
        pm_integral_q = ( (long)300 ) << 12;
        pm_integral_speed = ( (long)300 ) << 11;

        if (LastCounterInOneFreqCyleOmg < 0)
        {
            pm_integral_q       = -pm_integral_q;
            pm_integral_speed   = -pm_integral_speed;
        }
    } else {
        ReadSpeedBasedonFinished = 0;
        StartReadingSpeedBasedBemfCnt = 0;
        BemfA_Adc = 1;
        BemfB_Adc = 1;
        BemfC_Adc = 1;
        PostiveCnt = 0;
        NegativeCnt = 0;
        CounterInOneFreqCyle = 0;
        CounterInOneFreqCyleOmg = 0;
    }

/*
if (CounterInOneFreqCyleOmg < 0)  {
    DirSpeedData[0]++;
    GpioDataRegs.GPATOGGLE.bit.GPIO16 = 1;
}
else {
    DirSpeedData[1]++;
}*/
#if 0
{
static int EnableRecord = 0;
static int EnableRecordCnt = 0;
static int EnableRecord2 = 0;
if (gMainCmd.FreqReal >= 10000 && gMainCmd.Command.bit.Start == TRUE) {
    EnableRecord = 1;
}
if (EnableRecord == 1 && gMainCmd.Command.bit.Start == 0) {
    EnableRecordCnt++;
    if (EnableRecordCnt > 50) {
        EnableRecord2 = 1;
    }
}
if (EnableRecord2 == 1) {
ControlAllowRecord(1,4);
IsRecordThisSample(
        BemfA_Adc,
        BemfB_Adc,
        BemfC_Adc,
        LastLastCounterInOneFreqCyleOmg,
        LastCounterInOneFreqCyleOmg,
        CounterInOneFreqCyleOmg,
        20000 / (CounterInOneFreqCyleOmg * 1));
}
}
#endif
}
