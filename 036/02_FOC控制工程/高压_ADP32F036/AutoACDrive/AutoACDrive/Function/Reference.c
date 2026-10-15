
#include "Define.h"
#include "FrqSrc.h"
#include "Main.h"
#include "RunSrc.h"
#include "IO.h"
#include "Parameter.h"
#include "SubPrgInclude.h"
#include "MotorInclude.h"
#include "param_lr.h"

int32 frq;
int32 frqTmp;
int32 frqAim;
int32 frqAimTmp;
int32 frqAimTmp0;
int32 frqCurAim;

Uint32 upperFrq;
Uint16 lowerFrq;
Uint32 maxFrq;
Uint16 upperTorque;


void UpdateLimitFrq(void);
void TorqueCalc(void);


void FrqSrcDeal(void)
{
	// 上下限频率计算
    UpdateLimitFrq();

	// 转矩上限计算
    TorqueCalc();

    frqAimTmp0 = (long)RefSet * 100L / 60 * 4;//8500 / 60 * 4

    // 针对每一次运行，而不是第一次上电... ...
    static long Counter;
    static long Counter10ms;
    static long Counter10ms_1;

    // Ref 2ms level.
    if (runFlag.bit.common && (  DCStart_flag == 0 ) \
                            && (gMainStatus.RunStep == 5)) {
        Counter++;
        if (Counter == 5) {
            if (Counter10ms < 50000) {
                Counter10ms ++;
                Counter10ms_1 ++;
            }
            Counter = 0;
        }
    } else {
        Counter10ms = 0;
        Counter = 0;
        Counter10ms_1 = 0;
    }

#ifndef OPEN_LOOP_START

    if(Counter10ms < 100)
    {
        SetAccTimes(ACC1);
    }
    else
    {
        SetAccTimes(ACC2);
    }

#else
    if (uvGainWarpTune == OPEN_ACC || uvGainWarpTune == HALF_OPEN_ACC) {
        SetAccTimes(1500);
    } else {
        SetAccTimes(1500);
    }
#endif

    // 判断是否超过上限频率
    if (frqAimTmp0 > upperFrq) {
        frqAimTmp0 = upperFrq;
    } else if (frqAimTmp0 < -(int32)upperFrq) {
        frqAimTmp0 = -(int32)upperFrq;
	}

	// 赋值给frqAimTmp
    frqAimTmp = frqAimTmp0;
}

void UpdateFrqSetAim(void)
{
	// 运行方向
    if (FORWARD_DIR != runCmd.bit.dir) {
        frqAimTmp = -frqAimTmp;
    }   

	// 判断是否超过上限频率
    if ((frqAimTmp > upperFrq)&&(frqAimTmp>0)) {
        frqAimTmp = upperFrq;
    } else if (frqAimTmp < -(int32)upperFrq) {
        frqAimTmp = -(int32)upperFrq;
	}

	// 赋值给frqAim
    frqAim = frqAimTmp;
}

void TorqueCalc(void)
{
    upperTorque = TORQUE_CUR_LIMIT;
}

void UpdateLimitFrq(void)
{
    maxFrq = MAX_LIMIT_FREQ;
    upperFrq = MAX_LIMIT_FREQ;
    lowerFrq = LOWER_LIMIT_FREQ;
}
