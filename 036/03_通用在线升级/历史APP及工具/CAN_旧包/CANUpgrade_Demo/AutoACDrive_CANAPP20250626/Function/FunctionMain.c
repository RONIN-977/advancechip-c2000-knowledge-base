
#include "Main.h"
#include "RunSrc.h"
#include "FrqSrc.h"
#include "IO.h"
#include "MotorInclude.h"
#include "Parameter.h"

enum POWER_ON_STATUS powerOnStatus;             // 上电时的状态
enum UV_DEAL_STATUS uvDealStatus;


Uint16 UnderVolFlag;
Uint32 errorSpaceTicker;         // 故障自动复位间隔时间
Uint16 errAutoRstNum;                   // 自动复位故障次数计数
Uint16 errorOther;                      // 其他故障，包括外部故障、调谐故障、通讯故障、运行时间到达等
Uint16 errorCode = 0;

Uint16 frq2Core;
Uint16 frqCurAim2Core;
Uint16 rsvdData;
Uint16 NOTUSED = 0;
Uint16 errorCodeFromCore;
Uint16 currentOc;
Uint16 speedMotor;
Uint16 generatrixVoltage;
Uint16 outVoltage;
Uint16 outCurrent;
Uint16 FbackFry;                    // 反馈频率

Uint16 uvGainWarpTune;          	// UV两相增益偏差，辨识值
Uint16 InvCurrent;                  // 变频器额定电流
Uint16 HeatSink;
int32 frqRun;      					// 性能实际发出的频率(对VF而言，是同步转速), 0.01Hz

Uint16 * const pDataCore2Func[CORE_TO_FUNC_DATA_NUM] = // 12 15?????????
{
    &dspStatus.all,                 // 0     状态字
    &errorCodeFromCore,             // 1     核心故障代码
    &currentOc,                     // 2     过流时的电流
    &speedMotor,                    // 3     速度反馈
    &generatrixVoltage,             // 4     直流母线电压
    &outCurrent,                    // 5     输出电流
    &outVoltage,                    // 6     输出电压
    &HeatSink,    					// 7            散热器温度
    &uvGainWarpTune,				// 8
    &uvGainWarpTune,   				// 9
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
    FuncVariableInit();
}

void InitFuncCode(void)
{
    UpdateDataCore2Func();
}

void FuncVariableInit(void)
{
    UpdateDataFunc2Core();
}

void Main2msFunction(void)
{
    UpdateDataCore2Func();      // 更新性能传递给功能的交互数据
    FrqSrcDeal();               // 频率源
    RunSrcDeal();               // 命令源
    ErrorDeal();                // 更新errorCode
    ErrorReset();             	// 故障处理
    LEDControl();      			// LED 驱动
    UpdateDataFunc2Core();      // 更新功能传递给性能的交互数据
}

void Main05msFunction(void)
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

