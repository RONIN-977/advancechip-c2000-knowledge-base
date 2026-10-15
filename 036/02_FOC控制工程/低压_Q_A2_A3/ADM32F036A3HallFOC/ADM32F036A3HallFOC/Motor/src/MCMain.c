

#include "MotorInclude.h"
#include "Main.h"
#include "Record.h"
#include "Parameter.h"
#include "FuncUser.h"
#include "RunSrc.h"
#include "Reference.h"
#include "PreDriver.h"
#include "HallHandle.h"
#include "SpeedMeasHall.h"
#include "AngleMeasHall.h"

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


//int IsRelayCheckFinshed;
UDC_LIMIT_IT_STRUCT gUdcLimitIt;
long pm_fast_freq;
long LimitHGlobal = 4096;
int DCStart_flag = 0;
int IDFadeOverCnt;
int SHORT_GND_PERIOD = 5000;    // 10K SF

long speedApply;
long speedApplyNew;
//Uint32 speedApply;
//int SVCMODE = 1;//ONLY_FVC;//ONLY_SVC;
unsigned int TransToHall = 1;
unsigned int hallTuneEn = 0;

long TestSpeedPiVal[5];

/************************************************************
性能模块初始化程序：主循环前初始化性能部分的变量(所有等于0的变量无需初始化)
************************************************************/
void  InitForMotorApp(void)
{
    DisableDrive();
	TurnOffFan();

	gMainStatus.RunStep = STATUS_LOW_POWER;	//主步骤
	gMainStatus.SubStep = 1;				//辅步骤

	gMainStatus.LastErrorCode = gMainStatus.ErrorCode;

	gMainStatus.StatusWord.all = 0;
    gCBCProtect.EnableFlag = 1;             //默认启动逐波限流功能

	gADC.DelaySet = 100;

	gFcCal.FcBak = 80;
	gBasePar.FcSetApply = 80;

    gPWM.gPWMPrd = C_INIT_PRD;
    gPWM.gPWMPrdApply = SHORT_GND_PERIOD;

	pm_control_mode = PMSM_CONTROL_MODE_FOC;
    DCStart_flag = 0;

    Hall_Init();                // Hall初始化
}

/************************************************************
主程序的单2ms循环：用于执行电机控制程序
思路：数据输入->数据转换->控制算法->控制输出->自我保护
执行时间：不被中断打断的情况下约120us
************************************************************/
void SystemLeve2msMotor(void)
{
    CalHallTuneAngle();
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
	GetAIInfo();			// 0.36%
	GetTemperatureInfo();	// 0.74%
	ADCProcess();			// 1.05%
	calc_pm_coef();			// 22.1%
	GetUDCInfo();			// 0.9%
	GetIDCInfo();
	ChangeCurrent_2ms();
	PMSMSVC_2ms();
	PMCalcLineCurr();
}

