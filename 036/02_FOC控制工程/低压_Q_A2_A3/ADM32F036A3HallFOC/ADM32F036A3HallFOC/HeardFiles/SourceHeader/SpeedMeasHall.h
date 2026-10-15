/******************************************************************************
文件名                      : speed_meas_hall.h
作者                          : 李育明
功能描述                  : 转速计算（hall）模块声明
创建时间                  : 05-21-2019
版本                          : V1.0
修订时间                  :
修订内容                  :
*******************************************************************************/

#ifndef SPEED_MEAS_HALL_H
#define SPEED_MEAS_HALL_H


#ifdef __cplusplus
extern "C" {
#endif


// 速度计算(hall)相关结构体
typedef struct {
    unsigned long HallState;           // Input:
    unsigned long PreHallState;        // Variable:
    unsigned long LoopCnt;             // Variable:
    unsigned long LoopCntRecord[6];    // Variable:
    unsigned long LoopCntIndex;        // Variable:
    unsigned long LoopCntSum;          // Variable:
    unsigned long PosCnt;              // Variable:
    unsigned long NegCnt;              // Variable:
    unsigned long MintSpeedCnt;        // Parameter:
    unsigned long K1;                  // Parameter:
    unsigned int RotateDir;            // Output: 电机旋转方向，0—停止；1—正转；2—反转
    unsigned long Speed;               // Output: 电机转速（电频率），单位：Hz
    unsigned long RealSpeed;               // Output: 电机转速（电频率），单位：0.001Hz
    void  (*calc)();                   // Pointer to calculation function
    } SPEED_HALL;                      // Data type created

typedef SPEED_HALL *SPEED_HALL_handle;

/*-----------------------------------------------------------------------------
Default initalizer for the SPEED_HALL object.
-----------------------------------------------------------------------------*/
#define SPEED_HALL_DEFAULTS   { 0, \
                                0, \
                                0, \
                                {0,0,0,0,0,0}, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                0, \
                                (void (*)(unsigned long))speed_hall_calc }

/*------------------------------------------------------------------------------
Prototypes for the functions in speed_meas_hall.c
------------------------------------------------------------------------------*/
void speed_hall_calc(SPEED_HALL_handle);
void speed_hall_calc_opt(SPEED_HALL_handle);


//---------------------------------------------------------------------------
// speed_meas_hall External References & Function Declarations:
//
extern volatile SPEED_HALL speedMeasHall;
extern volatile float MeasurableSpeedMin;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of SPEED_MEAS_HALL_H definition

//===========================================================================
// No more.
//===========================================================================
