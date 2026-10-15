
/************************************************************************
 Project:             Automotive water pumps
 Filename:            LDF.c
 Partner Filename:    N/A
 Description:         Partner file of LDF.h
 Complier:            Code Composer Studio for ADM32F036, CCS Systems.
 CPU TYPE :           ADM32F036A2
*************************************************************************
 Copyright (c) 2024 **** Co., Ltd.
 All rights reserved.
*************************************************************************
*************************************************************************
 Revising History (ECL of this file):

************************************************************************/

/************************************************************************
 Beginning of File, do not put anything above here except notes
 Compiler Directives
*************************************************************************/

#include "Define.h"
#include "Main.h"

///////////////////////////////////////////////////////////////////////////////////////
// 控制板参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MAX_DC_VOL_3_3V      =  480;        // 0.1vdc
#define  MAX_PEAK_CUR_3_3V           2000         // 0.01A
unsigned long MAX_PEAK_CUR_3_3V_2   =  MAX_PEAK_CUR_3_3V;
unsigned long MAX_PEAK_CUR_3_3V_410 =  ((long)MAX_PEAK_CUR_3_3V << 12) / 10;

unsigned int OVER_DC_VOL_POINT    =  160;         // 0.1vdc
unsigned int OVER_DC_VOL_LIMIT    =  150;         // 0.1vdc
unsigned int UNDER_DC_VOL_POINT   =  75;          // 0.1vdc

#define DEAD_TIME                    50                                  // 死区时间，0.50US
#define DEAD_COMPENATION_TIME        0//25                                  // 死区补偿时间，0.25US
unsigned int DeadTimeInternal     =  ((long)DEAD_TIME * DSP_CLOCK) / 100;               // 死区时间
unsigned int DeadTimeCompInternal =  ((long)DEAD_COMPENATION_TIME * DSP_CLOCK) / 100;   // 死区补偿时间

#define HARDWARE_OVER_CUR            1600                                // 硬件过流比较值，10.00A
#define HARDWARE_CBC_CUR             1500                                // CBC过流比较值, 8.00A
unsigned int OverCurPointInternal =  (((long)HARDWARE_OVER_CUR << 9) / MAX_PEAK_CUR_3_3V) + 512;   // 硬件过流比较值
unsigned int CBCCurPointInternal  =  (((long)HARDWARE_CBC_CUR  << 9) / MAX_PEAK_CUR_3_3V) + 512;   // CBC过流比较值

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MOTOR_RATED_POWER  =  2000;    //0.1W为单位
unsigned int MOTOR_RATED_VOL    =  8;
unsigned int MOTOR_RATED_CUR    =  50;
unsigned int LOWER_LIMIT_FREQ   =  100;//0;

unsigned int PMSM_LD            =  77;          // 37uH
unsigned int PMSM_LQ            =  80;          // 50uH
unsigned int PMSM_RS            =  126;          // 11mohm
unsigned int PMS_L_R_UNIT       =  0;            //
#ifdef KE_WIDE_RANGE
unsigned int PMSM_KE            =  26;//43;           // Ke = 43
#else
unsigned int PMSM_KE            =  4;            // ke = 4
#endif

///////////////////////////////////////////////////////////////////////////////////////
// 开环切换闭环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int FRQ_OPEN_TO_CLOSE      =  1500;    // 要计算一下单位，不是0.01HZ
unsigned int OPEN_LOOP_TOR_START    =  1000;
unsigned int FRQ_PWM_SHIFT_EN       =  10000;   // 要计算一下单位，不是0.01HZ

unsigned int LowLimit               =  450;     // 开环启动低速 调制比系数
unsigned int HighLimit              =  1000;    // 开环启动高速 调制比系数

#define SpinControlHZ                  5625              // 顺风启动阈值点, 0.01HZ
long SpinControlInternal   =  (long)SpinControlHZ << 4;  // 顺风启动阈值点, 内部单位。OMG,must be long as pm_est_lpf_omg1 is long

///////////////////////////////////////////////////////////////////////////////////////
// 频率和加减速参数设置
///////////////////////////////////////////////////////////////////////////////////////
int RefSet                          =  12000;   // 120.00HZ
unsigned int ACC1                   =  50;//500;     // 低速段加速时间 5.0S
unsigned int ACC2                   =  200;     // 高速段加速时间 2.0S
unsigned int DCBrakeTime            =  10;      // 直流制动时间     1.0S

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int F_CHANGING_POINT_1_PI  =  200;
unsigned int F_CHANGING_POINT_2_PI  =  3000;
unsigned int LOW_SPEED_KI           =  25;//50;
unsigned int LOW_SPEED_KP           =  18;//40;
unsigned int HIGH_SPEED_KI_GAIN     =  6;//20;
unsigned int HIGH_SPEED_KP_GAIN     =  2;//10;


