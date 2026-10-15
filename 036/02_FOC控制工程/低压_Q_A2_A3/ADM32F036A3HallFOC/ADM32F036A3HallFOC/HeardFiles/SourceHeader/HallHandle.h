/*---------------------------------------------------------------------
  文件名           : HallHandle.h
  时间               : 2020.10.15
  描述 		: 霍尔传感器模块声明
CPU平台       : ADP32
  作者		: 湖南进芯电子科技有限公司
  版权		: V1.0 版权保留.          						   	
  控制对象	: PMSM | BLDCM		   									   	
----------------------------------------------------------------------*/

#ifndef _HallHandle_H_
#define _HallHandle_H_

//#include "speed_meas_hall.h"
//#include "angle_meas_hall.h"

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------
// Hall管脚定义
#define HA_IS_EFFECT()      (GpioDataRegs.GPBDAT.bit.GPIO34  == 1)
#define HB_IS_EFFECT()      (GpioDataRegs.GPADAT.bit.GPIO9 == 1)
#define HC_IS_EFFECT()      (GpioDataRegs.GPADAT.bit.GPIO1 == 1)

//---------------------------------------------------------------
// Hall相序宏定义（1#电机）
//#define ANGLE_OFFSET    2807124         // 3374526         // (33/360)*32767*2^10
//#define HALL_SWITCH_0   2//2
//#define HALL_SWITCH_1   6//3
//#define HALL_SWITCH_2   4//1
//#define HALL_SWITCH_3   5//5
//#define HALL_SWITCH_4   1//4
//#define HALL_SWITCH_5   3//6

// Hall相序宏定义（2#电机）
//#define ANGLE_OFFSET    2807124         // (33/360)*32767*2^10
//#define HALL_SWITCH_0   5
//#define HALL_SWITCH_1   4
//#define HALL_SWITCH_2   6
//#define HALL_SWITCH_3   2
//#define HALL_SWITCH_4   3
//#define HALL_SWITCH_5   1

// Hall相序宏定义（3#电机）
//#define ANGLE_OFFSET    2785279         // (33/360)*32767*2^10
//#define HALL_SWITCH_0   3
//#define HALL_SWITCH_1   1
//#define HALL_SWITCH_2   5
//#define HALL_SWITCH_3   4
//#define HALL_SWITCH_4   6
//#define HALL_SWITCH_5   2

// Hall相序宏定义（4#电机）
//#define ANGLE_OFFSET    2872660         // 3374526         // (33/360)*32767*2^10
//#define HALL_SWITCH_0   5
//#define HALL_SWITCH_1   4
//#define HALL_SWITCH_2   6
//#define HALL_SWITCH_3   2
//#define HALL_SWITCH_4   3
//#define HALL_SWITCH_5   1

// Hall相序宏定义（BM57L80电机）
//#define ANGLE_OFFSET    2807124         // 3374526         // (33/360)*32767*2^10
//#define HALL_SWITCH_0   2
//#define HALL_SWITCH_1   3
//#define HALL_SWITCH_2   1
//#define HALL_SWITCH_3   5
//#define HALL_SWITCH_4   4
//#define HALL_SWITCH_5   6

#define ANGLE_OFFSET    0         // 3374526         // (33/360)*32767*2^10
#define HALL_SWITCH_0   4
#define HALL_SWITCH_1   6
#define HALL_SWITCH_2   2
#define HALL_SWITCH_3   3
#define HALL_SWITCH_4   1
#define HALL_SWITCH_5   5

//---------------------------------------------------------------
// Hall切换点角度定义(360°对应2^25(为了提高精度用))
// 标准的30°、60°、90°……
//#define ANGLE_OFFSET    2796203         // (33/360)*32767*2^10
//#define ANGLE_0         0
//#define ANGLE_30        2796203
//#define ANGLE_60        5592405
//#define ANGLE_90        8388608
//#define ANGLE_120       11184811
//#define ANGLE_150       13981013
//#define ANGLE_180       16777216
//#define ANGLE_210       19573418
//#define ANGLE_240       22369621
//#define ANGLE_270       25165824
//#define ANGLE_300       27962027
//#define ANGLE_330       30758229
//#define ANGLE_360       33554431

