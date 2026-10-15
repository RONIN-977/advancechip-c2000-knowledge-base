
#ifndef __F_FRQSRC_H__
#define __F_FRQSRC_H__


#include "FuncCode.h"

#define ACC_DEC_FRQ_NONE    0
#define ACC_DEC_FRQ_WAIT    1
#define ACC_DEC_FRQ_DONE    2

#define PLC_STEP_MAX    8   // PLC、多段速段数

typedef struct
{
// input
    Uint16 frq;     // 跳跃频率
    Uint16 range;   // 跳跃频率幅度

// output
    Uint32 low;     // 跳跃频率范围的low
    Uint32 high;    // 跳跃频率范围的high
} JUMP_FRQ;


// 功能部分使用的PID
typedef struct
{
    void  (*calc)(void *);  // Pointer to calculation function

    int32  ref;             // Input: Reference input, Q15
    int32  fdb;             // Input: Feedback input, Q15

    int32  Kp;              // Parameter: Proportional gain
    int32  Ki;              // Parameter: Longegral gain
    int32  KiMin;           // Parameter: Longegral gain
    int32  Kd;              // Parameter: Derivative gain

    int16  Qp;              // Kp的Q格式
    int16  Qi;              // Ki的Q格式
    int16  Qd;              // Kd的Q格式

    int32  deltaMax;        // Q15, 两次输出之间的限幅
    int32  deltaMin;        // Q15
    int32  outMax;          // Q15
    int32  outMin;          // Q15
    int32  errorDead;       // Parameter: 小于偏差极限，则PID不调节, Q15
    int32  errorSmall;      // 保留，变积分使用
    int32  errorBig;        // 保留，变积分使用

    int32  error;           // Variable: Error, Q15, 注意可能超过0xffff
    int32  error1;
    int32  error2;

    int32  delta;           // 两次输出之间的差值

    int32  deltaPRem;       // P计算的Remainder
    int32  deltaIRem;       // I计算的Remainder
    int32  deltaDRem;       // D计算的Remainder
    int32  deltaRemainder;  // 两次输出之间限幅之后的余值，保留下次计算使用

    int32  out;             // Output: PID output, Q15
} PID_FUNC;


#define PID_FUNC_DEFAULTS         \
{                                 \
    (void (*)(void *))PidFuncCalc \
}

extern PID_FUNC pidFunc;

extern Uint16 bResetFrq;

extern int32 frq;
extern int32 frqTmp;
extern int32 frqAim;
extern int32 frqAimTmp;
extern int32 frqAimTmp0;
extern int32 frqCurAim;
extern Uint16 frqKeyUpDownDelta;
extern Uint32 upperFrq;
extern Uint16 lowerFrq;
extern Uint32 maxFrq;
extern Uint16 plcStep;
extern Uint32 plcTime;
extern Uint16 bPlcEndOneLoop;
extern Uint16 plcStepRemOld;
extern Uint32 plcTimeRemOld;

extern Uint16 upperTorque;

extern int32 frqDigitalTmp;
extern Uint16 bFrqDigital;

extern Uint16 frqSrcOld;

extern Uint16 bStopPlc;
extern Uint16 bAntiReverseRun;

extern int32 frqAimOld4Dir;

extern Uint16 bDormant;

void FrqSrcDeal(void);
void UpdateFrqSetAim(void);
void PidFuncCalc(PID_FUNC *);

extern Uint16 frqSrc;
extern Uint16 plcStepOld;
int32 UpdateMultiStepFrq(Uint16 step);

#endif // __F_FRQSRC_H__
