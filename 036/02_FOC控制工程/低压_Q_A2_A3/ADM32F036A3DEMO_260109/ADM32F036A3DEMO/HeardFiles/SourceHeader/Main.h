#ifndef __F_MAIN_H__
#define __F_MAIN_H__


#include "ADP32F03x_Device.h"
#include "Interface.h"


#define ABS_INT16(a) (((a) >= 0) ? (a) : (-(a)))
#define ABS_INT32(a) (((a) >= 0L) ? (a) : (-(a)))
#define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

#define RUN_CTRL_PERIOD         2       // 2MS 程序处理周期，_ms

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

// 功能传递给性能的数据
extern Uint16 frq2Core;
extern Uint16 frqCurAim2Core;

extern Uint16 ratingVoltage;
extern Uint16 ratingCurrent;
extern Uint16 ratingFrq;
extern Uint16 RunStatus;

// 性能传递给功能的数据
extern Uint16 errorCodeFromCore;
extern Uint16 * const pDataCore2Func[];

extern Uint16 errAutoRstNum;
extern Uint16 bUv;
extern unsigned short ResetCmd;

extern Uint16 ObserverMotorSpeedINMaxScalingQ15Format;
extern int32  ObserverMotorSpeedINOnePointHZ;

extern void CommsProtocalLayerTask2ms(void);
extern void ADP32F03x_usDelay(Uint32 Count);


#endif // __F_MAIN_H__

