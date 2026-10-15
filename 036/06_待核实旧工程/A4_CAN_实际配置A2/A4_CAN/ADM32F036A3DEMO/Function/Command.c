
#include "Define.h"
#include "RunSrc.h"
#include "Reference.h"
#include "Parameter.h"
#include "Main.h"
#include "MotorInclude.h"

union RUN_CMD runCmd;               // 运行命令字
union RUN_FLAG runFlag;             // 运行标识字
union DSP_MAIN_COMMAND dspMainCmd;  // 功能传递给性能的主命令字
union DSP_SUB_COMMAND dspSubCmd;    // 功能传递给性能的辅命令字
union DSP_STATUS dspStatus;         // 性能传递给功能的状态字

enum RUN_STATUS runStatus;              // 运行状态
enum START_RUN_STATUS startRunStatus;   // 启动时的运行状态
enum STOP_RUN_STATUS stopRunStatus;     // 停机时的运行状态


Uint16 runSrc;          // 运行命令源
Uint32 accFrqTime;      // 加速时间，单位同功能码
Uint32 decFrqTime;      // 减速时间

LOCALF Uint32 runTimeTicker;        // 运行时间计时
LOCALF Uint32 lowerDelayTicker;     // 低于下限频率停机时间计时
LOCALF Uint32 shuntTicker;          // 瞬停不停的电压回升时间计时
LOCALF Uint16 runTimeAddupTicker;   // 累计运行时间计时



Uint16 tuneCmd;                 // 调谐
Uint16 tuneFinshed;

Uint16 ACC_SET = 1500;
Uint16 DEC_SET = 3000;

unsigned short ParaSaveTriger;
unsigned short AutotuneParaSaveTriger;

void StartRunCtrl(void); // 启动
void NormalRunCtrl(void);
void StopRunCtrl(void);
void TuneRunCtrl(void);  // 调谐
void ShutDownRunCtrl(void);
void LowerThanLowerFrqDeal(void);

int RunSignal;
int AVRModeE;

void UpdateRunCmd(void)
{
	static unsigned char RunCmd = 0;
    Uint16 fwd, rev;

	Uint m_TempAD;
	static long m_TempADAcc;

	// OFF ~　DI state is 1.
    if (0) {
        if (1) {//GetSetupFreComs() > 100 && GetStartCMDComs() == 1) {
            RunCmd = 1;
        } else {
            RunCmd = 0;
        }
    } else if (0) {
		m_TempAD = AdcResult.ADCRESULT3;
		m_TempADAcc  += ((((long)m_TempAD << 16) - m_TempADAcc) >> 5);
		m_TempAD = (m_TempADAcc + 0x8000) >> 16;

		// 568     ~ 4096
		// 1000rpm ~ 7200RPM
		if (m_TempAD < 20) {
			RunCmd	= 0;
		} else if (m_TempAD > 40) {
			RunCmd	= 1;
		}
    } else {
        // 针对每一次运行，而不是第一次上电... ...
        static long Counter;
        static long Counter1s;

        // Ref 2ms level.
        Counter++;
        if (Counter == 500) {
            if (Counter1s < 1000) {
                Counter1s ++;
            }
            Counter = 0;
        }

        if (Counter1s  > 142) {
            RunCmd = 0;
            Counter1s = 0;
        } else if (Counter1s  > 131) {
            RunCmd = 1;
        } else if (Counter1s  > 130) {
            RunCmd = 0;
        } else if (Counter1s  >= 1) {
            RunCmd = 1;
        } else {
            RunCmd = 0;
        }
	}
RunCmd = RunSignal;
    fwd = RunCmd;
    rev = 0;

	// 2. 根据运行命令给定方式，更新命令字runCmd
    runCmd.bit.freeStop = 0;    // 先清除 freeStop, pause, tune
    runCmd.bit.errorReset = 0;
    runCmd.bit.pause = 0;

    if (!((fwd && (!rev)) || ((!fwd) && (rev))))              // fwdRun
    {
    	runCmd.bit.common0 = 0;     // 停机
    	if (RUN_STATUS_TUNE == runStatus) {
    		runCmd.bit.tune = RUN_CMD_TUNE_NO;
    	}
    } else {
    	if (tuneCmd) {
			runCmd.bit.tune = tuneCmd;
		} else {
			if ((!fwd) && (rev)) {      	// revRun
				runCmd.bit.dir = REVERSE_DIR;
				runCmd.bit.common0 = 1;
			} else if (fwd && (!rev)) {      // fwdRun
				runCmd.bit.dir = FORWARD_DIR;
				runCmd.bit.common0 = 1;
			}
		}
    }

	// common run
    runCmd.bit.common = 0;
    if (runCmd.bit.common0)     
    {
        Uint16 common = 0;
        
        if (runFlag.bit.common) {
            common = 1;
        } else { 
        	// 准备启动,设定频率不低于下限频率
            if (ABS_INT32(frqAimTmp) >= lowerFrq) {
                common = 1;
            } else {
        //        if (0 != mainLoopTicker) {   // 进入主循环的第1拍，不必处理otherStopLowerSrc(固定的启动保护)
        //            otherStopLowerSrc++;
        //        }
                runCmd.bit.common0 = 0;     // 清运行命令
            }
        }

        if (common) {
            runCmd.bit.common = 1; 			// 真正有效的运行命令
        }
    }

	// // 性能正在上电处理，不能发送运行/点动命令
    if (errorCode|| errorCodeFromCore || (POWER_ON_WAIT == powerOnStatus))
    {
        runCmd.bit.freeStop = 1;          	// 若是恢复出厂参数，仅仅是为了不响应运行命令
    }

    if ((runCmd.bit.freeStop)       		// 故障，自由停车端子，恢复出厂参数
        && (!runCmd.bit.pause)      		// 运行暂停引起的自由停车，不清运行命令
        )
    {
        runCmd.bit.common0 = 0;     		// common run
        runCmd.bit.common  = 0;
        runCmd.bit.tune    = 0;        		// 调谐时，自由停车有效
    }

//    bRunJog = 0;
}

