
#include "Define.h"
#include "Reference.h"
#include "Main.h"
#include "RunSrc.h"
#include "Parameter.h"
#include "LinLDF.h"
#include "MotorInclude.h"

int32 frq;
int32 frqTmp;
int32 frqAim;
int32 frqAimTmp;
int32 frqAimTmp0;
int32 frqCurAim;

Uint32 upperFrq;        // 上限频率
Uint16 lowerFrq;        // 下限频率
Uint32 maxFrq;          // 最大频率
Uint16 upperTorque;     // 速度控制时为转矩上限，转矩控制时为设定转矩


void UpdateLimitFrq(void);
void TorqueCalc(void);

void FrqSrcDeal(void)
{
	// 上下限频率计算
    UpdateLimitFrq();

	// 转矩上限计算
    TorqueCalc();

	// 频率源计算
    frqAimTmp0 = RefSet;//GetSetupFreComs();

#ifndef OPEN_LOOP_START
    if (TimeAccumulator.count_1s < 5) {
        SetAccTimes(3500);//ACC1);
    } else {
        SetAccTimes(ACC2);
    }
#else
    if (gMainStatus.RunStep == OPEN_ACC || gMainStatus.RunStep == HALF_OPEN_ACC) {
        SetAccTimes(ACC1);
    } else {
        SetAccTimes(ACC1);
    }
#endif

    // 判断是否超过上限频率
    if (frqAimTmp0 > (int32)upperFrq) {
        frqAimTmp0 = (int32)upperFrq;
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
    if ((frqAimTmp > upperFrq)&&(frqAimTmp > 0)) {
        frqAimTmp = upperFrq;
    } else if (frqAimTmp < -(int32)upperFrq) {
        frqAimTmp = -(int32)upperFrq;
	}
	// 赋值给frqAim
    frqAim = frqAimTmp;
}

//long PowerLimitData[10];
void POE_Limit(long PowerValve,long PowerKp,long PowerKi, long *PowerLimitOut)
{
    long PowerError;
    static long LastPowerError = 0;
    long PowerErrorDiff;
    static long PowerIntergral = 0;
    int PowerLimitIqHigh;
    int PowerLimitIqLow;
    long P_Term, I_Term, D_Term,Output;

    PowerLimitIqHigh = 4000;
    PowerLimitIqLow  = 0;

    // 功率误差
    PowerError = ActualPower - PowerValve;
    PowerError = __IQsat(PowerError, 1500L, -1500L);    // 100.0W

    // 计算误差变化率（近似微分）
    PowerErrorDiff = PowerError - LastPowerError;
    LastPowerError = PowerError;

    // 比例项
    P_Term = ((long long)PowerError * PowerKp) >> 8;

    // 对变化率进行限幅和平滑
    PowerErrorDiff = __IQsat(PowerErrorDiff, 200L, -200L);  // 限制变化率

    PowerIntergral = PowerIntergral + (PowerError * (long)PowerKi);

    // 积分限幅
    PowerIntergral = __IQsat(PowerIntergral,PowerLimitIqHigh * 256L,PowerLimitIqLow * 256L);

    // 积分项
    I_Term = (long)PowerIntergral >> 8;

    // 微分项
    D_Term = ((long long)PowerErrorDiff * PowerKp) >> 8;

    // 总和
    Output = I_Term + P_Term + D_Term;

    // 输出限幅
    Output = __IQsat(Output,PowerLimitIqHigh,PowerLimitIqLow);

    *PowerLimitOut = Output;
}

void TorqueCalc(void)
{
#if 0
    int TorqueSet;
    if (generatrixVoltage > 3000) {
        TorqueSet = 10240;
    } else if (generatrixVoltage < 2600) {
        TorqueSet = 8000;
    } else {
        // TorqueSet = ((long)(generatrixVoltage - 2600) * 224) / 400;
        TorqueSet = ((long)(generatrixVoltage - 2600) * 5730) >> 10;
        TorqueSet += 8000;
        if (TorqueSet > 10240) {
            TorqueSet = 10240;
        } else if (TorqueSet < 8000) {
            TorqueSet = 8000;
        }
    }
    TorqueSet = ((long)TorqueSet * TorqueLimitSet) >> 10;
    static long TorqueSetAcc = 1024UL << 16;
    TorqueSetAcc = ((((int32)TorqueSet << 16) - TorqueSetAcc) >> 5) + TorqueSetAcc;
    TorqueSet  = (TorqueSetAcc + 0x8000) >> 16;

	upperTorque = TorqueSet;
#else
    // 110V供电时限流值大
    upperTorque = TORQUE_CUR_LIMIT * 10;
#endif
}

void UpdateLimitFrq(void)
{
    maxFrq = MAX_LIMIT_FREQ;
    upperFrq = MAX_LIMIT_FREQ;
    lowerFrq = LOWER_LIMIT_FREQ;
}
