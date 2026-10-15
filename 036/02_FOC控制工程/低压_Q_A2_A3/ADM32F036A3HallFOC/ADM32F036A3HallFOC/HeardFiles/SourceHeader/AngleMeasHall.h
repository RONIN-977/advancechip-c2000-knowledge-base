/******************************************************************************
文件名                      : angle_meas_hall.h
作者                          : 李育明
功能描述                  : 角度计算模块声明（hall反馈）
创建时间                  : 05-21-2019
版本                          : V1.0
修订时间                  :
修订内容                  :
*******************************************************************************/

#ifndef ANGLE_MEAS_HALL_H
#define ANGLE_MEAS_HALL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ANGLE_DATA{
    long Angle0;                // 0度角
    long Angle30;               // 30度角
    long Angle60;               // 60度角
    long Angle90;               // 90度角
    long Angle120;              // 120度角
    long Angle150;              // 150度角
    long Angle180;              // 180度角
    long Angle210;              // 210度角
    long Angle240;              // 240度角
    long Angle270;              // 270度角
    long Angle300;              // 300度角
    long Angle330;              // 330度角
    long Angle360;              // 360度角
    } ANGLE;

// 角度位置计算相关结构体
typedef struct {
    unsigned int HallState;     // Input: 当前位置hall状态值
    unsigned int RotateDir;     // Input: 电机旋转方向，0—停止；1—正转；2—反转
    unsigned long Speed;        // Input: 电机转速，单位：Hz
    unsigned long K1;           // Parameter: 角度计算系数
    long CompAngle;             // Variable: 角度补偿量
    long SorElecAngle;          // Variable:
    unsigned int PosTransbound; // Variable: 正转从360度穿过到0度
    unsigned int NegTransbound; // Variable: 正转从0度穿过到360度
    long ElecAngle;             // Variable: 补偿后的电角度
    unsigned int Angle;         // Output: 电角度标幺化值
    void  (*calc)();
    } ANGLE_HALL;

typedef ANGLE_HALL *ANGLE_HALL_handle;
#define ANGLE_HALL_DEFAULTS   { 0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                (void (*)(unsigned long))angle_meas_calc}

/*------------------------------------------------------------------------------
Prototypes for the functions in angle_meas_hall.c
------------------------------------------------------------------------------*/
void angle_meas_calc(ANGLE_HALL_handle);

//---------------------------------------------------------------------------
// angle_meas_hall External References & Function Declarations:
//
extern void angle_Cal_Init(void);
extern volatile ANGLE angleInit;
extern volatile ANGLE_HALL angleMeasHall;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of ANGLE_MEAS_HALL_H definition

//===========================================================================
// No more.
//===========================================================================