void RunSrcDeal(void)
{
	// 加减速时间计算
    AccDecTimeCalc();

	// 更新运行命令
    UpdateRunCmd();
    
    if (runCmd.bit.freeStop)   // 故障码，端子自由停车，通讯自由停车，停机方式为自由停车，正在恢复出厂参数。
    {
        runStatus = RUN_STATUS_SHUT_DOWN;
    }
    else if (runCmd.bit.tune)
    {
//        if (RUN_STATUS_WAIT == runStatus) // 等待启动
//        {
//            runStatus = RUN_STATUS_TUNE;
//            tuneRunStatus = TUNE_RUN_STATUS_INIT;
//        }
    }
    else if (runCmd.bit.common)
    {
        if (RUN_STATUS_WAIT == runStatus) // 等待启动
        {
            runStatus = RUN_STATUS_START;
            startRunStatus = START_RUN_STATUS_INIT;
        }
    }
    
	// 根据运行方向,运行方式(点动还是普通运行),计算设定频率(目标频率frqAim).
	// 包括方向处理.
    UpdateFrqSetAim();

    frqCurAim = frqAim;      // 直流制动之前，就给性能传递 

	// runStatus处理
    if (1)      // 没有进入瞬停不停，才处理runStatus
    {
        switch (runStatus)
        {
            case RUN_STATUS_START:  // 启动
                StartRunCtrl();
                break;

            case RUN_STATUS_TUNE:   // 调谐运行
            	TuneRunCtrl();
                break;

            case RUN_STATUS_DI_BRAKE:  // 制动
                frqTmp = 0;
                runFlag.bit.run = 1;
                runFlag.bit.common = 1;
                dspMainCmd.bit.run = 1;
                dspMainCmd.bit.stopBrake = 1;
                break;
                
            default:
                break;
        }

        if (RUN_STATUS_NORMAL == runStatus) // normal run
        {
            NormalRunCtrl();
        }
    }

    if (RUN_STATUS_STOP == runStatus)       // 停机
    {
        StopRunCtrl();
    }

    if (RUN_STATUS_SHUT_DOWN == runStatus)  // shutdown, 关断
    {
        ShutDownRunCtrl();
    }

	// 给真正的frq赋值
    frq = frqTmp;

	// 给性能传递命令
    dspMainCmd.bit.motorCtrlMode = 0; 	                        // 保留。只用pm_control_mode
    dspMainCmd.bit.accDecStatus = runFlag.bit.accDecStatus; 	// 根据runFlag的加减速标志，给定dspMainCmd的加减速标志
    
    dspSubCmd.bit.softPWM = 0;                                  // 随机PWM使能
    dspSubCmd.bit.varFcByTem = 1;                      			// 载波频率随温度调整，280F一直有效
    dspSubCmd.bit.motorType = 0;                                // 0-普通AC；1-变频AC
    dspSubCmd.bit.overloadMode = 0;                 			// 电机过载保护
    dspSubCmd.bit.inPhaseLossProtect = 0;     					// 输入缺相保护
    dspSubCmd.bit.outPhaseLossProtect = 1;   					// 输出缺相保护
    dspSubCmd.bit.poffTransitoryNoStop = 0; 					// 瞬停不停
    dspSubCmd.bit.overModulation = 0;                           // 过调制使能
    dspSubCmd.bit.cbc = 1;                                   	// 逐波限流功能使能标志
    dspSubCmd.bit.loseLoadProtectMode = 0;   					// 输出掉载保护使能标志
    dspSubCmd.bit.fan = 0;  									// 风扇控制
}

