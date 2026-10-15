
#include "Main.h"
#include "RunSrc.h"
#include "Define.h"
#include "Reference.h"
#include "MotorInclude.h"
#include "Parameter.h"
#include "LinLDF.h"
#include "PreDriver.h"

enum POWER_ON_STATUS powerOnStatus;     // 上电时的状态
enum UV_DEAL_STATUS
{
    UV_READY_FOR_WRITE_EEPROM,          // 欠压状态，准备掉电记忆数据
    UV_WAIT_WRITE_EEPROM,               // 欠压状态，等待掉电记忆
    UV_WRITE_EEPROM_OK                  // 掉电记忆完成。等待恢复电压
};
enum UV_DEAL_STATUS uvDealStatus;

Uint16 bUv;
Uint32 errAutoRstClrWaitTicker;         // 故障自动复位次数清除的等待时间的ticker
Uint32 errorSpaceTicker;                // 故障自动复位间隔时间
Uint16 errAutoRstNum;                   // 自动复位故障次数计数
Uint16 frq2Core;
Uint16 frqCurAim2Core;

Uint16 NOTUSED = 0;
Uint16 errorCodeFromCore;
int32  ObserverMotorSpeedINOnePointHZ;  // 观测器转速（包括SPIN)发出的频率, 0.1Hz
Uint16 RrsDataFromMotor;                // ReserData

Uint16 * const pDataCore2Func[CORE_TO_FUNC_DATA_NUM] = // 12
{
    &dspStatus.all,                 // 0     状态字
    &errorCodeFromCore,             // 1     故障代码
    &RrsDataFromMotor,              // 2     保留
    &RrsDataFromMotor,              // 3     保留
    &RrsDataFromMotor,              // 4     保留
    &RrsDataFromMotor,              // 5     保留
    &RrsDataFromMotor,              // 6     保留
    &RrsDataFromMotor,    			// 7     保留
    &RrsDataFromMotor,		        // 8     保留
    &RrsDataFromMotor,              // 9     保留
    &RrsDataFromMotor,              // 10    保留
    &RrsDataFromMotor,              // 11    保留
};

void FuncVariableInit(void);
void UpdateDataCore2Func(void);
void UpdateDataFunc2Core(void);
void InternalOSCTrim(void);
void ErrorDeal(void);
void ErrorReset(void);
void InitFuncCode(void);
void LEDControl(void);
void SoftVersionDeal(void);
void GetOSCTrimConst(void);

int RefTempOffset = 2261;
int Osc1FineTrimSlope = -1476;
int Osc1FineTrimOffset = 16384;
int Osc1CoarseTrim = 1; //这个是芯片修调的通用值

void InitForFunctionApp(void)
{
	// 初始化功能码
    InitFuncCode();

	// 参数初始化
    LinData_init();
    FuncVariableInit();
    init_random_freq();
}

void InitFuncCode(void)
{
    UpdateDataCore2Func();
}

void FuncVariableInit(void)
{
    GetOSCTrimConst();
    UpdateDataFunc2Core();
}

// Uint16 drivertest =0;
void SystemLeve2msFunction(void)
{
    UpdateDataCore2Func();      // 更新性能传递给功能的交互数据

    FrqSrcDeal();               // 频率源
    RunSrcDeal();               // 命令源

    ErrorDeal();                // 更新errorCode
    CommsProtocalLayerTask2ms();

    ErrorReset();             	// 故障处理
    LEDControl();      			// LED 驱动
    UpdateLinDataToVar();
    InternalOSCTrim();

#if 0
    if(drivertest == 1)
    {
        PreDriverTest();
        drivertest = 0;
    }
#endif
    UpdateDataFunc2Core();      // 更新功能传递给性能的交互数据
}

