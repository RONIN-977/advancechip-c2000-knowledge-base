#ifndef __F_COMMAND_H__
#define __F_COMMAND_H__

#include "ADP32F03x_Device.h"

// 加减速
#define CONST_SPEED     0       // 恒速
#define ACC_SPEED       1       // 加速
#define DEC_SPEED       2       // 减速

// 加减速方式
#define FUNCCODE_accDecSpdCurve_LINE        0   // 直线加减速
#define FUNCCODE_accDecSpdCurve_S_CURVE_A   1   // S曲线1，普通二次方
#define FUNCCODE_accDecSpdCurve_S_CURVE_B   2   // S曲线2，参考三菱S曲线B

#define TIME_UNIT_ACC_DEC_SPEED         10      // 加减速时间单位, ms

//=====================================================================
// runCmd，运行命令字
//
#define FORWARD_DIR         0   // 正方向
#define REVERSE_DIR         1   // 反方向

#define RUN_CMD_NO_JOG      0   // 无点动命令
#define RUN_CMD_FWD_JOG     1   // 正向点动
#define RUN_CMD_REV_JOG     2   // 反向点动
#define RUN_CMD_FWD_REV_JOG 3   // 既有正向点动命令，又有反向点动命令

#define RUN_CMD_TUNE_NO     0   // 没有调谐命令
#define RUN_CMD_TUNE_STATIC 1   // 静态调谐命令
#define RUN_CMD_TUNE_WHOLE  2   // 完全调谐命令
struct RUN_CMD_BITS
{                               // bits  description
    Uint16 common0:1;           // 0     +, 普通运行命令(非点动、调谐)，中间值，且保留下次使用
    Uint16 common:1;            // 1     -, 逻辑处理之后的普通运行命令(非点动、调谐)
    Uint16 jog:2;               // 3:2   -, 0-no jog, 1-jog
    Uint16 tune:2;              // 5:4   -, 0-no tune; 1-static tune; 2-whole tune
    Uint16 dir:1;               // 6     +, 0-fwd, 1-rev. 表示运行方向，不包括点动方向
    Uint16 pause:1;             // 7     -, 运行暂停
    Uint16 freeStop:1;          // 8     -,
    Uint16 hurryStop:1;         // 9     ?, 目前暂未使用
    Uint16 otherStop:1;         // 10    +, 其他情况的停机/非默认停机，强制保护
    Uint16 startProtect:1;      // 11    +, 启动保护
    Uint16 errorReset:1;        // 12    -, 故障复位
    Uint16 rsvd:3;              // 15:13
};

union RUN_CMD
{
    Uint16              all;
    struct RUN_CMD_BITS bit;
};

extern union RUN_CMD runCmd;

//=====================================================================


//=====================================================================
// runFlag, 变频器运行过程中的状态字
//
struct RUN_FLAG_BITS
{                               // bits  description
    Uint16 run:1;               // 0    (总的)运行标志
    Uint16 common:1;            // 1    普通运行(非点动、非调谐)
    Uint16 jog:1;               // 2    点动运行
    Uint16 tune:1;              // 3    调谐运行
    Uint16 jogWhenRun:1;        // 4    运行中点动
    Uint16 accDecStatus:2;      // 6:5  0 恒速； 1 加速； 2 减速

    // 之下的bit位在shutdown时不要清除
    Uint16 plc:1;               // 7     PLC运行
    Uint16 pid:1;               // 8     PID运行
    Uint16 torque:1;            // 9     转矩控制
    Uint16 dir:1;               // 10    设定频率方向(功能码F0-12运行方向之前), 0-fwd, 1-rev
    Uint16 curDir:1;            // 11    当前运行频率方向, 0-fwd, 1-rev
    Uint16 dirReversing:1;      // 12    正在反向标志, 0-当前没有反向, 1-正在反向
    Uint16 dirFinal:1;          // 13    设定频率方向(功能码F0-12运行方向之后), 0-fwd, 1-rev
    Uint16 rsvd:2;              // 15:14
};

union RUN_FLAG
{
    Uint16               all;
    struct RUN_FLAG_BITS bit;
};

extern union RUN_FLAG runFlag;


//=====================================================================
// dspMainCmd, 转递给性能的主命令字
//
struct DSP_MAIN_COMMAND_BITS
{                               // bits  description
    Uint16 run:1;               // 0,    1:run 0:stop
    Uint16 speedTrack:1;        // 1,    speed track
    Uint16 stopBrake:1;         // 2,    STOP brake
    Uint16 fullTune:1;          // 3,    full tune
    Uint16 motorCtrlMode:2;     // 5:4   00-SVC, 01-VC, 10-VF
    Uint16 startBrake:1;        // 6,    start brake
    Uint16 staticTune:1;        // 7,    simple tune
    Uint16 pgDirRev:1;          // 8,    1:PG rev, 0:PG fwd.  码盘方向反向标志（未用）
    Uint16 errorDealing:1;      // 9,    1:ERROR TALK, 功能正在进行故障处理
    Uint16 accDecStatus:2;      // 11:10 0 恒速； 1 加速； 2 减速. //! 目前转差补偿使用了该标志
    Uint16 shortGnd:2;          // 13:12 上电对地短路检测标志
    Uint16  SecondPhase:1;
    Uint16  LastPhase:1;
};

union DSP_MAIN_COMMAND
{
    Uint16                       all;
    struct DSP_MAIN_COMMAND_BITS bit;
};

extern union DSP_MAIN_COMMAND dspMainCmd;
//=====================================================================