/************************************************************
主程序0.5ms循环：用于执行需要快速刷新的程序，要求程序非常简短
************************************************************/
Uint32 TestValueArray[5];
extern Uint32 MotorOneElecCircleTime;
Uint16 testChangeSpeedApply = 1;
Uint16 TestDeltaValue;
Uint16 TestDeltaValue1;
Uint16 RsUnitComp = 1; // 定子电阻放大倍数
void SystemLeve05msMotor(void)
{
    if (TransToHall == 1)
    {
    static long LastSpeedApply = 0;
    static long SpeedApplyTemp1 = 0;
    long tempSpeed = 0;
    {
        static Uint16 stallCnt  = 0; // 0速时间计数
        static Uint16 isStall   = 0; // 0速标志
        static Uint16 inStall1 = 0; // 堵转标志
        static Uint16 ExitIsStallCnt= 0; // 退出0速的计数器

        const Uint16 STALL_CNT_TH = 250;       // 电机0速判断阈值

        tempSpeed = speedMeasHall.RealSpeed;
        if (speedMeasHall.RotateDir == 2) tempSpeed = -((long)speedMeasHall.RealSpeed);
        SpeedApplyTemp1 = (tempSpeed * 1602) >> 10;
        speedApply = SpeedApplyTemp1;
//        speedApply = (speedApply + LastSpeedApply * 15)>> 4;
        speedApply = (speedApply + LastSpeedApply * 11 + pm_est_omg * 20)>> 5;
//        speedApply = pm_est_omg;
//        speedApply = (speedApply + LastSpeedApply * 15)>> 4;
        if ((speedMeasHall.Speed == 0) &&
            (speedMeasHall.RealSpeed == 0))
        {
            if (stallCnt < 0xFFFF)
                stallCnt++;

            if (stallCnt >= STALL_CNT_TH)
                isStall = 1;

            if ((gLineCur.CurPerFilter >> 5) > 1048)
            {
                inStall1 = 1;
            }
            else
            {
                inStall1 = 0;
            }
        }
        else
        {
            stallCnt = 0;
            isStall  = 0;
        }

        //电机0速
        if (isStall)
        {
            ExitIsStallCnt = 0;
            if (inStall1) // 堵转
            {
                speedApply = 0;
                RsUnitComp = 1;
            }
            else         // 低速
            {
                speedApply = pm_est_omg;
                RsUnitComp = 3;
            }
        }
        else
        {
            if (ExitIsStallCnt < 1000)
            {
                ExitIsStallCnt++;
            }
            else
            {
                RsUnitComp = 1;
            }
        }

        LastSpeedApply = speedApply;
    }
//    speed_pi();

#if 1
static int EnableRecord;
static Uint32 RecordCnt;
//if (runCmd.bit.common == 1) {
//    RecordCnt++;
//    if (RecordCnt > 2100)
//    {
//        RecordCnt = 2100;
//        EnableRecord = 1;
//    }
//}
//if (testChangeSpeedApply == 1)
//{
//    EnableRecord = 1;
//}
if (EnableRecord == 1) {
ControlAllowRecord(1,2);
IsRecordThisSample(
        angleMeasHall.Angle,
        (long)pm_est_angel * 1304 >> 14,
        hall.HallState,
        pm_ref_id,
        pm_ref_iq,
        0,
        0);
}
#endif
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

    // gInvInfo.BaseUdc = ((long)MOTOR_RATED_VOL*14480)>>10;
    gInvInfo.BaseUdc = ((long)MOTOR_RATED_VOL * 960) >> 6;

	// 过压失速点 351V = gOvUdc.Limit...
	gOvUdc.Limit = OVER_DC_VOL_LIMIT;

	// 设定变频器额定电流，实际产品可以考虑将它设置为固定值，比电机额定电流一致
	gInvInfo.InvCurrent = MOTOR_RATED_CUR;
	gInvInfo.InvOlCurrent = gInvInfo.InvCurrent;

	// gUDC.Coff = 9500, 代表3.3V ~ 950V and with 0.1 DP
	// 换上2802x 后直接将gUDC.Coff 改成一个固定值即可。
    // 443.3V, then 4433/
	gUDC.Coff = MAX_DC_VOL_3_3V;

	/*********************************************************/
	//设置电机电流太小的处理
	m_UData = gInvInfo.InvCurrent >> 2;
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
#ifdef  FREQUENCY_CONTROL_01HZ
    gBasePar.FullFreq = ((long)gBasePar.MaxFreq * 10) + 2000;
#else
    gBasePar.FullFreq = gBasePar.MaxFreq + 2000;
#endif

	gMotorInfo.FreqPer = ((Ulong)gMotorInfo.Frequency <<15) / gBasePar.FullFreq;

	//计算用实际值表示的运行频率
	m_UData = abs(gMainCmd.FreqSetApply);
	gMainCmd.FreqReal = ((Ulong)m_UData * (Ulong)gBasePar.FullFreq) >> 15;

    speedMeasHall.K1 = ((unsigned long)gBasePar.FcSetApply*100)<<10;
//    speedMeasHall.K2 = (unsigned long)gBasePar.FcSetApply*100;    // gBasePar.FcSetApply的单位是100Hz

//  speedMeasHall.MintSpeedCnt = (Uint32)((float)gBasePar.FcSetApply*16.666667/MeasurableSpeedMin);
    Uint MinSpeedCntTemp1 = gBasePar.FcSetApply * 100 / 6;
    MinSpeedCntTemp1 /= MeasurableSpeedMin;
    speedMeasHall.MintSpeedCnt = MinSpeedCntTemp1;

//  angleMeasHall.K1 = (unsigned long)(1024.0*327.67/(float)gBasePar.FcSetApply);    // 为提高检测精度，将这个系数乘了2^10.
    unsigned long AngleK1Temp = 33554432UL / gBasePar.FcSetApply;
    AngleK1Temp = AngleK1Temp / 100;
    angleMeasHall.K1 = AngleK1Temp;

//  gHallTune.AngleStep = 335544.32/(float)gBasePar.FcSetApply;     // 33554432=2^25
    unsigned long AngleStepTemp = 33554432UL / gBasePar.FcSetApply;
    AngleStepTemp = AngleStepTemp/100;
    gHallTune.AngleStep = AngleStepTemp;
}

/************************************************************
 计算载波频率：输入gBasePar.FcSet，输出gBasePar.FcSetApply
 2ms Level...
************************************************************/
void CalCarrierWaveFreq(void)
{
	Uint	m_TempLim,m_MaxFc,m_MinFc;
	long    data,data1,freqs,freqs1,fcSet;

    fcSet = gBasePar.FcSet;   //l0310

#ifdef DSP_CLOCK100
   	data = (500000L * 512);
#elif   DSP_CLOCK60
    data = (300000L * 512);
#else
    data = (200000L * 512);
#endif  
	
	if(gBasePar.FcSet <= 20) {
    	data1 = 7680L;
	} else {
		data1 = __IQsat(gBasePar.FcSet,200,20);

		// Auto tune, then Period = 3333, 6.0K SF
		data1 = data1 * 384L;
	}
	
	if (gMainStatus.RunStep != STATUS_RUN) {
		data = __IQsat(gBasePar.FcSet,200,20);
		gBasePar.FcSetApply = data;
		return;
	}

	if (pm_control_mode != 0) {
	   if ((gMainStatus.RunStep == STATUS_SPEED_CHECK)
	   		||(gMainCmd.Command.bit.StartDC == 1) 
	   		||(gMainCmd.Command.bit.StopDC  == 1))
	   {
			data = 180;//100;
			gBasePar.FcSetApply = data;
			return;
	   }
	  
	   data1 = pm_bem_kf;
	   if (data1 < 50) {
	       data1 = 50;
	   } 

	   // 102, then 4000000 / 102 = 39215
	   // means 24.38Hz,水泵电机大概5左右，所以 4000000 / 5 = 800000， 对用50HZ. 偏大
	   // 改成200000 / ke， 这样KE=5时候，50HZ 开始励磁减小到0
	   ///////////////////////////////////////////////////////////////////////////
	   if (MotorTestData5 == 1) {
	       freqs = 4000000L/data1;
	   } else {
	       freqs = 400000L/data1;
	   }

	   if (pm_control_mode == 1) {
		   data = pm_ref_speed;
	   } else {
		   data = pm_est_lpf_omg;
	   }
	   freqs1 = labs(data);	   

#define SF_INDEPAND_OF_FREQUENCY	1
#ifdef  SF_INDEPAND_OF_FREQUENCY
	   fcSet = gBasePar.FcSet;
#else
	   // 低速载频，默认FE。10 = 3.0KHz
	   if(freqs1 < 32767L)
	   {
		   fcSet = SF_LOW_SPEED;
	   } else if(freqs1 >= 65536L) {
	   	   fcSet = gBasePar.FcSet;
	   } else {
		   data = freqs1 - 32767L;
		   data = data * ((long)gBasePar.FcSet - (long)SF_LOW_SPEED)>>15;
	       fcSet = data + SF_LOW_SPEED;
	   }
	   if (SF_LOW_SPEED >= gBasePar.FcSet) {
			fcSet = gBasePar.FcSet;
	   }
#endif
	   
	   // 计算Id 的给定大小....
	   // 24.38Hz 以下线性变化...
	   //////////////////////////
	   if(freqs1 > 2*freqs) {
			data1 = 0;
	   } else if(freqs1 == 0) {
	  		pm_min_curr = 0;
	   } else {
	   		data = MIN_CUR_LOW_SPEED * 80;
            if (MotorTestData5 == 1) {
                if(pm_bem_kf >= 4000)
                {
                    data = data * 3000L/pm_bem_kf;
                }
            } else {
                if(pm_bem_kf >= 400)
                {
                    data = data * 300L/pm_bem_kf;
                }
            }
			data1 = data * freqs1/freqs;
	   		data1 = data - (data1>>1);
	   		data1 = __IQsat(data1,data,0);
	   }
	   // 数字设定转矩电流值，做限流用...
	   data = (long)TORQUE_CUR_LIMIT * 4096L/100;
	   data = data * data - (long)pm_ref_iq * pm_ref_iq;
	   if (data < 0) {
		   data = 0;
	   }
	   data = qsqrt(data);
	   if(data1 > data) {
		   data1 = data;
	   }
	   pm_min_curr = data1;
	}

	gFcCal.Time++;
	gFcCal.Cnt += gBasePar.FcSetApply;
	if (gFcCal.Cnt < 100) {
		return;
	}
	gFcCal.Cnt = 0;

	// 此处作FC和温度关系的计算
	m_TempLim = 95;
	if((gComPar.SubCommand.bit.VarFcByTem != 0) &&  //载频随温度调整功能选择
	   (gTemperature.Temp > (m_TempLim-5)))		    //温度大于（限制值-5）度
	{
		if(gFcCal.Time >= 2500)						//每5秒调整一次  l0310
		{
			gFcCal.Time = 0;
			if(gTemperature.Temp <= m_TempLim)		//载波频率升
			{
				gFcCal.FcBak += 5;
			} else if(gTemperature.Temp >= (m_TempLim+5)) {
				gFcCal.FcBak -= 5;
				gFcCal.FcBak = (gFcCal.FcBak<20)?20:gFcCal.FcBak;
			}
		}
	} else {
		gFcCal.Time = 0;
		gFcCal.FcBak = fcSet; 
	}
    gFcCal.FcBak = (gFcCal.FcBak>fcSet)?fcSet:gFcCal.FcBak;

	// 开始作FC的最大和最小限制
	m_MinFc = 20;
	m_MaxFc = 200;
	gFcCal.FcBak = __IQsat(gFcCal.FcBak,m_MaxFc,m_MinFc);

	// 开始调整载波频率，每40ms调整0.1KHz。
	if (gFcCal.FcBak > gBasePar.FcSetApply) {
		gBasePar.FcSetApply++;
	} else if(gFcCal.FcBak < gBasePar.FcSetApply) {
		gBasePar.FcSetApply--;
	}

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
	uDCLowLimt = gInvInfo.InvLowUDC + 10;
	switch (gMainStatus.SubStep)
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

				gPhase.IMPhase = GetTime() << 28;	//上电后随机选择初始相位
				//IsRelayCheckFinshed = 1;
			}
			break;

		default:
			DisConnectRelay();	
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

//int RunSpeedValue = 0;
/************************************************************
	转速跟踪状态处理
	2ms, 状态机...
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
	
	// 状态即，200ms, 然后进入到RUN 状态, gMainCmd.FreqSetApply 接下来从功能
	// 传递过来。。。。
	////////////////////////////////////////////////////////////////////////
	if (pm_control_mode != 0)
	{
		gFeisu.VoltCNT++;
		if(gFeisu.VoltCNT > 1)
		{
			gFeisu.VoltCNT = 0;
			data = (long long)pm_est_lpf_omg1 * 2045L / gBasePar.FullFreq;
            if ((pm_est_lpf_omg1 <= 24020) && (pm_est_lpf_omg1 >= (-24020)))
			{
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
/*
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
                        gMainStatus.RunStep = STATUS_RUN;
                        gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
//SpeedData[14]++;
//SpeedData[15] = gMainCmd.FreqSetApply;
//SpeedData[16] = pm_est_lpf_omg1;
//SpeedData[17] = frqAimTmp0;
//SpeedData[18] = pm_ref_speed;
                    }
                }
*/
                gMainStatus.RunStep = STATUS_RUN;
                gMainStatus.StatusWord.bit.SpeedSearchOver = 1;
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

extern long pm_integral_q;
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
        pmsvc_init();
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

long RsTuneData[35];
int RsTune;
int AngleRs;
/************************************************************
	正常运行过程处理
	运行状态机, 2ms...
************************************************************/
void RunCaseRun(void)
{
    static int IDRef;
    if((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
        DisableDrive();

        IDRef = 0;
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        return;
    }

    if (pm_control_mode != 0)
    {
        if ((gMainCmd.Command.bit.StartDC == 1) ||
            (gMainCmd.Command.bit.StopDC  == 1))
        {
#ifndef OPEN_LOOP_START
           // 3100, 2ms, 50*2 = 100ms
           // 25 * 100ms = 2500
           if (IDRef > DCBrakeCur) {
               IDRef = DCBrakeCur;
           } else {
               IDRef += 3;
           }
           pm_ref_id = IDRef;
           pm_ref_iq = 0;
#endif

           gMainCmd.FreqPreWs    = 0;
           gMainCmd.FreqSetApply = 0;
           return;
        }

        // Enable drive 200MS 内，没有RUNCASE Iq = 0. 不启动...
        if ((pm_check_ip_flag == 0)&&(DCStart_flag == 0)) {
            speed_pi();
        }
        return;
     }

	// VF mode only
	if (gMainCmd.FreqSetApply != 0) {
		gFeisu.SpeedLast = gMainCmd.FreqSetApply;
	}

	if ((gMainCmd.Command.bit.StartDC == 1) ||
	    (gMainCmd.Command.bit.StopDC  == 1)) {
	} else if (pm_control_mode == PMSM_CONTROL_MODE_VF) {
		VFFreqCal();
		CalOutVotInVFStatus();
		gOutVolt.VoltApply = gOutVolt.Volt;

		if (gMainCmd.FreqSetApply != 0) {
			gMainCmd.FreqLast = gMainCmd.FreqSetApply;
		}
		if(gMainCmd.FreqLast > 0)		gOutVolt.VoltPhaseApply = 8192;
		else if(gMainCmd.FreqLast < 0)	gOutVolt.VoltPhaseApply = -8192;
	} else {
	}
}

//long RunCaseShortGndData[8];
/************************************************************
    上电对地短路判断
    Alpha 底座采样的是V W，
    汇川 的底座采样的是U V，所以之前用U 相做对地保护。这里改成V相。
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
#ifndef CUT_DOWN_VER_HARDWARE
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
        if (BrakeLowBridgeStateCnt > 1000) {
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

#ifndef CUT_DOWN_VER_HARDWARE
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
//    HallStatusCheck();
	PrepareParForRun();
	pmsvc_init();
	PMFluxWeakInit();
	gMainStatus.SubStep = 1;
	if(gMainStatus.StatusWord.bit.RunEnable != 3)	
	{
		return;								//等待零漂检测完成
	}
/*
    if (SVCMODE == SVC_WITH_HALL) {
        if (hall.NOHallFitted == 1) {
            TransToHall = 0;
        } else {
            TransToHall = 2;
            pm_est_angel = (long)angleMeasHall.Angle * 3217L >> 8;
        }
    } else if (SVCMODE == ONLY_SVC) {
        TransToHall = 0;
    } else {
        TransToHall = 1;
    }
*/
#ifdef SHORT_GND_TEST
    if (gMainStatus.StatusWord.bit.ShortGndOver == 0)
    {
        gMainStatus.RunStep = STATUS_SHORT_GND;
        gMainStatus.SubStep = 1;
        return;
    }
#else
    gMainStatus.StatusWord.bit.ShortGndOver = 1;
#endif

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
            // 272, / 16 = 17
            // 20*3 = 60
            if (VolOffset < 7000) {
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
/* ------------------------------------------------------------
------------------------------------------------------------- */
            } else {
                gMainStatus.RunStep = STATUS_SPEED_CHECK;
            }
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
//	gMainStatus.StatusWord.bit.StartStop = 0;

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
    gDCBrake.PID.Total = 0;

	gMainStatus.StatusWord.all &= 0xFE6A;
    gIAmpTheta.ThetaFilter = gIAmpTheta.Theta;

	EALLOW;
	EPwm2Regs.TBPRD = gPWM.gPWMPrdApply;
	EPwm3Regs.TBPRD = gPWM.gPWMPrdApply;
	EPwm4Regs.TBPRD = gPWM.gPWMPrdApply;
    EPwm5Regs.TBPRD = gPWM.gPWMPrdApply;

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
#ifdef FREQUENCY_CONTROL_01HZ
    pm_fast_freq = (data * 16) / 10;
#else
    pm_fast_freq = (data * 16) / 100;
#endif
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
	if ((pm_control_mode  == 0) &&
	    (pm_check_ip_flag == 0) &&
	    (DCStart_flag == 0))
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
        SendPWM();
	} else {
		pm_control_isr();
	}
}