// 需要确保读取的值是否合理，不合理可能不是修调过的芯片....
void GetOSCTrimConst(void)
{
    RefTempOffset      = (*(int16 (*)(void))0x3D7EA2)();
    Osc1FineTrimSlope  = (*(int16 (*)(void))0x3D7E90)();
    Osc1FineTrimOffset = ((long)(*(int16 (*)(void))0x3D7E93)() << DPOINT15) + DQPRE_CONST;
    int Coarse         = (*(int16 (*)(void))0x3D7E96)();
    if (Coarse<0) Osc1CoarseTrim = (-Coarse) | 0x80;
    else          Osc1CoarseTrim = Coarse;
}

void InternalOSCTrim(void)
{
    long InternalNTCADC;
    InternalNTCADC = ADC_DSP_TEMP;
    long compOscFineTrim = (InternalNTCADC - RefTempOffset) * Osc1FineTrimSlope + Osc1FineTrimOffset >> 15;
    if (compOscFineTrim<0)       compOscFineTrim = 0;
    else if (compOscFineTrim>31) compOscFineTrim = 31;
    long INTOSCTRIM_CLAout = Osc1CoarseTrim + (compOscFineTrim << 9);
    EALLOW;
    SysCtrlRegs.INTOSC1TRIM.all = INTOSCTRIM_CLAout;
    EDIS;
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
        Lin_Communication.EcwpEna = 0;
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

    // #ifdef  FREQUENCY_CONTROL_01HZ
    ObserverMotorSpeedINOnePointHZ = ((signed long)((signed short)gMainCmd.FreqPreWs) * ((signed long)maxFrq + 200) + (1 << 14)) >> 15;
    if (ObserverMotorSpeedINOnePointHZ == frq2Core)  // 规避运算引起的误差
    {
        ObserverMotorSpeedINOnePointHZ = frq;
    }
}

void UpdateDataFunc2Core(void)
{
    // #ifdef  FREQUENCY_CONTROL_01HZ
    frq2Core = (frq << 15) / ((signed long)maxFrq + 200);

    gSendToMotorDataBuff[0]  = dspMainCmd.all;
    gSendToMotorDataBuff[1]  = frq2Core;
    gSendToMotorDataBuff[2]  = upperTorque;
    gSendToMotorDataBuff[3]  = frqCurAim2Core;

    // 将一些功能码的内容固化,注意核对一些参数，例如载频，温度等等
	///////////////////////////////////////////////////////////////
	gSendToMotorDataBuff[4]  = PWM_FREQUENCY; 	                // 载频，默认8KHz
	gSendToMotorDataBuff[5]  = MAX_LIMIT_FREQ; 	                // 上限频率
	gSendToMotorDataBuff[6] = NOTUSED; 	                        // 保留
	gSendToMotorDataBuff[7] = MAX_LIMIT_FREQ; 	                // 最大频率
	gSendToMotorDataBuff[8] = NOTUSED; 		                    // 保留
    gSendToMotorDataBuff[9] = NOTUSED;
	gSendToMotorDataBuff[10] = NOTUSED; 		                // 保留
	gSendToMotorDataBuff[11] = MOTOR_RATED_VOL;	                // 电机额定电压
	gSendToMotorDataBuff[12] = MOTOR_RATED_CUR;
	gSendToMotorDataBuff[13] = MOTOR_RATED_FRE;	                // 电机额定频率
	gSendToMotorDataBuff[14] = IdMaxScaling;
	gSendToMotorDataBuff[15] = NOTUSED;
	gSendToMotorDataBuff[16] = OVER_DC_VOL_POINT; 	            // 过压点: 18V
	gSendToMotorDataBuff[17] = UNDER_DC_VOL_POINT;              // 欠压点: 7V
	gSendToMotorDataBuff[19] = NOTUSED;                         // 保留
	gSendToMotorDataBuff[20] = NOTUSED; 	                    // 保留
	gSendToMotorDataBuff[21] = NOTUSED; 	                    // 保留
	gSendToMotorDataBuff[22] = NOTUSED;                         // 保留
	gSendToMotorDataBuff[23] = IqMaxSet;
	gSendToMotorDataBuff[24] = DSP_CLOCK; 		                // 时钟频率
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
}

