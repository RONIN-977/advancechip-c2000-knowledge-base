
/*******************************************************************************
 * 文件名称: InvProtect.c
 * 功    能: 电机控制部分保护任务模块 —
 *          包含主状态机、载波频率计算、运行状态处理
 *           （低压、参数辨识、转速检测、运行、开环加速、制动、短路检测）、
 *           软件/硬件错误处理、温度故障和缺项故障灯。
 * 作    者:
 * 版    本: V1.0
 * 修改日期:
 * 说    明: 系统周期调度入口：SystemLeve2msMotor（2ms）、
 ******************************************************************************/

#include "MotorInclude.h"
#include "Record.h"


/***************************************************************************
  Private Variables
 ***************************************************************************/
static int16 PhaseLossCnt;

/***************************************************************************
  Private Functions
 ***************************************************************************/
#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
static void OutputPhaseLoseDetectNew(void);
#endif
static void OutputPhaseLoseDetect(void);
static void CBCLimitCur(void);
static void SensorlessLockMonitor(void);
static void OutputLoseReset(void);

/************************************************************
	变频器保护处理
************************************************************/
void InvDeviceKontrol(void)			
{
	if (gADC.ResetTime < 500)	return;	//AD采样稳定后开始

	//TemperatureCheck();				// 温度检查
	OutputPhaseLoseDetect();			// 输出缺相检测
#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
	OutputPhaseLoseDetectNew();         // 单电阻不用...
#endif
	CBCLimitCur();
	SensorlessLockMonitor();
}