///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int D_CUR_KI               =  60;
unsigned int D_CUR_KP               =  60;
unsigned int Q_CUR_KI               =  60;
unsigned int Q_CUR_KP               =  60;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int PWM_FREQUENCY          =  200;     // 100 * 0.1kHz 载波频率
unsigned int AVR_MODE               =  1;       // AVR. 0,1: Enable, 2:Disabled

//--------------------- 频率范围参数----------------------/
unsigned int MAX_LIMIT_FREQ         =  4000;    // 300.00Hz
unsigned int MOTOR_RATED_FRE        =  12000;   //40000;
unsigned int COAST_STOP_FRQ         =  500;     // 1.0HZ,自由停机起始频率

//--------------------- 直流制动参数----------------------/
unsigned int DC_CONTROL_KP          = 15;
unsigned int DC_CONTROL_KI          = 10;
unsigned int DC_BRAKE_CUR           = 3100;

//--------------------- 电流限制百分比参数-------------/
unsigned int TORQUE_CUR_LIMIT       = 150;

//--------------------- 弱磁参数----------------------/
unsigned int FILED_WEEKING_MODE     = 1;       // FE-00, 0 ~ 2
unsigned int FILED_WEEKING_CUR_SCAL = 80;      // FE-01, 0 ~ 200
unsigned int FILED_WEEKING_SCAL     = 4;       // FE-02, 1 ~ 10
unsigned int FIELD_WEAK_REF         = 3950;    // FE-05, 0 ~ 4100
unsigned int IPD_PERIOD_SCALING     = 128;     // 避免更换电机后电感特别小导致运行电流很大
unsigned int IdMaxScaling           = 512;     // 弱磁电流占最大电流百分比，512 就是50%，Q10 格式
unsigned int IqMaxSet               = 400;     // NOT USED


unsigned int SOFT_OC_POINT          = 15000;   // Current limit : 3A,150%
                                               // 9000 * 5 / 4096 = 11A
unsigned int STALL_PR_TIME          = 8;       // 8s, 8000ms

unsigned int SF_LOW_SPEED            = 160;
unsigned int MIN_CUR_LOW_SPEED       = 20;

unsigned int OVER_SPEED_SCALING      = 130;


// #define  THETA_COM_0_1_DEGREE         20   // tmp_swDeltaThetaPu, 182 ~ 1.0Degree...
// 1000Hz, if Coff = 1000, tmp_swDeltaThetaPu = 620, that is 3.4Degree......
// For AC System, 250HZ Max,
// ThetaCoff    Degree
// 4000         3.4
// 10000        8.5
// 15000        12.7
unsigned int ThetaCoff              =  5000;
unsigned int AUTO_RESET_CNT         =  5;


//--------------------- FOC 磁链控制参数（一般不用修改）----------------------/
unsigned int SPEED_FEEDBACK_FILTER   = 56;
unsigned int FILED_WEEKING_OUTVOL_SC = 0;
unsigned int SPECIAL_FACTOR_1        = 0;
unsigned int SPEED_MEASURE_PAR_1     = 20;
unsigned int SPEED_MEASURE_PAR_2     = 20;
unsigned int START_PRESET_CUR        = 0;
unsigned int LOW_LIMIT_FREQUENCY     = 0;
unsigned int INIT_POS_DECTION_MODE   = 1;
unsigned int INIT_POS_DECTION_CUR    = 120;
unsigned int INIT_POS_DECTION_TIME   = 60;
unsigned int LOW_SPEED_FILTER        = 16;
unsigned int PMSM_RS_MIN             = 16;

//--------------------- 单电阻采样  参数----------------------/
unsigned int T_DELAY                = 150; // 1.5us
unsigned int T_SAMPLE               = 150; // 1.5us
//#define  CURR_SAMPLE_DELAY         (T_SAMPLE - T_DELAY)
//#define  MIN_SAMPLE_TIME           (T_SAMPLE + T_DELAY)
//#define  TWO_MIN_SAMPLE_TIME       (MIN_SAMPLE_TIME*2)

unsigned int FirstSamplePointShift  = 100; // 1.0us
unsigned int SecondSamplePointShift = 100; // 1.0us

unsigned int RatioForLowNoisePWM            = 1450;
unsigned int RatiotThresholdForLowNoisePWM  = 250;