//=====================================================================
//
// 启动控制
//
//=====================================================================
LOCALF void StartRunCtrl(void)
{
    runFlag.bit.run = 1;
    runFlag.bit.common = 1;
    dspMainCmd.bit.run = 1;

    if (!runCmd.bit.common)     // 启动时，有停机命令
    {
        if (START_RUN_STATUS_HOLD_START_FRQ == startRunStatus)
        {
            runStatus = RUN_STATUS_STOP;
            stopRunStatus = STOP_RUN_STATUS_INIT;
        }
        else
            runStatus = RUN_STATUS_SHUT_DOWN;

        runTimeTicker = 0;  // ticker清零
        
        return;
    }

    switch (startRunStatus)
    {
        case START_RUN_STATUS_SPEED_TRACK:
			dspMainCmd.bit.speedTrack = 1;
			if (dspStatus.bit.speedTrackEnd)    // 转速跟踪完成
			{
                dspMainCmd.bit.speedTrack = 0;
                if (DCStart_flag) {
                    startRunStatus = START_RUN_STATUS_BRAKE;
                } else {
                    runStatus = RUN_STATUS_NORMAL;
                    frqTmp = frqRun;
                }
			}
		break;

        case START_RUN_STATUS_BRAKE:
            ++runTimeTicker;
            if ((runTimeTicker >= (long)DCBrakeTime * (100 / RUN_CTRL_PERIOD)))
            {
                runTimeTicker = 0;
                DCStart_flag = 0;
                dspMainCmd.bit.startBrake = 0;
                dspMainCmd.bit.SecondPhase = 0;
                dspMainCmd.bit.LastPhase = 0;
                runStatus = RUN_STATUS_NORMAL;

//                pm_integral_d=0;pm_integral_q=0;pm_ref_id =0;
            } else {
                if ((runTimeTicker >= (long)DCBrakeTime * (100 / RUN_CTRL_PERIOD))) {
                    dspMainCmd.bit.LastPhase = 1;
//                } else if ((runTimeTicker >= 10 * (100 / RUN_CTRL_PERIOD))) {
//                    dspMainCmd.bit.SecondPhase = 1;
///////////////////////////////////////////////////////////////////////////////
                } else {
                    dspMainCmd.bit.startBrake = 1;
                }
            }
        break;

        case START_RUN_STATUS_HOLD_START_FRQ:
        break;

        default:
        break;
    }
}

//=====================================================================
//
// normal运行控制
//
//=====================================================================
LOCALF void NormalRunCtrl(void)
{
    if (!runCmd.bit.common) // 运行中有停机命令
    {        
        runStatus = RUN_STATUS_STOP;
        stopRunStatus = STOP_RUN_STATUS_INIT;
        return;
    }

	// 是否正反转
    if (runFlag.bit.dirReversing) // 正在反向
    {
        frqCurAim = 0;
    } else {
        LowerThanLowerFrqDeal();
    }

	// 加减速
    AccDecFrqCalc(accFrqTime, decFrqTime, FUNCCODE_accDecSpdCurve_LINE);
}

