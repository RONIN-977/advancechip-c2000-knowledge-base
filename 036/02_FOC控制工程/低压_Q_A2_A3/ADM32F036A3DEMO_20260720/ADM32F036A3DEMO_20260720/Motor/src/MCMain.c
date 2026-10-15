/*******************************************************************************
 * 文件名称: MCMain.c
 * 功    能: 电机控制主任务模块 — 包含主状态机、载波频率计算、运行状态处理
 *           （低压、参数辨识、转速检测、运行、开环加速、制动、短路检测）、
 *           软件/硬件错误处理、电流环及速度环调度。
 * 作    者:
 * 版    本: V1.0
 * 修改日期:
 * 说    明: 系统周期调度入口：SystemLeve2msMotor（2ms）、
 *           SystemLeve05msMotor（0.5ms），以及 ADC 中断服务
 *           ADCOverInterrupt 和 MotorControlISR。
 ******************************************************************************/
#include "Main.h"
#include "MotorInclude.h"
#include "Record.h"
#include "Parameter.h"
#include "FuncUser.h"
#include "RunSrc.h"
#include "Reference.h"
#include "PreDriver.h"
#include "MCQpr.h"

/*=============================================================================
 *   本文件内部私有函数声明（static）
 *============================================================================*/
enum {
    BRAKE_LOW_BRIGE_INIT,
    BRAKE_LOW_BRIGE_LOWON,
    BRAKE_LOW_BRIGE_CHECK,
    BRAKE_LOW_BRIGE_FINISH
};
typedef enum {
    McInit              = 0,
    McAlignRun          = 1,
    McRsIDRun           = 2,
    McOpenRun           = 3,
    McSpdChangeToClosed = 4,
    McSFinished         = 5
} MCP_SPDSTATE_DEF;
static MCP_SPDSTATE_DEF McSpdState;        // 全局变量在STOP 或其他状态可以清零操作
static UDC_LIMIT_IT_STRUCT gUdcLimitIt;
static int16 BrakeLowBridgeState;
static int16 BrakeLowBridgeStateCnt;
static int16 McSpdStateCnt;
static int16 IDForAlignAndOpenLoop;
#ifdef FULL_OPEN_LOOP_MODE
static int16 FrqTemp;
#endif
#ifndef OPEN_LOOP_START
static int16 IDForDCBrakeInCurrentLoopMode;
#endif
static const int16 SHORT_GND_PERIOD = 5000;

/*=============================================================================
 *   内部函数原形（static）
 *============================================================================*/
/***************************************************************************
  Private Functions
 ***************************************************************************/
static void ParameterChange(void);
static void CalCarrierWaveFreq(void);
static void RunCaseDeal(void);
static void RunCaseLowPower(void);
static void RunCaseGetPar(void);
static void RunCaseSpeedCheck(void);
static void OpenLoopAccRun(void);
static void RunCaseRun(void);
static void RunCaseShortGnd(void);
static void RunCaseBrakeLowBridge(void);
static void RunCaseStop(void);
static void PrepareParForRun(void);
static void GetCurrentExcursion(void);
static void CalUdcLimitIT(void);
static void MotorControlISR(void);
static void SpeedLoop(void);
static void ChangeCurrent_2ms(void);
static Uint16 IsInRegenState(void);

/************************************************************
性能模块初始化程序：主循环前初始化性能部分的变量(所有等于0的变量无需初始化)
************************************************************/
void  InitForMotorApp(void)
{
    DisableDrive();

    gMainStatus.RunStep = STATUS_LOW_POWER;
    gMainStatus.SubStep = 1;
    gMainStatus.StatusWord.all = 0;
    gADC.DelaySet = 100;
    gFcCal.FcBak = 100;
    gBasePar.FcSetApply = 100;
    gPWM.gPWMPrd = C_INIT_PRD;
    gPWM.gPWMPrdApply = C_INIT_PRD;
    pm_control_mode = PMSM_CONTROL_MODE_FOC;
    DCStart_flag = 0;
    MotorPWMModeForNoise = SINGLE_SHUNT_PWM_MODE;
#if ((SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2) || \
     (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE))
    LowNoseElmPWMMode = 1;
#endif

    // CurrentSampleType2   双电阻 单电阻（NORMAL)  单电阻（LOW)  单电阻（LOW_2)
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    CurrentSampleType2 = CURRENT_1SHUNT_LOW_NOISE_2;
#elif (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE)
    CurrentSampleType2 = CURRENT_1SHUNT_LOW_NOISE;
#else
    CurrentSampleType2 = CURRENT_1SHUNT_NORAML_NOISE;
#endif
#else
    CurrentSampleType2 = CURRENT_2SHUNT_NORAML_NOISE;
#endif
}

/************************************************************
主程序的单2ms循环：用于执行电机控制程序
思路：数据输入->数据转换->控制算法->控制输出->自我保护
执行时间：不被中断打断的情况下约120us
************************************************************/
void SystemLeve2msMotor(void)
{
    ParGetFromFunction();
    ParameterChange();
    PMFluxWeaking();
    CalCarrierWaveFreq();
    CalStepAngle();
    RunCaseDeal();
    InvDeviceKontrol();
    SoftWareErrorDeal();
    GetCurrentExcursion();
    ParSendToFunction();

    // from interrupt function
    GetAIInfo();
    GetTemperatureInfo();
    ADCProcess();
    calc_pm_coef();
    GetUDCInfo();
    GetIDCInfo();
    ChangeCurrent_2ms();
    PMSMSVC_2ms();
    PMCalcLineCurr();

    QPRParameterCal(&Qpr_q);
    QPRParameterCal(&Qpr_d);
}

/************************************************************
主程序0.5ms循环：用于执行需要快速刷新的程序，要求程序非常简短
************************************************************/
void SystemLeve05msMotor(void)
{
    OutputLoseAdd();

    // 注意考虑下速度环带宽，风扇等大负载系统速度环周期0.5K（2ms）基本OK。
    // 后期多尝试2K ~0.5K 后区别
    // 比如减速过程需要快速制动（过压) 逻辑，比如IQ ID 的变化逻辑是否及时？？
    ////////////////////////////////////////////////////////////////////
    if (pm_control_mode != 0)
    {
        return;
    }
    RunCaseDeal();
}

/************************************************************
主程序不等待循环：用于执行需要快速刷新的程序，要求程序非常简短
************************************************************/
void SystemLeve0msMotor(void)
{

}

