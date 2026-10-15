/******************************************************************************
 * File Name    : Command.c
 * Description  : Implements run-command processing,
 *                operating-state control,
 *                acceleration/deceleration management,
 *                linear ramp generation,
 *                and S-curve frequency reference calculation.
 *
 * Main Functions:
 *   - Update and validate run commands
 *   - Manage start, normal-run, stop, and shutdown states
 *   - Calculate acceleration and deceleration frequency references
 *   - Generate linear and S-curve speed ramps
 *
 * Notes:
 *   - Frequency and time calculations use fixed-point/integer arithmetic.
 *   - The module is executed periodically according to RUN_CTRL_PERIOD.
 *   - Ramp parameters and operating limits are defined in the associated
 *     parameter and header files.
 ******************************************************************************/

#include "Define.h"
#include "RunSrc.h"
#include "Reference.h"
#include "Parameter.h"
#include "Main.h"
#include "MotorInclude.h"
#include "SubPrgInclude.h"

/***************************************************************************
  Public Variables
 ***************************************************************************/
union RUN_CMD           runCmd;
union RUN_FLAG          runFlag;
union DSP_MAIN_COMMAND  dspMainCmd;
union DSP_STATUS        dspStatus;
enum RUN_STATUS         runStatus;
TIME_STRUCT             TimeAccumulator;

static void LineChangeCalc(LINE_CHANGE_STRUCT *p);

LINE_CHANGE_STRUCT frqLine =
{
    (void (*)(void *))LineChangeCalc,
    0,
    0,
    0,
    0,
    0
};

static enum START_RUN_STATUS startRunStatus;
static enum STOP_RUN_STATUS  stopRunStatus;
static Uint32 accFrqTime;
static Uint32 decFrqTime;
static int32  frqCurAimOld;
static int32  accTimeOld;
static int32  decTimeOld;
static Uint32 runTimeTicker;

#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
static Uint16 bFrqCurAimChg;
#endif

/***************************************************************************
  Private Functions
 ***************************************************************************/
static void UpdateRunCmd(void);
static void StartRunCtrl(void);
static void NormalRunCtrl(void);
static void StopRunCtrl(void);
static void ShutDownRunCtrl(void);
static void AccDecTimeCalc(void);
static void AccDecFrqCalc(int32 accTime, int32 decTime);
#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
static int32 SCurveOridinaryCalc(int32 frq0, int32 accDecTime);
#endif


int Rungisngle;
static void UpdateRunCmd(void)
{
	static unsigned char RunCmd = 0;
    Uint16 fwd, rev;

	// OFF ~　DI state is 1.
    if (0) {
        if (1) { // GetSetupFreComs() > 100 && GetStartCMDComs() == 1) {
            RunCmd = 1;
        } else {
            RunCmd = 0;
        }
    } else {
        if (TimeAccumulator.count_1s  >= 15) {
            RunCmd = 1;
        } else if (TimeAccumulator.count_1s  >= 14) {
            RunCmd = 0;
        } else if (TimeAccumulator.count_1s  >= 1) {
            RunCmd = 1;
        } else {
            RunCmd = 0;
        }
	}
RunCmd = Rungisngle;
    fwd = RunCmd;
    rev = 0;

	// 2. 根据运行命令给定方式，更新命令字runCmd
    runCmd.bit.freeStop = 0;                                    // 先清除 freeStop, pause, tune
    if (!((fwd && (!rev)) || ((!fwd) && (rev))))                // fwdRun
    {
    	runCmd.bit.common0 = 0;             // 停机
    } else {
        if ((!fwd) && (rev)) {      	    // revRun
            runCmd.bit.dir = REVERSE_DIR;
            runCmd.bit.common0 = 1;
        } else if (fwd && (!rev)) {         // fwdRun
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
                runCmd.bit.common0 = 0;     // 清运行命令
            }
        }
        if (common) {
            runCmd.bit.common = 1; 			// 真正有效的运行命令
        }
    }

	// 性能正在上电处理，不能发送运行/点动命令
    if (gMainStatus.ErrorCode || (POWER_ON_WAIT == powerOnStatus))
    {
        runCmd.bit.freeStop = 1;          	// 若是恢复出厂参数，仅仅是为了不响应运行命令
    }

    if (runCmd.bit.freeStop)      		    // 运行暂停引起的自由停车，不清运行命令
    {
        runCmd.bit.common0 = 0;     		// common run
        runCmd.bit.common  = 0;
    }
}

