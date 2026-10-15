
#include "MotorInclude.h"
#include "param_lr.h"
#include "Main.h"
#include "Parameter.h"
#include "Record.h"
#include "StaticVModelObs.h"
#include "MCQpr.h"

void ParameterChange(void);	
void CalCarrierWaveFreq(void);
void RunCaseDeal(void);
void RunCaseLowPower(void);
void RunCaseGetPar(void);
void RunCaseSpeedCheck(void);
void SpeedCheckUdcRiseDeal(void);
void SpeedCheckCycleOver(void);
void SpeedCheckSUB1(void);
void SpeedCheckCycleSUB2(void);
void PrepareForSpeedCheck(void);
void RunCaseRun(void);
void OpenLoopAccRun(void);
void RunCaseStop(void);
void RunCaseShortGnd(void);
void PrepareParForRun(void);
void CalStepAngle(void);
void CBCLimitCurPrepare(void);
void CalRatioFromVot(void);
void CalDeadBandComp(void);
void SoftPWMProcess(void);
void CalOutputPhase(void);
void GetCurrentExcursion(void);
void BrakeResControl(void);

void SoftWareErrorDeal(void);					
void CalUdcLimitIT(void);
void SoftDebug(void);

int SHORT_GND_PERIOD = 5000;    // 10K SF
UDC_LIMIT_IT_STRUCT gUdcLimitIt;
int SOFT_OC_SET = 250;
long pm_fast_freq;
long LimitHGlobal = 4096;
int DCStart_flag = 0;

/************************************************************
性能模块初始化程序：主循环前初始化性能部分的变量(所有等于0的变量无需初始化)
************************************************************/
void  InitForMotorApp(void)
{
    long data;

    DisableDrive();

	gMainStatus.RunStep = STATUS_LOW_POWER;
	gMainStatus.SubStep = 1;

	gMainStatus.LastErrorCode = gMainStatus.ErrorCode;

	gMainStatus.StatusWord.all = 0;
    gCBCProtect.EnableFlag = 1;

	gADC.DelaySet = 100;
	gADC.DelayApply = 600;
	gFcCal.FcBak = 80;
	gBasePar.FcSetApply = 80;

    gPWM.gPWMPrd = C_INIT_PRD;
    gPWM.gZeroLengthPhase = ZERO_VECTOR_NONE;

	//***********************************************************
	pm_control_mode = PMSM_CONTROL_MODE_FOC;

    //    data =  (long)75000UL * OVER_SPEED_SCALING; // 过速频率系数.Max = 20000 * 120
    //    pm_fast_freq = (data * 16)/100;             // 384000
    //    初始化一次，导致	gBasePar.MaxFreq 没有初始化... ...
    data =  (long)gBasePar.MaxFreq * OVER_SPEED_SCALING;

#ifdef FREQUENCY_CONTROL_01HZ
    pm_fast_freq = (data * 16) / 10;
#else
    pm_fast_freq = (data * 16) / 100;
#endif
    DCStart_flag = 0;
#ifdef ENABLE_NEW_STATIC_OBS
    NewOBS = 1;
#else
    NewOBS = 0;
#endif
}

/************************************************************
主程序的单2ms循环：用于执行电机控制程序
思路：数据输入->数据转换->控制算法->控制输出->自我保护
执行时间：不被中断打断的情况下约120us
************************************************************/
void Main2msMotor(void)		
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

	// from 125us interrupt function
	GetTemperatureInfo();	// 0.74%
	ADCProcess();			// 1.05%
	UpdatePMSMControlVariable();			// 22.1%
	GetUDCInfo();			// 0.9%
	ChangeCurrent_2ms();
	PMSMSVC_2ms();
	PMCalcLineCurr();
#ifdef ENABLE_NEW_STATIC_OBS
	Est_SetParams();
#endif
    QPRParameterCal(&Qpr_q);
    QPRParameterCal(&Qpr_d);
}

/************************************************************
主程序0.5ms循环：用于执行需要快速刷新的程序，要求程序非常简短
************************************************************/
void Main05msMotor(void)
{
   if(pm_control_mode != 0)
   {
		return;
   }
   RunCaseDeal();
}

/************************************************************
主程序不等待循环：用于执行需要快速刷新的程序，要求程序非常简短
************************************************************/
void Main0msMotor(void)
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
    gInvInfo.InvLowUDC = gInvInfo.InvLowUDCCoef;
    gInvInfo.InvUpUDC  = gInvInfo.InvUpUDCCoef;
    gInvInfo.BaseUdc = ((long)MOTOR_RATED_VOL * 14480) >> 10;

	// 过压失速点 351V = gOvUdc.Limit...
	gOvUdc.Limit = OVER_DC_VOL_LIMIT;

	// 设定变频器额定电流，实际产品可以考虑将它设置为固定值，比电机额定电流一致
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
	
	gADC.DelayApply = gADC.DelaySet;

	// 24.75A
    // MAX_PEAK_CUR_3_3V = 414, 代表的是 4.14A Peak. Rated is 2.0A
    // (4.14 / 2.0) * 2^12 = 8448,
    m_ULong = MAX_PEAK_CUR_3_3V_410;
    m_ULong = m_ULong / gMotorInfo.Current;
    gCurSamp.Coff = m_ULong;

	//计算频率表示
#ifdef  FREQUENCY_CONTROL_01HZ
    gBasePar.FullFreq = ((long)gBasePar.MaxFreq * 10) + 2000;
#else
    gBasePar.FullFreq = gBasePar.MaxFreq + 2000;
#endif

	gMotorInfo.FreqPer = ((Ulong)gMotorInfo.Frequency << 15) / gBasePar.FullFreq;

	//计算用实际值表示的运行频率
	m_UData = abs(gMainCmd.FreqSetApply);
	gMainCmd.FreqReal = ((Ulong)m_UData * (Ulong)gBasePar.FullFreq) >> 15;
}

