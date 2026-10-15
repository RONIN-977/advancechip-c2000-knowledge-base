

#include "MotorInclude.h"
#include "Main.h"
#include "Record.h"
#include "Parameter.h"
#include "FuncUser.h"
#include "RunSrc.h"
#include "Reference.h"
#include "PreDriver.h"

void ParameterChange(void);	
void CalCarrierWaveFreq(void);
void RunCaseDeal(void);
void RunCaseLowPower(void);
void RunCaseGetPar(void);
void RunCaseSpeedCheck(void);
void PrepareForSpeedCheck(void);
void RunCaseRun(void);
void OpenLoopAccRun(void);
void RunCaseBrakeLowBridge(void);
void RunCaseStop(void);
void RunCaseShortGnd(void);
void PrepareParForRun(void);
void CalStepAngle(void);

void CalRatioFromVot(void);
void CalDeadBandComp(void);
void SoftPWMProcess(void);
void CalOutputPhase(void);
void GetCurrentExcursion(void);

void SoftWareErrorDeal(void);					
void CalUdcLimitIT(void);
void SoftDebug(void);
unsigned short IsInRegenState(void);

//int IsRelayCheckFinshed;
UDC_LIMIT_IT_STRUCT gUdcLimitIt;
DC_BRAKE_STRUCT     gDCBrake2;
long pm_fast_freq;
long LimitHGlobal = 4096;
int DCStart_flag = 0;
int IDFadeOverCnt;
int SHORT_GND_PERIOD = 5000;    // 10K SF

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
}