void RunSrcDeal(void)
{
    AccDecTimeCalc();
    UpdateTimeAccumulator(&TimeAccumulator);
    UpdateRunCmd();
    
    if (runCmd.bit.freeStop)   // 故障码，各种自由停车，通讯自由停车，停机方式为自由停车
    {
        runStatus = RUN_STATUS_SHUT_DOWN;
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
    switch (runStatus)
    {
        case RUN_STATUS_START:    StartRunCtrl();   break;
        case RUN_STATUS_NORMAL:   NormalRunCtrl();  break;
        case RUN_STATUS_STOP:     StopRunCtrl();    break;
        case RUN_STATUS_SHUT_DOWN:ShutDownRunCtrl();break;
        default:                                    break;
    }

	// 给真正的frq赋值
    frq = frqTmp;

	// 给性能传递命令
    dspMainCmd.bit.accDecStatus = runFlag.bit.accDecStatus; 	// 根据runFlag的加减速标志，给定dspMainCmd的加减速标志
}

static void StartRunCtrl(void)
{
    runFlag.bit.run = 1;
    runFlag.bit.common = 1;
    dspMainCmd.bit.run = 1;

#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
    bFrqCurAimChg = 1;
#endif

    if (!runCmd.bit.common)
    {
        runStatus = RUN_STATUS_SHUT_DOWN;
        runTimeTicker = 0;
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
                    frqTmp = 0;
                } else {
                    runStatus = RUN_STATUS_NORMAL;
                    frqTmp = ObserverMotorSpeedINOnePointHZ; // pm_est_lpf_omg1 / 161;//
                }
			}
		break;

        case START_RUN_STATUS_BRAKE:
            ++runTimeTicker;
            if ((runTimeTicker >= (int32)DCBrakeTime * (100 / RUN_CTRL_PERIOD)))
            {
                runTimeTicker = 0;
                DCStart_flag = 0;
                dspMainCmd.bit.startBrake = 0;
                dspMainCmd.bit.SecondPhase = 0;
                dspMainCmd.bit.LastPhase = 0;
                runStatus = RUN_STATUS_NORMAL;

                // pm_integral_d=0;
                // pm_integral_q=0;
                // pm_ref_id =0;
            } else {
                // Expression is always false because 'else if' condition matches previous condition at line 186 but seems as
                // Does't Matter anything!!!
                if ((runTimeTicker >= (int32)DCBrakeTime * (100 / RUN_CTRL_PERIOD))) {
                    dspMainCmd.bit.LastPhase = 1;
#if 0
                } else if ((runTimeTicker >= (DCBrakeTime >> 1) * (100 / RUN_CTRL_PERIOD))) {
                    dspMainCmd.bit.SecondPhase = 1;
#endif
                } else {
                    dspMainCmd.bit.startBrake = 1;
                }
            }
        break;

        default:
        break;
    }
}

static void NormalRunCtrl(void)
{
    if (!runCmd.bit.common) // 运行中有停机命令
    {
        runStatus = RUN_STATUS_STOP;
        stopRunStatus = STOP_RUN_STATUS_INIT;
        return;
    }

	// 是否正反转
    //    if (runFlag.bit.dirReversing) // 正在反向
    //    {
    //        frqCurAim = 0;
    //    } else {
    //        LowerThanLowerFrqDeal();
    //    }
    ///////////////////////////////////////////////
	// 加减速
    AccDecFrqCalc(accFrqTime, decFrqTime);
}

static void StopRunCtrl(void)
{
    if (runCmd.bit.common)      // 停机时，有启动命令
    {
        runStatus = RUN_STATUS_NORMAL;
        runTimeTicker = 0;      // ticker清零
        return;
    }

    switch (stopRunStatus)
    {
        case STOP_RUN_STATUS_DEC_STOP:  // 减速停车
            frqCurAim = 0;              // 目标频率为0
            AccDecFrqCalc(accFrqTime, decFrqTime);
			if (ABS_INT32(frqTmp) <= COAST_STOP_FRQ) {
                runStatus = RUN_STATUS_SHUT_DOWN;
			}
			  runStatus = RUN_STATUS_SHUT_DOWN;
		break;
        default:
        break;
    }
}

static void ShutDownRunCtrl(void)
{
    dspMainCmd.bit.run = 0;
    dspMainCmd.bit.speedTrack = 0;
    dspMainCmd.bit.startBrake = 0;
    dspMainCmd.bit.accDecStatus = 0;

    runTimeTicker = 0;          // ticker清零
    runFlag.bit.run = 0;        // 只清一部分标志
    runFlag.bit.common = 0;
    runFlag.bit.accDecStatus = 0;
    frqTmp = 0;                 // 运行频率清零
    frqCurAimOld = 0;           // S曲线使用
    accTimeOld = 0;             // S曲线使用
    decTimeOld = 0;             // S曲线使用
    frqLine.remainder = 0;      // 清零
    runStatus = RUN_STATUS_WAIT;// 停机完成，等待再次启动
}