#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
int32 MadddData[30];
static void OutputPhaseLoseDetectNew(void)
{
    static int16 m_U,m_V,m_W;
    static int32 IuAcc,IvAcc,IwAcc;
    static Uint16 IuAccCounter;

    // ADM32F036, Positive Logic, OutPhase = 0;  U:2000, VW -1000
    // “不是直流制动” 或 “处于运行或开环或纯开环或SPIN 以外” 模式下 不执行输出NEW缺项检测
    if ((gMainCmd.Command.bit.StartDC != 1) ||
        ((gMainStatus.RunStep != STATUS_RUN         ) &&
         (gMainStatus.RunStep != HALF_OPEN_ACC      ) &&
         (gMainStatus.RunStep != OPEN_ACC           ) &&
         (gMainStatus.RunStep != STATUS_SPEED_CHECK )))
/* -----------------------------------------------------------------
    直流制动且处于运行或开环或纯开环或SPIN 之一模式下执行输出NEW缺项检测
    if ((gMainCmd.Command.bit.StartDC == 1) &&
        ((gMainStatus.RunStep == STATUS_RUN         ) ||
         (gMainStatus.RunStep == HALF_OPEN_ACC      ) ||
         (gMainStatus.RunStep == OPEN_ACC           ) ||
         (gMainStatus.RunStep == STATUS_SPEED_CHECK )))
-------------------------------------------------------------------*/
    {
//MadddData[5]++;
        IuAcc = IvAcc = IwAcc = 0;
        IuAccCounter = 0;
        return;
    }
    if (IuAccCounter == 256) { //
        m_U = IuAcc >> 8; if (m_U < 0) m_U = -m_U;
        m_V = IvAcc >> 8; if (m_V < 0) m_V = -m_V;
        m_W = IwAcc >> 8; if (m_W < 0) m_W = -m_W;
//MadddData[0]++;
//MadddData[1]=m_U;
//MadddData[2]=m_V;
//MadddData[3]=m_W;
        if ((m_U < 350) || (m_V < 350) || (m_W < 350)) {
//MadddData[6]=gMainCmd.Command.bit.StartDC;
//MadddData[7]=gMainStatus.RunStep;
//MadddData[8]=gPhase.OutPhase;
//MadddData[9]++;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_LOSE_PHASE_OUTPUT;
        }
    } else if (IuAccCounter < 256) {
        IuAccCounter++;
//MadddData[4]++;
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
#endif

/************************************************************
	输出缺相检测
************************************************************/
static void OutputPhaseLoseDetect(void)
{
    Uint16 m_U = 0,m_V,m_W = 0;
    Uint32 m_Max = 0,m_Min = 0;

    if ((gMainCmd.FreqReal      < 500       ) ||
        (gMainStatus.ErrorCode != ERROR_NONE) ||
        (gMainCmd.Command.bit.StartDC == 1  ) ||
        (gMainStatus.RunStep != STATUS_RUN  ))
    {
        PhaseLossCnt = 0;
        OutputLoseReset();
        return;
    }

    // 0.5ms, then 0.5s here....
    // Half Cycle:0.5S, then 1HZ,
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
//MadddData[21]=m_Max;
//MadddData[22]=m_Min;
//MadddData[23]=m_U;
//MadddData[24]=m_V;
//MadddData[25]=m_W;
        PhaseLossCnt++;
//GpioDataRegs.GPBTOGGLE.bit.GPIO32  = 1;
//MadddData[26]=PhaseLossCnt;
        if (PhaseLossCnt >= 5) {
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_LOSE_PHASE_OUTPUT;
        }
    } else {
//MadddData[27]++;
//MadddData[28]=m_Max;
//MadddData[29]=m_Min;
        if (PhaseLossCnt > 1) PhaseLossCnt--;
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

static void OutputLoseReset(void)		//输出缺相检测复位寄存器处理
{
	gPhaseLose.Cnt = 0;
	gPhaseLose.TotalU = 0;
	gPhaseLose.TotalV = 0;
	gPhaseLose.TotalW = 0;
	gPhaseLose.Time   = 0;
}

/************************************************************
    判断UVW三相是否处于逐波限流状态，并且设置标志
************************************************************/
static void CBCLimitCur(void)
{
    EALLOW;
    if ((EPwm2Regs.TZFLG.bit.CBC     == 1) ||
        (EPwm3Regs.TZFLG.bit.CBC     == 1) ||
        (EPwm4Regs.TZFLG.bit.CBC     == 1) ||
        (EPwm2Regs.TZFLG.bit.DCBEVT2 == 1) ||
        (EPwm3Regs.TZFLG.bit.DCBEVT2 == 1) ||
        (EPwm4Regs.TZFLG.bit.DCBEVT2 == 1))
    {
        // Clear the Flag otherwise the flag will always be 1 even NO CBC
        EPwm2Regs.TZCLR.bit.CBC = 1;
        EPwm3Regs.TZCLR.bit.CBC = 1;
        EPwm4Regs.TZCLR.bit.CBC = 1;

        EPwm2Regs.TZCLR.bit.DCBEVT2 = 1;
        EPwm3Regs.TZCLR.bit.DCBEVT2 = 1;
        EPwm4Regs.TZCLR.bit.DCBEVT2 = 1;

        gCBCProtect.CbcTimes++;
    } else {
        if (gCBCProtect.CbcTimes > 1) gCBCProtect.CbcTimes--;
    }
    EDIS;

    if (gCBCProtect.CbcTimes > 5000) {
        DisableDrive();
        gMainStatus.ErrorCode = ERROR_CBC;
    }
}

//long SoftErrorData[10];
/*************************************************************
    软件故障处理
*************************************************************/
void SoftWareErrorDeal(void)
{
    static int16 SoftwareOCCnt;

    // 欠压状态下不判断软件故障
    if ((gMainStatus.RunStep   == STATUS_LOW_POWER        ) ||
        (gMainStatus.ErrorCode == ERROR_MOTOR_SHORT_TO_GND))
    {
        return;
    }

    // 开始故障复位
    if (gMainCmd.Command.bit.ErrorOK == 1)
    {
        gMainStatus.ErrorCode = ERROR_NONE;
        if (gMainStatus.ErrFlag.bit.OvCurFlag == 1)
        {
            gMainStatus.ErrFlag.bit.OvCurFlag = 0;
            ClearAllTZFlags();
        }
    }

    /////////////////////////////////////////////////开始判断软件故障
    // 软件过流实际意义不大，但就当多一重保护而已吧.用硬件相同的过流点...
    if ((gLineCur.CurPer            > SOFT_OC_POINT) &&
        (gMainCmd.Command.bit.Start == TRUE        ))
    {
        SoftwareOCCnt++;
        if (SoftwareOCCnt >= 10) {
            SoftwareOCCnt = 0;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_OC_SOFTWARE;
        }
    } else {
        if (SoftwareOCCnt > 1) SoftwareOCCnt--;
    }

    if (gUDC.uDC > gInvInfo.InvUpUDC)           //过压判断,使用大滤波电压
    {
        DisableDrive();
        gMainStatus.ErrorCode = ERROR_OV_ACC_SPEED;
    }
    else if (gUDC.uDC < gInvInfo.InvLowUDC)   //欠压判断,使用大滤波电压
    {
        DisableDrive();
        DisConnectRelay();
        gMainStatus.RunStep = STATUS_LOW_POWER;
        gMainStatus.SubStep = 0;
        gMainStatus.ErrorCode = ERROR_UV;
        gMainStatus.StatusWord.bit.LowUDC = 1;
    }

    if (gMainCmd.Command.bit.Start != TRUE)
    {
        SoftwareOCCnt = 0;
    }

    // ERROR_STALL 处理
    static int16 InstallCnt2;
    if ((labs(pm_ref_speed >> 4)       > STALL_FAULT_REF          ) &&     // Ref > 80Hz,
        (labs(pm_est_omg   >> 4)       < STALL_FAULT_OMG          ) &&     // omg > 80Hz,
        (gMainStatus.ErrorCode        != ERROR_LOSE_PHASE_OUTPUT  ) &&
        (gMainCmd.Command.bit.StartDC != 1                        ) &&
        (labs(pm_ref_iq)               > labs(pm_curr_limitH - 100))) {
        InstallCnt2++;
        if (InstallCnt2 > 250) {
            InstallCnt2 = 0;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_STALL;
        }
    } else {
        // 当计数器为 1 时，InstallCnt2 -= 2 得到 -1。下次触发检查 OK，但负值意味着计数器
        // 计数需要更长的时间才能回到触发阈值——实质上改变了保护行为。行为也可能因有符号实现定义的行为而变差。
        // if (InstallCnt2 > 0)  InstallCnt2 -= 2;
        if (InstallCnt2 > 0)  InstallCnt2 -= 1;
    }

#if 0
    // 风扇的闭环启动堵转问题
    // 核心痛点：
    // PM_EST_OMG 值波动比较大，有时候会超过PM_REF_SPEED.所以常规做法无法检测
    // 考虑用电流法：
    // 运行100HZ 频率看IQ 值，比如2000. 那么闭环启动阶段（1~ 60HZ 左右）时候正常电流都比这个小
    // 堵转的时候，电流会上升很快，
    // 用 2000 * 1.5 做比较点，单纯考虑就是频率在80HZ 下电流不会超过 3000，否则堵转！！！
    /////////////////////////////////////////////////////////////////////////////////////
    //  ^                                           . 1900
    //  |
    //  |
    //  |
    //  |                                      . 1550
    //  |
    //  |
    //  |                                  . 1300
    //  |
    //  |                              . 1000
    //  |                          .800
    //  |                     .600
    //  |                .350
    //  |
    //  | __________________________________________________> HZ
    // 0                40    50   60   70  80  90  100
    /////////////////////////////////////////////////////////////////////////////////////
    #define IRMS_AT_100HZ  1900
    #define IRMS_AT_90HZ   1550
    #define IRMS_AT_80HZ   1300
    #define IRMS_AT_70HZ   1000
    #define IRMS_AT_60HZ   800
    #define IRMS_AT_50HZ   600
    #define IRMS_AT_40HZ   350

    int16 RMSTripPointForStall;
    int32 Data;
    Data = IRMS_AT_100HZ;
    RMSTripPointForStall = (Uint16)qsqrt(Data * Data + pm_ref_id * pm_ref_id);
    RMSTripPointForStall *= 3;
    RMSTripPointForStall /= 2;

    // 应该计算一个对应频率下正常运行电流...
    static int16 InstallCnt2;
    if ((gMainCmd.Command.bit.StartDC         != 1                  ) &&
        (gMainStatus.RunStep                  == STATUS_RUN         ) &&
        (pm_ref_speed                         < 160000              ) &&
        (pm_ref_iq                            > RMSTripPointForStall)) {
SoftErrorData[6]++;
       InstallCnt2++;
         if (InstallCnt2 > 800) {    // 2S
SoftErrorData[7]++;
             InstallCnt2 = 0;
             DisableDrive();
             gMainStatus.ErrorCode = ERROR_STALL;
         }
     } else {
SoftErrorData[8]++;
          if (InstallCnt2 > 0)  InstallCnt2 -= 1;
     }
#endif

#ifdef OPEN_LOOP_START
    // Open Loop state Stall Protection...
    static int16 InstallCnt3;
    if ((gMainStatus.RunStep   == OPEN_ACC                  ) &&
        (gMainStatus.ErrorCode != ERROR_LOSE_PHASE_OUTPUT   ) &&
        (gMainCmd.Command.bit.StartDC != 1                  ) &&
        (gLineCur.CurPerFilter >> 5 > (8000 - 300  )        )) {
        InstallCnt3++;
        if (InstallCnt3 > 500) {
            InstallCnt3 = 0;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_STALL;
        }
    } else {
        if (InstallCnt3 > 0)  InstallCnt3 --;
    }
#endif

// For test
#if 0
    // 超速监测，速度超调过大。
    // 一般情况：
    // 1：没接电机，速度观察值不对。
    // 2：高速PI 太小，加速时候速度超调太多。（比如冰箱压机负载，重再启动可能会）
    // 视情况，可以屏蔽。
    static int16 InstallCnt1;
    if (pm_fast_freq != 0)
    {
        if ((pm_est_omg > pm_fast_freq) || (pm_est_omg < -pm_fast_freq))
        {
            InstallCnt1++;
            if (InstallCnt1 > 5) {
                DisableDrive();
                gMainStatus.ErrorCode = ERROR_ERROR_KE;
            } else {
                InstallCnt1 = 0;
            }
        }
    }
#endif

    // 干烧保护。速度达到一定值且负载很小的情况，持续一定时间。。。
    ////////////////////////////////////////////////////////
    static int16 InstallCnt4;
    if ((labs(pm_ref_speed >> 4)        > NO_LOAD_FAULT_FREQUENCY) &&     // Ref > 80Hz,
        (labs(pm_est_omg   >> 4)        > NO_LOAD_FAULT_FREQUENCY) &&     // omg > 80Hz,
        (gLineCur.CurPer                < NO_LOAD_FAULT_IRMS_CUR ) &&     // 1 / 5 额定值
        (gMainCmd.Command.bit.StartDC  != 1                      )) {
        InstallCnt4++;
        if (InstallCnt4 > 1000) {
            InstallCnt4 = 0;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_NO_LOAD;
        }
    } else {
        if (InstallCnt4 > 0)  InstallCnt4 -= 1;
    }
}

#define OMG_RUN_MIN 40              // 50HZ
#define BEM_OMG_RATIO_MIN   1000    // 180HZ*1602/276 = 1040.
                                    // 180HZ*1602/442 = 665.
// F(HZ)    PM_EST_BEM(0.4.Noraml)      UN_NORMAL_VALUE(0.65)
// 40       95                          60
// 60       142                         92
// 80       190                         116
// 120      290                         184
// 180      442                         276
// K = 0.4,
// K = 0.65, 1.625,避免误报的情况
/////////////////////////////////////////////


#define IQ_HIGH_TH  2048            // 50HZ
#define BEM_LOW_TH  184             // 50HZ
// DEMO_ Motor, RatedCUR = 20A, 150HZ RMS=0A, BEM = 368

//extern int32 pm_est_bem;
int32 PhaseUNLockData[10];
static void SensorlessLockMonitor(void)
{
#if 0
    static Uint16 bem_omega_mismatch_cnt;
    static Uint16 bem_omega_mismatch_flag;
    static Uint16 stall_suspect_cnt;
    static Uint16 stall_suspect_flag;

    if ((abs(pm_est_omg_1 >> 4) < OMG_RUN_MIN) ||
        (gMainStatus.ErrorCode != ERROR_NONE ) ||
        (gMainStatus.RunStep != STATUS_RUN   ))
    {
PhaseUNLockData[0]++;
        bem_omega_mismatch_cnt = 0;
        stall_suspect_cnt = 0;
        return;
    }

    if (abs(pm_est_omg_1 >> 4) > OMG_RUN_MIN) {
        if (abs(pm_est_omg_1 / pm_est_bem) > BEM_OMG_RATIO_MIN) {
            bem_omega_mismatch_cnt++;
PhaseUNLockData[1]++;
            if (bem_omega_mismatch_cnt > 2000) {
                bem_omega_mismatch_flag = 1;
            }
        } else {
            if (bem_omega_mismatch_cnt > 2) bem_omega_mismatch_cnt -= 2;
        }
    }

    if (abs(pm_ref_iq) > IQ_HIGH_TH || abs(pm_est_bem) < BEM_LOW_TH) { // && run_time_after_switch > T_BLANK) {
        stall_suspect_cnt++;
PhaseUNLockData[2]++;
        if (stall_suspect_cnt > 2000) {
            stall_suspect_flag = 1;
        }
    } else {
        if (stall_suspect_cnt > 2) stall_suspect_cnt -= 2;
    }
PhaseUNLockData[3]=bem_omega_mismatch_cnt;
PhaseUNLockData[4]=stall_suspect_cnt;
PhaseUNLockData[5]=abs(pm_est_omg_1 / pm_est_bem);
PhaseUNLockData[6]=BEM_OMG_RATIO_MIN;

    if ((stall_suspect_flag == 1) || (bem_omega_mismatch_flag == 1)) {
PhaseUNLockData[7]++;
//        DisableDrive();
//        gMainStatus.ErrorCode = 77;
    }
#endif
}
