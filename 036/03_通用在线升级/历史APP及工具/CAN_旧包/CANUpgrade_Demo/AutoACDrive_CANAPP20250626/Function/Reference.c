
#include "Define.h"
#include "FrqSrc.h"
#include "Main.h"
#include "RunSrc.h"
#include "IO.h"
#include "Parameter.h"
#include "MotorInclude.h"

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
    // 针对每一次运行，而不是第一次上电... ...
    static long Counter = 0;
    static long Counter1s = 0;
    static int Flag = 0;

    // Ref 2ms level.
    if(runFlag.bit.common)
    {
        Counter++;
        if (Counter == 500)
        {
            if (Counter1s < 5000)
            {
                Counter1s ++;
            }
            Counter = 0;
        }
    }
    else
    {
        Flag = 0;
        Counter1s = 0;
        Counter = 0;
    }

    if((Counter1s < 20) && (Flag == 0))
    {
        frqAimTmp0 = 1000;
    }
    else
    {
        Flag = 1;
        frqAimTmp0 = RefSet;
    }

#ifndef OPEN_LOOP_START
    if (Counter1s < 3) {
        SetAccTimes(ACC1);
    } else {
        SetAccTimes(ACC2);
    }
#else
    if (uvGainWarpTune == OPEN_ACC || uvGainWarpTune == HALF_OPEN_ACC) {
        SetAccTimes(1500);
    } else {
        SetAccTimes(1500);
    }
#endif

    // 上下限频率计算
    UpdateLimitFrq();

    // 转矩上限计算
    TorqueCalc();

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
    upperTorque = TORQUE_CUR_LIMIT * 10;
}

void UpdateLimitFrq(void)
{
    maxFrq = MAX_LIMIT_FREQ;
    upperFrq = MAX_LIMIT_FREQ;
    lowerFrq = LOWER_LIMIT_FREQ;
}