void SetAccTimes(int16 AccTimeIn001S)
{
    // 加减速时间, 对于冰箱选择1S 加速率.
    accFrqTime = AccTimeIn001S;
    decFrqTime = DEC;
}

static void AccDecTimeCalc(void)
{

}

static void AccDecFrqCalc(int32 accTime, int32 decTime)
{
    int32 accDecTime;
    static int32 frq0;
    frq0 = frqTmp;
#if (SPEED_RAMP_MODE == SPEED_RAMP_LINEAR_MODE) // 直线加减速
    static int32 maxValue;
#endif
    if (frqCurAimOld != frqCurAim)              // 目标频率发生改变
    {
        frqCurAimOld = frqCurAim;
#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE) // 直线加减速
        bFrqCurAimChg = 1;
#endif
    }

    if (frqCurAim == frq0)                      // 恒速
    {
        runFlag.bit.accDecStatus = CONST_SPEED;
        frqLine.remainder = 0;                  // 上一条直线轨迹已完成，清除余数
    }
    else
    {
        if (((frq0 >= 0) && (frqCurAim > frq0)) ||
            ((frq0 <= 0) && (frqCurAim < frq0))) // 加速
        {
            runFlag.bit.accDecStatus = ACC_SPEED;
            accDecTime = accTime;
            if (accTimeOld != accTime)
            {
#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
                bFrqCurAimChg = 1;
#endif
            }
#if (SPEED_RAMP_MODE == SPEED_RAMP_LINEAR_MODE)     // 直线加减速
            maxValue = ABS_INT32(frqCurAim);   		// 加速时为本次的设定频率
#endif
        }
        else                    					// 减速
        {
            runFlag.bit.accDecStatus = DEC_SPEED;
            accDecTime = decTime;
            if (decTimeOld != decTime)
            {
#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
                bFrqCurAimChg = 1;
#endif
            }
        }
        
        accDecTime = accDecTime * (TIME_UNIT_ACC_DEC_SPEED / RUN_CTRL_PERIOD); // 转换成ticker
        // if (0 == funcCode.code.accDecTimeBase)   // 加减速时间的基准频率。0-最大频率，1-设定频率
        // {
        //     maxValue = maxFrq;
        // }
        /////////////////////////////////////////
#if (SPEED_RAMP_MODE == SPEED_RAMP_LINEAR_MODE) // 直线加减速
		maxValue = maxFrq;
        frqLine.aimValue = frqCurAim;
        frqLine.tickerAll = accDecTime;
        frqLine.maxValue = maxValue;
        frqLine.curValue = frq0;
        frqLine.calc(&frqLine);
        frq0 = frqLine.curValue;
#else
        frq0 = SCurveOridinaryCalc(frq0, accDecTime);
#endif
    }

#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
    bFrqCurAimChg = 0;
#endif
    decTimeOld = decTime;
    accTimeOld = accTime;
    frqTmp = frq0;
}

//=====================================================================
//
// 加减速直线计算
//
//=====================================================================
static void LineChangeCalc(LINE_CHANGE_STRUCT *p)
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