/*************************************************************
参数计算程序：完成参数转换、运行参数准备等工作
*************************************************************/
static void ParameterChange(void)
{
    Uint32  m_ULong;
    Uint16  m_UData;

    // gInvInfo.InvVoltageType
    gInvInfo.InvVolt = MOTOR_RATED_VOL;

    // 确定过压和欠压点的大小...
    // LowDc = 200V
    // InvUpUDC = OverDc = 390V
    gInvInfo.InvLowUDC = UNDER_DC_VOL_POINT;//gInvInfo.InvLowUDCCoef;
    gInvInfo.InvUpUDC  = OVER_DC_VOL_POINT; //gInvInfo.InvUpUDCCoef;

#if 1
    // 12V Drive, ONLY 7 8 9 Can be used.
    // * 14480) >> 10 = 113
    // * 960) >> 6    = 120
    gInvInfo.BaseUdc = ((int32)MOTOR_RATED_VOL * 14480) >> 10;
#else
    gInvInfo.BaseUdc = ((int32)MOTOR_RATED_VOL * 960) >> 6;
#endif

    // 设定额定电流，实际产品可以考虑将它设置为固定值，比电机额定电流一致
    gInvInfo.InvCurrent = MOTOR_RATED_CUR;

    // gUDC.Coff = 9500, 代表3.3V ~ 950V and with 0.1 DP
    // 换上2802x 后直接将gUDC.Coff 改成一个固定值即可。
    // 443.3V, then 4433/
    gUDC.Coff = MAX_DC_VOL_3_3V;

    // 没有通过功能传递的必要！！
    gMotorInfo.Current = MOTOR_RATED_CUR;//gMotorInfo.CurrentGet;

    // 计算死区和死区补偿参数, 实际产品设置为固定 2us
    gDeadBand.DeadBand = DeadTimeInternal;
    gDeadBand.Comp     = DeadTimeCompInternal;

    // 初始化完成后，死区不会更改。通用变频器可以通过改变机型改变死区，但此处不需要
    //  EALLOW;
    //  EPwm2Regs.DBFED = gDeadBand.DeadBand;
    //  EPwm2Regs.DBRED = gDeadBand.DeadBand;
    //  EPwm3Regs.DBFED = gDeadBand.DeadBand;
    //  EPwm3Regs.DBRED = gDeadBand.DeadBand;
    //  EPwm4Regs.DBFED = gDeadBand.DeadBand;
    //  EPwm4Regs.DBRED = gDeadBand.DeadBand;
    //  EDIS;

    // 24.75A
    // MAX_PEAK_CUR_3_3V = 414, 代表的是 4.14A Peak. Rated is 2.0A
    // (4.14 / 2.0) * 2^12 = 8448,
    m_ULong = MAX_PEAK_CUR_3_3V_410;
    m_ULong = m_ULong / gMotorInfo.Current;
    gCurSamp.Coff = m_ULong;

    //计算频率表示
    // #ifdef  FREQUENCY_CONTROL_01HZ
    gBasePar.FullFreq = ((int32)gBasePar.MaxFreq * 10) + 2000;
    gMotorInfo.FreqPer = ((Uint32)gMotorInfo.Frequency <<15) / gBasePar.FullFreq;

    //计算用实际值表示的运行频率
    m_UData = abs(gMainCmd.FreqSetApply);
    gMainCmd.FreqReal = ((Uint32)m_UData * (Uint32)gBasePar.FullFreq) >> 15;

    gMainCmd.Command.all     = dspMainCmd.all;                    // 控制命令
    gMainCmd.FreqSet         = frq2Core;                          // 设定频率基准值
    gMainCmd.VCTorqueLim     = upperTorque;                       // 限流点
    gBasePar.FcSet           = PWM_FREQUENCY;                     // 载频频率
    gMotorInfo.Votage        = MOTOR_RATED_VOL;                   // 额定电压
    gMotorInfo.Frequency     = MOTOR_RATED_FRE;                   // 额定频率
    gBasePar.MaxFreq         = MAX_LIMIT_FREQ;                    // 最大频率
    gBasePar.UpFreq          = MAX_LIMIT_FREQ;                    // 上限频率（影响加减速时间，默认和最大频率一样）
    IdMaxSet                 = IdMaxScaling;                      // 弱磁深度系数,Q10,1024 为不限制弱磁电流
    IqFWLimit                = IqMaxSet;                          // 弱磁保留系数（默认400)
    gADC.DelaySet            = DSP_CLOCK;                         // 时钟频率，100MHZ
    DCBrakeCur               = DC_BRAKE_CUR;                      // 直流制动大小, Q12

    SpeedFeedbackFilter      = SPEED_FEEDBACK_FILTER;             // 速度环滤波系数
    MinCurrentLowSpeed       = MIN_CUR_LOW_SPEED;                 // 低速励磁电流值
    SpecailFactor            = SPECIAL_FACTOR_1;                  // 特殊系数, 禁止发电。
    SpeedMeasureFactor1      = SPEED_MEASURE_PAR_1;               // 观测器修正系数1
    SpeedMeasureFactor2      = SPEED_MEASURE_PAR_2;               // 观测器修正系数2
    StartPresetCurrent       = START_PRESET_CUR;                  // 启动电流
    FieldWeakMode            = FILED_WEEKING_MODE;                // 弱磁模式
    FieldWeakCurScale        = FILED_WEEKING_CUR_SCAL;            // 弱磁电流系数,80
    FieldWeakScale           = FILED_WEEKING_SCAL;                // 弱磁调节系数
    FieldWeakRef             = FIELD_WEAK_REF;                    // 弱磁基准
    IPDAutoDectScaling       = IPD_PERIOD_SCALING;                // IPD 电流系数
    AVREnableMode            = AVR_MODE;                          // AVR. 0,1: Enable, 2:Disabled
    MotorTestData1           = T_DELAY;                           // #define  T_DELAY      150L;
    cof_uwMaxThetaPerCntTcPu = ThetaCoff;                         // 相位补偿值。
    NoNegativeOMG            = 0;                                 // 禁止反转，1：禁止反转。
    MotorTestData4           = T_SAMPLE;                          // #define  T_SAMPLE      150L;

    //#ifdef KE_WIDE_RANGE
    MotorTestData5           = 1;                                 // KE 放大10倍
}

/************************************************************
 计算载波频率：输入gBasePar.FcSet，输出gBasePar.FcSetApply
 计算励磁电流：输入pm_ref_speed,   输出pm_min_curr
 2ms Level...
************************************************************/
static void CalCarrierWaveFreq(void)
{
    int32 data,data1,freqs1,fcSet;
    fcSet = gBasePar.FcSet;

    if (pm_control_mode != 0)
    {
        // data = pm_est_omg_1;
        // data = pm_ref_speed;
        // 开环阶段，pm_ref_speed 没有值，导致ID 计算不对。
        if ((gMainStatus.RunStep == HALF_OPEN_ACC) || (gMainStatus.RunStep == OPEN_ACC)) {
            data = frq * 161;
        } else {
            data = pm_ref_speed;
        }
        freqs1 = labs(data);
#if (SF_INDEPAND_OF_FREQUENCY == SW_RANDOM_DISABLE)
        fcSet = gBasePar.FcSet;
#else
        // Low switch frenquency based on pm_ref_speed
        // In order to avoid the problem of low-speed carrier frequency being greater than
        // the set carrier frequency, and to avoid situations where the divisor is 0
        if ((SF_LOW_SPEED >= gBasePar.FcSet) || (HighFreqencyOFSF <= LowFreqencyOFSF)) {
            fcSet = gBasePar.FcSet;
        } else {
            if (freqs1 < LowFreqencyOFSF) {
                fcSet = SF_LOW_SPEED;
            } else if (freqs1 >= HighFreqencyOFSF) {
                fcSet = gBasePar.FcSet;
            } else {
                data = freqs1 - LowFreqencyOFSF;
                data = data * ((int32)gBasePar.FcSet - (int32)SF_LOW_SPEED) / (HighFreqencyOFSF - LowFreqencyOFSF);
                fcSet = data + SF_LOW_SPEED;
            }
        }
#endif
        ////////////////////////
        // data = pm_est_omg_1;
        // freqs1 = labs(data);
        ////////////////////////
        if (freqs1 > IdMaxInternal) {
            data1 = 0;
        } else if (freqs1 <= IdMinInternal) {
            data1 = MIN_CUR_LOW_SPEED * 80;
        } else {
            data = MIN_CUR_LOW_SPEED * 80;  // freqs  = 80000(50Hz)
            data1 = data * (IdMaxInternal - freqs1) / (IdMaxInternal - IdMinInternal);
            data1 = __IQsat(data1,data,0);  // Id > 0
        }

        // 数字设定转矩电流值，做限流用...
        data = (int32)TORQUE_CUR_LIMIT * 4096L / 100;
        data = data * data - pm_ref_iq * pm_ref_iq;
        if (data < 0) {
            data = 0;
        }
        data = qsqrt(data);
        if (data1 > data) {
            data1 = data;
        }
        pm_min_curr = data1;
    }

#if (SWITCH_FREQUENCY_EMC_TYPE == RANDOM_SWICTH_FREQUENCY)
    // From AI to Get Random value
    // PWM_FREQUENCY            ----  180
    // SWITCH_FREQUENCY_AMP     ----- 20
    // Range: 16.0KSF ~ 20.0KSF
    //////////////////////////////////////
    fcSet = random_frequency(PWM_FREQUENCY, SWITCH_FREQUENCY_AMP);
    fcSet = __IQsat(fcSet,PWM_FREQUENCY + SWITCH_FREQUENCY_AMP,PWM_FREQUENCY - SWITCH_FREQUENCY_AMP);  // Id > 0
    gFcCal.FcBak = fcSet;
    gBasePar.FcSetApply = gFcCal.FcBak;
#else
    gFcCal.FcBak = fcSet;
    if (gFcCal.FcBak > gBasePar.FcSetApply) {
        gBasePar.FcSetApply++;
    } else if(gFcCal.FcBak < gBasePar.FcSetApply) {
        gBasePar.FcSetApply--;
    }
#endif

    // Seven and Five Modulation selection.
    gPWM.PWMModle = MODLE_SEVEN_PWM;
}

static void RunCaseDeal(void)
{
    switch(gMainStatus.RunStep)
    {
        case STATUS_LOW_POWER:  //上电缓冲状态/欠压状态
            RunCaseLowPower();
            break;

        case STATUS_GET_PAR:    //参数辨识状态
            RunCaseGetPar();
            break;

        case STATUS_SPEED_CHECK://转速跟踪状态
            RunCaseSpeedCheck();
            break;

        case STATUS_RUN:        //运行状态，区分VF/FVC/SVC运行
            RunCaseRun();
            CalUdcLimitIT();
            break;

        case OPEN_ACC:
        case HALF_OPEN_ACC:
            OpenLoopAccRun();
            break;

        case STATUS_BRAKE:
            RunCaseBrakeLowBridge();
            break;

        case STATUS_SHORT_GND:  //上电对地短路判断状态
            RunCaseShortGnd();
            break;

        default:                //停机状态：所有其它状态认为是该状态
            RunCaseStop();
            break;
    }
}

