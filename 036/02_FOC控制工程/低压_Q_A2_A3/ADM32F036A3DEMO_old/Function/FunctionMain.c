
#include "Main.h"
#include "RunSrc.h"
#include "Define.h"
#include "Reference.h"
#include "FuncUser.h"
#include "MotorInclude.h"
#include "Parameter.h"
#include "LinLDF.h"
#include "PreDriver.h"

#define YellowLED_ON    	    	GpioDataRegs.GPASET.bit.GPIO18  = 1
#define YellowLED_OFF    	    	GpioDataRegs.GPACLEAR.bit.GPIO18    = 1

#define TRIP_FLASHING_TIME  		100

#define WAIT_POWER_ON_TIME          10000       // 进入主程序之后，等待_*0.5ms开始响应欠压处理和运行命令
enum POWER_ON_STATUS powerOnStatus;             // 上电时的状态

enum UV_DEAL_STATUS
{
    UV_READY_FOR_WRITE_EEPROM,      // 欠压状态，准备掉电记忆数据
    UV_WAIT_WRITE_EEPROM,           // 欠压状态，等待掉电记忆
    UV_WRITE_EEPROM_OK              // 掉电记忆完成。等待恢复电压
};
LOCALF enum UV_DEAL_STATUS uvDealStatus;

Uint16 bUv;
LOCALF Uint32 errAutoRstClrWaitTicker;  // 故障自动复位次数清除的等待时间的ticker
LOCALF Uint32 errorSpaceTicker;         // 故障自动复位间隔时间
Uint16 errAutoRstNum;                   // 自动复位故障次数计数
Uint16 errorOther;                      // 其他故障，包括外部故障、调谐故障、通讯故障、运行时间到达等
Uint16 errorCode = 0;

Uint16 frq2Core;
Uint16 frqCurAim2Core;
Uint16 rsvdData;
Uint16 SF_SET = 100;

Uint16 errorCodeFromCore;
Uint16 currentOc;
Uint16 speedMotor;
Uint16 generatrixVoltage;
Uint16 outVoltage;
Uint16 outCurrent;
Uint16 FbackFry;                    // 反馈频率
Uint16 outPower;    				// 单位为0.1KW

Uint16 uvGainWarpTune;          	// UV两相增益偏差，辨识值
Uint16 InvCurrent;                  // 变频器额定电流
Uint16 HeatSink;
int32 frqRun;      					// 性能实际发出的频率(对VF而言，是同步转速), 0.01Hz
Uint16 RandomPWM;

Uint16 * const pDataCore2Func[CORE_TO_FUNC_DATA_NUM] = // 12 15?????????
{
    &dspStatus.all,                 // 0     状态字
    &errorCodeFromCore,             // 1     核心故障代码
    &currentOc,                     // 2     过流时的电流
    &speedMotor,                    // 3     速度反馈
    &generatrixVoltage,             // 4     直流母线电压
    &outCurrent,                    // 5     输出电流
    &outVoltage,                    // 6     输出电压
    &HeatSink,    					// 7               散热器温度
    &uvGainWarpTune,				// 8     DSP软件版本号，放入FF-07

	// 性能与功能在同一块CPU上，这时不用处理，直接读取功能码的RAM就可以了
    &uvGainWarpTune,   				// 9     性能转矩输出   机型rsvdData,
    &FbackFry,                      // 10    反馈频率       电压校正系数
                                    //       在参数辨识的时候发送以下数据
    &InvCurrent,                    // 14    变频器额定电流 同步速度
};


Uint16 mainLoopTicker;      // 主循环计数器

void FuncVariableInit(void);
void UpdateDataCore2Func(void);
void UpdateDataFunc2Core(void);
void ErrorDeal(void);
void ErrorReset(void);
void InitFuncCode(void);
void LEDControl(void);
void SoftVersionDeal(void);


void InitForFunctionApp(void)
{
	// 初始化功能码
    InitFuncCode();

	// 参数初始化
    LinData_init();
    FuncVariableInit();

    //    DRIVE_ENABLE();
}