//=====================================================================
// dspSubCmd, 转递给性能的辅命令字
//
struct DSP_SUB_COMMAND_BITS
{                                       // bits  description
   Uint16    softPWM:1;                 // 0,    随机PWM使能
   Uint16    varFcByTem:1;              // 1,    载波频率随温度调整
   Uint16    motorType:1;               // 2,    0-普通AC；1-变频AC
   Uint16    overloadMode:1;            // 3,    电机过载保护使能
   Uint16    inPhaseLossProtect:1;      // 4,    输入缺相保护
   Uint16    outPhaseLossProtect:1;     // 5,    输出缺相保护
   Uint16    poffTransitoryNoStop:1;    // 6,    瞬停不停使能
   Uint16    overModulation:1;          // 7,    过调制使能
   Uint16    fanRunWhenWaitStopBrake:1; // 8,    停机直流制动等待时间内风扇运行标志
   Uint16	 cbc:1;                     // 9,    逐波限流功能使能标志
   Uint16    loseLoadProtectMode:1;     // 10,   输出掉载保护使能标志
   Uint16    fan:1;                     // 11    风扇控制，功能码可以设置为上电后一直转动
   Uint16    rsvd:4;                    // 15:12 保留
};

//=====================================================================

//=====================================================================
// 性能传递给功能的状态字
//
struct DSP_STATUS_BITS
{                                   // bits  description
   Uint16    tuneStepOver:1;        // 0     参数辨识步骤标志
   Uint16    tuneOver:1;            // 1     参数辨识结束标志
   Uint16    speedTrackEnd:1;       // 2     转速跟踪结束标志
   Uint16    torqueLimit:1;         // 3     转矩限定生效标志
   Uint16    run:1;                 // 4     运行/停机状态标志
   Uint16    curADErr:1;            // 5     电流检测故障标志
   Uint16    uv:1;                  // 6     母线电压欠压故障标志
   Uint16    inverterPreOl:1;       // 7     变频器过载预报警标志
   Uint16    motorPreOl:1;          // 8     电机过载预报警标志
   Uint16    fan:1;                 // 9     风扇运行标志，主轴伺服使用；其他保留
   Uint16    tuneDelay:1;           // 10    参数辨识结束后延时完成
   Uint16    pdpLow:1;              // 11    已经有一次PDP生效标志
   Uint16    shortGndOver:1;        // 12    对地短路检测完毕标志
   Uint16    outAirSwitchOff:1;     // 13    变频器输出空开断开标志，即掉载标志
   Uint16    runEnable:2;		    // 15:14 11-初始化完成，可以运行标志
};

union DSP_STATUS {
   Uint16                   all;
   struct DSP_STATUS_BITS   bit;
};

extern union DSP_STATUS dspStatus;
//-----------------------------------------------------


//-----------------------------------------------------
// runStatus，当前运行状态/步骤
//
enum RUN_STATUS
{
    RUN_STATUS_WAIT,        // 等待启动
    RUN_STATUS_START,       // 启动
    RUN_STATUS_NORMAL,      // (正常)运行
    RUN_STATUS_STOP,        // 停机
    RUN_STATUS_JOG,         // 点动运行

    RUN_STATUS_SERVO,       // 伺服控制
    
    RUN_STATUS_TUNE,        // 调谐运行
    RUN_STATUS_DI_BRAKE,    // DI端子的直流制动(非启动直流制动和停机直流制动)
    RUN_STATUS_LOSE_LOAD,   // 掉载运行
    RUN_STATUS_SHUT_DOWN    // shut down, 关断
};
extern enum RUN_STATUS runStatus;
//-----------------------------------------------------

//-----------------------------------------------------
enum START_RUN_STATUS
{
    START_RUN_STATUS_SPEED_TRACK,       // 转速跟踪
    START_RUN_STATUS_BRAKE,             // 启动直流制动
    START_RUN_STATUS_HOLD_START_FRQ     // 启动频率保持
};
#define START_RUN_STATUS_INIT           START_RUN_STATUS_SPEED_TRACK
extern enum START_RUN_STATUS startRunStatus;
//-----------------------------------------------------

//-----------------------------------------------------
enum STOP_RUN_STATUS
{
    STOP_RUN_STATUS_DEC_STOP,           // 减速停车
    STOP_RUN_STATUS_WAIT_BRAKE,         // 停机直流制动等待
    STOP_RUN_STATUS_BRAKE               // 停机直流制动
};
#define STOP_RUN_STATUS_INIT            STOP_RUN_STATUS_DEC_STOP
extern enum STOP_RUN_STATUS stopRunStatus;


// 直线变化的计算：加减速，PID给定的计算
// 已知从 0 到 最大值maxValue 的变化时间为 tickerAll，
// 每次新的计算，remainder应该清零，但是影响很小。
//
typedef struct
{
    void (*calc)(void *);       // 函数指针

    int32 maxValue;             // 最大值
    int32 aimValue;             // 目标值
    int32 curValue;             // 当前值

    Uint32 tickerAll;           // 从0到最大值的ticker
    int32 remainder;            // 计算delta的余值
} LINE_CHANGE_STRUCT;

#define LINE_CHANGE_STRTUCT_DEFALUTS       \
{                                          \
    (void (*)(void *))LineChangeCalc       \
}
//-----------------------------------------------------

extern Uint32 accFrqTime;
extern Uint32 decFrqTime;

extern Uint16 tuneCmd;
extern int32 frqCurAimOld;
extern LINE_CHANGE_STRUCT frqLine;


// function
void UpdateRunCmd(void);
void RunSrcDeal(void);
void AccDecTimeCalc(void);
void LineChangeCalc(LINE_CHANGE_STRUCT *p);
void AccDecFrqCalc(int32 accTime, int32 decTime, Uint16 mode);
void SetAccTimes(short AccTimeIn001S);


#endif  // __F_COMMAND_H__