static void RunCaseLowPower(void)
{
    Uint16 uDCLowLimt;
    uDCLowLimt = gInvInfo.InvLowUDC + 10;
    switch (gMainStatus.SubStep)
    {
        case 2:
            if ((gUDC.uDCFilter > uDCLowLimt) && (gUDC.uDCFilter <= gLowPower.UDCOld))
            {
                if ((gLowPower.WaiteTime++) >= 100) {
                    gExcursionInfo.EnableCount = 199;
                    gMainStatus.SubStep ++;
                    gLowPower.WaiteTime = 0;
                }
            } else {
                gLowPower.WaiteTime = 0;
                gLowPower.UDCOld = gUDC.uDCFilter;
            }
        break;
        case 3:
            // 200ms延时
            if (((gLowPower.WaiteTime++) >= 100) && (gExcursionInfo.EnableCount >= 200))
            {
                gMainStatus.RunStep = STATUS_STOP;
                gMainStatus.SubStep = 1;
                gMainStatus.StatusWord.bit.LowUDC = 0;
                gMainStatus.StatusWord.bit.RunEnable = 1;
                if ((gMainStatus.ErrorCode == ERROR_UV) || (gMainStatus.ErrorCode == ERROR_OC_HARDWARE))
                {
                    gMainStatus.ErrorCode = 0;
                    if (gMainStatus.ErrFlag.bit.OvCurFlag == 1)  //修改欠压后无法进入过流中断的错误
                    {
                        gMainStatus.ErrFlag.bit.OvCurFlag = 0;
                        ClearAllTZFlags();
                        EALLOW;
                        EPwm2Regs.TZCLR.bit.INT = 1;
                        EDIS;
                    }

                    // Reset LIN_EN and Pre Drive
                    ResetLINAndPreDrive();
                }
            }
        break;
        default:
            // DisConnectRelay();
            gMainStatus.ErrorCode = ERROR_UV;               //错标志
            gMainStatus.StatusWord.bit.LowUDC = 1;
            gLowPower.WaiteTime = 0;
            gLowPower.UDCOld = gUDC.uDCFilter;
            gMainStatus.SubStep = 2;
        break;
    }
}

/************************************************************
    参数辨识阶段(该程序用于检测U相和V相电流检测通道的增益偏差)
    同时检测定子电阻值。
    2ms 状态机...
************************************************************/
static void RunCaseGetPar(void)
{
}

//long SpeedData[20];
/************************************************************
    转速跟踪状态处理
    2ms, 状态机...
    风机：利用反电动势提高性能
    水泵或其他：直接电流闭环..
************************************************************/
static void RunCaseSpeedCheck(void)
{
    int32 data;
    if ((gMainCmd.Command.bit.Start == FALSE) ||        //停机命令
        (gMainStatus.ErrorCode      != 0    ))          //故障
    {
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        DisableDrive();
        return;
    }
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    PaseShiftEnable = 1;
#endif
    // 状态即，200ms, 然后进入到RUN 状态, gMainCmd.FreqSetApply 接下来从功能
    // 传递过来。。。。
    ////////////////////////////////////////////////////////////////////////
    if (pm_control_mode != 0)
    {
        gFeisu.VoltCNT++;
#ifdef SPIN_CONTROL_WITH_BEMF
        if (ReadSpeedBasedonFinished == 1)
#else
        if (gFeisu.VoltCNT > 100)
#endif
        {
            gFeisu.VoltCNT = 0;
#ifdef SPIN_CONTROL_WITH_BEMF
            pm_est_lpf_omg1 = (int32)gBemf.Spd_Hz * 1602L;
#endif
            data = (int64)pm_est_lpf_omg1 * 2045L / gBasePar.FullFreq;
//SpeedData[1]=gBemf.Spd_Hz;
//SpeedData[2]=pm_est_lpf_omg1;
//SpeedData[3]++;
            if ((pm_est_lpf_omg1 <= 16020) && (pm_est_lpf_omg1 >= (-16020)))
            {
//SpeedData[4]++;
#ifdef IPD_PERIOD_AUTO_DECT
                pm_check_ip_flag = 1;
                data = 0;
#else
                pm_check_ip_flag = 0;
                DCStart_flag = 1;
#endif
                gIPMInitPos.SubStep = 1;
            }
            gMainCmd.FreqSetApply  = data;
//SpeedData[5]=data;
            gMainCmd.FreqPreWs     = data;
            gMainCmd.FreqSet       = data;
            gMainCmd.FreqFeed      = data;
            gMainCmd.FreqDesired   = data;

            gMainStatus.SubStep = 1;
            if (DCStart_flag == 1) {
#ifdef OPEN_LOOP_START
#ifdef FULL_OPEN_LOOP_MODE
                gMainStatus.RunStep = OPEN_ACC;
#else
                gMainStatus.RunStep = HALF_OPEN_ACC;
#endif
#else
                gMainStatus.RunStep = STATUS_RUN;
#endif
                gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
//SpeedData[6]++;
            } else {
#ifndef IPD_PERIOD_AUTO_DECT
                if ((pm_est_lpf_omg1 > -SpinControlInternal) && (pm_est_lpf_omg1 < SpinControlInternal)) {
                    gMainStatus.RunStep = STATUS_BRAKE;
                } else {
                    if (((frqAimTmp0 > 0) && (pm_est_lpf_omg1 < -SpinControlInternal)) ||    //  56hz
                        ((frqAimTmp0 < 0) && (pm_est_lpf_omg1 >  SpinControlInternal))) {
                        gMainStatus.RunStep = STATUS_BRAKE;
                    } else {
                        pm_integral_q = gBemf.OutVolt;
                        pm_integral_speed = ((int32)OPEN_LOOP_TOR_START << 8);
                        if(gBemf.DIR == 1)
                        {
                            pm_integral_speed = -pm_integral_speed;
                        }
                        gMainStatus.RunStep = STATUS_RUN;//STATUS_PRE_RUN;//STATUS_RUN;
                        gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
                    }
                }
#else
                gMainStatus.RunStep = STATUS_RUN;//STATUS_PRE_RUN;//STATUS_RUN;
                gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
#endif
            }

            // EnableDrive();
            if (gMainStatus.RunStep != STATUS_BRAKE) {
                EnableDrive();
            }
        }
        return;
    }
    else
    {
        gMainCmd.FreqSetApply  = 0;
        gMainCmd.FreqPreWs     = 0;
        gMainCmd.FreqSet       = 0;
        gMainCmd.FreqFeed      = 0;
        gMainCmd.FreqDesired   = 0;
        gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
        gMainStatus.SubStep = 1;
        gMainStatus.RunStep = STATUS_RUN;

        // VF MODE, SPIN_CONTROL_WITH_BEMF, 前面已经封锁导致不能发波。
        EnableDrive();
    }
}