void Main0msFunction(void)
{

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
#else
    frqRun = ((signed long)((signed short)speedMotor) * ((signed long)maxFrq + 2000) + (1 << 14)) >> 15;
#endif

    if (speedMotor == frq2Core)  // 规避运算引起的误差
    {
        frqRun = frq;
    }
}

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
    gSendToMotorDataBuff[0] = dspMainCmd.all;
    gSendToMotorDataBuff[1] = frq2Core;
    gSendToMotorDataBuff[2] = upperTorque;
    gSendToMotorDataBuff[3] = frqCurAim2Core;

	// 将一些功能码的内容固化,注意核对一些参数，例如载频，温度等等
	///////////////////////////////////////////////////////////////
	gSendToMotorDataBuff[4] = PWM_FREQUENCY; 	// 载频，默认8KHz
	gSendToMotorDataBuff[5] = MAX_LIMIT_FREQ; 	// 上限频率
	gSendToMotorDataBuff[6] = NOTUSED; 		    // 保留
	gSendToMotorDataBuff[7] = MAX_LIMIT_FREQ; 	// 最大频率

	gSendToMotorDataBuff[8] = NOTUSED; 		    // 转差补偿增益
	gSendToMotorDataBuff[9] = NOTUSED; 		    // 定子电阻 ，不用
	gSendToMotorDataBuff[10] = NOTUSED; 		// 电机额定功率，不用

	gSendToMotorDataBuff[11] = MOTOR_RATED_VOL; // 电机额定电压

	gSendToMotorDataBuff[12] = MOTOR_RATED_CUR;
	gSendToMotorDataBuff[13] = MOTOR_RATED_FRE;	// 电机额定频率

	gSendToMotorDataBuff[14] = NOTUSED;
	gSendToMotorDataBuff[15] = ThetaCoff;           // 角度补偿值
	gSendToMotorDataBuff[16] = OVER_DC_VOL_POINT; 	// 过压故障点选择
	gSendToMotorDataBuff[17] = UNDER_DC_VOL_POINT; 	// 欠压点

	gSendToMotorDataBuff[19] = NOTUSED; 		    // 变频器机型
	gSendToMotorDataBuff[20] = NOTUSED; 	        // 电压校正系数
	gSendToMotorDataBuff[21] = NOTUSED; 	        // 电流校正系数
	gSendToMotorDataBuff[22] = NOTUSED;
	gSendToMotorDataBuff[23] = NOTUSED;

	gSendToMotorDataBuff[24] = 30; 		        // 电流检测延时补偿

	gSendToMotorDataBuff[25] = DC_BRAKE_CUR;    // 死区补偿模式

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
    gSendToMotorDataBuff[40] = TS_L_VDC_SCALING;
    gSendToMotorDataBuff[41] = IPD_PERIOD_SCALING;

	gSendToMotorDataBuff[42] = AVR_Mode;


    gSendToMotorDataBuff[43] = 0;
    gSendToMotorDataBuff[44] = 0;
    gSendToMotorDataBuff[45] = 0;

    if (0 == mainLoopTicker)    // 进入主循环的第1拍。从下一拍开始才考虑启动频率/下限频率的启动保护
    {
        mainLoopTicker = 1;
    }
}

