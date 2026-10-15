
#include "Define.h"
#include "RunSrc.h"
#include "FrqSrc.h"
#include "IO.h"
#include "Main.h"
#include "MotorInclude.h"
#include "Parameter.h"

union RUN_CMD runCmd;                   // 运行命令字
union RUN_FLAG runFlag;                 // 运行标识字
union DSP_MAIN_COMMAND dspMainCmd;      // 功能传递给性能的主命令字
union DSP_STATUS dspStatus;             // 性能传递给功能的状态字
enum RUN_STATUS runStatus;              // 运行状态
enum START_RUN_STATUS startRunStatus;   // 启动时的运行状态
enum STOP_RUN_STATUS stopRunStatus;     // 停机时的运行状态
TIME_STRUCT TimeAccumulator;
Uint32 accFrqTime;                      // 加速时间，单位同功能码
Uint32 decFrqTime;                      // 减速时间
Uint32 runTimeTicker;                   // 运行时间计时


void StartRunCtrl(void);                // 启动
void NormalRunCtrl(void);
void StopRunCtrl(void);
void ShutDownRunCtrl(void);

int RunSignal;
void UpdateRunCmd(void)
{
	static unsigned char RunCmd = 0;
    Uint16 fwd, rev;

#if 0
    if (TimeAccumulator.count_1s  >= 19) {
        RunCmd = 1;
    } else if (TimeAccumulator.count_1s  >= 18) {
        RunCmd = 0;
    } else if (TimeAccumulator.count_1s  >= 3) {
        RunCmd = 1;
    } else {
        RunCmd = 0;
    }
#else
	RunCmd = RunSignal;
#endif
    fwd = RunCmd;
    rev = 0;

	// 2. 根据运行命令给定方式，更新命令字runCmd
    runCmd.bit.freeStop = 0;    // 先清除 freeStop, pause, tune

    if (!((fwd && (!rev)) || ((!fwd) && (rev))))                // fwdRun
    {
    	runCmd.bit.common0 = 0;                                 // 停机
    } else {
        if ((!fwd) && (rev)) {      	                        // revRun
            runCmd.bit.dir = REVERSE_DIR;
            runCmd.bit.common0 = 1;
        } else if (fwd && (!rev)) {                             // fwdRun
            runCmd.bit.dir = FORWARD_DIR;
            runCmd.bit.common0 = 1;
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
                runCmd.bit.common0 = 0;                         // 清运行命令
            }
        }

        if (common) {
            runCmd.bit.common = 1; 			                    // 有效的运行命令
        }
    }

	// // 性能正在上电处理，不能发送运行/点动命令
    if (errorCode|| errorCodeFromCore || (POWER_ON_WAIT == powerOnStatus))
    {
        runCmd.bit.freeStop = 1;
    }

    if (runCmd.bit.freeStop)
    {
        runCmd.bit.common0 = 0;
        runCmd.bit.common  = 0;
    }
}