void InitFuncCode(void)
{
#if 0
    Uint32 baseTime;
    baseTime = GetTime();

	// 发生EEPROM故障，立即退出读取进入主循环，显示err
    while (FUNCCODE_INIT_STATUS_OVER != funcCodeInitStatus)
    {
        switch (funcCodeInitStatus)
        {
            case FUNCCODE_INIT_STATUS_VERIFY_EEPROM: {
                funcCodeInitStatus = FUNCCODE_INIT_STATUS_NEW_EEPROM;
            }
            break;

            case FUNCCODE_INIT_STATUS_NEW_EEPROM:
            	funcCodeInitStatus = FUNCCODE_INIT_STATUS_OVER;
            break;

            case FUNCCODE_INIT_STATUS_OLD_EEPROM: {
            }
            break;

            default:
            break;
        } // switch (funcCodeInitStatus)

        if (baseTime - GetTime() >= (60000UL << 1)) // 2ms
        {
            baseTime = GetTime();
            UpdateDataCore2Func();      // 需要性能传递AI的采样值
        }
    }

	// 软件版本处理
    SoftVersionDeal();
#else
    UpdateDataCore2Func();      // 需要性能传递AI的采样值
#endif
}

void FuncVariableInit(void)
{
    UpdateDataFunc2Core();
}

Uint16 drivertest =0;
void SystemLeve2msFunction(void)
{
    UpdateDataCore2Func();      // 更新性能传递给功能的交互数据

    FrqSrcDeal();               // 频率源
    RunSrcDeal();               // 命令源

    ErrorDeal();                // 更新errorCode
    CommsProtocalLayerTask2ms();

    ErrorReset();             	// 故障处理
    //LEDControl();      			// LED 驱动
    UpdateLinDataToVar();

    if(gMainStatus.ErrorCode == 12)
    {
        GetPerDriverState();      // 预驱状态读取//580us
    }
    if(drivertest == 1)
    {
        PreDriverTest();
        drivertest = 0;
    }

    UpdateDataFunc2Core();      // 更新功能传递给性能的交互数据
}

void SystemLeve05msFunction(void)
{
    static Uint16 waitPowerOnTicker;

    if ((dspStatus.bit.shortGndOver)            // 母线电压建立完毕
        && (0x03 == dspStatus.bit.runEnable))   // 上电对地短路检测完毕
    {
        powerOnStatus = POWER_ON_CORE_OK;       // 上电准备OK
    }

    if (++waitPowerOnTicker >= WAIT_POWER_ON_TIME) // 进入主循环的时达絖时间
    {
        if (POWER_ON_WAIT == powerOnStatus)
        {
            powerOnStatus = POWER_ON_FUNC_WAIT_OT; // 功能的等待时间超时
        }
    }
}

// long DataTest[5];
Uint16 error1 = 0;
Uint16 testERnum = 0;
void SystemLeve0msFunction(void)
{
    //boot
    if (gDSPActiveTime.flash_IAP_flag == 1)
    {
        DINT;   //SETC INTM ; 禁用中断
        DisableDrive();
//GpioDataRegs.GPATOGGLE.bit.GPIO16  = 1;
        //Lin_Communication.EcwpEna = 0;
        gDSPActiveTime.IAP_Delay = 0;

        ResetDSP();
        // (*Jump_Boot)();
        // DINT;   //SETC INTM ; 禁用中断
        //asm(" MOVL XAR7, #0x3F7FF6"); //0x3F7FF6是BootLoader程序的入口地址
        //asm(" LB *XAR7");
    }
    else{
        gDSPActiveTime.IAP_Delay = 0;
    }
    error1 = ((LinaRegs.SCIFLR.all & 0xFF000000)>>24);
    if(error1 > 0)
    {
        LinaRegs.SCIFLR.bit.BE = 1;
        LinaRegs.SCIFLR.bit.PBE = 1;
        LinaRegs.SCIFLR.bit.CE = 1;
        LinaRegs.SCIFLR.bit.ISFE = 1;
        LinaRegs.SCIFLR.bit.NRE = 1;
        LinaRegs.SCIFLR.bit.FE = 1;
        LinaRegs.SCIFLR.bit.OE = 1;
        LinaRegs.SCIFLR.bit.PE = 1;
        testERnum++;
    }
}


