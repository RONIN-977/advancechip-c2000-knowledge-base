#ifndef __F_MAIN_H__
#define __F_MAIN_H__


#include "DSP2803x_Device.h"
#include "Interface.h"


#define ABS_INT16(a) (((a) >= 0) ? (a) : (-(a)))
#define ABS_INT32(a) (((a) >= 0L) ? (a) : (-(a)))

#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

#define DoNothing()                 // 空处理

#define  GetTime() 	(CpuTimer1.RegsAddr->TIM.all)


// 程序中一些模块的调用周期
#define FUNC_DEAL_PERIOD        2       // 功能程序处理周期，_ms
#define RUN_CTRL_PERIOD         2       // 命令源程序RunControl()处理周期，_ms
//#define COMM_DEAL_PERIOD        2       // 通讯处理的周期
#define ERROR_DEAL_PERIOD       2       // 故障处理errorDeal()处理周期，_ms
#define FRQ_SRC_PERIOD          2       // 频率源程序frqSrc()处理周期，_ms
#define PULSE_IN_CALC_PERIOD    2       // 脉冲输入的计算周期
#define AI_CALC_PERIOD          2       // AI的计算周期
#define DI_CALC_PERIOD          2       // AI的计算周期
#define DO_CALC_PERIOD          2       // AI的计算周期
#define PID_CALC_PERIOD         2       // PID的计算周期



#define MAX_UINT32  0xFFFFFFFFUL      // 32bit的无符号最大值
#define MAX_UINT16  0xFFFF          // 16bit的无符号最大值 

// 变频器机型，1-7， 2相
// 变频器机型，>= 8，3相
// 性能传递的母线电压含有一位小数点
#define RATING_UDC_TWO_PHASE            3111    // 2相220V输入时的标准母线电压, 220*2^0.5*10
#define RATING_UDC_THREE_PHASE          5374    // 3相380V输入时的标准母线电压, 380*2^0.5*10
#define RATING_UDC_690                  9758    // 3相380V输入时的标准母线电压, 690*2^0.5*10
#define UV_POINT_TWO_PHASE              2000    // 两相220V输入时，欠压点
#define UV_POINT_THREE_PHASE            3500    // 三相380V输入时，欠压点
#define INVERTER_TYPE_THREE_PHASE_START 8       // 3相的起始机型
#define INVERTER_TYPE_BIG_ACC_DEC_START 21      // 加减速时间出厂值为大值的起始机型
#define INVERTER_TYPE_690_START         41      // 690V的起始机型
#define INVERTER_TYPE_1140_START        70                  // 1140V的起始机型
#define INVERTER_TYPE_PARA_NUMBER       6       // 与机型相关的功能码，放在INVERTER_TYPE_RELATED_PARA中的个数

typedef union
{
    Uint16 all[INVERTER_TYPE_PARA_NUMBER];

    struct
    {
        Uint16 carrierFrq;          // F0-11   载波频率

        Uint16 ratingPower;         // F1-00   电机额定功率
        Uint16 ratingCurrent;       // F1-02   电机额定电流
        Uint16 zeroLoadCurrent;     // F1-15   空载电流
        Uint16 statorResistance;    // F1-16   定子电阻
        Uint16 antiVibrateGain;     // F1-17   振荡抑制增益
    } element;
} INVERTER_TYPE_RELATED_PARA;


enum POWER_ON_STATUS
{
    POWER_ON_WAIT,              // 等待上电准备OK。
    POWER_ON_CORE_OK,           // (性能)上电准备OK。母线电压建立完毕，上电对地短路检测完毕
    POWER_ON_FUNC_WAIT_OT       // 功能等待时间超时。功能的等待时间超过_时间，性能上电准备还没有完毕。
};

enum {
	LENGTH_DATA_FRAME	= 25
};

extern enum POWER_ON_STATUS powerOnStatus;
extern unsigned short (* const TransmitArray[LENGTH_DATA_FRAME])(void);
extern void (* const ReciveArray[LENGTH_DATA_FRAME])(short);
extern unsigned short ParaSaveTriger;
extern unsigned short AutotuneParaSaveTriger;