/*************************************************************
	随机PWM处理，使载波周期和输出相位生效
*************************************************************/
void SoftPWMProcess(void)
{
    // 不同场景下可能摆动的范围有区别。
}

long OCData[6];
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
OCData[4]++;
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
		gUDC.uDCBigFilter = gUDC.uDCFilter;
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
	// 2.95A RMS
	// 4066 会导致不运行电机时候，基准较小电流值不对...
#ifdef MOTOR_TYPE_DISHWASH
	if ((gLineCur.CurBaseInv 		> 6100) &&
		(gMainCmd.Command.bit.Start == TRUE))
#else
	if ((gLineCur.CurBaseInv 		 > SOFT_OC_POINT) &&
		(gMainCmd.Command.bit.Start == TRUE		 ))
#endif
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
    if ((pm_ref_speed           > 80000                     ) &&    // Ref > 50Hz,
        (pm_est_omg             < 60000                     ) &&    // omg < 37Hz,
        (gMainStatus.ErrorCode != ERROR_LOSE_PHASE_OUTPUT   ) &&
        (gMainCmd.Command.bit.StartDC               != 1    ) &&
        (pm_ref_iq              > (LimitHGlobal - 50        ))) {
        InstallCnt2++;
        if (InstallCnt2 > 250) {
            InstallCnt2 = 0;
//            DisableDrive();
//            gMainStatus.ErrorCode = ERROR_STALL;
        }
    } else {
        if (InstallCnt2 > 0)  InstallCnt2 -= 5;
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
    static int InstallCnt3;
    if ((pm_ref_speed                   > 130000) &&     // Ref > 80Hz,
        (pm_est_omg                     > 130000) &&     // omg > 80Hz,
        ((gLineCur.CurPerFilter >> 5)   < 800   ) &&     // 1 / 5 额定值
        (gLineCur.CurPer                > 120   ) &&     // 接了电机，是空转. 不接电机平均50以下
        (gMainCmd.Command.bit.StartDC  != 1     )) {
        InstallCnt3++;
        if (InstallCnt3 > 1000) {
            InstallCnt3 = 0;
            //DisableDrive();
            //gMainStatus.ErrorCode = ERROR_NO_LOAD;
        }
    } else {
        if (InstallCnt3 > 0)  InstallCnt3 -= 3;
    }

}