void UpdateDataCore2Func(void)
{
    int16 i;
    for (i = CORE_TO_FUNC_DATA_NUM - 1; i >= 0; i--)    // 性能传递给功能
    {
        *pDataCore2Func[i] = gSendToFunctionDataBuff[i];
    }

#ifdef  FREQUENCY_CONTROL_01HZ
    frqRun = ((signed long)((signed short)speedMotor) * ((signed long)maxFrq + 200) + (1 << 14)) >> 15;
    //frqRun2 = ((int32)((int16)speedMotor2) * ((int32)maxFrq + 200) + (1 << 14)) >> 15;
#else
    frqRun = ((signed long)((signed short)speedMotor) * ((signed long)maxFrq + 2000) + (1 << 14)) >> 15;
#endif

    if (speedMotor == frq2Core)  // 规避运算引起的误差
    {
        frqRun = frq;
    }
}

int IdMaxScaling    = 1024;     //1200
int IqMaxSet        = 400;      //1400;//400
int ThetaCoff       = 6000;
int NOTUSED         = 0;
void UpdateDataFunc2Core(void)
{
#ifdef  FREQUENCY_CONTROL_01HZ
    frq2Core = (frq << 15) / ((signed long)maxFrq + 200);
    frqCurAim2Core = (frqCurAim << 15) / ((signed long)maxFrq + 200);
#else
    frq2Core = (frq << 15) / ((signed long)maxFrq + 2000);
    frqCurAim2Core = (frqCurAim << 15) / ((signed long)maxFrq + 2000);
#endif

	// 0 ~ 70...71，可以考虑将功能码的数据将其在此处固定化...
    //    for (i = FUNC_TO_CORE_DATA_NUM - 1; i >= 0; i--)    // 功能传递给性能
    //    {
    //        gSendToMotorDataBuff[i] = *pDataFunc2Core[i];
    //    }
    ///////////////////////////////////////////////////////////////////////////
    gSendToMotorDataBuff[0]  = dspMainCmd.all;
    gSendToMotorDataBuff[1]  = frq2Core;
    gSendToMotorDataBuff[2]  = upperTorque;
    gSendToMotorDataBuff[3]  = frqCurAim2Core;

    // 将一些功能码的内容固化,注意核对一些参数，例如载频，温度等等
	///////////////////////////////////////////////////////////////
	gSendToMotorDataBuff[4]  = PWM_FREQUENCY; 	// 载频，默认8KHz
	gSendToMotorDataBuff[5]  = MAX_LIMIT_FREQ; 	// 上限频率

	gSendToMotorDataBuff[6] = NOTUSED; 	        // 保留

	gSendToMotorDataBuff[7] = MAX_LIMIT_FREQ; 	// 最大频率
	gSendToMotorDataBuff[8] = NOTUSED; 		    // 保留

	gSendToMotorDataBuff[9] = NOTUSED; 		    // 保留

	gSendToMotorDataBuff[10] = NOTUSED; 		// 保留
	gSendToMotorDataBuff[11] = MOTOR_RATED_VOL;	// 电机额定电压

	gSendToMotorDataBuff[12] = MOTOR_RATED_CUR;
	gSendToMotorDataBuff[13] = MOTOR_RATED_FRE;	// 电机额定频率

	gSendToMotorDataBuff[14] = IdMaxScaling;
	gSendToMotorDataBuff[15] = NOTUSED;
	gSendToMotorDataBuff[16] = OVER_DC_VOL_POINT; 	            // 过压点: 18V
	gSendToMotorDataBuff[17] = UNDER_DC_VOL_POINT;              // 欠压点: 7V


	gSendToMotorDataBuff[19] = NOTUSED;         // 保留
	gSendToMotorDataBuff[20] = NOTUSED; 	    // 保留
	gSendToMotorDataBuff[21] = NOTUSED; 	    // 保留
	gSendToMotorDataBuff[22] = NOTUSED;         // 保留
	gSendToMotorDataBuff[23] = IqMaxSet;

	gSendToMotorDataBuff[24] = DSP_CLOCK; 		// 时钟频率

	gSendToMotorDataBuff[25] = DC_BRAKE_CUR;

    gSendToMotorDataBuff[26] = TORQUE_CUR_LIMIT;
    gSendToMotorDataBuff[27] = OVER_SPEED_SCALING;
    gSendToMotorDataBuff[28] = SPEED_FEEDBACK_FILTER;
    gSendToMotorDataBuff[29] = SF_LOW_SPEED;
    gSendToMotorDataBuff[30] = MIN_CUR_LOW_SPEED;
    gSendToMotorDataBuff[31] = SPECIAL_FACTOR_1;
    gSendToMotorDataBuff[32] = SPEED_MEASURE_PAR_1;
    gSendToMotorDataBuff[33] = SPEED_MEASURE_PAR_2;
    gSendToMotorDataBuff[34] = START_PRESET_CUR;
    gSendToMotorDataBuff[35] = LOW_LIMIT_FREQUENCY;
    gSendToMotorDataBuff[36] = FILED_WEEKING_MODE;
    gSendToMotorDataBuff[37] = FILED_WEEKING_CUR_SCAL;
    gSendToMotorDataBuff[38] = FILED_WEEKING_SCAL;
    gSendToMotorDataBuff[39] = FIELD_WEAK_REF;
    gSendToMotorDataBuff[40] = 0;
    gSendToMotorDataBuff[41] = IPD_PERIOD_SCALING;

    gSendToMotorDataBuff[42] = AVR_MODE;                // AVR. 0,1: Enable, 2:Disabled
    gSendToMotorDataBuff[43] = T_DELAY;                 // #define  T_DELAY      150L;
    gSendToMotorDataBuff[44] = ThetaCoff;               // 相位补偿值。
    gSendToMotorDataBuff[45] = 0;                       // 禁止反转，1：禁止反转。
    gSendToMotorDataBuff[46] = T_SAMPLE;                // #define  T_SAMPLE      150L;

#ifdef KE_WIDE_RANGE
    gSendToMotorDataBuff[47] = 1;
#else
    gSendToMotorDataBuff[47] = 0;
#endif

    if (0 == mainLoopTicker)    // 进入主循环的第1拍。从下一拍开始才考虑启动频率/下限频率的启动保护
    {
        mainLoopTicker = 1;
    }
}