// 1#电机
#define ANGLE_0         0
#define ANGLE_30        2660044
#define ANGLE_60        5320088         // 5592235
#define ANGLE_90        8224838
#define ANGLE_120       11129589        // 11184469
#define ANGLE_150       13955807
#define ANGLE_180       16782026        // 16776704
#define ANGLE_210       19442108
#define ANGLE_240       22102190        // 22368939
#define ANGLE_270       24978747
#define ANGLE_300       27855305        // 27961173
#define ANGLE_330       30704868
#define ANGLE_360       33554431

// 2#电机
//#define ANGLE_0         0
//#define ANGLE_30        2976581
//#define ANGLE_60        5867163         // 5592405  5592235
//#define ANGLE_90        8538936
//#define ANGLE_120       11140810        // 11184811 11184469
//#define ANGLE_150       14208538
//#define ANGLE_180       17259366        // 16777216 16776704
//#define ANGLE_210       19818555
//#define ANGLE_240       223476745        // 22369621 22368939
//#define ANGLE_270       25302978
//#define ANGLE_300       28216511        // 27962026 27961173
//#define ANGLE_330       30907821
//#define ANGLE_360       33554431

// 3#电机
//#define ANGLE_0         0
//#define ANGLE_30        2804907
//#define ANGLE_60        5609815         // 5592235
//#define ANGLE_90        8468130
//#define ANGLE_120       11326445        // 11184469
//#define ANGLE_150       13950487
//#define ANGLE_180       16574529        // 16776704
//#define ANGLE_210       19550109
//#define ANGLE_240       22525689        // 22368939
//#define ANGLE_270       25244704
//#define ANGLE_300       27963719        // 27961173
//#define ANGLE_330       30759075
//#define ANGLE_360       33554431

// 4#电机
//#define ANGLE_0         0
//#define ANGLE_30        2788703
//#define ANGLE_60        5577406         // 5592235
//#define ANGLE_90        8307819
//#define ANGLE_120       11038233        // 11184469
//#define ANGLE_150       13732081
//#define ANGLE_180       16425929        // 16776704
//#define ANGLE_210       19299416
//#define ANGLE_240       22172903        // 22368939
//#define ANGLE_270       24999867
//#define ANGLE_300       27826832        // 27961173
//#define ANGLE_330       30690631
//#define ANGLE_360       33554431

//---------------------------------------------------------------
// hall数据类型定义
typedef struct _SENSOR_DATA
{
    unsigned int    HallState;           // Input:
    unsigned long   Speed;
    unsigned int    Angle;
}SENSOR_HALL;

typedef struct HALL_TUNE_ANGLE_DATA
{
    unsigned long Angle0;       //
    unsigned long Angle30;      //
    unsigned long Angle60;      //
    unsigned long Angle90;      //
    unsigned long Angle120;     //
    unsigned long Angle150;     //
    unsigned long Angle180;     //
    unsigned long Angle210;     //
    unsigned long Angle240;     //
    unsigned long Angle270;     //
    unsigned long Angle300;     //
    unsigned long Angle330;     //
    unsigned long Angle360;     //
}HALL_TUNE_ANGLE;

typedef struct HALL_TUNE_DATA
{
    unsigned int TuneStep;          //
    unsigned int TuneFreq;          //
    unsigned long TunePeriod;          //
    unsigned long PeriodFilter;          //
    unsigned long AngleStep;
    unsigned int HallState[6];      //
    unsigned long HallAngle[6];     //
    unsigned long HallOffset;
    unsigned int Index;             //
    unsigned long TuneNum;          //
    unsigned int TuneOK;            //
    unsigned int HallErr;
}HALL_TUNE;

#define HALL_TUNE_DEFAULT   { 1,    \
                              1,    \
                              0,    \
                              0,    \
                              0,    \
                              {0,0,0,0,0,0},    \
                              {0,0,0,0,0,0},    \
                              0,    \
                              0,    \
                              0,    \
                              0}

//---------------------------------------------------------------
// 函数及变量声明
extern Uint32 MotorOneElecCircleTimeBase;
extern Uint32 MotorOneElecCircleTime;

extern unsigned int GET_HALL_STATE(void);
extern void Get_SensorAngleRef(void);
extern void Hall_Process(void);
extern void Hall_Init(void);
extern void Sensor_Handle(void);
extern unsigned int HallTune(void);
extern void CalHallTuneAngle(void);
extern void RecordHallAngle(void);

extern volatile SENSOR_HALL hall;
extern volatile HALL_TUNE_ANGLE hallTuneAngle;
extern volatile HALL_TUNE gHallTune;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