void ErrorDeal(void)
{
    if (((dspStatus.bit.uv) && (POWER_ON_WAIT != powerOnStatus))    // 当上电完成之后，才判断是否欠压
        || (POWER_ON_FUNC_WAIT_OT == powerOnStatus))                // 功能等待时间超时，认为欠压
    {
        bUv = 1;
    }

    if (bUv)                        // 欠压
    {
        if (runFlag.bit.run)
        {
            runFlag.all = 0;        // 欠故
        }

        // 立即关断PWM
        dspMainCmd.bit.run = 0;     // dspMainCmd.all &= 0xf330;
        dspMainCmd.bit.speedTrack = 0;
        dspMainCmd.bit.startBrake = 0;
        dspMainCmd.bit.accDecStatus = 0;

        switch (uvDealStatus)
        {
            case UV_READY_FOR_WRITE_EEPROM:
                uvDealStatus = UV_WAIT_WRITE_EEPROM;
            break;
            case UV_WAIT_WRITE_EEPROM:
                uvDealStatus = UV_WRITE_EEPROM_OK;
            break;
            case UV_WRITE_EEPROM_OK:
                if (!dspStatus.bit.uv)
                {
                    uvDealStatus = UV_READY_FOR_WRITE_EEPROM;
                    bUv = 0;
                    runStatus = RUN_STATUS_WAIT;
                }
            break;
            default:
            break;
        }
    }// if (bUv)

	// 通讯故障55 不能停机....
    if (errorCodeFromCore)
    {
		runFlag.bit.run = 0;

		// 当前拍立即关断PWM
		dspMainCmd.bit.run = 0;
		dspMainCmd.bit.startBrake = 0;
		dspMainCmd.bit.accDecStatus = 0;
	}
}

void ErrorReset(void)
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
    static Uint32 AutoResetDelay2ms = 3000000;
    switch (ErrorDealStatus) {
    case ERROR_DEAL_INIT: {
        // 有些故障最好不要复位，这样可以查找问题，如电流检测故障.
/*
        if (ERROR_OC_HARDWARE == errorCodeFromCore) {                   // 过流
            ErrorDealStatus = ERROR_DEAL_RST;
            AutoResetDelay2ms = CURRENT_FAULT_RECOVER_TIME;
            AutoResetDelay2ms >>= 1;
#if (ADM32F036_TYPE == ADM32F036_A3)
            GetPerDriverState();                                        // 预取状态读取
#endif
        } else if (ERROR_STALL == errorCodeFromCore) {                  // 堵转
            ErrorDealStatus = ERROR_DEAL_RST;
            AutoResetDelay2ms = STALL_FAULT_RECOVER_TIME;
            AutoResetDelay2ms >>= 1;
        } else if (ERROR_LOSE_PHASE_OUTPUT == errorCodeFromCore) {      // 缺项
            ErrorDealStatus = ERROR_DEAL_RST;
            AutoResetDelay2ms = PHASELOSS_FAULT_RECOVER_TIME;
            AutoResetDelay2ms >>= 1;
        } else if (ERROR_OT_IGBT == errorCodeFromCore) {                // 过温
            ErrorDealStatus = ERROR_DEAL_RST;
            AutoResetDelay2ms = TEMP_FAULT_RECOVER_TIME;
            AutoResetDelay2ms >>= 1;
        } else if (ERROR_OV_ACC_SPEED == errorCodeFromCore) {           // 过压
            ErrorDealStatus = ERROR_DEAL_RST;
            AutoResetDelay2ms = VOLT_FAULT_RECOVER_TIME;
            AutoResetDelay2ms >>= 1;
        }
*/      if(0)
        {

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
    }
    break;
    case ERROR_DEAL_SHUNT_DOWN: {
        errAutoRstClrWaitTicker = 0;

        // 立即关断PWM
        dspMainCmd.bit.run = 0;
        dspMainCmd.bit.speedTrack = 0;
        dspMainCmd.bit.startBrake = 0;
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
        errorCodeFromCore = 0;
        runStatus = RUN_STATUS_WAIT;
    }
    break;
    default:
    break;
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