void ErrorDeal(void)
{
    if (((dspStatus.bit.uv) && (POWER_ON_WAIT != powerOnStatus))    // 当上电完成之后，才判断是否欠压
        || (POWER_ON_FUNC_WAIT_OT == powerOnStatus))                // 功能等待时间超时，认为欠压
    {
        bUv = 1;
    }

    if (bUv)          // 欠压
    {
        if (runFlag.bit.run)        // 不要等待掉电保存完毕才显示故障。立即显示故障
                                    // 欠压时有运行命令，runFlag.bit.run也为1。但不发送PWM，立即报警
        {
            runFlag.all = 0;        // 欠故庇性诵忻畈灰猺un灯亮一下
            errorOther = 14;        // 运行时不会有(其他)故障
        }

        // 立即关断PWM
        dspMainCmd.bit.run = 0;     // dspMainCmd.all &= 0xf330;
        dspMainCmd.bit.speedTrack = 0;
        dspMainCmd.bit.stopBrake = 0;
        dspMainCmd.bit.fullTune = 0;
        dspMainCmd.bit.startBrake = 0;
        dspMainCmd.bit.staticTune = 0;
        dspMainCmd.bit.accDecStatus = 0;

        switch (uvDealStatus)
        {
            case UV_READY_FOR_WRITE_EEPROM:
                uvDealStatus = UV_WAIT_WRITE_EEPROM;
            break;

            case UV_WAIT_WRITE_EEPROM:              // 等待掉电保存完毕
                uvDealStatus = UV_WRITE_EEPROM_OK;  // 直接进入UV_WRITE_EEPROM_OK
            break;

            case UV_WRITE_EEPROM_OK:
                if (!dspStatus.bit.uv)              // 出现了欠压标志，当掉电保存完毕之后，才判断是否已经不欠压
                {
                    uvDealStatus = UV_READY_FOR_WRITE_EEPROM;
                    bUv = 0;

                    // 注意，如果掉电不完全，则会出现不会再重新启动现象，原因是：
                    // runStatus 处于Noraml 状态，而应该是 RUN_STATUS_WAIT
                    runStatus = RUN_STATUS_WAIT;
                }
                break;

            default:
                break;
        }
    } // if (bUv)

	if ((bUv)&&(errorCodeFromCore != 14))   // 欠压下有性能故障
	{
		errorCodeFromCore = 0;
		dspMainCmd.bit.errorDealing = 1;    // 等待性能清除故障码
	}

	// 通讯故障55 不能停机....
    if (errorCode || errorCodeFromCore)
    {
        tuneCmd = 0;
		runFlag.bit.run = 0;                // 欠压时有运行命令不要run灯亮一下

		// 当前拍立即关断PWM
		dspMainCmd.bit.run = 0;
		dspMainCmd.bit.stopBrake = 0;
		dspMainCmd.bit.startBrake = 0;
		dspMainCmd.bit.accDecStatus = 0;
	}

    //    if (errorCode || errorOther || errorCodeFromCore) {
    //        DRIVE_DISABLE();
    //    } else {
    //        DRIVE_ENABLE();
    //    }
}

