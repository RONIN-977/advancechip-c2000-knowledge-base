
#include "Define.h"
#include "Reference.h"
#include "Main.h"
#include "RunSrc.h"
#include "Parameter.h"
#include "LinLDF.h"
#include "MotorInclude.h"

int32 frq;              // 瞬时值, 功能传递给性能的运行频率
int32 frqTmp;           // 计算frq的临时变量
int32 frqAim;           // 设定(目标)频率
int32 frqAimTmp;        // 计算frqAim的临时变量, 下一次计算不会使用。跳跃频率之后，点动频率之前。
int32 frqAimTmp0;       // 计算frqAim的临时变量, 下一次计算会使用。频率源设定之后，跳跃频率之前。
int32 frqCurAim;        // 当前目标频率，注意在每次调用AccDecFrqCalc()之前要更新frqCurAim

Uint32 upperFrq;        // 上限频率
Uint16 lowerFrq;        // 下限频率
Uint32 maxFrq;          // 最大频率
Uint16 upperTorque;     // 速度控制时为转矩上限，转矩控制时为设定转矩
long   SpeedRefAcc;

Uint16 MaxRPM_SET = 7000;
Uint16 MinRPM_SET = 50;

void UpdateLimitFrq(void);
void TorqueCalc(void);

extern long pm_est_omg;
extern long pm_ref_speed;

unsigned int FrqTest[20];
void FrqSrcDeal(void)
{
	// 上下限频率计算
    UpdateLimitFrq();

	// 转矩上限计算
    TorqueCalc();

	// 频率源计算
    if (1) {
    	frqAimTmp0 = RefSet;//GetSetupFreComs();
    	FbackFry = RefSet/10;
    } else {
    	unsigned int FreqSet = AdcResult.ADCRESULT3;
    	static unsigned long FreqSetFilter = 0;
    	static unsigned long FreTemp;

    	FreqSetFilter = FreqSetFilter - (FreqSetFilter >> 5) + (FreqSet >> 5);

    	FreTemp = ((unsigned long)FreqSetFilter * upperFrq) >> 12;
    	FreTemp = ((unsigned long)FreTemp * 4140) >> 12;

    	if (FreTemp > upperFrq) {
    		FreTemp = upperFrq;
    	} else if (FreTemp < lowerFrq) {
    		FreTemp = lowerFrq;
    	}
    	frqAimTmp0 = FreTemp;
    }

    {
        // 针对每一次运行，而不是第一次上电... ...
        static long Counter;
        static long Counter1s;

        // Ref 2ms level.
        if (runFlag.bit.common) {
            Counter++;
            if (Counter == 500) {
                if (Counter1s < 5000) {
                    Counter1s ++;
                }
                Counter = 0;
            }
        } else {
            Counter1s = 0;
            Counter = 0;
        }
#ifndef OPEN_LOOP_START
        if (Counter1s < 2) {
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
    }


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

extern unsigned long IbusActual;
unsigned long ActualPower = 0;
long PoeError,PoePout,PoeIout,PoePIout,IntegralPoe;
unsigned int PowerCoff = 2540;
void POE_Limit(long PowerValve,long PowerKp,long PowerKi)
{
#if 0
    static unsigned int FilterRatio;
    static long FilterRatioAcc;

    static unsigned int FilterOutVolt;
    static long FilterOutVoltAcc;

    static unsigned int FilterOutCur;
    static long FilterOutCurAcc;

    FilterOutVoltAcc  += ((((long)outVoltage << 16) - FilterOutVoltAcc) >> 6);
    FilterOutVolt = (FilterOutVoltAcc + 0x8000) >> 16;

    FilterOutCurAcc  += ((((long)outCurrent << 16) - FilterOutCurAcc) >> 6);
    FilterOutCur = (FilterOutCurAcc + 0x8000) >> 16;

    FilterRatio = ((long)FilterOutVolt * generatrixVoltage)/ 310;
    FilterRatio = (long)FilterRatio * 380 >> 12;
    FilterRatio = (long)FilterRatio * FilterOutCur >> 11;
    //FilterRatio = (long)FilterRatio * 502 >> 10;
    FilterRatio = (long)FilterRatio * PowerCoff >> 10;

    FilterRatioAcc  += ((((long)FilterRatio << 16) - FilterRatioAcc) >> 6);
    FilterRatio = (FilterRatioAcc + 0x8000) >> 16;

    ActualPower = FilterRatio;
    PoeError = ActualPower - PowerValve;
    PoeIout = PoeError * PowerKi;
    PoePout = PoeError * PowerKp;
    IntegralPoe += (PoeIout>>6);
    if(IntegralPoe >= 32000)
       IntegralPoe = 32000;
    else if(IntegralPoe <= -32000)
       IntegralPoe = -32000;
    PoePIout = (PoePout>>6) + (IntegralPoe>>5);
    if(PoePIout >= 1000)
       PoePIout =  1000;
    else if(PoePIout <= -1000)
       PoePIout =  -1000;
    //frqAimTmp0 -= PoePIout
#else
    ActualPower = (long)gUDC.uDCFilter * IbusActual;
    ActualPower = ActualPower/100;//0.1W

    PoeError = PowerValve -ActualPower;
    //PoeError = ActualPower - PowerValve;
    PoeIout = (PoeError * PowerKi);
    PoePout = (PoeError * PowerKp);
    IntegralPoe += (PoeIout>>6);

    if(IntegralPoe >= 384000)
       IntegralPoe = 384000;
    else if(IntegralPoe <= 0)
       IntegralPoe = -0;

    PoePIout = (PoePout>>6) + (IntegralPoe>>5);
    if(PoePIout >= 12000)
       PoePIout =  12000;
    else if(PoePIout <= 0)
       PoePIout =  0;
   // frqAimTmp0 -= PoePIout;
    //pm_ref_iq = PoePIout;

#endif

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