//=====================================================================
//
// 停机控制
//
//=====================================================================
LOCALF void StopRunCtrl(void)
{
    int32 tmp;
#ifndef MOTOR_TYPE_REFR_BLDC
    int32 frqTmpAbs;
    int32 tmpAbs;
#endif

    if (runCmd.bit.common)     // 停机时，有启动命令
    {
        if (STOP_RUN_STATUS_WAIT_BRAKE == stopRunStatus)
        {
            //dspMainCmd.bit.run = 1; 不必要
            runStatus = RUN_STATUS_START; //+= 由于停机直流制动等待时间是关断PWM，所以重新开始整个过程
            startRunStatus = START_RUN_STATUS_INIT;
        }
        else
        {
            dspMainCmd.bit.stopBrake = 0;
            runStatus = RUN_STATUS_NORMAL;
        }

        runTimeTicker = 0;  // ticker清零

        return;
    }
    
    switch (stopRunStatus)
    {
        case STOP_RUN_STATUS_DEC_STOP:  // 减速停车
            frqCurAim = 0;              // 目标频率为0
            AccDecFrqCalc(accFrqTime, decFrqTime, FUNCCODE_accDecSpdCurve_LINE);
            
            tmp = frqRun;

#ifdef MOTOR_TYPE_REFR_BLDC
			// 功能给定频率为0，且性能给定的实际转速小于1Hz，关断, 暂时不考虑停不下来的情况
			if ((!frqTmp) && (!tmp)) {
#else
			// 冰箱压缩机，选择自由停机。
			if (frqTmp < 0) {
				frqTmpAbs = -frqTmp;
			} else {
			    frqTmpAbs = frqTmp;
			}
			if (tmp < 0) {
				tmpAbs = -tmp;
			} else {
			    tmpAbs = tmp;
			}
			if ((tmpAbs < (200UL << 0)) && (frqTmpAbs < (200UL << 0))) {
#endif
				runStatus = RUN_STATUS_SHUT_DOWN;
			}
//test BEM
runStatus = RUN_STATUS_SHUT_DOWN;
			break;

        case STOP_RUN_STATUS_WAIT_BRAKE:    // 停机直流制动等待
        case STOP_RUN_STATUS_BRAKE:     	// 直流制动
        break;

        default:
        break;
    }
}

//=====================================================================
//
// 调谐控制
//
//=====================================================================
LOCALF void TuneRunCtrl(void)
{
#if 0
    if (RUN_CMD_TUNE_NO == runCmd.bit.tune)
	{
		runStatus = RUN_STATUS_SHUT_DOWN;

		return;
	}

	switch (tuneRunStatus)
	{
		case TUNE_RUN_STATUS_WAIT:
			if (++runTimeTicker >= TUNE_STEP_OVER_TIME_MAX * (TIME_UNIT_MS_PER_SEC / RUN_CTRL_PERIOD))
			{
				errorOther = 19;
				break;
			}
			runFlag.bit.run = 1;
			runFlag.bit.tune = 1;

			dspMainCmd.bit.run = 1;
			dspMainCmd.bit.staticTune = 1;
			if (RUN_CMD_TUNE_WHOLE == tuneCmd)
			{
				dspMainCmd.bit.fullTune = 1;
			}

			if (dspStatus.bit.tuneStepOver)
			{
				if (RUN_CMD_TUNE_STATIC == tuneCmd)
				{
					tuneRunStatus = TUNE_RUN_STATUS_END;
				}
				else
				{
					tuneRunStatus = TUNE_RUN_STATUS_ACC;
					runTimeTicker = 0;
				}
			}
			break;

		case TUNE_RUN_STATUS_ACC:
			if (++runTimeTicker >= TUNE_OVER_TIME_MAX * (TIME_UNIT_MS_PER_SEC / RUN_CTRL_PERIOD))
			{
				errorOther = 19;
				break;
			}
			frqCurAim = 8000;
			AccDecFrqCalc(accFrqTime, decFrqTime, FUNCCODE_accDecSpdCurve_LINE);

			if (dspStatus.bit.tuneOver)
			{
				tuneRunStatus = TUNE_RUN_STATUS_DEC;
			}

			break;

		case TUNE_RUN_STATUS_DEC:
			frqCurAim = 0;
			AccDecFrqCalc(accFrqTime, decFrqTime, FUNCCODE_accDecSpdCurve_LINE);
			if (!frqTmp)
			{
				tuneRunStatus = TUNE_RUN_STATUS_END;
			}
			break;

		case TUNE_RUN_STATUS_END:
			dspStatus.bit.tuneDelay = 1;
			dspMainCmd.bit.TuneOver = 1;

			tuneFinshed = 1;
			AutotuneParaSaveTriger = 88;
			runStatus = RUN_STATUS_SHUT_DOWN;
			break;

		default:
			break;
	}
#endif
}


//=====================================================================
// 
// 关断PWM控制
//
//=====================================================================
LOCALF void ShutDownRunCtrl(void)
{
    dspMainCmd.bit.run = 0;
    dspMainCmd.bit.speedTrack = 0;
    dspMainCmd.bit.stopBrake = 0;
    dspMainCmd.bit.startBrake = 0;
    dspMainCmd.bit.accDecStatus = 0;
    dspMainCmd.bit.fullTune = 0;
    dspMainCmd.bit.staticTune = 0;
    
    runCmd.bit.tune = 0;
    tuneCmd = 0;

    {
//        Uint16 pause = runCmd.bit.pause;

//        if (runFlag.bit.common)    // common运行停机，包括故障停机
//        {
//            if (!pause)                                 // 非运行暂停引起的停机
//            {
//                stopRemFlag = STOP_REM_WAIT;            // 停机标志为1
//            }
//            else
//            {
//                stopRemFlag = STOP_REM_PAUSE_JUDGE;     // 运行之后，运行暂停了。
//            }
//        }

        runTimeTicker = 0;      // ticker清零
        lowerDelayTicker = 0;
        shuntTicker = 0;

        runFlag.bit.run = 0;    // 只清一部分标志
        runFlag.bit.common = 0;
        runFlag.bit.accDecStatus = 0;
        
        frqTmp = 0;      // 运行频率清零

        frqCurAimOld = 0;   // S曲线使用
        frqLine.remainder = 0;  // 清零

        runStatus = RUN_STATUS_WAIT; // 停机完成，等待再次启动
    }
}

//=====================================================================
//
// 运行过程中低于下限频率处理
//
//=====================================================================
LOCALF void LowerThanLowerFrqDeal(void)
{

}

void SetAccTimes(short AccTimeIn001S)
{
    // 加减速时间, 对于冰箱选择1S 加速率.
    accFrqTime = AccTimeIn001S;
    decFrqTime = AccTimeIn001S;
}

void AccDecTimeCalc(void)
{
	// 加减速时间, 对于冰箱选择1S 加速率.
#ifdef MOTOR_TYPE_REFR_BLDC
	accFrqTime = 100;
#else
	// may be not right.
//	accFrqTime = 500;//ACC_SET;
#endif

    //decFrqTime = DEC_SET;
}

LINE_CHANGE_STRUCT frqLine = LINE_CHANGE_STRTUCT_DEFALUTS;
int32 frqCurAimOld;
void AccDecFrqCalc(int32 accTime, int32 decTime, Uint16 mode)
{
    int32 accDecTime;
    int32 frq0 = frqTmp;
    static int32 maxValue;

    if (frqCurAimOld != frqCurAim)  // 目标频率发生改变
    {
        frqCurAimOld = frqCurAim;
    }

    if (frqCurAim == frq0)                        	// 恒速
    {
        runFlag.bit.accDecStatus = CONST_SPEED;
    }
    else
    {
        if (((frq0 >= 0) && (frqCurAim > frq0))
            || ((frq0 <= 0) && (frqCurAim < frq0))) // 加速
        {
            runFlag.bit.accDecStatus = ACC_SPEED;
            accDecTime = accTime;
            maxValue = ABS_INT32(frqCurAim);   		// 加速时为本次的设定频率
        }
        else                    					// 减速
        {
            runFlag.bit.accDecStatus = DEC_SPEED;
            accDecTime = decTime;
        }
        
        accDecTime = accDecTime * (TIME_UNIT_ACC_DEC_SPEED / RUN_CTRL_PERIOD); // 转换成ticker
        // if (0 == funcCode.code.accDecTimeBase)   // 加减速时间的基准频率。0-最大频率，1-设定频率
        // {
        //     maxValue = maxFrq;
        // }
        /////////////////////////////////////////
		maxValue = maxFrq;

        if (FUNCCODE_accDecSpdCurve_LINE == mode) // 直线加减速
        {
            frqLine.aimValue = frqCurAim;
            frqLine.tickerAll = accDecTime;
            frqLine.maxValue = maxValue;
            frqLine.curValue = frq0;
            frqLine.calc(&frqLine);
            frq0 = frqLine.curValue;
        }
    }

    frqTmp = frq0;
}

//=====================================================================
//
// 加减速直线计算
//
//=====================================================================
void LineChangeCalc(LINE_CHANGE_STRUCT *p)
{
    int32 delta;

    if (!p->tickerAll)
    {
        p->curValue = p->aimValue;
    }
    else
    {
        delta = ((int32)p->maxValue + p->remainder) / p->tickerAll;
        p->remainder = ((int32)p->maxValue + p->remainder) % p->tickerAll;

        if (p->aimValue > p->curValue)
        {
            p->curValue += delta;
            if (p->curValue > p->aimValue) {
                p->curValue = p->aimValue;
            }
        }
        else
        {
            p->curValue -= delta;
            if (p->curValue < p->aimValue) {
                p->curValue = p->aimValue;
            }
        }
    }
}