void ErrorReset(void)
{
	// #define ERROR_NONE                0      // 0   -- 无
	// #define ERROR_OC_ACC_SPEED        12     // 12  -- 过电流
	// #define ERROR_OV_ACC_SPEED        13     // 13  -- 过电压
	// #define ERROR_UV                  14     // 14  -- 欠压故障
	// #define ERROR_LOSE_PHASE_OUTPUT   15     // 15  -- 输出缺相
	// #define ERROR_OT_IGBT             16     // 16  -- 散热器过热
	// #define ERROR_CURRENT_SAMPLE      17     // 17  -- 电流检测故障
	// #define ERROR_TUNE                18     // 18  -- 电机调谐故障
	// #define ERROR_STALL               20     // 20  -- 电机堵转故障
	// #define ERROR_NTC	             21     // 21  -- NTC故障
	// #define ERROR_OC_SOFTWARE         33     // 21  -- 软件过流
	// #define ERROR_ERROR_KE            44     // 44  -- 软件过流
	// #define ERROR_SCI            	 55     // 55  -- 通讯故障

	// 过流保护在这里智能是做保护，封IGBT之后，延时10S 重新再启动
	// 重新启动 3次，如果超过3次，则认为硬件短路故障或软件性能不行
	// to to note!!
	// 考虑到实际运行时间非常长，不停机，可以考虑24H 内故障3此以内
	// OK！ 超过3次故障。
	//////////////////////////////////////////////////////////
	{
#define AUTO_RESET_CNT			5
#define AUTO_RESET_TIME_HOURS	24

		enum {
			ERROR_DEAL_INIT,
			ERROR_DEAL_SHUNT_DOWN,
			ERROR_DEAL_RST,
			ERROR_DEAL,
			ERROR_DEAL_WAIT,
			ERROR_DEAL_OK
		};
		static int ErrorDealStatus = ERROR_DEAL_INIT;
		static int ErrorDealWaitCNT;
		static Uint16 bResetError = 0;
		static Uint32 DriveOKTime;
		static Uint32 AutoResetDelay2ms = 3000000;

		switch (ErrorDealStatus) {
		case ERROR_DEAL_INIT: {
			// 有些故障最好不要复位，这样可以查找问题，如电流检测故障.
/*			if ((12 == errorCodeFromCore) ||
				(13 == errorCodeFromCore) ||
				(14 == errorCodeFromCore) ||
				(15 == errorCodeFromCore) ||
				(16 == errorCodeFromCore) ||
				(17 == errorCodeFromCore) ||
				(20 == errorCodeFromCore) ||
				(33 == errorCodeFromCore) ||
				(44 == errorCodeFromCore))
*/
		    if(0)
			{
                ErrorDealStatus = ERROR_DEAL_RST;
                AutoResetDelay2ms = 2500;   // 5s reset
			} else {
				// 考虑到一个问题，出故障后复位正常工作8H 后，将故障复位次数清零，这样可以重新累计
				// 2ms. 500*3600 ~ 1H
				DriveOKTime ++;
				if (DriveOKTime > 3600000) {
					DriveOKTime = 0;
					errAutoRstNum = 0;
				}
			}
		}
		break;
		case ERROR_DEAL_SHUNT_DOWN: {
			errAutoRstClrWaitTicker = 0;

			// 立即关断PWM
			dspMainCmd.bit.run = 0;
			dspMainCmd.bit.speedTrack = 0;
			dspMainCmd.bit.stopBrake = 0;
			dspMainCmd.bit.fullTune = 0;
			dspMainCmd.bit.startBrake = 0;
			dspMainCmd.bit.staticTune = 0;
			dspMainCmd.bit.accDecStatus = 0;

			ErrorDealStatus = ERROR_DEAL_RST;
		}
		break;
		case ERROR_DEAL_RST: {
			if (errAutoRstNum < AUTO_RESET_CNT) {
				if (++errorSpaceTicker >= AutoResetDelay2ms)
				{
					// 2MS tick function, 2000 * 2 = 4s
					errAutoRstNum++;
					bResetError = 1;
					ErrorDealStatus = ERROR_DEAL;
				}
			}
		}
		break;
		case ERROR_DEAL: {
			if (bResetError) {
				errorSpaceTicker = 0;
				if (errorCodeFromCore) {
					dspMainCmd.bit.errorDealing = 1;
				}
				ErrorDealStatus = ERROR_DEAL_WAIT;
			}
		}
		break;
		case ERROR_DEAL_WAIT: {
			ErrorDealWaitCNT++;
			if (ErrorDealWaitCNT > 0) { //50) {
				ErrorDealStatus = ERROR_DEAL_OK;
			}
		}
		break;
		case ERROR_DEAL_OK: {
			ErrorDealStatus = ERROR_DEAL_INIT;
			dspMainCmd.bit.errorDealing = 0;
			bResetError = 0;
			ErrorDealWaitCNT = 0;
			errorCode = 0;
			errorOther = 0;
			errorCodeFromCore = 0;
			runStatus = RUN_STATUS_WAIT;
		}
		break;
		default:
		break;
		}

        //
        //        if (errorCodeFromCore == 0 || errorCodeFromCore == 14) {
        //            ErrorDealStatus = ERROR_DEAL_INIT;
        //        }
    }
}