void RunSrcDeal(void)
{
	// 加减速时间计算
    AccDecTimeCalc();
    UpdateTimeAccumulator(&TimeAccumulator);

	// 更新运行命令
    UpdateRunCmd();
    
    if (runCmd.bit.freeStop)
    {
        runStatus = RUN_STATUS_SHUT_DOWN;
    } else if (runCmd.bit.common) {
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

    //    switch (runStatus)
    //    {
    //        case RUN_STATUS_START:    StartRunCtrl();   break;
    //        case RUN_STATUS_NORMAL:   NormalRunCtrl();  break;
    //        case RUN_STATUS_STOP:     StopRunCtrl();    break;
    //        case RUN_STATUS_SHUT_DOWN:ShutDownRunCtrl();break;
    //        default:                                    break;
    //    }
    ////////////////////////////////////////////////////////////
    // 第一步：处理 START 状态（可能改变 runStatus）
    if (RUN_STATUS_START == runStatus)
    {
        StartRunCtrl();
    }

    // 第二步：根据当前 runStatus（可能已被 StartRunCtrl 修改）执行对应的控制函数
    switch (runStatus)
    {
        case RUN_STATUS_NORMAL:   NormalRunCtrl();   break;
        case RUN_STATUS_STOP:     StopRunCtrl();     break;
        case RUN_STATUS_SHUT_DOWN:ShutDownRunCtrl(); break;
        default:                                     break;
    }

	// 给真正的frq赋值
    frq = frqTmp;

	// 给性能传递命令
    dspMainCmd.bit.motorCtrlMode = 0;
    dspMainCmd.bit.accDecStatus = runFlag.bit.accDecStatus;
}

//=====================================================================
//
// 启动控制
//
//=====================================================================
void StartRunCtrl(void)
{
    runFlag.bit.run = 1;
    runFlag.bit.common = 1;
    dspMainCmd.bit.run = 1;

    if (!runCmd.bit.common)     // 启动时，有停机命令
    {
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
			frqTmp = speedMotor;                // 更新当前跟踪的频率
		break;

        case START_RUN_STATUS_BRAKE:
            ++runTimeTicker;
             if ((runTimeTicker >= 20L * (100 / RUN_CTRL_PERIOD)))
             {
                 runTimeTicker = 0;
                 DCStart_flag = 0;
                 dspMainCmd.bit.startBrake = 0;
                 dspMainCmd.bit.SecondPhase = 0;
                 dspMainCmd.bit.LastPhase = 0;
                 runStatus = RUN_STATUS_NORMAL;
             } else {
                 if ((runTimeTicker >= 20L * (100 / RUN_CTRL_PERIOD))) {
                     dspMainCmd.bit.LastPhase = 1;
                } else if ((runTimeTicker >= (DCBrakeTime >> 1) * (100 / RUN_CTRL_PERIOD))) {
                    dspMainCmd.bit.SecondPhase = 1;
                 } else {
                     dspMainCmd.bit.startBrake = 1;
                 }
             }
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
void NormalRunCtrl(void)
{
    if (!runCmd.bit.common) // 运行中有停机命令
    {        
        runStatus = RUN_STATUS_STOP;
        stopRunStatus = STOP_RUN_STATUS_INIT;
        return;
    }

	// 加减速
    AccDecFrqCalc(accFrqTime, decFrqTime, FUNCCODE_accDecSpdCurve_LINE);
}

//=====================================================================
//
// 停机控制
//
//=====================================================================
void StopRunCtrl(void)
{
    int32 tmp;
#ifndef MOTOR_TYPE_REFR_BLDC
    int32 frqTmpAbs;
    int32 tmpAbs;
#endif

    if (runCmd.bit.common)     // 停机时，有启动命令
    {
        runStatus = RUN_STATUS_NORMAL;
        runTimeTicker = 0;  // ticker清零
        return;
    }
    
    switch (stopRunStatus)
    {
    case STOP_RUN_STATUS_DEC_STOP:  // 减速停车
        frqCurAim = 0;              // 目标频率为0
        AccDecFrqCalc(accFrqTime, decFrqTime, FUNCCODE_accDecSpdCurve_LINE);
#if 1
        tmp = speedMotor;

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
        if ((tmpAbs <= COAST_STOP_FRQ) && (frqTmpAbs <= COAST_STOP_FRQ)) {
            runStatus = RUN_STATUS_SHUT_DOWN;
#else
            runStatus = RUN_STATUS_SHUT_DOWN;
#endif
        }
        break;

        default:
        break;
    }
}

//=====================================================================
// 
// 关断PWM控制
//
//=====================================================================
void ShutDownRunCtrl(void)
{
    dspMainCmd.bit.run = 0;
    dspMainCmd.bit.speedTrack = 0;
    dspMainCmd.bit.startBrake = 0;
    dspMainCmd.bit.accDecStatus = 0;
    dspMainCmd.bit.SecondPhase = 0;
    
    runTimeTicker = 0;              // ticker清零
    runFlag.bit.run = 0;            // 只清一部分标志
    runFlag.bit.common = 0;
    runFlag.bit.accDecStatus = 0;

    frqTmp = 0;                     // 运行频率清零
    frqLine.remainder = 0;          // 清零

    runStatus = RUN_STATUS_WAIT;    // 停机完成，等待再次启动
}

void SetAccTimes(short AccTimeIn001S)
{
    // 加减速时间, 对于冰箱选择1S 加速率.
    accFrqTime = AccTimeIn001S;
}

void AccDecTimeCalc(void)
{
    decFrqTime = ACC1;
}

LINE_CHANGE_STRUCT frqLine = LINE_CHANGE_STRTUCT_DEFALUTS;
void AccDecFrqCalc(int32 accTime, int32 decTime, Uint16 mode)
{
    int32 accDecTime;
    int32 frq0 = frqTmp;
    static int32 maxValue;

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