void ErrorDeal(void)
{
    if (((dspStatus.bit.uv) && (POWER_ON_WAIT != powerOnStatus))    // 当上电完成之后，才判断是否欠压
        || (POWER_ON_FUNC_WAIT_OT == powerOnStatus)) // 功能等待时间超时，认为欠压
    {
        UnderVolFlag = 1;
    }

    if (UnderVolFlag)          // 欠压
    {
        if (runFlag.bit.run)        // 不要等待掉电保存完毕才显示故障。立即显示故障
                                    // 欠压时有运行命令，runFlag.bit.run也为1。但不发送PWM，立即报警
        {
            runFlag.all = 0;        // 欠故庇性诵忻畈灰猺un灯亮一下
            errorOther = 14;  // 运行时不会有(其他)故障
        }

        // 立即关断PWM
        dspMainCmd.bit.run = 0;         // dspMainCmd.all &= 0xf330;
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

            case UV_WAIT_WRITE_EEPROM:          // 等待掉电保存完毕
                uvDealStatus = UV_WRITE_EEPROM_OK;  // 直接进入UV_WRITE_EEPROM_OK
            break;

            case UV_WRITE_EEPROM_OK:
                if (!dspStatus.bit.uv)      // 出现了欠压标志，当掉电保存完毕之后，才判断是否已经不欠压
                {
                    uvDealStatus = UV_READY_FOR_WRITE_EEPROM;
                    UnderVolFlag = 0;

                    // 注意，如果掉电不完全，则会出现不会再重新启动现象，原因是：
                    // runStatus 处于Noraml 状态，而应该是 RUN_STATUS_WAIT
                    runStatus = RUN_STATUS_WAIT;
                }
                break;

            default:
                break;
        }
    } // if (UnderVolFlag)

	//if ((UnderVolFlag)&&(errorCodeFromCore != 14)) // 欠压下有性能故障
	//{
	//	errorCodeFromCore = 0;
	//	dspMainCmd.bit.errorDealing = 1;    // 等待性能清除故障码
	//}

	// 通讯故障55 不能停机....
    if (errorCode || errorCodeFromCore)
    {
        tuneCmd = 0;
		runFlag.bit.run = 0;        // 欠压时有运行命令不要run灯亮一下

		// 当前拍立即关断PWM
		dspMainCmd.bit.run = 0;
		dspMainCmd.bit.stopBrake = 0;
		dspMainCmd.bit.startBrake = 0;
		dspMainCmd.bit.accDecStatus = 0;
	}
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
		static Uint32 AutoResetDelay2ms = 10000;    //20s

		switch (ErrorDealStatus) {
		case ERROR_DEAL_INIT: {
			// 有些故障最好不要复位，这样可以查找问题，如电流检测故障.
			if( (ERROR_OC_HARDWARE          == errorCodeFromCore) ||
			    (ERROR_OT_IGBT              == errorCodeFromCore) ||
				(ERROR_OC_SOFTWARE          == errorCodeFromCore) )
			{
			    ErrorDealStatus   = ERROR_DEAL_RST;
				AutoResetDelay2ms = 150000;             //5min
			}
			else if ( (ERROR_OV_ACC_SPEED         == errorCodeFromCore) ||
				      (ERROR_LOSE_PHASE_OUTPUT    == errorCodeFromCore) ||
				      (ERROR_STALL                == errorCodeFromCore) )
			{
				// 5s restart, 2500
				ErrorDealStatus = ERROR_DEAL_RST;
				AutoResetDelay2ms = 30000;               //1Min
			}
			else if (ERROR_NONE != errorCodeFromCore)
			{
			    ErrorDealStatus = ERROR_DEAL_RST;
			    AutoResetDelay2ms = 5000;
			}
			else {
				// 考虑到一个问题，出故障后复位正常工作8H 后，将故障复位次数清零，这样可以重新累计
				// 2ms. 500*3600 ~ 1H
				DriveOKTime ++;
				if (DriveOKTime > 3600000) {
					DriveOKTime = 0;
					errAutoRstNum = 0;
				}
			}
			errorSpaceTicker = 0;
		}
		break;
		case ERROR_DEAL_SHUNT_DOWN: {
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
			if (ErrorDealWaitCNT > 50) {
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
    }
}

// 软件版本处理
void SoftVersionDeal(void)
{

}

void LEDControl(void)
{
    static int  LedOnOrOffTime = 0;
    static int  DelayCounter = 0;
    int  Tripnumber = 0;
    if (errorCodeFromCore != 0) {
        if (errorCodeFromCore        == ERROR_OL_INVERTER) {
            Tripnumber = 1;
        } else if (errorCodeFromCore == ERROR_OC_HARDWARE) {
            Tripnumber = 2;
        } else if (errorCodeFromCore == ERROR_OV_ACC_SPEED) {
            Tripnumber = 3;
        } else if (errorCodeFromCore == ERROR_UV) {
            Tripnumber = 4;
        } else if (errorCodeFromCore == ERROR_LOSE_PHASE_OUTPUT) {
            Tripnumber = 5;
        } else if (errorCodeFromCore == ERROR_OT_IGBT) {
            Tripnumber = 6;
        } else if (errorCodeFromCore == ERROR_CURRENT_SAMPLE) {
            Tripnumber = 7;
        } else if (errorCodeFromCore == ERROR_STALL) {
            Tripnumber = 8;
        } else if (errorCodeFromCore == ERROR_CBC) {
            Tripnumber = 9;
        } else {
            Tripnumber = 10;
        }
    }

    enum {
        WAIT_TIME,
        FLASH_TIME
    };
    static int  YellowLampFlashTimes ;
    static int  mStatusLedFlashing = WAIT_TIME;

    // 2ms tick, then 500 is 1s
    if(++DelayCounter < 500) {
        return;                             //等待零漂检测完成
    }
    DelayCounter = 500;

    if (Tripnumber) {
        switch(mStatusLedFlashing)
        {
            case WAIT_TIME:
                LedOnOrOffTime++;
                if (LedOnOrOffTime < TRIP_FLASHING_TIMEX4){
                } else {
                    LedOnOrOffTime          = 0;
                    mStatusLedFlashing      = FLASH_TIME;
                    YellowLampFlashTimes    = Tripnumber;
                }
                break;

            case FLASH_TIME:
                LedOnOrOffTime++;
                if (LedOnOrOffTime < TRIP_FLASHING_TIME){
                    YellowLED_ON;
                } else if (LedOnOrOffTime < TRIP_FLASHING_TIMEX2){
                    YellowLED_OFF;
                } else {
                    LedOnOrOffTime = 0;
                    YellowLampFlashTimes--;
                    if (YellowLampFlashTimes <= 0){
                        mStatusLedFlashing = WAIT_TIME;
                    }
                }
                break;

            default:
                mStatusLedFlashing = WAIT_TIME;
                break;
        }
    } else {
        if (runFlag.bit.run == 1){
            YellowLED_ON;
        } else {
            YellowLED_OFF;
        }
        YellowLampFlashTimes = 0;
        mStatusLedFlashing = WAIT_TIME;
    }
}