// 软件版本处理
void SoftVersionDeal(void)
{

}

void LEDControl(void)
{
	enum {
		INIT,
		FIRST_BIT,
		WAIT_FIRST,
		SECOND_BIT,
		WAIT_SECOND
	};
	static int AlarmDisplay;
	static int AlarmNum;
	static int AlarmNumFirstBit;
	static int AlarmNumSecondBit;
	static int  LedOnOrOffTime = 0;
	static int  DelayCounter = 0;

	if (errorCodeFromCore != 0) {
		AlarmNum = errorCodeFromCore;
	} else if (errorCode != 0) {
		AlarmNum = errorCode;
	} else {
		AlarmNum = 0;
	}
	if (AlarmNum != 0) {
		switch (AlarmDisplay) {
		case INIT:
			AlarmDisplay = FIRST_BIT;
			AlarmNumFirstBit = AlarmNum / 10;
			AlarmNumSecondBit = AlarmNum % 10;
		break;
		case FIRST_BIT:
			LedOnOrOffTime++;
			if (LedOnOrOffTime < TRIP_FLASHING_TIME){
				YellowLED_ON;
			} else if (LedOnOrOffTime < 2 * TRIP_FLASHING_TIME){
				YellowLED_OFF;
			} else {
				LedOnOrOffTime = 0;
				AlarmNumFirstBit--;
				if (AlarmNumFirstBit <= 0){
					AlarmDisplay = WAIT_FIRST;
				}
			}
		break;
		case WAIT_FIRST:
			YellowLED_OFF;
			DelayCounter++;
			if (DelayCounter > 800) {
				DelayCounter = 0;
				AlarmDisplay = SECOND_BIT;
			}
		break;
		case SECOND_BIT:
			LedOnOrOffTime++;
			if (LedOnOrOffTime < TRIP_FLASHING_TIME){
				YellowLED_ON;
			} else if (LedOnOrOffTime < 2 * TRIP_FLASHING_TIME){
				YellowLED_OFF;
			} else {
				LedOnOrOffTime = 0;
				AlarmNumSecondBit--;
				if (AlarmNumSecondBit <= 0){
					AlarmDisplay = WAIT_SECOND;
				}
			}
		break;
		case WAIT_SECOND:
			YellowLED_OFF;
			DelayCounter++;
			if (DelayCounter > 2000) {
				DelayCounter = 0;
				AlarmDisplay = INIT;
			}
		break;
		default:
		break;
		}
	} else {
	    // 针对每一次运行，而不是第一次上电... ...
        static long Counter;
        static long Counter1s;

        // Ref 2ms level.
        Counter++;
        if (Counter == 500) {
            if (Counter1s < 100) {
                Counter1s ++;
            }
            Counter = 0;
        }

        if (runFlag.bit.run == 1) {
            YellowLED_OFF;
        } else {
            YellowLED_ON;
        }
	}
}