int MaxAllowUdcLimit = 160;
int MidAllowUdcLimit = 155;
/*************************************************************
	矢量控制的过压抑制功能，通过母线电压限制输出转矩(发电转矩)的最大值
*************************************************************/
void CalUdcLimitIT(void)
{
    static int LowToruqeCurLimit = 0;
    int m_Udc;
    m_Udc = gUDC.uDC;

	gVFPar.ovGain = 0;//gVFPar.VFOverUDCCoff;
    if (gVFPar.ovGain != 0)
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
        if(m_Udc < gOvUdc.Limit - (gVFPar.ovGain<<2) - gUdcLimitIt.UDCDeta)
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
            gUdcLimitIt.UdcPid.KP = 10000 + 200*gVFPar.ovGain;
            gUdcLimitIt.UdcPid.KI = 2000 + 40*gVFPar.ovGain;
            gUdcLimitIt.UdcPid.KD = 0;
            gUdcLimitIt.UdcPid.QP = 0;
            gUdcLimitIt.UdcPid.QI = 0;
            gUdcLimitIt.UdcPid.Max = pm_curr_limitH;
            gUdcLimitIt.UdcPid.Min = LowToruqeCurLimit;

            if (0) {
                if (m_Udc > MaxAllowUdcLimit) {
                    if (gUdcLimitIt.FirstOvUdcFlag2 == 0) {
                        gUdcLimitIt.FirstOvUdcFlag2 = 1;
                        gUdcLimitIt.UdcPid.Total = 0;
                    }
                    LowToruqeCurLimit = -(pm_curr_limitH >> 1);
                } else if ((m_Udc > MidAllowUdcLimit) && (gUdcLimitIt.UdcPid.Total > 0)) {
                    gUdcLimitIt.FirstOvUdcFlag2 = 1;
                    gUdcLimitIt.UdcPid.Total = 0;
                } else {
                    LowToruqeCurLimit = 0;
                    if (m_Udc < gOvUdc.Limit) {
                        gUdcLimitIt.FirstOvUdcFlag2 = 0;
                    }
                }
            } else {
                LowToruqeCurLimit = 0;
                gUdcLimitIt.FirstOvUdcFlag2 = 0;
            }

            gUdcLimitIt.UdcPid.Deta = gOvUdc.Limit - m_Udc;
            PID2((PID_STRUCT_2 *) &gUdcLimitIt.UdcPid);
            gUdcLimitIt.UDCLimit = gUdcLimitIt.UdcPid.Out>>16;
        }
    }
    else
    {
        if (pm_forbid_revtorq == 1) {
            gUdcLimitIt.UDCLimit = 0;
        } else {
            gUdcLimitIt.UDCLimit = pm_curr_limitH;// >> 1;;
        }
        gUdcLimitIt.UdcPid.Total = 0;
        gUdcLimitIt.FirstOvUdcFlag = 0;
    }
}