/************************************************************
主程序0.5ms循环：用于执行需要快速刷新的程序，要求程序非常简短
************************************************************/
void SystemLeve05msMotor(void)
{
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
void ParameterChange(void)		
{
	Ulong	m_ULong;
	Uint 	m_UData;

	// gInvInfo.InvVoltageType
	gInvInfo.InvVolt = MOTOR_RATED_VOL;

	// 确定过压和欠压点的大小...
	// LowDc = 200V
	// InvUpUDC = OverDc = 390V
	gInvInfo.InvLowUDC = gInvInfo.InvLowUDCCoef;
	gInvInfo.InvUpUDC  = gInvInfo.InvUpUDCCoef;

#if 0
	// 12V Drive, ONLY 7 8 9 Can be used.
	// * 14480) >> 10 = 113
	// * 960) >> 6    = 120
	gInvInfo.BaseUdc = ((long)MOTOR_RATED_VOL * 14480) >> 10;
#else
    gInvInfo.BaseUdc = ((long)MOTOR_RATED_VOL * 960) >> 6;
#endif

	// 设定额定电流，实际产品可以考虑将它设置为固定值，比电机额定电流一致
	gInvInfo.InvCurrent = MOTOR_RATED_CUR;

	// gUDC.Coff = 9500, 代表3.3V ~ 950V and with 0.1 DP
	// 换上2802x 后直接将gUDC.Coff 改成一个固定值即可。
    // 443.3V, then 4433/
	gUDC.Coff = MAX_DC_VOL_3_3V;

	//设置电机电流太小的处理
	gMotorInfo.Current = gMotorInfo.CurrentGet;

	// 计算死区和死区补偿参数, 实际产品设置为固定 2us
    gDeadBand.DeadBand = DeadTimeInternal;
    gDeadBand.Comp     = DeadTimeCompInternal;

	EALLOW;									//设置死区时间
	EPwm2Regs.DBFED = gDeadBand.DeadBand;
	EPwm2Regs.DBRED = gDeadBand.DeadBand;
	EPwm3Regs.DBFED = gDeadBand.DeadBand;
	EPwm3Regs.DBRED = gDeadBand.DeadBand;
	EPwm4Regs.DBFED = gDeadBand.DeadBand;
	EPwm4Regs.DBRED = gDeadBand.DeadBand;
	EDIS;
	
	// 24.75A
    // MAX_PEAK_CUR_3_3V = 414, 代表的是 4.14A Peak. Rated is 2.0A
    // (4.14 / 2.0) * 2^12 = 8448,
    m_ULong = MAX_PEAK_CUR_3_3V_410;
    m_ULong = m_ULong / gMotorInfo.Current;
    gCurSamp.Coff = m_ULong;

	//计算频率表示
    // #ifdef  FREQUENCY_CONTROL_01HZ
    gBasePar.FullFreq = ((long)gBasePar.MaxFreq * 10) + 2000;
	gMotorInfo.FreqPer = ((Ulong)gMotorInfo.Frequency <<15) / gBasePar.FullFreq;

	//计算用实际值表示的运行频率
	m_UData = abs(gMainCmd.FreqSetApply);
	gMainCmd.FreqReal = ((Ulong)m_UData * (Ulong)gBasePar.FullFreq) >> 15;
}

/************************************************************
 计算载波频率：输入gBasePar.FcSet，输出gBasePar.FcSetApply
 计算励磁电流：输入pm_ref_speed,   输出pm_min_curr
 2ms Level...
************************************************************/
void CalCarrierWaveFreq(void)
{
	long data,data1,freqs1,fcSet;
    fcSet = gBasePar.FcSet;

	if (pm_control_mode != 0)
	{
        // data = pm_est_omg_1;
        data = pm_ref_speed;
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
                data = data * ((long)gBasePar.FcSet - (long)SF_LOW_SPEED) / (HighFreqencyOFSF - LowFreqencyOFSF);
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
        data = (long)TORQUE_CUR_LIMIT * 4096L / 100;
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

void RunCaseDeal(void)
{
	switch(gMainStatus.RunStep)
	{
		case STATUS_LOW_POWER:	//上电缓冲状态/欠压状态
			RunCaseLowPower();
			break;

		case STATUS_GET_PAR:	//参数辨识状态
			RunCaseGetPar();
			break;

		case STATUS_SPEED_CHECK://转速跟踪状态
			RunCaseSpeedCheck();
			break;

		case STATUS_RUN:		//运行状态，区分VF/FVC/SVC运行
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

        case STATUS_SHORT_GND:	//上电对地短路判断状态
			RunCaseShortGnd();
			break;

		default:				//停机状态：所有其它状态认为是该状态
			RunCaseStop();
			break;
	}	
}

void RunCaseLowPower(void)
{
	Uint uDCLowLimt;
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
        		if (gMainStatus.ErrorCode == ERROR_UV)
                {
                    gMainStatus.ErrorCode = 0;
		            if(gMainStatus.ErrFlag.bit.OvCurFlag == 1)  //修改欠压后无法进入过流中断的错误
		            {
			            gMainStatus.ErrFlag.bit.OvCurFlag = 0;
			            EALLOW;
			            EPwm2Regs.TZCLR.bit.OST = 1;
			            EPwm3Regs.TZCLR.bit.OST = 1;
			            EPwm4Regs.TZCLR.bit.OST = 1;

                        EPwm2Regs.TZCLR.bit.CBC = 1;
                        EPwm3Regs.TZCLR.bit.CBC = 1;
                        EPwm4Regs.TZCLR.bit.CBC = 1;

                        // EPWM.PDF TI, Page 64
                        // The TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag bit will remain set until it is manually
                        // cleared by writing to the TZCLR[DCAEVT1/2] or TZCLR[DCBEVT1/2] bit. If the DC trip event is still
                        // present when the TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag is cleared, then it will again be
                        // immediately set
                        EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
                        EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
                        EPwm4Regs.TZCLR.bit.DCAEVT1 = 1;

                        EPwm2Regs.TZCLR.bit.DCBEVT2 = 1;
                        EPwm3Regs.TZCLR.bit.DCBEVT2 = 1;
                        EPwm4Regs.TZCLR.bit.DCBEVT2 = 1;

                        EPwm2Regs.TZCLR.bit.INT = 1;
			            EDIS;
		            }     
        		}
			}
		break;
		default:
			// DisConnectRelay();
            gMainStatus.ErrorCode = ERROR_UV;				//错标志
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
void RunCaseGetPar(void)
{
}

long SpeedData[20];
/************************************************************
	转速跟踪状态处理
	2ms, 状态机...
	风机：利用反电动势提高性能
	水泵或其他：直接电流闭环..
************************************************************/
void RunCaseSpeedCheck(void)
{
	long data;
	if ((gMainCmd.Command.bit.Start == FALSE) ||        //停机命令
	    (gMainStatus.ErrorCode      != 0    ))			//故障
	{ 
		gMainStatus.RunStep = 3;
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
            pm_est_lpf_omg1 = (long)gBemf.Spd_Hz * 1602L;
#endif
			data = (long long)pm_est_lpf_omg1 * 2045L / gBasePar.FullFreq;
SpeedData[1]=gBemf.Spd_Hz;
SpeedData[2]=pm_est_lpf_omg1;
SpeedData[3]++;
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
SpeedData[5]=data;
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
//SpeedData[7]++;
//SpeedData[8] = SpinControlInternal;
//SpeedData[9] = pm_est_lpf_omg1;
                } else {
                    // if (pm_est_lpf_omg1 <= (90000)) {
                    // if ((gMainCmd.FreqSetApply > 0) && (pm_est_lpf_omg1 < -90000) ||    //  56hz
                    //     (gMainCmd.FreqSetApply < 0) && (pm_est_lpf_omg1 >  90000)) {
                    ////////////////////////////////////////////////////////////////////////////////////////////
                    if (((frqAimTmp0 > 0) && (pm_est_lpf_omg1 < -SpinControlInternal)) ||    //  56hz
                        ((frqAimTmp0 < 0) && (pm_est_lpf_omg1 >  SpinControlInternal))) {
                        gMainStatus.RunStep = STATUS_BRAKE;
//SpeedData[10]++;
//SpeedData[11] = SpinControlInternal;
//SpeedData[12] = pm_est_lpf_omg1;
//SpeedData[13] = frqAimTmp0;
                    } else {
                        pm_integral_q = gBemf.OutVolt;
                        pm_integral_speed = ((long)OPEN_LOOP_TOR_START << 8);
                        if(gBemf.DIR == 1)
                        {
                            pm_integral_speed = -pm_integral_speed;
                        }
                        gMainStatus.RunStep = STATUS_RUN;//STATUS_PRE_RUN;//STATUS_RUN;
                        gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
//SpeedData[14]++;
//SpeedData[15] = gMainCmd.FreqSetApply;
//SpeedData[16] = pm_est_lpf_omg1;
//SpeedData[17] = frqAimTmp0;
//SpeedData[18] = pm_ref_speed;
                    }
                }
#else
                gMainStatus.RunStep = STATUS_RUN;
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
	}
}

//long AccRunData[10];
//int RecordOpen;
typedef enum {
    McInit              = 0,
    McAlignRun          = 1,
    McRsIDRun           = 2,
    McOpenRun           = 3,
    McSpdChangeToClosed = 4,
    McSFinished         = 5
} MCP_SPDSTATE_DEF;
MCP_SPDSTATE_DEF McSpdState;        // 全局变量在STOP 或其他状态可以清零操作
static int McSpdStateCnt;           // 有时候会出现，2MS 间隔时间导致功能没有进入直流制动，OpenLoopAcc 判断直接进入OpenAcc
int IDRef;
int FrqTemp;
void OpenLoopAccRun(void)
{
    if ((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
//AccRunData[0]++;
        DisableDrive();
        IDRef = 0;
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
             if (IDRef > DCBrakeCur) {
                 IDRef = DCBrakeCur;
             } else {
                 IDRef += 30;
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
            frqLine.curValue = pm_est_omg / 160;
            frqTmp = frqLine.curValue;
            ///////////////////////////////////////////////
#endif
            McSpdState = McSpdChangeToClosed;
        } else {
            IDRef = DCBrakeCur;
        }
    break;
    case McSpdChangeToClosed: {
#ifdef FULL_OPEN_LOOP_MODE
        pmsvc_init();
        PMFluxWeakInit();
//AccRunData[8]++;
        //pm_coef_alfa = 170666;
        if (gMainCmd.FreqSetApply < 0) {
            pm_est_angel = (long)(gPhase.OutPhase + 16384) * 3217L >> 8;
        } else {
            pm_est_angel = (long)(gPhase.OutPhase - 16384) * 3217L >> 8;
        }
        pm_integral_d = 0;
        pm_integral_q = (long)1200 << 12;
        if (gMainCmd.FreqSetApply < 0) {
            pm_integral_q = -pm_integral_q;
        }
        gMainStatus.RunStep = STATUS_RUN;
        McSpdState = McInit;
        PaseShiftEnable = 1;

        // Frq = 10.0HZ, 160200 / 16 = 10200
        // #ifdef  FREQUENCY_CONTROL_01HZ
        FrqTemp = (long)gMainCmd.FreqReal / 10 + 200;

        if (gMainCmd.FreqSetApply < 0) {
            FrqTemp = -FrqTemp;
        }
        frqLine.curValue = FrqTemp;
        frqTmp = FrqTemp;
        pm_min_curr2 = 2000;
#else
        gMainStatus.RunStep = STATUS_RUN;
        McSpdState = McInit;
        pm_est_angel = (long)(gPhase.OutPhase - pm_dq_angle) * 3217L >> 8;

        // 切入闭环的IQ 初始给定...
        pm_integral_speed = ((long)OPEN_LOOP_TOR_START << 8);
        // pm_integral_speed = ((long)gIMT.T << 8);
#endif
    }
    break;
    case McSFinished:
    break;
    }

    pm_ref_iq = 0;
    pm_ref_id = IDRef;
#if 0
if (RecordOpen == 1) {
ControlAllowRecord(1,1);
IsRecordThisSample(
        McSpdState,
        gMainStatus.RunStep,   // IV
        gMainCmd.FreqSetApply,
        runStatus,//gCurSamp.V,
        gMainCmd.Command.bit.StartDC,
        1,
        gPhase.OutPhase//gIAmpTheta.Theta//gPWM_CM
        );
}
#endif
}

int IDRef2;
/************************************************************
	正常运行过程处理
	运行状态机, 2ms...
************************************************************/
void RunCaseRun(void)
{
#if (SINGLE_SHUNT_PWM_MODE == NORMAL_NOISE_PWM_MODE)
    static unsigned int PhaseShiftHysteresis;
#endif
    if ((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
        DisableDrive();

        IDRef2 = 0;
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        return;
    }

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
           if (IDRef2 > DCBrakeCur) {
               IDRef2 = DCBrakeCur;
           } else {
               IDRef2 += 30;
           }
           pm_ref_id = IDRef2;
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
		if(gMainCmd.FreqLast > 0)		gOutVolt.VoltPhaseApply = 8192;
		else if(gMainCmd.FreqLast < 0)	gOutVolt.VoltPhaseApply = -8192;
	}
}

//long RunCaseShortGndData[8];
/************************************************************
    上电对地短路判断
    Alpha 底座采样的是V W，
    HC    底座采样的是U V，所以之前用U 相做对地保护。这里改成V相。
    即：V+相发波，检测V相电流大小
************************************************************/
void RunCaseShortGnd(void)
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
void RunCaseBrakeLowBridge(void)
{
    static int BrakeLowBridgeState;
    static int BrakeLowBridgeStateCnt;
    enum {
        BRAKE_LOW_BRIGE_INIT,
        BRAKE_LOW_BRIGE_LOWON,
        BRAKE_LOW_BRIGE_CHECK,
        BRAKE_LOW_BRIGE_FINISH
    };
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
void RunCaseStop(void)
{
    // 针对水泵，转速追踪不需要，为了跟风扇程序统一，判断条件保持一致。对于水泵，
    // 这三个AD口的值为155.....
    ////////////////////////////////////////////////////////////////////
    //    Adc_Temp_V1 = AdcResult.ADCRESULT4;
    //    Adc_Temp_V2 = AdcResult.ADCRESULT5;
    //    Adc_Temp_V3 = AdcResult.ADCRESULT6;
    //    VolOffset = (Adc_Temp_V1 + Adc_Temp_V2 + Adc_Temp_V3);
    ////////////////////////////////////////////////////////////
    DisableDrive();							//停机封锁输出

	PrepareParForRun();
	pmsvc_init();
	PMFluxWeakInit();
	gMainStatus.SubStep = 1;
    McSpdState = McInit;                    // MCOPEN 时候欠压，则没没办法重新开始DC--OPENRUN

    if(gMainStatus.StatusWord.bit.RunEnable != 3)
	{
		return;								//等待零漂检测完成
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
    //	if (gMainCmd.Command.bit.TunePart == 1)	//判断是否需要参数辨识
    //	{
    //		gMainStatus.RunStep = STATUS_GET_PAR;
    //		gMainStatus.SubStep = 1;
    //		return;
    //	}
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
                pm_check_ip_flag = 0;
                DCStart_flag = 1;
                gMainCmd.FreqSetApply  = 0;
                gMainCmd.FreqPreWs     = 0;
                gMainCmd.FreqSet       = 0;
                gMainCmd.FreqFeed      = 0;
                gMainCmd.FreqDesired   = 0;
                gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
#else
                gMainStatus.RunStep = STATUS_SPEED_CHECK;
#endif
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
            gMainStatus.RunStep = STATUS_RUN;
            gMainStatus.SubStep = 1;
            EnableDrive();
        }
	}
}

/************************************************************
	启动电机运行前的数据初始化处理，为电机运行准备初始参数
	停机过程变量清零处理...
************************************************************/
void PrepareParForRun(void)
{
	gMainCmd.FreqSetApply = 0;
	gMainCmd.FreqPreWs = 0;
	gMainCmd.FreqLast = 0;
	gMainCmd.FreqReal = 0;
	
	gOutVolt.Volt = 0;
	gOutVolt.VoltApply = 0;
	IDRef = 0;
	IDRef2 = 0;

	if(gMainCmd.FreqDesired > 0)		//根据目标频率方向确定初始相位角
	{
		gOutVolt.VoltPhaseApply = 8192;
	} else {
		gOutVolt.VoltPhaseApply = -8192;
	}		
	gPhase.OutPhase = (int)(gPhase.IMPhase>>16) + gOutVolt.VoltPhaseApply;

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

    //    data =  (long)75000UL * OVER_SPEED_SCALING; // 过速频率系数.Max = 20000 * 120
    //    pm_fast_freq = (data * 16)/100;             // 384000
    //    初始化一次，导致  gBasePar.MaxFreq 没有初始化... ...
    long data;
    data =  (long)gBasePar.MaxFreq * OVER_SPEED_SCALING;
    //#ifdef FREQUENCY_CONTROL_01HZ
    pm_fast_freq = (data * 16) / 10;
}

/************************************************************
	计算零漂程序
	4066，所以不能一直读取, main2ms call
************************************************************/
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
void GetCurrentExcursion(void)
{
    int m_ErrIv,m_ErrUu,m_ErrIbus;
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
    //	if ((IsCurExcursionFinshied == 1) && (IsRelayCheckFinshed == 1)) {
    //		gMainStatus.StatusWord.bit.RunEnable = 3;
    //		return;
    //	}
	//////////////////////////////////////////////////////////////////////
	gExcursionInfo.TotalIv += (int)ADC_IV;
	gExcursionInfo.TotalUu += (int)ADCRESULT_UU;
	gExcursionInfo.TotalIbus += (int)ADC_IBUS;
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
		if ((abs(m_ErrIv-2048) < 500) && (1))   // 电压判断？？
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
void GetCurrentExcursion(void)
{
    int m_ErrIu,m_ErrIv,m_ErrIw;
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

    gExcursionInfo.TotalIu += (int)(ADC_IU - (Uint)32768);
    gExcursionInfo.TotalIv += (int)(ADC_IV - (Uint)32768);
    gExcursionInfo.TotalIw += (int)(ADC_IW - (Uint)32768);
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
void ADCOverInterrupt()
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
#ifdef LOW_NOISE_PWM_MODE
        OutPutPWMPhaseShiftForLowNoiseFullMode();
#else
        OutPutPWMPhaseShift();
#endif
        UpdatPWMRegs();
	} else {
	    MotorControlISR();
	}
}

/*************************************************************
	随机PWM处理，使载波周期和输出相位生效
*************************************************************/
void SoftPWMProcess(void)
{
    // 不同场景下可能摆动的范围有区别。
}

//long OCData[6];
/*************************************************************
	过流中断处理程序（可屏蔽中断，电平触发）
*************************************************************/
void HardWareErrorDeal()
{
	DisableDrive();

	gMainStatus.ErrFlag.bit.OvCurFlag = 1;
	if (gMainStatus.RunStep == STATUS_SHORT_GND) {
	    gShortGnd.Flag = 1;                     //上电对地短路
	} else if(gMainStatus.ErrorCode != ERROR_OC_HARDWARE) {
		gMainStatus.ErrorCode = ERROR_OC_HARDWARE;
	}
//OCData[0]=gCurSamp.U;
//OCData[1]=gCurSamp.V;
//OCData[2]=gCurSamp.W;
//OCData[3]=gRatio;
//OCData[4]++;
	EALLOW;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EPwm4Regs.TZCLR.bit.OST = 1;

    // EPWM.PDF TI, Page 64
    // The TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag bit will remain set until it is manually
    // cleared by writing to the TZCLR[DCAEVT1/2] or TZCLR[DCBEVT1/2] bit. If the DC trip event is still
    // present when the TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag is cleared, then it will again be
    // immediately set
    EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
    EPwm4Regs.TZCLR.bit.DCAEVT1 = 1;
	EDIS;
}
long SoftErrorData[10];
/*************************************************************
	软件故障处理
*************************************************************/
void SoftWareErrorDeal(void)					
{
	static int SoftwareOCCnt;

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

			EALLOW;
			EPwm2Regs.TZCLR.bit.OST = 1;
			EPwm3Regs.TZCLR.bit.OST = 1;
			EPwm4Regs.TZCLR.bit.OST = 1;
			EPwm2Regs.TZCLR.bit.INT = 1;

            // EPWM.PDF TI, Page 64
            // The TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag bit will remain set until it is manually
            // cleared by writing to the TZCLR[DCAEVT1/2] or TZCLR[DCBEVT1/2] bit. If the DC trip event is still
            // present when the TZFLG[DCAEVT1/2] or TZFLG[DCBEVT1/2] flag is cleared, then it will again be
            // immediately set
            /////////////////////////////////////////////////////////////////////////////////////////////////////
            EPwm2Regs.TZCLR.bit.DCAEVT1 = 1;
            EPwm3Regs.TZCLR.bit.DCAEVT1 = 1;
            EPwm4Regs.TZCLR.bit.DCAEVT1 = 1;
			EDIS;
		}
	}

	/////////////////////////////////////////////////开始判断软件故障
	// 软件过流实际意义不大，但就当多一重保护而已吧.用硬件相同的过流点...
	if ((gLineCur.CurPer 		    > SOFT_OC_POINT) &&
		(gMainCmd.Command.bit.Start == TRUE		   ))
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

	if (gUDC.uDC > gInvInfo.InvUpUDC)	        //过压判断,使用大滤波电压
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
	static int InstallCnt2;
    if ((pm_ref_speed - pm_est_omg                  > 16000 ) &&    // Ref > 50Hz,
        (pm_est_omg             < 60000                     ) &&    // omg < 37Hz,
        (gMainStatus.ErrorCode != ERROR_LOSE_PHASE_OUTPUT   ) &&
        (gMainCmd.Command.bit.StartDC               != 1    ) &&
        (pm_ref_iq              > (LimitHGlobal - 100       ))) {
        InstallCnt2++;
        if (InstallCnt2 > 250) {
            InstallCnt2 = 0;
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_STALL;
        }
    } else {
        if (InstallCnt2 > 0)  InstallCnt2 -= 2;
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

    int RMSTripPointForStall;
    long Data;
    Data = IRMS_AT_100HZ;
    RMSTripPointForStall = (Uint)qsqrt(Data * Data + pm_ref_id * pm_ref_id);
    RMSTripPointForStall *= 3;
    RMSTripPointForStall /= 2;

    // 应该计算一个对应频率下正常运行电流...
    static int InstallCnt2;
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
          if (InstallCnt2 > 0)  InstallCnt2 -= 2;
     }
#endif

    // Open Loop state Stall Protection...
    static int InstallCnt3;
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

    // 超速监测，速度超调过大。
    // 一般情况：
    // 1：没接电机，速度观察值不对。
    // 2：高速PI 太小，加速时候速度超调太多。（比如冰箱压机负载，重再启动可能会）
    // 视情况，可以屏蔽。
    static int InstallCnt1;
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

    // 干烧保护。速度达到一定值且负载很小的情况，持续一定时间。。。
    ////////////////////////////////////////////////////////
    static int InstallCnt4;
    if ((pm_ref_speed                   > 130000) &&     // Ref > 80Hz,
        (pm_est_omg                     > 130000) &&     // omg > 80Hz,
        ((gLineCur.CurPerFilter >> 5)   < 800   ) &&     // 1 / 5 额定值
        (gLineCur.CurPer                > 120   ) &&     // 接了电机，是空转. 不接电机平均50以下
        (gMainCmd.Command.bit.StartDC  != 1     )) {
        InstallCnt4++;
        if (InstallCnt4 > 1000) {
            InstallCnt4 = 0;
            //DisableDrive();
            //gMainStatus.ErrorCode = ERROR_NO_LOAD;
        }
    } else {
        if (InstallCnt4 > 0)  InstallCnt4 -= 3;
    }
}

/*************************************************************
	矢量控制的过压抑制功能，通过母线电压限制输出转矩(发电转矩)的最大值
*************************************************************/
void CalUdcLimitIT(void)
{
    int IQRegenLimit;
    int IQTemp;

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
        pm_integral_speed = (long)IQRegenLimit << 8;
    }
    gUdcLimitIt.UDCLimit = pm_curr_limitH;// >> 1;;
}