#if (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE)
enum S_CURVE_A_STATUS
{
    SA_STEP_START_S,
    SA_STEP_LINE,
    SA_STEP_END_S,
    SA_STEP_OK
};
static int32 SCurveOridinaryCalc(int32 frq0, int32 accDecTime)
{
    static enum S_CURVE_A_STATUS sCurveAStatus;
    static int32 ticker;
    static int32 t[3];      // S曲线起始段、直线段、S曲线结束段的时间, ticker
    static int32 a[3];      // S曲线起始段、直线段、S曲线结束段的系数
    static Uint16 q[3];     // a[]对应的Q格式
    static int64 remainder;
    int32 delta;
    int64 tmp;
    int32 tmp2;

    if (!accDecTime)    // 加减速时间为0
    {
        frq0 = frqCurAim;
        return frq0;
    }

    if (!maxFrq)        // 防止计算剩余时间时除0，参数异常时保持当前频率
    {
        return frq0;
    }

    Uint16 startTime;
    Uint16 lineTime;   // 直线段时间的2倍
    Uint16 endTime;
    startTime = 1;     // Must make sure that  Division by NOT zero!!!
    if (gMainStatus.RunStep == OPEN_ACC || gMainStatus.RunStep == HALF_OPEN_ACC) {
        endTime   = 1; // 避免结束段系数计算时除0
    } else {
        endTime   = 500;
    }
    lineTime = 2000 - startTime - endTime;

    if (bFrqCurAimChg)
    {
        int32 tickerAll;  // 整个阶段需要的时间，调用次数为单位

        int32 frqAimDelta;
        int64 tmp1;
        int64 tmp3;

        sCurveAStatus = SA_STEP_START_S;
        ticker = 0;
        remainder = 0;
        frqAimDelta = frqCurAim - frq0;
        if (!frqAimDelta)   // 已到达目标，无需重新生成S曲线
        {
            sCurveAStatus = SA_STEP_OK;
            return frq0;
        }
#define Q   16
        tmp3 = ((int64)accDecTime * ABS_INT32(frqAimDelta) << Q) / (int32)maxFrq;
        if (!tmp3)          // 避免后续以tmp3作除数
        {
            sCurveAStatus = SA_STEP_OK;
            return frqCurAim;
        }
        tickerAll = tmp3 >> Q;
        if (!tickerAll)     // 剩余时间不足一个控制周期，直接完成本次轨迹
        {
            sCurveAStatus = SA_STEP_OK;
            return frqCurAim;
        }
        t[0] = ((int64)tickerAll * startTime + 0) / 1000;
        t[2] = ((int64)tickerAll * endTime + 0) / 1000;
        t[1] = tickerAll - (t[0] + t[2]);

        q[0] = 20;  // 初始化a[0], a[1]对应的Q格式
        q[1] = 35;

        // 计算a[1], 直线段的一次项系数，同时更新对应的Q格式
        tmp = ((int64)maxFrq * 2000 << q[1]) / ((int64)accDecTime * lineTime);
        while (tmp >= 1L << 18)
        {
            tmp >>= 1;
            q[1]--;
        }
        a[1] = tmp;

        // 计算a[0]，S曲线起始段的二次项系数
        tmp1 = ((int64)a[1] * 500 << (q[0] + Q)) / tmp3;
#undef Q
        q[0] += q[1];
        q[2] = q[0];
        tmp = tmp1 / startTime;
        while (tmp >= 1L << 18)
        {
            tmp >>= 1;
            q[0]--;
        }
        a[0] = tmp;

        // 计算a[2]，S曲线结束段的二次项系数
        tmp = tmp1 / endTime;
        while (tmp >= 1L << 18)
        {
            tmp >>= 1;
            q[2]--;
        }
        a[2] = tmp;
    }

    if (sCurveAStatus <= SA_STEP_END_S)
    {
        if (ticker >= t[sCurveAStatus])     // 该段已经完成
        {
            ticker = 0; // 初始化成0
            sCurveAStatus++;

            // 先判断状态是否结束，再访问t[]，避免t[3]越界
            while ((SA_STEP_OK != sCurveAStatus) && (t[sCurveAStatus] == 0))
            {
                sCurveAStatus++;
            }

            remainder = 0;
        }

        if (SA_STEP_OK == sCurveAStatus) // 曲线结束，避免访问a[3]和q[3]
        {
            frq0 = frqCurAim;
            return frq0;
        }

        ticker++;

        if (SA_STEP_START_S == sCurveAStatus)
        {
            tmp2 = 2 * ticker - 1;
        }
        else if (SA_STEP_LINE == sCurveAStatus)
        {
            tmp2 = 1;
        }
        else //if (SA_STEP_END_S == sCurveAStatus)
        {
            tmp2 = 2 * t[2] - 2 * ticker + 1;     // 仔细推导，这里为+1, 而不是-1/0
        }

        tmp = (int64)tmp2 * a[sCurveAStatus] + remainder;
        delta = tmp >> q[sCurveAStatus];
        remainder = tmp - ((int64)delta << q[sCurveAStatus]);

        if (frq0 < frqCurAim)
        {
            frq0 += delta;
            if (frq0 > frqCurAim)   // 防止超过目标频率
            {
                frq0 = frqCurAim;
            }
        }
        else
        {
            frq0 -= delta;
            if (frq0 < frqCurAim)   // 防止超过目标频率
            {
                frq0 = frqCurAim;
            }
        }
    }

    if (SA_STEP_OK == sCurveAStatus)
    {
        frq0 = frqCurAim;
    }
    return frq0;
}
#endif /* (SPEED_RAMP_MODE != SPEED_RAMP_LINEAR_MODE) */