// 功能传递给性能的数据
extern Uint16 frq2Core;
extern Uint16 frqCurAim2Core;
extern Uint16 vfCurve;
extern Uint16 torqueBoost;
extern Uint16 slipCompCoef;
extern Uint16 antiVibrateGain;
extern Uint16 statorResistance;
extern Uint16 zeroLoadCurrent;
extern Uint16 ratingPower;
extern Uint16 ratingVoltage;
extern Uint16 ratingCurrent;
extern Uint16 ratingFrq;
extern Uint16 MoupperFrq;                                   // 性能上限频率
extern Uint16 speedMotor;

extern Uint16 FbackFry;                                     // 输出反馈频率
extern Uint16 OutputTorque;                                 // 输出转矩计算
// 性能传递给功能的数据
extern Uint16 errorCodeFromCore;
extern Uint16 currentOc;
extern Uint16 generatrixVoltage;
extern Uint16 outVoltage;
extern Uint16 outCurrent;
extern Uint16 outPower;
extern Uint16 HeatSink;

extern Uint16 uvGainWarpTune;
extern Uint16 statorResistanceTune;
extern Uint16 rotorResistanceTune;
extern Uint16 leakInductanceTune;
extern Uint16 mutualInductanceTune;
extern Uint16 zeroLoadCurrentTune;
extern Uint16 rsvdData;

extern long PMSM_LD_SET;
extern long PMSM_LQ_SET;
extern long PMSM_RS_SET;
/*
extern int D_CUR_KI_SET;
extern int D_CUR_KP_SET;
extern int Q_CUR_KI_SET;
extern int Q_CUR_KP_SET;
*/
extern int32 frqRun;
extern Uint16 * const pDataCore2Func[];

extern void DSP28x_usDelay(Uint32 Count);

extern Uint16 errorEeprom;
extern Uint16 errorCode;
extern Uint16 errAutoRstNum;
extern Uint16 bUv;
extern Uint16 errorOther;
extern Uint16 invRatingVoltage;
extern unsigned short ResetCmd;

#define MOTOR_SN_1  0   // 第1电机
#define MOTOR_SN_2  1   // 第2电机

extern Uint16 motorSn;
extern Uint16 mainLoopTicker;

extern void CommsProtocalLayerTask2ms(void);

extern unsigned short GetKP(void);
extern unsigned short GetKI(void);
extern void SendKP(short Value);
extern void SendKI(short Value);

extern unsigned short GetCurrentLmit(void);
extern void SendCurrentLmit(short Value);

extern unsigned short GetRS(void);
extern unsigned short GetLD(void);
extern unsigned short GetLQ(void);
extern void SendRS(short Value);
extern void SendLD(short Value);
extern void SendLQ(short Value);

extern unsigned short GetCurrentKPD(void);
extern unsigned short GetCurrentKPQ(void);
extern unsigned short GetCurrentKID(void);
extern unsigned short GetCurrentKIQ(void);
extern void SendCurrentKPD(short Value);
extern void SendCurrentKPQ(short Value);
extern void SendCurrentKID(short Value);
extern void SendCurrentKIQ(short Value);

extern unsigned short GetMaxRPM(void);
extern void SendMaxRPM(short Value);
extern unsigned short GetMinRPM(void);
extern void SendMinRPM(short Value);

extern unsigned short GetAccelRate(void);
extern void SendAccelRate(short Value);
extern unsigned short GetDecelRate(void);
extern void SendDecelRate(short Value);

extern unsigned short GetSF(void);
extern void SendSF(short Value);

extern unsigned short GetMotorType(void);
extern void SendMotorType(short Value);
extern unsigned short GetKE(void);
extern void SendKE(short Value);

extern unsigned short GetSoftOC(void);
extern void SendSoftOC(short Value);

extern unsigned short GetHeatSink(void);


#endif // __F_MAIN_H__