//extern long pm_est_bem;
//extern Uint32 runTimeTicker;
void MotorControlISR()
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
            CalOutVoltInDCBrakeStatus();
            CalRatioFromVot();

            // OutPhase -32767 ~ 32767, that is -180 ~ 180degree.
            // 60degree ~ 10922...
            if (gMainCmd.Command.bit.SecondDCPhase == 1) {
                gPhase.OutPhase = 10922;
            } else {
                gPhase.OutPhase = 0;
            }
            gPhase.IMPhase = 0;

            // pm_est_angel = (long)gPhase.OutPhase * 3217L >> 8;
            // pm_est_angel = (long)(gPhase.OutPhase + 16384) * 3217L >> 8;
            pm_est_angel = (long)(gPhase.OutPhase + 1) * 3217L >> 8;
            if (pm_est_angel < 0) {
                pm_est_angel += 823549;
            }
#ifdef OPEN_LOOP_START
            pm_integral_d = (long)gOutVolt.VoltApply << 12;
            pm_integral_q = 1L << 12;
#endif
            OutPutPWMVF();
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
            OutPutPWMPhaseShiftForLowNoiseFullMode();
#endif
            UpdatPWMRegs();
        } else {
            if (pm_check_ip_flag == 0) {
                FluxPostionCalation();
                CurrentLoop();
                VolVectCalation();
                CalRatioFromVot();
                VolAngleCalation();
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
static int eddd;
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
static int EnableRecord;
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
long IQLimit;
volatile long POWERKI = 30;
volatile long POWERKP = 30;
// 2ms level tick...
// long SpeedData2[8];
void SpeedLoop()
{
    long data,data1,LimitH,LimitL;
    long pm_error_speed,pm_error_speed1;

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
    LimitL = (long)pm_curr_limitL;
    LimitH = (long)pm_curr_limitH;

    if (pm_forbid_revtorq == 0) {
        LimitL = -gUdcLimitIt.UDCLimit;
    }
    LimitHGlobal = LimitH;

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
            data = (long)gBasePar.UpFreq * 160L;
        } else {
            data = -(long)gBasePar.UpFreq * 160L;
        }
        pm_ref_speed = data;
    } else {
        // 50Hz, then pm_ref_speed = 5000*16 = 80000
        // data = (long)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2037;
        // 注意，在FreSet 求取时候用的是2045， 这里也要取2045
        /////////////////////////////////////////////////////////////////
        data = (long long)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2040;
        pm_ref_speed = data;
        if (pm_ref_speed != 0) {
            data1 = (long)LowLimitFrequency * 16;
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
    data = (long)pm_integral_speed>>8;
    data1 = data + ((long long)pm_error_speed * (long)pm_kp_speed_app>>8);
    pm_ref_iq = __IQsat(data1,LimitH,LimitL);

    // POE_Limit(SetPower,SetPowerKp,SetPowerKi);
    //    POE_Limit(400,30,30,&IQLimit);
    //    pm_ref_iq -= IQLimit;
    //    pm_ref_iq = __IQsat(pm_ref_iq,LimitH,LimitL);

    // 30 * 80 = 2400,pm_min_curr 通过线性变化，到24.38Hz...
    data = pm_min_curr;
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
static int EnableRecord;
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
void ChangeCurrent_2ms(void)
{
    Ulong m_Long;
    long m_Long1;
    long m_Long2;

    //  m_Long = (((long)gIAlphBeta.Alph * (long)gIAlphBeta.Alph) +
    //            ((long)gIAlphBeta.Beta * (long)gIAlphBeta.Beta));
    m_Long1 = gIAlphBeta.Alph;
    m_Long1 *= m_Long1;
    m_Long2 = gIAlphBeta.Beta;
    m_Long2 *= m_Long2;
    m_Long = m_Long1 + m_Long2;

    gIAmpTheta.Amp = (Uint)qsqrt(m_Long);
    gLineCur.CurPer = (gIAmpTheta.Amp>>1) + (gLineCur.CurPer>>1);
    gLineCur.CurPerFilter += gLineCur.CurPer - (gLineCur.CurPerFilter>>5);

    OutputLoseAdd();
}

void AllowSelectBottomOnly(unsigned short Selection)
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

unsigned short IsInRegenState(void)
{
    unsigned short ReturnValue = 0;
    if (((pm_ref_speed > 0) && (pm_ref_iq < 0)) ||
        ((pm_ref_speed < 0) && (pm_ref_iq > 0))) {
        ReturnValue = 1;
    }
    return ReturnValue;
}