//extern long pm_est_bem;
//extern Uint32 runTimeTicker;
long Id_ref = 1500;
long IdRefTemp = 0;
void pm_control_isr()
{
    RecordHallAngle();
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    SVPWM_1ShuntGetPhaseCurrent();
#else
    SVPWM_3ShuntGetPhaseCurrent();
#endif
    ChangeCurrent();

    if (gMainCmd.Command.bit.Start == TRUE)
    {//gMainCmd.Command.bit.StartDC = 0;
        // pm_check_ip_flag = PE.18 启动是否检测初始位置...
        // 使能后，开始检测位置，然后将该位置 0，
        if ((gMainCmd.Command.bit.StartDC == 1) ||
            (gMainCmd.Command.bit.StopDC  == 1)) /****流制动状态****/
        {
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
        }
        if (pm_check_ip_flag == 0) {
            if (hallTuneEn == 1)
            {
                if (IdRefTemp < Id_ref) {
                    IdRefTemp++;
                } else {
                    IdRefTemp = Id_ref;
                }
                pm_ref_id = IdRefTemp;
                pm_ref_iq = 0;
            }

            if (1) {
                FluxPostionCalation();
            }
            CurrentLoop();
            VolVectCalation();
            CalRatioFromVot();
//            if (TransToHall != 1) {
                VolAngleCalation();
//            }
            CalDeadBandComp();
            OutPutPWMVF();
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifdef LOW_NOISE_PWM_MODE
        OutPutPWMPhaseShiftForLowNoiseFullMode();
#else
        OutPutPWMPhaseShift();
#endif
#endif
            SendPWM();
        } else {
            SynInitPosDetect();
        }
    } else {
        RunBoostState = 0;
        ResetIndex();
        //        RecordEnable = 0;
    }
//    CBCLimitCurPrepare();                   //逐波限流保护程序
#if 0
static int RecordEnable;
//if (gMainCmd.Command.bit.Start == 1 && gMainCmd.Command.bit.StartDC == 1) {
if (runTimeTicker >= 499) {
    RecordEnable = 1;
}
if (RecordEnable == 1) {// && gMainCmd.Command.bit.Start == 1) {
ControlAllowRecord(1,1);    // 1ms * 128 = 128ms
IsRecordThisSample(
        pm_ref_id,
        pm_ref_iq,
        gCurSamp.W,
        gRatio,//gPWM.U,//gPWM.U,//AdcResult.ADCRESULT3,   // IU
        gLineCur.CurPer,//
        gPhase.OutPhase,
        gDCBrake.PID.Total >> 16//
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
int iqset;
// 2ms level tick...
void speed_pi()
{
    long data,data1,LimitH,LimitL;
    long pm_error_speed,pm_error_speed1;
#if 1
    long pm_omg_temp;
    static long pm_omg_acc_temp;
    pm_omg_temp = pm_est_omg_1 * 10L / 1605;  // 1000.00HZ, then
    pm_omg_acc_temp = (((pm_omg_temp << 16) - pm_omg_acc_temp) >> 6) + pm_omg_acc_temp;
    pm_omg_show  = (pm_omg_acc_temp + 0x8000) >> 16;
#endif

    if (gMainStatus.RunStep == STATUS_SPEED_CHECK) {
        pm_ref_iq = 0;
        pm_ref_id = 0;
        return;
    }

    if ((gMainCmd.Command.bit.StartDC == 1) ||
        (gMainCmd.Command.bit.StopDC  == 1)) {
        return;
    }

    //    if (gMainCmd.Command.bit.TunePart != 0) {
    //        pm_ref_iq = 0;
    //        return;
    //    }
    ///////////////////////////////////////////////
    if (pm_control_mode == 0) {
        return;
    }

    pm_kp_speed_app = pm_kp_speed;
    pm_ki_speed_app = pm_ki_speed;
    pm_speed_lpf_k1 = pm_speed_lpf_k;

    if (gMainCmd.FreqSetApply == 0) {
        LimitL = (long)pm_curr_limitL*7>>3;
        LimitH = (long)pm_curr_limitH*7>>3;
    } else {
        LimitL = (long)pm_curr_limitL;
        LimitH = (long)pm_curr_limitH;
    }

#if 0
#else
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
            LimitL = -400;
        }
        if(LimitL > LimitH) {
            LimitL = LimitH;
        }
    }
#endif

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
        data = (long long)gMainCmd.FreqSetApply * gBasePar.FullFreq / 2040;
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

    if (TransToHall == 1) {
//        if (testChangeSpeedApply == 0)
//        {
//            pm_error_speed = pm_ref_speed - (long)pm_est_omg;
//        }
//        else
//        {
            pm_error_speed = pm_ref_speed - speedApply;
//        }
    } else {
        pm_error_speed = pm_ref_speed - (long)pm_est_omg;
    }
TestSpeedPiVal[0] = pm_error_speed;
    // *16 about 50000, PI calculation...
    // 限制值不能太小，不然容易震荡。
    pm_error_speed = __IQsat(pm_error_speed,50000L,-50000L);//30HZ

    pm_error_speed1 = pm_error_speed * pm_ki_speed_app>>7;
    pm_integral_speed = pm_integral_speed + pm_error_speed1;
    pm_integral_speed = __IQsat(pm_integral_speed,LimitH * 256L,LimitL * 256L);
    data = (long)pm_integral_speed>>8;
    data1 = data + ((long long)pm_error_speed * (long)pm_kp_speed_app>>8);
    pm_ref_iq = __IQsat(data1,LimitH,LimitL);

    //POE_Limit(SetPower,SetPowerKp,SetPowerKi);
if (iqset != 0)
pm_ref_iq = iqset;

    // 30 * 80 = 2400,pm_min_curr 通过线性变化，到24.38Hz...
    data = pm_min_curr;
    if (0) {//gFluxWeak.FluxWeakCurr < 0) {
        pm_ref_id = gFluxWeak.FluxWeakCurr;
    } else {
        if (TransToHall != 0) {
            pm_ref_id = 10;
        } else {
            // 起到一个滤波作用，即将pm_ref_id 逐渐过渡到data..
            pm_ref_id = pm_ref_id + (data>>3) - (pm_ref_id>>3);
            if (pm_ref_id <= 10) {
                pm_ref_id = 10;
            }
        }
    }
    if (pm_ref_speed == 0) {
        pm_ref_id = 0;
    }

#if 0
    if (gUDC.uDC > 3900) {
        pm_ref_id = 4000;
    }
#elif 0
    static int IdRef = 128;
    if (gUDC.uDC > 3850) {
        IdRef += 20;
        if (IdRef > 4000) {
            IdRef = 4000;
        }
    } else if (gUDC.uDC < 3800) {
        IdRef -= 10;
        if (IdRef < 128) {
            IdRef = 128;
        }
    }
    pm_ref_id = IdRef;
#endif

    pm_iq_show = pm_ref_iq * 1000L>>12;
    if (gMainCmd.Command.bit.Start != TRUE) {
        return;
    }
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
    gLineCur.CurPerFilter += gLineCur.CurPer - (gLineCur.CurPerFilter>>5);

    m_Long = (Ulong)gLineCur.CurPer * (Ulong)gMotorInfo.Current;
    gLineCur.CurBaseInv = m_Long/gInvInfo.InvCurrent;
    gLineCur.CurBaseInvFilter += gLineCur.CurBaseInv - (gLineCur.CurBaseInvFilter>>9);
    OutputLoseAdd();

#if 0
    // output power calculation here
    ////////////////////////////////
    {
        // 避免出现负数时候计算出来的Temp 非常大...
        signed short TorqueCurAbs;
        TorqueCurAbs = gIMT.T;
        if (TorqueCurAbs < 0) {
            TorqueCurAbs = -TorqueCurAbs;
        }

        // add 128 as min flux current
        TorqueCurAbs += 128;

        m_Temp = (long)TorqueCurAbs * 200 >> 12;        // 100　~ 1.00A
        m_Temp *= 220;                                  // 100 * 220 = 220w(22000)
        m_Temp = (long)m_Temp * gRatio >> 12;           // Ratio, 2048..
        m_Temp /= 100;
        Power = m_Temp;
        m_TempAcc = ((((long)Power << 16) - m_TempAcc) >> 5) + m_TempAcc;
        Power  = (m_TempAcc + 0x8000) >> 16;
        gUDC.Power = Power;
    }
#endif
}

void AllowSelectBottomOnly(unsigned short Selection)
{
    EALLOW;
    switch (Selection) {
        case 0:                         // U_BOTTOM_ONLY
#ifndef CUT_DOWN_VER_HARDWARE
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
#ifndef CUT_DOWN_VER_HARDWARE
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
#ifndef CUT_DOWN_VER_HARDWARE
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
#ifndef CUT_DOWN_VER_HARDWARE
            EPwm2Regs.DBCTL.all = 0x0007;
            EPwm3Regs.DBCTL.all = 0x0007;
            EPwm4Regs.DBCTL.all = 0x0007;
#else
            EPwm2Regs.DBCTL.all = 0x000B;
            EPwm3Regs.DBCTL.all = 0x000B;
            EPwm4Regs.DBCTL.all = 0x000B;
#endif
        break;
        case 4:                         // Normal PWM Pattern
#ifndef CUT_DOWN_VER_HARDWARE
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