/************************************************************
 计算载波频率：输入gBasePar.FcSet，输出gBasePar.FcSetApply
 2ms Level...
************************************************************/
void CalCarrierWaveFreq(void)
{
	long data,data1,freqs1,fcSet;
    fcSet = gBasePar.FcSet;   //l0310
   	data = (500000L * 512);

	if (pm_control_mode != 0) {
#if 1
	    data = pm_ref_speed;
#else
	    data = pm_est_omg_1;
#endif
	    freqs1 = labs(data);

// See Predifine -- CCS Set
#if (SF_INDEPAND_OF_FREQUENCY == SW_RANDOM_DISABLE)
	    fcSet = gBasePar.FcSet;
#else
	   // 低速载频，默认FE。10 = 3.0KHz
	    if (freqs1 < 32767L) {
	        fcSet = SF_LOW_SPEED;
	    } else if (freqs1 >= 65536L) {
	        fcSet = gBasePar.FcSet;
	    } else {
            data = freqs1 - 32767L;
            data = data * ((long)gBasePar.FcSet - (long)SF_LOW_SPEED) >> 15;
            fcSet = data + SF_LOW_SPEED;
        }
	    if (SF_LOW_SPEED >= gBasePar.FcSet) {
			fcSet = gBasePar.FcSet;
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
        data = data * data - (long)pm_ref_iq * pm_ref_iq;
        if (data < 810000) {
            data = 810000;
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
	gFcCal.Cnt += gBasePar.FcSetApply;
	if (gFcCal.Cnt < 100) {
		return;
	}
	gFcCal.Cnt = 0;

    gFcCal.FcBak = fcSet;
    if (gFcCal.FcBak > gBasePar.FcSetApply) {
        gBasePar.FcSetApply++;
    } else if(gFcCal.FcBak < gBasePar.FcSetApply) {
        gBasePar.FcSetApply--;
    }
#endif

#if 0
    if (gMainCmd.FreqReal > 25000 + 300)
    {
        gPWM.PWMModle = MODLE_DPWM;
    }
    else if( gMainCmd.FreqReal < 25000)
    {
        gPWM.PWMModle = MODLE_CPWM;
    }
#else
    gPWM.PWMModle = MODLE_CPWM;
#endif

#ifdef FIVEPWM_ENABLE
    if (gMainCmd.FreqReal > FIVEPWM_FRQ_SET + DPWM_FRQ_LEVEL)
    {
        FiveSegModeEn = 1;
    }
    else if( gMainCmd.FreqReal < FIVEPWM_FRQ_SET)
    {
        FiveSegModeEn = 0;
    }
#else
    FiveSegModeEn = 0;
#endif
}

/************************************************************
	按照变频器不同的运行状态分别处理的主要程序
	说明：	
		1）上电后进入1状态；电压稳定后跳变到停机状态
		2）软件在任何时候判断是否欠压，欠压则进入1状态
		3）任何其它状态都由停机状态转换过去
		4）任何其它状态结束后都返回停机状态
************************************************************/
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

		case STATUS_SHORT_GND:	//上电对地短路判断状态
			RunCaseShortGnd();
			break;

        case OPEN_ACC:
        case HALF_OPEN_ACC:
            OpenLoopAccRun();
            break;

        default:				//停机状态：所有其它状态认为是该状态
			RunCaseStop();
			break;
	}	
}

/************************************************************
	变频器的上电处理过程：判断母线电压稳定，屏蔽上电缓冲电阻
判断依据：1）  母线电压大于欠压点
       && 2）母线电压没有再增加 
       && 3）持续200ms
		  4）判断母线电压稳定后再延时200ms，进入停机状态。
    2ms 状态机....
************************************************************/
void RunCaseLowPower(void)
{
	Uint uDCLowLimt;
	uDCLowLimt = gInvInfo.InvLowUDC + 100;//200;
	switch(gMainStatus.SubStep)
	{
		case 2:
			if((gUDC.uDCFilter > uDCLowLimt) && (gUDC.uDCFilter <= gLowPower.UDCOld))
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
        		if(gMainStatus.ErrorCode == ERROR_UV)
                {
                    gMainStatus.ErrorCode = 0;
		            if (gMainStatus.ErrFlag.bit.OvCurFlag == 1)     // 修改欠压后无法进入过流中断的错误
		            {
			            gMainStatus.ErrFlag.bit.OvCurFlag = 0;
			            EALLOW;
			            EPwm2Regs.TZCLR.bit.OST = 1;
			            EPwm3Regs.TZCLR.bit.OST = 1;
			            EPwm4Regs.TZCLR.bit.OST = 1;
			            /*
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
			            */
			            EPwm2Regs.TZCLR.bit.INT = 1;
			            EDIS;
		            }     
        		}

				gPhase.IMPhase = GetTime() << 28;	//上电后随机选择初始相位
			}
			break;

		default:
		    //	DisConnectRelay();
            gMainStatus.ErrorCode = ERROR_UV;				//错标志
        	gMainStatus.StatusWord.bit.LowUDC = 1;
			gMainStatus.StatusWord.bit.StartStop = 0;
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

//int RunSpeedValue = 0;
/************************************************************
	转速跟踪状态处理
	2ms, 状态机...
************************************************************/
void RunCaseSpeedCheck(void)
{
	long data;
	if ((gMainCmd.Command.bit.Start == FALSE) || // 停机命令
	    (gMainStatus.ErrorCode      != 0    ))	 // 故障
	{ 
		gMainStatus.RunStep = 3;
		gMainStatus.SubStep = 1;
		gMainStatus.StatusWord.bit.StartStop = 0;
		DisableDrive();
		return;
	}

	gMainStatus.StatusWord.bit.StartStop = 1;
	
	// 状态即，200ms, 然后进入到RUN 状态, gMainCmd.FreqSetApply 接下来从功能
	// 传递过来。。。。
	////////////////////////////////////////////////////////////////////////
	static int VoltCNT;
	if (pm_control_mode != 0)
	{
		VoltCNT++;
		if (VoltCNT > 100)
		{
            VoltCNT = 0;
            data = (long long)pm_est_lpf_omg1 * 2045L / gBasePar.FullFreq;
            if (labs(pm_est_lpf_omg1) <= 24030) // 15 HZ * 1602 = 24000
            {
#ifdef IPD_PERIOD_AUTO_DECT
                pm_check_ip_flag = 1;
#else
                pm_check_ip_flag = 0;
                DCStart_flag = 1;
#endif
                gIPMInitPos.SubStep = 1;
                data = 0;
            }
            gIPMInitPos.SubStep = 1;
            gMainCmd.FreqSetApply  = data;
            gMainCmd.FreqPreWs     = data;
            gMainCmd.FreqSet       = data;
            gMainCmd.FreqFeed      = data;
            gMainCmd.FreqDesired   = data;
            gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
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
            } else {
                gMainStatus.RunStep = STATUS_RUN;
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

//extern long pm_integral_q;
//int AngleSet;
void OpenLoopAccRun(void)
{
    typedef enum {
        McInit              = 0,
        McAlignRun          = 1,
        McRsIDRun           = 2,
        McOpenRun           = 3,
        McSpdChangeToClosed = 4,
        McSFinished         = 5
    } MCP_SPDSTATE_DEF;
    static MCP_SPDSTATE_DEF McSpdState;        // 全局变量在STOP 或其他状态可以清零操作
    static int IDRef;
    if ((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
        DisableDrive();
        IDRef = 0;
        McSpdState = McInit;
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        return;
    }

    switch (McSpdState) {
    case McInit:
        McSpdState = McAlignRun;
    break;
    case McAlignRun:
         if ((gMainCmd.Command.bit.StartDC == 1) ||
             (gMainCmd.Command.bit.StopDC  == 1))
         {
             gMainCmd.FreqPreWs    = 0;
             gMainCmd.FreqSetApply = 0;
             if (IDRef > DCBrakeCur) {
                 IDRef = DCBrakeCur;
             } else {
                 IDRef += 35;
             }
         } else {
             McSpdState = McOpenRun;
         }
    break;
    case McRsIDRun:
    break;
    case McOpenRun:
        if (gMainCmd.FreqReal > FRQ_OPEN_TO_CLOSE) { // 20HZ
#if 0
            if (IDRef > pm_min_curr + 10) {
                IDRef -= 15;
            } else if (IDRef < pm_min_curr - 10) {
                IDRef += 15;
            } else {
                IDRef = pm_min_curr;
                McSpdState = McSpdChangeToClosed;
            }
#else
            IDRef = pm_min_curr;
            McSpdState = McSpdChangeToClosed;
#endif
        } else {
            IDRef = DCBrakeCur;
        }
    break;
    case McSpdChangeToClosed: {
#ifdef FULL_OPEN_LOOP_MODE
        PMSVCInition();
        PMFluxWeakInit();

        OPenACCCNT++;
        if (OPenACCCNT > 10) {
            gMainStatus.RunStep = STATUS_RUN;
            McSpdState = McInit;
        }
#else
        gMainStatus.RunStep = STATUS_RUN;
        McSpdState = McInit;

        // pm_est_angel = (long)(gPhase.OutPhase - 8192) * 3217L >> 8;
        pm_est_angel = (long)(gPhase.OutPhase - pm_dq_angle) * 3217L >> 8;

        // 切入闭环的IQ 初始给定...
        pm_integral_speed = ((long)OPEN_LOOP_TOR_START << 8);
#endif
    }
    break;
    case McSFinished:
    break;
    }
//AngleSet = McSpdState;

    pm_ref_iq = 0;
    pm_ref_id = IDRef;
#if 0
static int EnableRecord;
if (gMainCmd.FreqReal >= FRQ_OPEN_TO_CLOSE) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,2);
IsRecordThisSample(
        gRatio,
        pm_est_angel,   // IV
        gMainCmd.FreqReal,
        pm_uq,//
        pm_dq_angle,
        gCurSamp.W,
        gPhase.OutPhase//gIAmpTheta.Theta//gPWM_CM
        );
}
#endif
}

/************************************************************
	正常运行过程处理
	运行状态机, 2ms...
************************************************************/
int AngleRs = 0;
void RunCaseRun(void)
{
    static int IDRef;
    static int IDRefCnt;

	if((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
	{
		DisableDrive();

		gMainStatus.RunStep = STATUS_STOP;
		gMainStatus.SubStep = 1;
		return;
	}
	gMainStatus.StatusWord.bit.StartStop = 1;

	if (pm_control_mode != 0)
	{
	    if ((gMainCmd.Command.bit.StartDC == 1) ||
	   	    (gMainCmd.Command.bit.StopDC  == 1))
	   	{
			gMainCmd.FreqPreWs    = 0;
			gMainCmd.FreqSetApply = 0;	

            // 200ms Ramp, 10KSF, 2048. while 2ms
			IDRefCnt += 20;
            if (IDRefCnt < 2048) {
                IDRef = ((long)DCBrakeCur * IDRefCnt) >> 11;
            } else {
                IDRefCnt = 2048;
            }


            if (gMainCmd.Command.bit.SecondDCPhase == 1) {
                // 100ms rade over to the target
                if (AngleRs <= 10922) {
                    AngleRs += 200;
                } else {
                    AngleRs = 10922;
                }
            } else {
                AngleRs = 0;
            }

            pm_ref_id = IDRef;
            pm_ref_iq = 0;
			return;
	   	}
	   IDRefCnt = 0;
	   AngleRs = 0;

	    // Enable drive 200MS 内，没有RUNCASE Iq = 0. 不启动...
       if ((pm_check_ip_flag == 0)&&(DCStart_flag == 0)) {
			speed_pi();
		}
		return;
	}

	// VF mode only
	if ((gMainCmd.Command.bit.StartDC == 1) ||
	    (gMainCmd.Command.bit.StopDC  == 1)) {
	} else if(pm_control_mode == PMSM_CONTROL_MODE_VF) {
		VFFreqCal();
		CalOutVotInVFStatus();
		gOutVolt.VoltApply = gOutVolt.Volt;

		if(gMainCmd.FreqSetApply != 0) {
			gMainCmd.FreqLast = gMainCmd.FreqSetApply;
		}
		if(gMainCmd.FreqLast > 0)		gOutVolt.VoltPhaseApply = 8192;
		else if(gMainCmd.FreqLast < 0)	gOutVolt.VoltPhaseApply = -8192;
	} else {
	}
}

void RunCaseShortGnd(void)
{

}

/************************************************************
	停机过程处理
************************************************************/
void RunCaseStop(void)
{
	DisableDrive();							//停机封锁输出

	PrepareParForRun();
	PMSVCInition();
#ifdef ENABLE_NEW_STATIC_OBS
	Est_Init();
#endif
	PMFluxWeakInit();
	gMainStatus.SubStep = 1;
	if(gMainStatus.StatusWord.bit.RunEnable != 3)	
	{
		return;								//等待零漂检测完成
	}

	gMainStatus.StatusWord.bit.ShortGndOver = 1;

	/////////////////////////////////////////
	if(gMainCmd.Command.bit.TunePart == 1)	//判断是否需要参数辨识
	{
		gMainStatus.RunStep = STATUS_GET_PAR;
		gMainStatus.SubStep = 1;		
		return;			
	}

	if ((gMainStatus.ErrorCode == ERROR_NONE) && (gMainCmd.Command.bit.Start == TRUE))
	{
		// 转速跟踪起动, 目前固定为该模式启动......
		// 注意逻辑，这里先开波，PWM，200ms 左右IPD，然后正常启动.
		// 如果这里禁止开波，启动过流，原因待查...
		// 开波时候有个电流尖，因为是4066 原因，没有开波的时候电流没有但4066 检测的有电平
		if (gMainCmd.Command.bit.SpeedSearch == TRUE)
		{
			gMainStatus.RunStep = STATUS_SPEED_CHECK;
			gMainStatus.SubStep = 1;
			EnableDrive();
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
	gMainStatus.StatusWord.bit.StartStop = 0;

	gMainCmd.FreqSetApply = 0;
	gMainCmd.FreqPreWs = 0;
	gMainCmd.FreqLast = 0;
	gMainCmd.FreqReal = 0;
	gMainCmd.SpeedFalg = 0x8000;
	
	gOutVolt.Volt = 0;
	gOutVolt.VoltApply = 0;

	if(gMainCmd.FreqDesired > 0)		//根据目标频率方向确定初始相位角
	{
		gOutVolt.VoltPhaseApply = 8192;
	} else {
		gOutVolt.VoltPhaseApply = -8192;
	}		
	gPhase.OutPhase = (int)(gPhase.IMPhase>>16) + gOutVolt.VoltPhaseApply;

	gRatio = 0;
	gCurSamp.U = 0;
	gCurSamp.V = 0;
	gCurSamp.W = 0;
	gCurSamp.UErr = 600;
	gCurSamp.VErr = 600;
	gIUVW.U = 0;
	gIUVW.V = 0;
	gIUVW.W = 0;
	gLineCur.CurPerShow = 0;
	gLineCur.CurBaseInv = 0;
	gLineCur.CurPer = 0;
	gLineCur.CurPerFilter = 0;
	gIMT.M = 0;
	gIMT.T = 0;

	gOvUdc.StepApply = 0;
    gOvUdc.LastStepApply = 0;
    gOvUdc.AccTimes = 0;
    gDCBrake.Time = 0;
    AngleRs = 0;

	gMainStatus.StatusWord.all &= 0xFE6A;
    gPWM.gZeroLengthPhase = ZERO_VECTOR_NONE;
    gIAmpTheta.ThetaFilter = gIAmpTheta.Theta;

	EALLOW;
	EPwm2Regs.TBPRD = gPWM.gPWMPrdApply;
	EPwm2Regs.CMPB  = 0;
	EPwm3Regs.TBPRD = gPWM.gPWMPrdApply;
	EPwm4Regs.TBPRD = gPWM.gPWMPrdApply;

	EPwm2Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
	EPwm3Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
	EPwm4Regs.CMPA.half.CMPA = gPWM.gPWMPrdApply/2;
	EDIS;
}

/************************************************************
	计算零漂程序
	4066，所以不能一直读取, main2ms call
************************************************************/
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
    //	if ((IsCurExcursionFinshied == 1) && (IsRelayCheckFinshed == 1)) {
    //		gMainStatus.StatusWord.bit.RunEnable = 3;
    //		return;
    //	}
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
		if ((abs(m_ErrIu) < 5120) && (abs(m_ErrIv) < 5120) && (abs(m_ErrIw) < 5120))
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

/*************************************************************
**************************************************************
周期中断：完成模拟量采样、电流计算、VC电流环控制等操作
**************************************************************
*************************************************************/
void ADCOverInterrupt()
{
	if ((pm_control_mode  == 0) &&
	    (pm_check_ip_flag == 0) &&
        (DCStart_flag    == 0))
	{
		ADCProcess();
		GetCurrentInfo();
   		GetUDCInfo();
		GetTemperatureInfo();
		ChangeCurrent();
   		CalRatioFromVot();
   		CalDeadBandComp();
	} else {
		pm_control_isr();
	}
}


/*************************************************************
**************************************************************
下溢中断：更新PWM处理,VF 调用。。。
          PMSM SVC 算法均在周期中断进行..
**************************************************************
*************************************************************/
void PWMZeroInterrupt()
{
	// 开环矢量模式下下溢出中断不会执行...
	// pm_check_ip_flag = 1,启动时候会有检测初始位置一会阶段...
	///////////////////////////////////////////////////////////
	if ((pm_control_mode  == 0) &&
	    (pm_check_ip_flag == 0))
	{
		SoftPWMProcess();
   		CalOutputPhase();

#ifdef NEW_CUR_SAMPLE_POINT_MODE
        OutPutPWMPhaseShift();
#endif
   		OutPutPWMVF();

        CBCLimitCurPrepare();                   //逐波限流保护程序
		BrakeResControl();
	}
}

/*************************************************************
	随机PWM处理，使载波周期和输出相位生效
*************************************************************/
void SoftPWMProcess(void)
{

}

/*************************************************************
	过流中断处理程序（可屏蔽中断，电平触发）
*************************************************************/
void HardWareErrorDeal()
{
	DisableDrive();

	gMainStatus.ErrFlag.bit.OvCurFlag = 1;
	if(gMainStatus.RunStep == STATUS_SHORT_GND) {
	} else if(gMainStatus.ErrorCode != ERROR_OC_HARDWARE) {
		gMainStatus.ErrorCode = ERROR_OC_HARDWARE;
	}

	EALLOW;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EPwm4Regs.TZCLR.bit.OST = 1;

	// 我看飞洋的加了这个，WHY??
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

/************************************************************
	制动电阻控制
************************************************************/
void BrakeResControl(void)
{
	return;
}

/*************************************************************
	软件故障处理
*************************************************************/
void SoftWareErrorDeal(void)					
{
	static int SoftwareOCCnt;

	// 欠压状态下不判断软件故障
	if (gMainStatus.RunStep == STATUS_LOW_POWER)
	{
		return;										
	}
	if (gMainStatus.LastErrorCode != gMainStatus.ErrorCode )
	{
	    gMainStatus.LastErrorCode = gMainStatus.ErrorCode;
		// 故障复位后运行角度发生变化 
        if (ERROR_NONE != gMainStatus.ErrorCode) {
			gPhase.IMPhase += 0x40000000L;
		}				           
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
			/*
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
			 */
			EPwm2Regs.TZCLR.bit.INT = 1;
			EDIS;
		}
	}

	int SoftwareOC;
	SoftwareOC = 6000;//SOFT_OC_POINT;

	/////////////////////////////////////////////////开始判断软件故障
	// 软件过流实际意义不大，但就当多一重保护而已吧.用硬件相同的过流点...
	if ((gLineCur.CurBaseInv 		 > SoftwareOC	) &&
		(gMainCmd.Command.bit.Start == TRUE			))
	{
		SoftwareOCCnt++;
		if (SoftwareOCCnt >= 10) {
			SoftwareOCCnt = 0;
			DisableDrive();
			gMainStatus.ErrorCode = ERROR_OC_SOFTWARE;
			gLineCur.ErrorShow = gLineCur.CurPer;
		}
	} else {
	    if (SoftwareOCCnt > 1) SoftwareOCCnt--;
	}

	if (gUDC.uDCFilter > gInvInfo.InvUpUDC)	//过压判断,使用大滤波电压
	{
		DisableDrive();
		gMainStatus.ErrorCode = ERROR_OV_ACC_SPEED;
	}
	else if (gUDC.uDCFilter < gInvInfo.InvLowUDC) //欠压判断,使用大滤波电压
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

    if (((pm_ref_speed - pm_est_omg) > 6000                 ) &&    // Delta > 10.0Hz
        (pm_ref_speed           > 14000                     ) &&    // Ref > 40Hz, that is 60
        (gMainStatus.ErrorCode != ERROR_LOSE_PHASE_OUTPUT   ) &&
        (pm_ref_iq              > (LimitHGlobal - 50))      ) {
        InstallCnt2++;
        // 1200, atual time is about 3.00S
        // so 8s means about
        if (InstallCnt2 > 500) {
            InstallCnt2 = 0;
//            DisableDrive();
//            gMainStatus.ErrorCode = ERROR_STALL;
        }
    } else {
        if (InstallCnt2 > 0)  InstallCnt2 --;
    }

    // 超速监测，速度超调过大。
    // 一般情况：
    // 1：没接电机，速度观察值不对。
    // 2：高速PI 太小，加速时候速度超调太多。（比如冰箱压机负载，重再启动可能会）
    // 视情况，可以屏蔽。
    if (pm_fast_freq != 0)
    {
        if((pm_est_omg > pm_fast_freq)||(pm_est_omg < -pm_fast_freq))
        {
            DisableDrive();
            gMainStatus.ErrorCode = ERROR_ERROR_KE;
        }
    }
}

/*************************************************************
	矢量控制的过压抑制功能，通过母线电压限制输出转矩(发电转矩)的最大值
*************************************************************/
void CalUdcLimitIT(void)
{
    static int LowToruqeCurLimit = 0;
    int m_Udc, ovGain;
    m_Udc = gUDC.uDC;

	ovGain = 30;//
    if (ovGain != 0)
    {
        gUdcLimitIt.UDCBakCnt++;
        if(gUdcLimitIt.UDCBakCnt >= 5)
        {
            gUdcLimitIt.UDCBakCnt = 0;
            gUdcLimitIt.UDCDeta = m_Udc - gUdcLimitIt.UDCBak;
            gUdcLimitIt.UDCBak = m_Udc;
            if(gUdcLimitIt.UDCDeta < 200)
            {
                gUdcLimitIt.UDCDeta = 0;
            }
        }
        if(m_Udc < gOvUdc.Limit - 100 - (ovGain<<2) - gUdcLimitIt.UDCDeta)
        {
            gUdcLimitIt.UdcPid.Total = (long)-gIMT.T<<16;
            gUdcLimitIt.UDCLimit = pm_curr_limitH;
        }
        else if((m_Udc < gOvUdc.Limit) && (gUdcLimitIt.FirstOvUdcFlag == 0))
        {
            //第一次进入过压危险区，力矩不再增加即可
            gUdcLimitIt.FirstOvUdcFlag = 1;
            gUdcLimitIt.UDCLimit = gUdcLimitIt.UdcPid.Total>>16;
        }
        else
        {
            gUdcLimitIt.UdcPid.KP = 10000;
            gUdcLimitIt.UdcPid.KI = 2000;
            gUdcLimitIt.UdcPid.KD = 0;
            gUdcLimitIt.UdcPid.QP = 0;
            gUdcLimitIt.UdcPid.QI = 0;
            gUdcLimitIt.UdcPid.Max = pm_curr_limitH;
            gUdcLimitIt.UdcPid.Min = LowToruqeCurLimit;

            LowToruqeCurLimit = 0;
            gUdcLimitIt.FirstOvUdcFlag2 = 0;

            gUdcLimitIt.UdcPid.Deta = gOvUdc.Limit - m_Udc;
            PID_MD((PID_STRUCT_MD *) &gUdcLimitIt.UdcPid);
            gUdcLimitIt.UDCLimit = gUdcLimitIt.UdcPid.Out>>16;
        }
    }
    else
    {
        if (pm_forbid_revtorq == 1) {
            gUdcLimitIt.UDCLimit = 0;
        } else {
            gUdcLimitIt.UDCLimit = pm_curr_limitH >> 1;;
        }
        gUdcLimitIt.UdcPid.Total = 0;
        gUdcLimitIt.FirstOvUdcFlag = 0;
    }
}

void pm_control_isr()
{
#if 0
    GetCurrentInfo();
    ChangeCurrent();

    if (gMainCmd.Command.bit.Start == TRUE)
    {
        // pm_check_ip_flag = PE.18 启动是否检测初始位置...
        // 使能后，开始检测位置，然后将该位置 0，
        if ((gMainCmd.Command.bit.StartDC == 1) ||
            (gMainCmd.Command.bit.StopDC  == 1))
        {
            gPhase.OutPhase = AngleRs;
            gPhase.IMPhase = (long)gPhase.OutPhase << 16;
            pm_est_angel = (long)gPhase.OutPhase * 3217L >> 8;
            if (pm_est_angel < 0) {
                pm_est_angel += 823549;
            }
        }

        if (pm_check_ip_flag == 0) {
            FluxPostionCalation();
            CurrentLoop();
            VolVectCalation();
            CalRatioFromVot();
            VolAngleCalation();
            CalDeadBandComp();

#ifdef NEW_CUR_SAMPLE_POINT_MODE
            OutPutPWMPhaseShift();
#endif
            OutPutPWMVF();
        } else {
            SynInitPosDetect();
        }
    } else {
    }
    CBCLimitCurPrepare();                   //逐波限流保护程序
#else
    GetCurrentInfo();
    ChangeCurrent();

    if(gMainCmd.Command.bit.Start == TRUE)
    {
        // pm_check_ip_flag = PE.18 启动是否检测初始位置...
        // 使能后，开始检测位置，然后将该位置 0，
        if ((gMainCmd.Command.bit.StartDC == 1)||
             (gMainCmd.Command.bit.StopDC == 1)) /****流制动状态****/
        {
             CalOutVoltInDCBrakeStatus();        //直流制动,计算输出电压
             CalRatioFromVot();

             // OutPhase -32767 ~ 32767, that is -180 ~ 180degree.
             // 60degree ~ 10922...
             if (gMainCmd.Command.bit.SecondDCPhase == 1) {
                 gPhase.OutPhase = 10922;
             } else {
                 gPhase.OutPhase = 0;
             }
             gPhase.IMPhase = 0;

             OutPutPWMVF();
             pm_est_angel = (long)gPhase.OutPhase * 3217L >> 8;
             if (pm_est_angel < 0) {
                 pm_est_angel += 823549;
             }
        } else {
            if (pm_check_ip_flag == 0) {
                FluxPostionCalation();
#ifdef ENABLE_NEW_STATIC_OBS
                Est_calc();
#endif
                CurrentLoop();
                VolVectCalation();
                CalRatioFromVot();
                VolAngleCalation();
                CalDeadBandComp();
                OutPutPWMVF();

            } else {
                SynInitPosDetect();
            }
        }
    } else {
    }
    CBCLimitCurPrepare();                   //逐波限流保护程序
#endif
#if 0
{
static int EnableRecord = 0;
if (gMainCmd.Command.bit.Start == TRUE && runTimeTicker > 998) {
    EnableRecord = 1;
}
if (EnableRecord == 1) {
ControlAllowRecord(1,1);
IsRecordThisSample(
        pm_ref_speed >> 4,//gMainCmd.Command.bit.Start,//phase,//gCurSamp.U,
        gMainStatus.RunStep,//frq,//gIMT.M,
        gCurSamp.U,//runCmd.all,//gIMT.T,
        runFlag.all,//pm_ref_id,
        runStatus,//pm_ref_iq,
        pm_ref_id,//((long)gLineCur.CurPer * 250) >> 12,//
        pm_ref_iq,//gDeadBand.CompV,
        pm_min_curr,//gCurSamp.W,//gIAmpTheta.Theta,
        pm_uq,//gCurSamp.U,
        gRatio);//pm_ref_iq);
}
}
#endif
}

// 2ms level tick...
void speed_pi()
{
    long data,data1,LimitH,LimitL;
    long pm_error_speed,pm_error_speed1;

    if (gMainStatus.RunStep == STATUS_SPEED_CHECK) {
        pm_ref_iq = 0;
        pm_ref_id = 0;
        return;
    }

    if((gMainCmd.Command.bit.StartDC == 1) ||
       (gMainCmd.Command.bit.StopDC  == 1)) {
        return;
    }

    if (gMainCmd.Command.bit.TunePart != 0) {
        pm_ref_iq = 0;
        return;
    }

    if (pm_control_mode == 0) {
        return;
    }

    pm_kp_speed_app = pm_kp_speed;
    pm_ki_speed_app = pm_ki_speed;
    pm_speed_lpf_k1 = pm_speed_lpf_k;

    LimitL = (long)pm_curr_limitL;
    LimitH = (long)pm_curr_limitH;

    // 如果转矩可正可负，则过压失速会起作用，需要限制母线电压，否则没有负力矩...
    if (pm_forbid_revtorq == 0) {
        LimitL = -gUdcLimitIt.UDCLimit;
    }
    LimitHGlobal = LimitH;

    // 弱磁模式...0,直接计算，1：自动调节
    if (gFluxWeak.Mode == 1) {
        if(LimitL < - gFluxWeak.IqMax) {
            LimitL = - gFluxWeak.IqMax;
        }
        if(LimitH > gFluxWeak.IqMax) {
            LimitH = gFluxWeak.IqMax;
        }
        if((LimitH < 400)&&(pm_curr_limitH>400)) {
            LimitH = 400;
        }
        if((LimitL > -400)&&(pm_curr_limitL<-400)) {
//            LimitL = -400;
        }

        if(LimitL > LimitH) {
            LimitL = LimitH;
        }
    }

    // 2037 = Q15*100/2*Pi*Q8;
    if (pm_control_mode == 2)
    {
#ifdef  FREQUENCY_CONTROL_01HZ
        if (gMainCmd.VCTorqueLim >= 0) {
            data = (long)gBasePar.UpFreq * 160L;
        } else {
            data = -(long)gBasePar.UpFreq * 160L;
        }
#else
        if (gMainCmd.VCTorqueLim >= 0) {
            data = (long)gBasePar.UpFreq * 16L;
        } else {
            data = -(long)gBasePar.UpFreq * 16L;
        }
#endif
        pm_ref_speed = data;
    } else {
        // 50Hz, then pm_ref_speed = 5000*16 = 80000
        // data = (long)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2037;
        // 注意，在FreSet 求取时候用的是2045， 这里也要取2045
        /////////////////////////////////////////////////////////////////
        data = (long long)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2039;//2045;
        pm_ref_speed = data;
        if (pm_ref_speed != 0) {
            data1 = (long)LowLimitFrequency * 16;
            if ((data >= 0) && (data < data1)) {
                data = data1;
                pm_ref_speed = data;
            } else if((data<0)&&(data>-data1)) {
                data = -data1;
                pm_ref_speed = data;
            }
        }
    }
#ifdef ENABLE_NEW_STATIC_OBS
    SpeedUsed = gEst.Speed_pu * gMotorInfo.Frequency >> 15;
    SpeedUsed <<= 4;
    pm_error_speed = pm_ref_speed - SpeedUsed;
#else
    pm_error_speed = pm_ref_speed - (long)pm_est_omg;
#endif

    // *16 about 50000, PI calculation...
    // 限制值不能太小，不然容易震荡。
    pm_error_speed = __IQsat(pm_error_speed,90000L,-90000L);//30HZ

    pm_error_speed1 = pm_error_speed * pm_ki_speed_app>>7;
    pm_integral_speed = pm_integral_speed + pm_error_speed1;
    pm_integral_speed = __IQsat(pm_integral_speed,LimitH * 256L,LimitL * 256L);
    data = (long)pm_integral_speed>>8;
    data1 = data + ((long long)pm_error_speed * (long)pm_kp_speed_app>>8);
    pm_ref_iq = __IQsat(data1,LimitH,LimitL);

    // 30 * 80 = 2400,pm_min_curr 通过线性变化，到24.38Hz...
    data = pm_min_curr;
    if (gFluxWeak.FluxWeakCurr < 0) {
        pm_ref_id = gFluxWeak.FluxWeakCurr;
    } else {
        // 起到一个滤波作用，即将pm_ref_id 逐渐过渡到data..
        pm_ref_id = pm_ref_id + (data>>1) - (pm_ref_id>>1);
        if (pm_ref_id <= 10) {
            pm_ref_id = 10;
        }
    }
    if (pm_ref_speed == 0) {
        pm_ref_id = 0;
    }

    // pm_iq_show = pm_ref_iq * 1000L>>12;
    // data = pm_ref_id * 1000>>12;
    // pm_id_show = pm_id_show + (data>>4) - (pm_id_show>>4);
    // if (gMainCmd.Command.bit.Start != TRUE) {
    //     return;
    // }
}

/************************************************************
    判断UVW三相是否处于逐波限流状态，并且设置标志
************************************************************/
void CBCLimitCurPrepare(void)
{
    EALLOW;
    if ((EPwm4Regs.TZFLG.bit.CBC == 1) ||
        (EPwm2Regs.TZFLG.bit.CBC == 1) ||
        (EPwm3Regs.TZFLG.bit.CBC == 1))
    {
        EPwm4Regs.TZCLR.bit.CBC = 1;
        EPwm2Regs.TZCLR.bit.CBC = 1;
        EPwm3Regs.TZCLR.bit.CBC = 1;        //逐波限流源相同,对于2和3可以不清除

        gCBCProtect.CbcTimes++;
        gCBCProtect.CbcFlag = 1;
    }
    EDIS;
}

void ChangeCurrent_2ms(void)
{
    Ulong   m_Long;
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
    gLineCur.CurPerFilter += gLineCur.CurPer - (gLineCur.CurPerFilter>>7);

    m_Long = (Ulong)gLineCur.CurPer * (Ulong)gMotorInfo.Current;
    gLineCur.CurBaseInv = m_Long/gInvInfo.InvCurrent;
    gLineCur.CurBaseInvFilter += gLineCur.CurBaseInv - (gLineCur.CurBaseInvFilter>>9);
    OutputLoseAdd();
}

#define MOTOR_Ls_d_H            0.0175
#define MOTOR_Ls_q_H            0.0225
#define MTPA_LUT_INDEX_LD_MAX   20
#define MTPA_LUT_INDEX_LQ_MAX   20

const float MTPA_Ld_tableData_H[MTPA_LUT_INDEX_LD_MAX + 1] =                   \
{                                                                              \
    MOTOR_Ls_d_H * 1.025f,       /* index =  0, Is = 0.00(A) */                \
    MOTOR_Ls_d_H * 1.020f,       /* index =  1, Is = 0.50(A) */                \
    MOTOR_Ls_d_H * 1.015f,       /* index =  2, Is = 1.00(A) */                \
    MOTOR_Ls_d_H * 1.010f,       /* index =  3, Is = 1.50(A) */                \
    MOTOR_Ls_d_H * 1.005f,       /* index =  4, Is = 2.00(A) */                \
    MOTOR_Ls_d_H * 1.000f,       /* index =  5, Is = 2.50(A) */                \
    MOTOR_Ls_d_H * 0.995f,       /* index =  6, Is = 3.00(A) */                \
    MOTOR_Ls_d_H * 0.990f,       /* index =  7, Is = 3.50(A) */                \
    MOTOR_Ls_d_H * 0.985f,       /* index =  8, Is = 4.00(A) */                \
    MOTOR_Ls_d_H * 0.980f,       /* index =  9, Is = 4.50(A) */                \
    MOTOR_Ls_d_H * 0.975f,       /* index = 10, Is = 5.00(A) */                \
    MOTOR_Ls_d_H * 0.970f,       /* index = 11, Is = 5.50(A) */                \
    MOTOR_Ls_d_H * 0.965f,       /* index = 12, Is = 6.00(A) */                \
    MOTOR_Ls_d_H * 0.960f,       /* index = 13, Is = 6.50(A) */                \
    MOTOR_Ls_d_H * 0.955f,       /* index = 14, Is = 7.00(A) */                \
    MOTOR_Ls_d_H * 0.950f,       /* index = 15, Is = 7.50(A) */                \
    MOTOR_Ls_d_H * 0.945f,       /* index = 16, Is = 8.00(A) */                \
    MOTOR_Ls_d_H * 0.940f,       /* index = 17, Is = 8.50(A) */                \
    MOTOR_Ls_d_H * 0.935f,       /* index = 18, Is = 9.00(A) */                \
    MOTOR_Ls_d_H * 0.930f,       /* index = 19, Is = 9.50(A) */                \
    MOTOR_Ls_d_H * 0.925f,       /* index = 20, Is = 10.0(A) */                \
};

//! \Defines the Lq array according to the specification of the motor,
//!          the user has to ask the motor manufacture to get the data
//!          the lab with FAST can't measure/identify the inductance changes
const float MTPA_Lq_tableData_H[MTPA_LUT_INDEX_LQ_MAX + 1] =               \
{                                                                              \
    MOTOR_Ls_q_H * 1.050f,        /* index =  0, Is = 0.00(A) */               \
    MOTOR_Ls_q_H * 1.040f,        /* index =  1, Is = 0.50(A) */               \
    MOTOR_Ls_q_H * 1.030f,        /* index =  2, Is = 1.00(A) */               \
    MOTOR_Ls_q_H * 1.020f,        /* index =  3, Is = 1.50(A) */               \
    MOTOR_Ls_q_H * 1.010f,        /* index =  4, Is = 2.00(A) */               \
    MOTOR_Ls_q_H * 1.000f,        /* index =  5, Is = 2.50(A) */               \
    MOTOR_Ls_q_H * 0.990f,        /* index =  6, Is = 3.00(A) */               \
    MOTOR_Ls_q_H * 0.980f,        /* index =  7, Is = 3.50(A) */               \
    MOTOR_Ls_q_H * 0.970f,        /* index =  8, Is = 4.00(A) */               \
    MOTOR_Ls_q_H * 0.960f,        /* index =  9, Is = 4.50(A) */               \
    MOTOR_Ls_q_H * 0.950f,        /* index = 10, Is = 5.00(A) */               \
    MOTOR_Ls_q_H * 0.940f,        /* index = 11, Is = 5.50(A) */               \
    MOTOR_Ls_q_H * 0.930f,        /* index = 12, Is = 6.00(A) */               \
    MOTOR_Ls_q_H * 0.920f,        /* index = 13, Is = 6.50(A) */               \
    MOTOR_Ls_q_H * 0.910f,        /* index = 14, Is = 7.00(A) */               \
    MOTOR_Ls_q_H * 0.900f,        /* index = 15, Is = 7.50(A) */               \
    MOTOR_Ls_q_H * 0.890f,        /* index = 16, Is = 8.00(A) */               \
    MOTOR_Ls_q_H * 0.880f,        /* index = 17, Is = 8.50(A) */               \
    MOTOR_Ls_q_H * 0.870f,        /* index = 18, Is = 9.00(A) */               \
    MOTOR_Ls_q_H * 0.860f,        /* index = 19, Is = 9.50(A) */               \
    MOTOR_Ls_q_H * 0.850f,        /* index = 20, Is = 10.0(A) */               \
};

/*
void MTPA(void)
{
    // Iraeted = 10.0A, 100 * 9270 >> 10 = 14.14A * 2^6
    // 9270 >> 10 = (1.4142 * 2^16 / 10) * 2^-10
    // 所以表达的意思是，实际电流峰值 再 放大 2^6 倍
    gPMFwCtrl.CurrCoef = (long)gMotorInfo.Current * 9270L>>10;  //Q6

    long imSet,data,data1,isSet;
    if(gPMFwIn.Ld < gPMFwIn.Lq)
    {
        // 10.0A, 4096, data = 4096 * 14.14 * 2^5 / 4096 = 14.14 * 2^6
        isSet = abs(gPMFwIn.IsAsrOut);                              //Q12
        data = gPMFwCtrl.CurrCoef * isSet>>12;                      //Q6 = 6+12-12



        // 没找到Lq 的来源，但Q20 估计是 H * 2^20. 举例,洗衣机电机，
        // LD = 18MH
        // LQ = 22MH
        // LD = 18*2^20/1000 = 18874
        // LQ = 22*2^20/1000 = 23068
        // data*data1, I*H * 8* 2^20 !!! 8*(Lq - Ld)^2*Iq^2*2^20!!!
        data = (gPMFwIn.Lq - gPMFwIn.Ld) * data>>13;                //Q13 = 20+6-13;
        data1 = data>>3;                                            //Q10
        data = data * data1;                                        //Q13+Q10 =8*Q20


        // data = data + data1 = Wb*Wb + 8*(LQ-LD)^2*iQ^2
        // 关键单位Bemf，WB = V/H/2PI.V iSRMS, ke=130*V/F,WB=2^20*KE/141!!!
        data1 = (long)gPMFwIn.Bemf * gPMFwIn.Bemf;                  //Q20
        data = data + data1;                                        //Q20
        data = qsqrt(data);                                         //Q10
        data = ((long)gPMFwIn.Bemf - data) * 1024L;                 //Q20;
        data1 = (long)(gPMFwIn.Lq - gPMFwIn.Ld) * 4;                //4*Q20;
        data = (long long)data * 1024L/data1;                       //Q10 = 20+10-20;
        data = data * 256L/gPMFwCtrl.CurrCoef;                      //Q12 标幺值 = 10+8-6
        imSet =  __IQsat(data,0,-isSet);
    }
    else
    {
        imSet = 0;
        return;
    }
}
*/

//long hkhkl[8];
void AllowSelectBottomOnly(unsigned short Selection)
{
    EALLOW;
    switch (Selection) {
        case 1:                         // U_BOTTOM_ONLY
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
//hkhkl[0]++;
            EPwm3Regs.AQCSFRC.all = 0x05;
            EPwm4Regs.AQCSFRC.all = 0x05;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
#endif
        break;
        case 2:                         // V_BOTTOM_ONLY
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x0A;
            EPwm3Regs.AQCSFRC.all = 0x06;
            EPwm4Regs.AQCSFRC.all = 0x0A;
#else
            EPwm2Regs.AQCSFRC.all = 0x05;
//hkhkl[1]++;
            EPwm3Regs.AQCSFRC.all = 0x09;
            EPwm4Regs.AQCSFRC.all = 0x05;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
#endif
        break;
        case 3:                         // W_BOTTOM_ONLY
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x0A;
            EPwm3Regs.AQCSFRC.all = 0x0A;
            EPwm4Regs.AQCSFRC.all = 0x06;
#else
            EPwm2Regs.AQCSFRC.all = 0x05;
//hkhkl[2]++;
            EPwm3Regs.AQCSFRC.all = 0x05;
            EPwm4Regs.AQCSFRC.all = 0x09;
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
#endif
        break;
        case 4:                         // Normal PWM Pattern
            EPwm2Regs.AQCSFRC.all = 0x00;
            EPwm3Regs.AQCSFRC.all = 0x00;
//hkhkl[3]++;
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
        case 5:                         // Turn all phase OFF
#ifndef POSTIVE_DRIVE_LOGIC
            EPwm2Regs.AQCSFRC.all = 0x0A;
            EPwm3Regs.AQCSFRC.all = 0x0A;
            EPwm4Regs.AQCSFRC.all = 0x0A;
#else
            EPwm2Regs.AQCSFRC.all = 0x05;
            EPwm3Regs.AQCSFRC.all = 0x05;
//hkhkl[4]++;
            EPwm4Regs.AQCSFRC.all = 0x05;
#endif
            EPwm2Regs.DBCTL.all     = 0;
            EPwm3Regs.DBCTL.all     = 0;
            EPwm4Regs.DBCTL.all     = 0;
        break;
        default:
//hkhkl[5]++;
        break;
    }
    EDIS;
//hkhkl[6]=Selection;
}

void ResetAdcANDPwm()
{
    InitSetPWM();
    InitSetAdc();
}