//long AccRunData[10];
//int RecordOpen;
static void OpenLoopAccRun(void)
{
    if ((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
//AccRunData[0]++;
        DisableDrive();
        IDForAlignAndOpenLoop = 0;
        McSpdStateCnt = 0;
        McSpdState = McInit;
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        return;
    }

    switch (McSpdState) {
    case McInit:
//AccRunData[1]++;
        if (pm_control_mode == 1) {
            McSpdState = McAlignRun;
            OL.OL_Running = 1;
        } else {
            gMainStatus.RunStep = STATUS_RUN;
        }
    break;
    case McAlignRun:
//AccRunData[2]++;
         if (gMainCmd.Command.bit.StartDC == 1)
         {
//AccRunData[3]++;
             gMainCmd.FreqPreWs    = 0;
             gMainCmd.FreqSetApply = 0;
             if (IDForAlignAndOpenLoop > DCBrakeCur) {
                 IDForAlignAndOpenLoop = DCBrakeCur;
             } else {
                 IDForAlignAndOpenLoop += 30;
             }
//GpioDataRegs.GPATOGGLE.bit.GPIO16  = 1; // 偶尔在刹车后直流制动期间确实没有进来这个地方。
             // 低速刹车后进入直流制动，会没有影响导致直流制动期间误报 输出缺项
             PaseShiftEnable = 1;
         } else {
//AccRunData[4]++;
             McSpdStateCnt++;
             if (McSpdStateCnt > 2) {
                 McSpdStateCnt = 0;
                 McSpdState = McOpenRun;
             }
         }
    break;
    case McRsIDRun:
//AccRunData[5]++;
    break;
    case McOpenRun:
#ifdef FULL_OPEN_LOOP_MODE
#if (SINGLE_SHUNT_PWM_MODE == NORMAL_NOISE_PWM_MODE)
        PaseShiftEnable = 0;
#else
        PaseShiftEnable = 1;
#endif
#else
        PaseShiftEnable = 1;
#endif
//AccRunData[6]++;
        if (gMainCmd.FreqReal > FRQ_OPEN_TO_CLOSE) { // 20HZ
#ifndef FULL_OPEN_LOOP_MODE
            ///////////////////////////////////////////////
            frqLine.curValue = gMainCmd.FreqReal / 10;
            frqTmp = frqLine.curValue;
            if (gMainCmd.FreqSetApply < 0) {
                frqLine.curValue = -frqLine.curValue;
                frqTmp = -frqTmp;
            }
            OL.OlToClose = 1;
            ///////////////////////////////////////////////
#else
            OL.OL_Running = 0;
#endif
            McSpdState = McSpdChangeToClosed;
        } else {
            IDForAlignAndOpenLoop = DCBrakeCur;
        }
    break;
    case McSpdChangeToClosed: {
#ifdef FULL_OPEN_LOOP_MODE
        pmsvc_init();
        PMFluxWeakInit();
//AccRunData[8]++;
        //pm_coef_alfa = 170666;
        if (gMainCmd.FreqSetApply < 0) {
            pm_est_angel = (int32)(gPhase.OutPhase + 16384) * 3217L >> 8;
        } else {
            pm_est_angel = (int32)(gPhase.OutPhase - 16384) * 3217L >> 8;
        }
        pm_integral_d = 0;
        pm_integral_q = (int32)300 << 12;
        if (gMainCmd.FreqSetApply < 0) {
            pm_integral_q = -pm_integral_q;
        }
        gMainStatus.RunStep = STATUS_RUN;
        McSpdState = McInit;
        PaseShiftEnable = 1;

        // Frq = 10.0HZ, 160200 / 16 = 10200
        // #ifdef  FREQUENCY_CONTROL_01HZ
        FrqTemp = (int32)gMainCmd.FreqReal / 10 + 200;

        if (gMainCmd.FreqSetApply < 0) {
            FrqTemp = -FrqTemp;
        }
        frqLine.curValue = FrqTemp;
        frqTmp = FrqTemp;
#else
        if(OL.OL_Running == 0)
        {
            gMainStatus.RunStep = STATUS_RUN;
            McSpdState = McInit;
        }
#endif
    }
    break;
    case McSFinished:
    break;
    }

    if(OL.OL_Running == 1)
    {
        pm_ref_iq = IDForAlignAndOpenLoop;
        pm_ref_id = 0;//pm_min_curr;
    }
#if 0
static int16 ReCord;
if (gMainCmd.FreqReal > (FRQ_OPEN_TO_CLOSE - 30)) {
    ReCord = 1;
}
if (ReCord == 1) {
ControlAllowRecord(1,1);
IsRecordThisSample(
        frqLine.curValue,//McSpdState,
        frq,//gMainStatus.RunStep,   // IV
        gMainCmd.FreqSetApply,
        pm_est_omg >> 4,//gCurSamp.V,
        frq2Core,//gPhase.OutPhase,
        frqTmp,//pm_ref_iq,
        pm_ref_id,
        gCurSamp.V//gIAmpTheta.Theta//gPWM_CM
        );
}
#endif
}

/************************************************************
    正常运行过程处理
    运行状态机, 2ms...
************************************************************/
static void RunCaseRun(void)
{
#if (SINGLE_SHUNT_PWM_MODE == NORMAL_NOISE_PWM_MODE)
    static Uint16 PhaseShiftHysteresis;
#endif
    if ((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
        DisableDrive();

#ifndef OPEN_LOOP_START
        IDForDCBrakeInCurrentLoopMode = 0;
#endif
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        return;
    }

    // 检查宏定义后面有PaseShiftEnable = 1; 语法上是重复的，那么什么情况下需要PaseShiftEnable = 0
    // 之前是做纯开环+闭环。。。纯开环 = 0，闭环后一段时间为0目的不要噪音，但是可能波形会比较差，所以这个模式基本不用了
    // 改用低噪音模式，全程可以开启！！！
    // 所以理论上可以直接PaseShiftEnable = 1
#if (SINGLE_SHUNT_PWM_MODE == NORMAL_NOISE_PWM_MODE)
    if (gMainCmd.FreqSetApply < 0) {
        PhaseShiftHysteresis = 3000;
    } else {
        PhaseShiftHysteresis = 400;
    }
    if (gMainCmd.FreqReal > FRQ_PWM_SHIFT_EN) {                 // 100HZ
        PaseShiftEnable = 1;
    } else if (gMainCmd.FreqReal > FRQ_OPEN_TO_CLOSE + PhaseShiftHysteresis) {
        PaseShiftEnable = 0;
    }
#else
    PaseShiftEnable = 1;
#endif
    PaseShiftEnable = 1;
    if (pm_control_mode != 0)
    {
        if ((gMainCmd.Command.bit.StartDC == 1) ||
            (gMainCmd.Command.bit.StopDC  == 1))
        {
#ifndef OPEN_LOOP_START
           // 3100, 2ms, 50*2 = 100ms
           // 25 * 100ms = 2500
           if (IDForDCBrakeInCurrentLoopMode > DCBrakeCur) {
               IDForDCBrakeInCurrentLoopMode = DCBrakeCur;
           } else {
               IDForDCBrakeInCurrentLoopMode += 30;
           }
           pm_ref_id = IDForDCBrakeInCurrentLoopMode;
           pm_ref_iq = 0;
#endif

           gMainCmd.FreqPreWs    = 0;
           gMainCmd.FreqSetApply = 0;
           return;
        }

        // Enable drive 200MS 内，没有RUNCASE Iq = 0. 不启动...
        if ((pm_check_ip_flag == 0) && (DCStart_flag == 0)) {
            SpeedLoop();
        }
        return;
     }

    // VF mode only
    if (gMainCmd.Command.bit.StartDC == 0)
    {
        VFFreqCal();
        CalOutVotInVFStatus();
        gOutVolt.VoltApply = gOutVolt.Volt;

        if (gMainCmd.FreqSetApply != 0) {
            gMainCmd.FreqLast = gMainCmd.FreqSetApply;
        }
        if(gMainCmd.FreqLast > 0)       gOutVolt.VoltPhaseApply = 8192;
        else if(gMainCmd.FreqLast < 0)  gOutVolt.VoltPhaseApply = -8192;
    }
}

//long RunCaseShortGndData[8];
/************************************************************
    上电对地短路判断
    Alpha 底座采样的是V W，
    HC    底座采样的是U V，所以之前用U 相做对地保护。这里改成V相。
    即：V+相发波，检测V相电流大小
************************************************************/
static void RunCaseShortGnd(void)
{
    switch(gMainStatus.SubStep)
    {
        case 1:
            gShortGnd.Comper = SHORT_GND_PERIOD;
            gShortGnd.Flag = 0;
            gShortGnd.BaseUDC = gUDC.uDC;
            gShortGnd.ShortCur = 0;

            EALLOW;
            EPwm2Regs.TBPRD = SHORT_GND_PERIOD;
            EPwm2Regs.CMPA.half.CMPA = SHORT_GND_PERIOD - 1;
            EPwm2Regs.CMPB           = SHORT_GND_PERIOD - 1;
            EPwm3Regs.TBPRD = SHORT_GND_PERIOD;
            EPwm4Regs.TBPRD = SHORT_GND_PERIOD;

            //强制关闭某些桥臂
            EPwm2Regs.AQCSFRC.all = 0x00;//4;
            EPwm3Regs.AQCSFRC.all = 0x05;
            EPwm4Regs.AQCSFRC.all = 0x05;

            EPwm2Regs.DBCTL.all = 0x0B;//0;
            EPwm3Regs.DBCTL.all = 0;
            EPwm4Regs.DBCTL.all = 0;
            EDIS;

            gMainStatus.SubStep = 2;
            break;

        case 2:
            gMainStatus.SubStep = 3;
            EnableDrive();
            break;
        case 3:
            if ((gShortGnd.Flag          != 0       ) ||
                (abs(gShortGnd.ShortCur) > 1000     )) //(30 * 32)) ||// 410*2 为峰值电流，与变频器额定电流10%比较
            {
//RunCaseShortGndData[0]=gShortGnd.Flag;
//RunCaseShortGndData[1]++;
//RunCaseShortGndData[2]=gShortGnd.ShortCur;
//RunCaseShortGndData[3]=gShortGnd.BaseUDC;
//RunCaseShortGndData[4]=gUDC.uDC;
                // 上电对地短路处理
                DisableDrive();
                gMainStatus.ErrorCode = ERROR_MOTOR_SHORT_TO_GND;
                gMainStatus.SubStep = 4;
                break;
            }
//RunCaseShortGndData[6]=gShortGnd.ShortCur;
            if (gShortGnd.Comper <= (SHORT_GND_PERIOD - (SHORT_GND_PERIOD / 5)))
            {
                gMainStatus.SubStep = 4;
//RunCaseShortGndData[5]++;
            }
            else
            {
                gShortGnd.Comper -= SHORT_GND_CMPR_INC;
                EALLOW;
                EPwm2Regs.CMPA.half.CMPA = gShortGnd.Comper;
                EPwm2Regs.CMPB           = gShortGnd.Comper;
                EDIS;
            }
            break;

        case 4:
            DisableDrive();

            EALLOW;
            EPwm2Regs.DBCTL.all = 0x000B;
            EPwm3Regs.DBCTL.all = 0x000B;
            EPwm4Regs.DBCTL.all = 0x000B;

            EPwm2Regs.AQCSFRC.all = 0x0;
            EPwm3Regs.AQCSFRC.all = 0x0;
            EPwm4Regs.AQCSFRC.all = 0x0;
            EDIS;

            gMainStatus.StatusWord.bit.ShortGndOver = 1;
            gMainStatus.RunStep = STATUS_STOP;
            gMainStatus.SubStep = 0;
            if (gMainStatus.ErrFlag.bit.OvCurFlag == 1)
            {
                gMainStatus.ErrFlag.bit.OvCurFlag = 0;
                EALLOW;
                EPwm2Regs.TZCLR.bit.INT = 1;
                EDIS;
            }
            break;

        default:
            break;
    }
}

//long CatchSpinData[25];
static void RunCaseBrakeLowBridge(void)
{
//CatchSpinData[11]++;
//CatchSpinData[17]=BrakeLowBridgeState;
    switch (BrakeLowBridgeState)
    {
    case BRAKE_LOW_BRIGE_INIT:
//CatchSpinData[12]++;
//GpioDataRegs.GPATOGGLE.bit.GPIO16  = 1;
        BrakeLowBridgeStateCnt = 0;
        BrakeLowBridgeState = BRAKE_LOW_BRIGE_LOWON;
    break;
    case BRAKE_LOW_BRIGE_LOWON:
        // GpioRegs.MCRA.all &= 0xF03F;
//CatchSpinData[13]++;
//GpioDataRegs.GPATOGGLE.bit.GPIO16  = 1;
        EnableDrive();

        EALLOW;
        // Turn on Low bridge
#ifndef POSTIVE_DRIVE_LOGIC
        EPwm2Regs.AQCSFRC.all = 0x06;
        EPwm3Regs.AQCSFRC.all = 0x06;
        EPwm4Regs.AQCSFRC.all = 0x06;
#else
        // 1001 --
        // 10   -- Forces a continuous high on output B
        // 01   -- Forces a continuous low on output A
        EPwm2Regs.AQCSFRC.all = 0x09;
        EPwm3Regs.AQCSFRC.all = 0x09;
        EPwm4Regs.AQCSFRC.all = 0x09;
#endif
        // Disable Dead time ... ...
        EPwm2Regs.DBCTL.all     = 0;
        EPwm3Regs.DBCTL.all     = 0;
        EPwm4Regs.DBCTL.all     = 0;
        EDIS;

        BrakeLowBridgeState = BRAKE_LOW_BRIGE_CHECK;
    break;
    case BRAKE_LOW_BRIGE_CHECK:
        // 判断电流大小，临时调试固定延时方式 2ms, 1000 ~ 2s
        BrakeLowBridgeStateCnt++;
        if (BrakeLowBridgeStateCnt > 1500) {
            BrakeLowBridgeStateCnt = 0;
            BrakeLowBridgeState = BRAKE_LOW_BRIGE_FINISH;
//CatchSpinData[14]++;
            DisableDrive();
        }
    break;
    case BRAKE_LOW_BRIGE_FINISH:
//CatchSpinData[15]++;
        EnableDrive();

        EALLOW;
        EPwm2Regs.AQCSFRC.all = 0x00;
        EPwm3Regs.AQCSFRC.all = 0x00;
        EPwm4Regs.AQCSFRC.all = 0x00;

#ifndef POSTIVE_DRIVE_LOGIC
        EPwm2Regs.DBCTL.all = 0x0007;
        EPwm3Regs.DBCTL.all = 0x0007;
        EPwm4Regs.DBCTL.all = 0x0007;
#else
        EPwm2Regs.DBCTL.all = 0x000B;
        EPwm3Regs.DBCTL.all = 0x000B;
        EPwm4Regs.DBCTL.all = 0x000B;
#endif
        EDIS;

        pmsvc_init();
        PMFluxWeakInit();
        PrepareParForRun();
        DCStart_flag = 1;
#ifdef OPEN_LOOP_START
#ifdef FULL_OPEN_LOOP_MODE
        gMainStatus.RunStep = OPEN_ACC;
#else
        gMainStatus.RunStep = HALF_OPEN_ACC;
#endif
#else
        gMainStatus.RunStep = STATUS_RUN;
#endif
        BrakeLowBridgeState = BRAKE_LOW_BRIGE_INIT;
        gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
    break;
    default:
//CatchSpinData[16]++;
    break;
    };
}

/************************************************************
    停机过程处理
************************************************************/
//int VolOffset;
//int Adc_Temp_V1;
//int Adc_Temp_V2;
//int Adc_Temp_V3;
static void RunCaseStop(void)
{
    // 针对水泵，转速追踪不需要，为了跟风扇程序统一，判断条件保持一致。对于水泵，
    // 这三个AD口的值为155.....
    ////////////////////////////////////////////////////////////////////
    //    Adc_Temp_V1 = AdcResult.ADCRESULT4;
    //    Adc_Temp_V2 = AdcResult.ADCRESULT5;
    //    Adc_Temp_V3 = AdcResult.ADCRESULT6;
    //    VolOffset = (Adc_Temp_V1 + Adc_Temp_V2 + Adc_Temp_V3);
    ////////////////////////////////////////////////////////////
    DisableDrive();                         //停机封锁输出

    PrepareParForRun();
#ifdef OPEN_LOOP_START
    OL_ParamReset();
#endif
    pmsvc_init();
    PMFluxWeakInit();
    gMainStatus.SubStep = 1;

    McSpdState = McInit;                      // MCOPEN 时候欠压，则没没办法重新开始DC--OPENRUN
    BrakeLowBridgeState = BRAKE_LOW_BRIGE_INIT; // BRAKE  时候欠压，重置状态机，否则逻辑会乱
    BrakeLowBridgeStateCnt = 0;

    if(gMainStatus.StatusWord.bit.RunEnable != 3)
    {
        return;                             //等待零漂检测完成
    }

#ifdef SHORT_GND_TEST
    if (VolOffset < 80) {
        if (gMainStatus.StatusWord.bit.ShortGndOver == 0)
        {
            gMainStatus.RunStep = STATUS_SHORT_GND;
            gMainStatus.SubStep = 1;
            return;
        }
    }
#else
    gMainStatus.StatusWord.bit.ShortGndOver = 1;
#endif

    /////////////////////////////////////////
    //  if (gMainCmd.Command.bit.TunePart == 1) //判断是否需要参数辨识
    //  {
    //      gMainStatus.RunStep = STATUS_GET_PAR;
    //      gMainStatus.SubStep = 1;
    //      return;
    //  }
    //////////////////////////////////////////////////////////////////
    if ((gMainStatus.ErrorCode == ERROR_NONE) && (gMainCmd.Command.bit.Start == TRUE))
    {
        if (gMainCmd.Command.bit.SpeedSearch == TRUE)
        {
            // 272, / 16 = 17
            // 水泵一般没有反电动势采样，风机的需要确认该值！！！
#ifdef SPIN_CONTROL_WITH_BEMF
            if (VolOffset < BemfOffset) {
#else
            if (0) {
#endif
#ifdef OPEN_LOOP_START
#ifdef FULL_OPEN_LOOP_MODE
                gMainStatus.RunStep = OPEN_ACC;
#else
                gMainStatus.RunStep = HALF_OPEN_ACC;
#endif
#else
                gMainStatus.RunStep = STATUS_RUN;
#endif
#ifdef IPD_PERIOD_AUTO_DECT
                pm_check_ip_flag = 1;
                DCStart_flag = 0;
#else
                pm_check_ip_flag = 0;
                DCStart_flag = 1;
#endif
                gMainCmd.FreqSetApply  = 0;
                gMainCmd.FreqPreWs     = 0;
                gMainCmd.FreqSet       = 0;
                gMainCmd.FreqFeed      = 0;
                gMainCmd.FreqDesired   = 0;
                gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
                EnableDrive();
            } else {
#ifdef SPIN_CONTROL_WITH_BEMF
                DisableDrive();
#else
                EnableDrive();
#endif
                gMainStatus.RunStep = STATUS_SPEED_CHECK;
            }
            gMainStatus.SubStep = 1;
        }
        else
        {
#ifdef OPEN_LOOP_START
#ifdef FULL_OPEN_LOOP_MODE
            gMainStatus.RunStep = OPEN_ACC;
#else
            gMainStatus.RunStep = HALF_OPEN_ACC;
#endif
#else
            gMainStatus.RunStep = STATUS_RUN;
#endif
            gMainStatus.SubStep = 1;
            EnableDrive();
        }
    }
}

/************************************************************
    启动电机运行前的数据初始化处理，为电机运行准备初始参数
    停机过程变量清零处理...
************************************************************/
static void PrepareParForRun(void)
{
    gMainCmd.FreqSetApply = 0;
    gMainCmd.FreqPreWs = 0;
    gMainCmd.FreqLast = 0;
    gMainCmd.FreqReal = 0;
    gOutVolt.Volt = 0;
    gOutVolt.VoltApply = 0;

#ifndef OPEN_LOOP_START
    IDForDCBrakeInCurrentLoopMode = 0;
#else
    IDForAlignAndOpenLoop= 0;
#endif

    if(gMainCmd.FreqDesired > 0)        //根据目标频率方向确定初始相位角
    {
        gOutVolt.VoltPhaseApply = 8192;
    } else {
        gOutVolt.VoltPhaseApply = -8192;
    }
    gPhase.OutPhase = (int16)(gPhase.IMPhase>>16) + gOutVolt.VoltPhaseApply;

    gRatio = 0;
    pm_ref_speed = 0;   // 开环切闭环，让ID 变化和第一次启动一致。

    gCurSamp.U = 0;
    gCurSamp.V = 0;
    gCurSamp.W = 0;

    gIUVW.U = 0;
    gIUVW.V = 0;
    gIUVW.W = 0;

    gLineCur.CurPer = 0;
    gLineCur.CurPerFilter = 0;

    gIMT.M = 0;
    gIMT.T = 0;

    gDCBrake.Time = 0;
    gDCBrake.PID.Total = 0;

    gMainStatus.StatusWord.all &= 0xFE6A;
    gIAmpTheta.ThetaFilter = gIAmpTheta.Theta;

    EALLOW;
#if (SINGLE_SHUNT_PWM_MODE == LOW_NOISE_PWM_MODE_2)
    LowNoseElmPWMMode = 1;
    LastLowNoseElmPWMMode = 1;
    EPwm2Regs.TBPRD = 2 * gPWM.gPWMPrdApply;
    EPwm3Regs.TBPRD = 2 * gPWM.gPWMPrdApply;
    EPwm4Regs.TBPRD = 2 * gPWM.gPWMPrdApply;
    EPwm5Regs.TBPRD = 2 * gPWM.gPWMPrdApply;
#else
    EPwm2Regs.TBPRD = gPWM.gPWMPrdApply;
    EPwm3Regs.TBPRD = gPWM.gPWMPrdApply;
    EPwm4Regs.TBPRD = gPWM.gPWMPrdApply;
    EPwm5Regs.TBPRD = gPWM.gPWMPrdApply;
#endif

    EPwm2Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
    EPwm3Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
    EPwm4Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
    EPwm5Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
    EPwm5Regs.CMPB = gPWM.gPWMPrdApply / 2;

    gPWM_CMP.U_CMP_U = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.U_CMP_V = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.U_CMP_W = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.D_CMP_U = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.D_CMP_V = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.D_CMP_W = EPwm2Regs.CMPA.half.CMPA;

    gPWM_CMP.T1CMP = 100;
    gPWM_CMP.T2CMP = 500;
    EPwm5Regs.CMPA.half.CMPA = gPWM_CMP.T1CMP;
    EPwm5Regs.CMPB = gPWM_CMP.T2CMP;
    EDIS;

    CURR_SAMPLE_DELAY   = MotorTestData4 - MotorTestData1;  // (T_SAMPLE - T_DELAY)
    MIN_SAMPLE_TIME     = MotorTestData4 + MotorTestData1;  // (T_SAMPLE + T_DELAY)
    TWO_MIN_SAMPLE_TIME = MIN_SAMPLE_TIME * 2;              // MIN_SAMPLE_TIME * 2;
}

/************************************************************
    计算零漂程序
    4066，所以不能一直读取, main2ms call
************************************************************/
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
static void GetCurrentExcursion(void)
{
    int16 m_ErrIv,m_ErrUu,m_ErrIbus;
    if((gMainStatus.RunStep    != STATUS_LOW_POWER) &&
       (gMainStatus.RunStep    != STATUS_STOP     ))
    {
        gExcursionInfo.EnableCount = 0;
        return;
    }

    gExcursionInfo.EnableCount++;
    gExcursionInfo.EnableCount = (gExcursionInfo.EnableCount>200) ? 200 : gExcursionInfo.EnableCount;
    if ((gExcursionInfo.EnableCount < 200))
    {
        gExcursionInfo.TotalUu = 0;
        gExcursionInfo.TotalIv = 0;
        gExcursionInfo.TotalIbus= 0;
        gExcursionInfo.Count = 0;
        return;
    }

    // 4066: one time
    // speed check: bit = 3
    //  if ((IsCurExcursionFinshied == 1) && (IsRelayCheckFinshed == 1)) {
    //      gMainStatus.StatusWord.bit.RunEnable = 3;
    //      return;
    //  }
    //////////////////////////////////////////////////////////////////////
    gExcursionInfo.TotalIv += (int16)ADC_IV;
    gExcursionInfo.TotalUu += (int16)ADCRESULT_UU;
    gExcursionInfo.TotalIbus += (int16)ADC_IBUS;
    gExcursionInfo.Count++;

    if (gExcursionInfo.Count >= 32)
    {
        m_ErrIv = gExcursionInfo.TotalIv >> 5;
        if (-32768 == m_ErrIv) {
            m_ErrIv = -32767;
        }
        m_ErrUu = gExcursionInfo.TotalUu >> 5;
        if (-32768 == m_ErrUu) {
            m_ErrUu = -32767;
        }
        m_ErrIbus = gExcursionInfo.TotalIbus >>5;
        if (-32768 == m_ErrIbus) {
            m_ErrIbus = -32767;
        }

        gExcursionInfo.TotalIv = 0;
        gExcursionInfo.TotalUu = 0;
        gExcursionInfo.TotalIbus = 0;
        gExcursionInfo.Count = 0;

        gMainStatus.StatusWord.bit.RunEnable = 3;
        if (abs(m_ErrIv - 2048) < 683)   // 电压判断, 683 ~ 0.5V, 1V ~ 2V之间。
        {
            gExcursionInfo.ErrUu = m_ErrUu;
            gExcursionInfo.ErrIv = m_ErrIv;
            gExcursionInfo.ErrIbus = m_ErrIbus;
            gExcursionInfo.ErrCnt = 0;
        } else {
            gMainStatus.ErrorCode = ERROR_CURRENT_SAMPLE;
            gExcursionInfo.ErrCnt = 0;
            gExcursionInfo.EnableCount = 0;
        }
    }
}
#else
static void GetCurrentExcursion(void)
{
    int16 m_ErrIu,m_ErrIv,m_ErrIw;
    if((gMainStatus.RunStep    != STATUS_LOW_POWER) &&
       (gMainStatus.RunStep    != STATUS_STOP     ))
    {
        gExcursionInfo.EnableCount = 0;
        return;
    }

    gExcursionInfo.EnableCount++;
    gExcursionInfo.EnableCount = (gExcursionInfo.EnableCount>200) ? 200 : gExcursionInfo.EnableCount;
    if ((gExcursionInfo.EnableCount < 200))
    {
        gExcursionInfo.TotalIu = 0;
        gExcursionInfo.TotalIv = 0;
        gExcursionInfo.TotalIw = 0;
        gExcursionInfo.Count = 0;
        return;
    }

    // 4066: one time
    // speed check: bit = 3
    //  if ((IsCurExcursionFinshied == 1) && (IsRelayCheckFinshed == 1)) {
    //      gMainStatus.StatusWord.bit.RunEnable = 3;
    //      return;
    //  }
    /////////////////////////////////////////////////////////////////////

    gExcursionInfo.TotalIu += (int16)(ADC_IU - (Uint16)32768);
    gExcursionInfo.TotalIv += (int16)(ADC_IV - (Uint16)32768);
    gExcursionInfo.TotalIw += (int16)(ADC_IW - (Uint16)32768);
    gExcursionInfo.Count++;

    if (gExcursionInfo.Count >= 32)
    {
        m_ErrIu = gExcursionInfo.TotalIu >> 5;
        m_ErrIv = gExcursionInfo.TotalIv >> 5;
        m_ErrIw = gExcursionInfo.TotalIw >> 5;
        if (-32768 == m_ErrIu) {
            m_ErrIu = -32767;
        }
        if (-32768 == m_ErrIv) {
            m_ErrIv = -32767;
        }
        if (-32768 == m_ErrIw) {
            m_ErrIw = -32767;
        }
        gExcursionInfo.TotalIu = 0;
        gExcursionInfo.TotalIv = 0;
        gExcursionInfo.TotalIw = 0;
        gExcursionInfo.Count = 0;

        gMainStatus.StatusWord.bit.RunEnable = 3;
        //if ((abs(m_ErrIu) < 5120) && (abs(m_ErrIv) < 5120) && (abs(m_ErrIw) < 5120))
        if ((abs(m_ErrIu) < 5120) && (abs(m_ErrIv) < 5120))// && (abs(m_ErrIw) < 5120))
        {
            gExcursionInfo.ErrIu = m_ErrIu;
            gExcursionInfo.ErrIv = m_ErrIv;
            gExcursionInfo.ErrIw = m_ErrIw;
            gExcursionInfo.ErrCnt = 0;
        } else {
            gMainStatus.ErrorCode = ERROR_CURRENT_SAMPLE;
            gExcursionInfo.ErrCnt = 0;
            gExcursionInfo.EnableCount = 0;
        }
    }
}
#endif

/*************************************************************
**************************************************************
周期中断：完成模拟量采样、电流计算、VC电流环控制等操作
**************************************************************
*************************************************************/
void ADCOverInterrupt(void)
{
    if (pm_control_mode  == 0)
    {
        ADCProcess();
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
        SVPWM_1ShuntGetPhaseCurrent();
#else
        SVPWM_3ShuntGetPhaseCurrent();
#endif
        GetUDCInfo();
        GetAIInfo();
        ChangeCurrent();
        CalRatioFromVot();
        CalDeadBandComp();
        CalOutputPhase();
        OutPutPWMVF();
        OutPutPWMPhaseShiftForLowNoiseFullMode();
        UpdatPWMRegs();
    } else {
        MotorControlISR();
    }
}

int32 OCData[6];
/*************************************************************
    过流中断处理程序（可屏蔽中断，电平触发）
*************************************************************/
void HardWareErrorDeal(void)
{
    DisableDrive();

    gMainStatus.ErrFlag.bit.OvCurFlag = 1;
    if (gMainStatus.RunStep == STATUS_SHORT_GND) {
        gShortGnd.Flag = 1;
    } else if(gMainStatus.ErrorCode != ERROR_OC_HARDWARE) {
        gMainStatus.ErrorCode = ERROR_OC_HARDWARE;
    }
//OCData[0]=gCurSamp.U;
//OCData[1]=gCurSamp.V;
//OCData[2]=gCurSamp.W;
//OCData[3]=gRatio;
OCData[4]++;
    ClearAllTZFlags();
}

/*************************************************************
    矢量控制的过压抑制功能，通过母线电压限制输出转矩(发电转矩)的最大值
*************************************************************/
static void CalUdcLimitIT(void)
{
    int16 IQRegenLimit;
    int16 IQTemp;

    // 求一条抛物线的函数关系式，该抛物线经过（160，-500）和（170,0）两点，开口向下，并画出该抛物线
    // y = -5*(x-170)^2
    if (gUDC.uDC < OVER_DC_VOL_LIMIT) {
        IQTemp = -500;
    } else if (gUDC.uDC > MaxAllowUdcLimit) {
        IQTemp = 0;
    } else {
        IQTemp = (gUDC.uDC - MaxAllowUdcLimit) * (gUDC.uDC - MaxAllowUdcLimit);
        IQTemp *= -5;
    }

    IQRegenLimit = IQTemp;
    if ((pm_ref_iq < IQRegenLimit) && (IsInRegenState() == 1)) {
        pm_ref_iq = IQRegenLimit;
        pm_integral_speed = (int32)IQRegenLimit << 8;
    }
    gUdcLimitIt.UDCLimit = pm_curr_limitH; // >> 1;;
}

static void MotorControlISR(void)
{
#ifdef SPIN_CONTROL_WITH_BEMF
    GetBemf_DirSpeed();
#endif
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    SVPWM_1ShuntGetPhaseCurrent();
#else
    SVPWM_3ShuntGetPhaseCurrent();
#endif
    ChangeCurrent();

    if (gMainCmd.Command.bit.Start == TRUE)
    {
        // pm_check_ip_flag = PE.18 启动是否检测初始位置...
        // 使能后，开始检测位置，然后将该位置 0，
        if ((gMainCmd.Command.bit.StartDC == 1) ||
            (gMainCmd.Command.bit.StopDC  == 1))
        {
            CalOutVoltInDCBrakeStatus();        //直流制动,计算输出电压 2.8us
            CalRatioFromVot();

            // OutPhase -32767 ~ 32767, that is -180 ~ 180degree.
            // 60degree ~ 10922...
            if (gMainCmd.Command.bit.SecondDCPhase == 1) {
                gPhase.OutPhase = 10922;
            } else {
                gPhase.OutPhase = 0;
            }

            if (gMainStatus.RunStep == HALF_OPEN_ACC){
                gPhase.IMPhase = 49151L << 16;
            }
            else{
                gPhase.IMPhase = 0;
            }

            // pm_est_angel = (int32)gPhase.OutPhase * 3217L >> 8;
            // pm_est_angel = (int32)(gPhase.OutPhase + 16384) * 3217L >> 8;
            pm_est_angel = (int32)(gPhase.OutPhase + 1) * 3217L >> 8;
            if (pm_est_angel < 0) {
                pm_est_angel += 823549;
            }

            // 解决VF DC Brake 到开环衔接时候电流不连续的问题
            pm_integral_d = 0;
            pm_integral_q = (long)gOutVolt.VoltApply << 12;

            OutPutPWMVF();
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
            OutPutPWMPhaseShiftForLowNoiseFullMode();
#endif
            UpdatPWMRegs();
        } else {
            if (pm_check_ip_flag == 0) {
                FadeOverVarsForOL2CL();
                FluxPostionCalation();
                CurrentLoop();
                VolVectCalation();
                CalRatioFromVot();

                // VolAngleCalation();
                VolAngleCalation1();
                CalDeadBandComp();
                OutPutPWMVF();
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
            OutPutPWMPhaseShiftForLowNoiseFullMode();
#endif
                UpdatPWMRegs();
            } else {
                SynInitPosDetect();
            }
        }
    } else {
        RunBoostState = 0;
    }

#if 0
static int16 eddd;
if (frq > 148) {
    eddd = 1;
}
if (eddd == 1) {
ControlAllowRecord(1,20);    // 1ms * 128 = 128ms
IsRecordThisSample(
        gCurSamp.U,
        gMainCmd.FreqSetApply,//gPhase.OutPhase,
        frq,//gIMT.T,//pm_ref_id,
        123,
        pm_ref_iq,
        pm_ref_speed >> 4,
        pm_est_omg >> 4,
        pm_dq_angle
        );
}
#endif
#if 0
static int16 EnableRecord;
if (gMainCmd.FreqReal >= 1838) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,10);
IsRecordThisSample(
        pm_ref_id,
        pm_ref_iq,
        pm_dq_angle,//pm_est_angel,   // IV
        gCurSamp.W,//pm_est_bem,//pm_uq,//
        pm_ref_speed >> 4,//pm_dq_angle,
        pm_est_omg >> 4,//gCurSamp.W,
        gPhase.OutPhase//gIAmpTheta.Theta//gPWM_CM
        );
}
#endif
}

//long IQLimit;
//volatile long POWERKI = 30;
//volatile long POWERKP = 30;
// 2ms level tick...
// long SpeedData2[8];
static void SpeedLoop(void)
{
    int32 data,data1,LimitH,LimitL;
    int32 pm_error_speed,pm_error_speed1;

    if (gMainStatus.RunStep == STATUS_SPEED_CHECK) {
        pm_ref_iq = 0;
        pm_ref_id = 0;
        return;
    }

    if (gMainCmd.Command.bit.StartDC == 1) {
        return;
    }

    // if (gMainCmd.Command.bit.TunePart != 0) {
    //     pm_ref_iq = 0;
    //     return;
    // }
    ///////////////////////////////////////////////
    if (pm_control_mode == 0) {
        return;
    }

    pm_kp_speed_app = pm_kp_speed;
    pm_ki_speed_app = pm_ki_speed;
    pm_speed_lpf_k1 = pm_speed_lpf_k;
///////////////////////////////////////////////
//#ifdef OPEN_LOOP_START
//    static int KPSet;
//    if (KPSet < (pm_kp_speed << 2)) KPSet++;
//    pm_kp_speed_app = (KPSet >> 2);
//#endif
///////////////////////////////////////////////
    LimitL = (int32)pm_curr_limitL;
    LimitH = (int32)pm_curr_limitH;

    if (pm_forbid_revtorq == 0) {
        LimitL = -gUdcLimitIt.UDCLimit;
    }

    // 弱磁模式...0,直接计算，1：自动调节
    if (gFluxWeak.Mode == 1) {
        if (LimitL < - gFluxWeak.IqMax) {
            LimitL = - gFluxWeak.IqMax;
        }
        if (LimitH > gFluxWeak.IqMax) {
            LimitH = gFluxWeak.IqMax;
        }
        if ((LimitH < 400)&&(pm_curr_limitH>400)) {
            LimitH = 400;
        }
#if 0
        if ((LimitL > -400)&&(pm_curr_limitL<-400)) {
            LimitL = -400;
        }
#endif
        if (LimitL > LimitH) {
            LimitL = LimitH;
        }
    }

    // Torque Control Mode ...
    if (pm_control_mode == 2)
    {
        // #ifdef  FREQUENCY_CONTROL_01HZ
        if (gMainCmd.VCTorqueLim >= 0) {
            data = (int32)gBasePar.UpFreq * 160L;
        } else {
            data = -(int32)gBasePar.UpFreq * 160L;
        }
        pm_ref_speed = data;
    } else {
        // 50Hz, then pm_ref_speed = 5000*16 = 80000
        // data = (int32)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2037;
        // 注意，在FreSet 求取时候用的是2045， 这里也要取2045
        /////////////////////////////////////////////////////////////////
        data = (int64)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2040;
        pm_ref_speed = data;
        if (pm_ref_speed != 0) {
            data1 = (int32)LOW_LIMIT_FREQUENCY * 16;
            if ((data >= 0) && (data < data1)) {
                data = data1;
                pm_ref_speed = data;
            } else if ((data < 0)&&(data > -data1)) {
                data = -data1;
                pm_ref_speed = data;
            }
        }
    }

    pm_error_speed = pm_ref_speed - pm_est_omg;

    // *16 about 50000, PI calculation...
    // 限制值不能太小，不然容易震荡。
    pm_error_speed = __IQsat(pm_error_speed,50000L,-50000L);    //30HZ

    pm_error_speed1 = pm_error_speed * pm_ki_speed_app>>7;
    pm_integral_speed = pm_integral_speed + pm_error_speed1;
    pm_integral_speed = __IQsat(pm_integral_speed,LimitH * 256L,LimitL * 256L);
    data = (int32)pm_integral_speed>>8;
    data1 = data + ((int64)pm_error_speed * (int32)pm_kp_speed_app >> 8);
    pm_ref_iq = __IQsat(data1,LimitH,LimitL);

    // POE_Limit(SetPower,SetPowerKp,SetPowerKi);
    // POE_Limit(400,30,30,&IQLimit);
    // pm_ref_iq -= IQLimit;
    // pm_ref_iq = __IQsat(pm_ref_iq,LimitH,LimitL);
#ifdef OPEN_LOOP_START
    if (OL.ToClose_Idref > pm_min_curr) {
        OL.ToClose_Idref -= 25;
    } else {
        OL.ToClose_Idref += 25;
    }
    data = OL.ToClose_Idref;
#else
    data = pm_min_curr;
#endif
    if (gFluxWeak.FluxWeakCurr < 0) {
        pm_ref_id = gFluxWeak.FluxWeakCurr;
    } else {
        // pm_ref_id = pm_ref_id + (data>>3) - (pm_ref_id>>3);
        pm_ref_id = pm_ref_id + (data >> 2) - (pm_ref_id >> 2);
        if (pm_ref_id <= 10) {
            pm_ref_id = 10;
        }
    }

    if (pm_ref_speed == 0) {
        pm_ref_id = 0;
    }
#if 0
//if (gMainStatus.RunStep == OPEN_ACC && gMainCmd.Command.bit.StartDC == 0) {
static int16 EnableRecord;
if (gMainCmd.Command.bit.Start == 1 && frq > 0) { // gRatio > RatioForLowNoisePWM) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,1);
IsRecordThisSample(
    pm_ref_speed,
    pm_est_omg,
    pm_ref_iq,                 // pm_ref_iq,
    gOutVolt.Volt,          // pm_ref_id,
    gCurSamp.U,
    gCurSamp.V,
    frq,
    gDCBrake2.PID.Total >> 16//
    );
}
#endif

    if (gMainCmd.Command.bit.Start != TRUE) {
        return;
    }
}

// long TSSSS;
static void ChangeCurrent_2ms(void)
{
    Uint32 m_Long;
    int32 m_Long1;
    int32 m_Long2;

    //  m_Long = (((int32)gIAlphBeta.Alph * (int32)gIAlphBeta.Alph) +
    //            ((int32)gIAlphBeta.Beta * (int32)gIAlphBeta.Beta));
    m_Long1 = gIAlphBeta.Alph;
    m_Long1 *= m_Long1;
    m_Long2 = gIAlphBeta.Beta;
    m_Long2 *= m_Long2;
    m_Long = m_Long1 + m_Long2;

    gIAmpTheta.Amp = (Uint16)qsqrt(m_Long);
    gLineCur.CurPer = (gIAmpTheta.Amp>>1) + (gLineCur.CurPer>>1);
    gLineCur.CurPerFilter += gLineCur.CurPer - (gLineCur.CurPerFilter>>5);
}

void AllowSelectBottomOnly(Uint16 Selection)
{
    EALLOW;
    switch (Selection) {
        case 0:                         // U_BOTTOM_ONLY
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x06;
            EPwm3Regs.AQCSFRC.all = 0x0A;
            EPwm4Regs.AQCSFRC.all = 0x0A;
#else
            // B3 B2 -- CSFB
            // B1 B0 -- CSFA
            // 00 -- Force Disable
            // 01 -- Low
            // 10 -- High
            // 11 -- Disabled and No effect
            EPwm2Regs.AQCSFRC.all = 0x09;
            EPwm3Regs.AQCSFRC.all = 0x05;
            EPwm4Regs.AQCSFRC.all = 0x05;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
#endif
        break;
        case 1:                         // V_BOTTOM_ONLY
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x0A;
            EPwm3Regs.AQCSFRC.all = 0x06;
            EPwm4Regs.AQCSFRC.all = 0x0A;
#else
            EPwm2Regs.AQCSFRC.all = 0x05;
            EPwm3Regs.AQCSFRC.all = 0x09;
            EPwm4Regs.AQCSFRC.all = 0x05;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
#endif
        break;
        case 2:                         // W_BOTTOM_ONLY
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x0A;
            EPwm3Regs.AQCSFRC.all = 0x0A;
            EPwm4Regs.AQCSFRC.all = 0x06;
#else
            EPwm2Regs.AQCSFRC.all = 0x05;
            EPwm3Regs.AQCSFRC.all = 0x05;
            EPwm4Regs.AQCSFRC.all = 0x09;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
#endif
        break;
        case 3:                         // Normal PWM Pattern
            EPwm2Regs.AQCSFRC.all = 0x00;
            EPwm3Regs.AQCSFRC.all = 0x00;
            EPwm4Regs.AQCSFRC.all = 0x00;
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.DBCTL.all = 0x0007;
            EPwm3Regs.DBCTL.all = 0x0007;
            EPwm4Regs.DBCTL.all = 0x0007;
#else
            EPwm2Regs.DBCTL.all = 0x000B;
            EPwm3Regs.DBCTL.all = 0x000B;
            EPwm4Regs.DBCTL.all = 0x000B;
#endif
        break;
        case 4:                         // Disable ALL PWM
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x0A;
            EPwm3Regs.AQCSFRC.all = 0x0A;
            EPwm4Regs.AQCSFRC.all = 0x0A;
#else
            EPwm2Regs.AQCSFRC.all = 0x05;
            EPwm3Regs.AQCSFRC.all = 0x05;
            EPwm4Regs.AQCSFRC.all = 0x05;
#endif
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
        break;
        default:
        break;
    }
    EDIS;
}

static Uint16 IsInRegenState(void)
{
    Uint16 ReturnValue = 0;
    if (((pm_ref_speed > 0) && (pm_ref_iq < 0)) ||
        ((pm_ref_speed < 0) && (pm_ref_iq > 0))) {
        ReturnValue = 1;
    }
    return ReturnValue;
}

void ResetADCAndPWM(void)
{
    EALLOW;
    EPwm2Regs.AQCSFRC.all = 0;
    EPwm3Regs.AQCSFRC.all = 0;
    EPwm4Regs.AQCSFRC.all = 0;
    EDIS;

    InitSetPWM();
    InitSetAdc();
}

// EPWM.PDF TI, Page 64
// The TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag bit will remain set until it is manually
// cleared by writing to the TZCLR[DCAEVT1/2] or TZCLR[DCBEVT1/2] bit. If the DC trip event is still
// present when the TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag is cleared, then it will again be
// immediately set
////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearAllTZFlags(void)
{
    EALLOW;

    // 修复1: 先清除触发源(DCAEVT1, DCBEVT2, CBC)，再清除汇总标志(OST, INT)
    EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm4Regs.TZCLR.bit.DCAEVT1 = 1;

    // 修复2: 补充缺失的 DCBEVT2 和 CBC 清零
    EPwm2Regs.TZCLR.bit.DCBEVT2 = 1;
    EPwm3Regs.TZCLR.bit.DCBEVT2 = 1;
    EPwm4Regs.TZCLR.bit.DCBEVT2 = 1;

    EPwm2Regs.TZCLR.bit.CBC = 1;
    EPwm3Regs.TZCLR.bit.CBC = 1;
    EPwm4Regs.TZCLR.bit.CBC = 1;

    // 最后清除 OST 和 INT 汇总标志
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm3Regs.TZCLR.bit.OST = 1;
    EPwm4Regs.TZCLR.bit.OST = 1;

    EDIS;
}

void ClearAllTZFlagsForOCAtFristPowerUP(void)
{
    EALLOW;

    // Step 1: 清除源事件标志
    EPwm2Regs.TZCLR.all = 0x7F;
    EPwm3Regs.TZCLR.all = 0x7F;
    EPwm4Regs.TZCLR.all = 0x7F;

    PieCtrlRegs.PIEIFR2.all = 0x0000;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
    EDIS;
}
