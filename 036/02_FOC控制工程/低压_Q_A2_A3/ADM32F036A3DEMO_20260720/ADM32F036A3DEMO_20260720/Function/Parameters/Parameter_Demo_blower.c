
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
#include "Parameter.h"

///////////////////////////////////////////////////////////////////////////////////////
// 控制板参数设置
///////////////////////////////////////////////////////////////////////////////////////
Uint16 MAX_DC_VOL_3_3V    =  480;        // 0.1vdc
#define  MAX_PEAK_CUR_3_3V         4000//5000        // 0.01A
Uint32 MAX_PEAK_CUR_3_3V_2   =  MAX_PEAK_CUR_3_3V;
Uint32 MAX_PEAK_CUR_3_3V_410 =  ((int32)MAX_PEAK_CUR_3_3V << 12) / 10;

Uint16 OVER_DC_VOL_POINT    =  180;         // 0.1vdc
Uint16 OVER_DC_VOL_LIMIT    =  150;         // 0.1vdc
Uint16 UNDER_DC_VOL_POINT   =  75;          // 0.1vdc
Uint16  OVER_DC_VOL_KP      =  100;
Uint16 MaxAllowUdcLimit     =  170;
Uint16 MidAllowUdcLimit     =  155;

#define DEAD_TIME                    50                                  // 死区时间，0.50US
#define DEAD_COMPENATION_TIME        15                                  // 死区补偿时间，0.25US
Uint16 DeadTimeInternal     =  ((int32)DEAD_TIME * DSP_CLOCK) / 100;               // 死区时间
Uint16 DeadTimeCompInternal =  ((int32)DEAD_COMPENATION_TIME * DSP_CLOCK) / 100;   // 死区补偿时间

#define HARDWARE_OVER_CUR            2800                                // 硬件过流比较值，10.00A
#define HARDWARE_CBC_CUR             2800                                // CBC过流比较值, 8.00A
Uint16 OverCurPointInternal =  ((int32)HARDWARE_OVER_CUR << 9) / MAX_PEAK_CUR_3_3V + 512;   // 硬件过流比较值
Uint16 CBCCurPointInternal  =  ((int32)HARDWARE_CBC_CUR  << 9) / MAX_PEAK_CUR_3_3V + 512;   // CBC过流比较值

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
Uint16 MOTOR_RATED_POWER  =  2000;    //0.1W为单位
Uint16 MOTOR_RATED_VOL    =  8;
Uint16 MOTOR_RATED_CUR    =  100;
Uint16 LOWER_LIMIT_FREQ   =  0;

Uint16 PMSM_LD            =  122;//33;          // 74~88
Uint16 PMSM_LQ            =  130;//36;          // 50uH
Uint16 PMSM_RS            =  45;//18;          // 47mohm
Uint16 PMS_L_R_UNIT       =  0;           //
Uint16 PMSM_KE            =  40;//27;          // 0.836V~41.69hz,Ke = 6.67

///////////////////////////////////////////////////////////////////////////////////////
// 开环切换闭环参数设置
///////////////////////////////////////////////////////////////////////////////////////
Uint16 FRQ_OPEN_TO_CLOSE      =  1500;    // 要计算一下单位，不是0.01HZ
Uint16 OPEN_LOOP_TOR_START    =  300;
Uint16 FRQ_PWM_SHIFT_EN       =  10000;   // 要计算一下单位，不是0.01HZ

Uint16 LowLimit               =  300;     // 开环启动低速 调制比系数
Uint16 HighLimit              =  600;     // 开环启动高速 调制比系数

#define SpinControlHZ            5625                       // 顺风启动阈值点, 0.01HZ
int32 SpinControlInternal   =  (int32)SpinControlHZ << 4;   // 顺风启动阈值点, 内部单位。OMG,must be long as pm_est_lpf_omg1 is long
Uint16 BemfOffset             = 135;

///////////////////////////////////////////////////////////////////////////////////////
// 频率和加减速参数设置
///////////////////////////////////////////////////////////////////////////////////////
int16 RefSet                  =  1800;     // 120.00HZ
Uint16 ACC1                   =  2000;     // 低速段加速时间 5.0S
Uint16 ACC2                   =  500;    // 高速段加速时间 2.0S
Uint16 DEC                    =  2000;    // 高速段加速时间 2.0S
Uint16 DCBrakeTime            =  20;      // 直流制动时间     1.0S

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
Uint16 F_CHANGING_POINT_1_PI  =  1000;
Uint16 F_CHANGING_POINT_2_PI  =  2000;
Uint16 LOW_SPEED_KI           =  20;
Uint16 LOW_SPEED_KP           =  10;
Uint16 HIGH_SPEED_KI_GAIN     =  10;
Uint16 HIGH_SPEED_KP_GAIN     =  15;


///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
Uint16 D_CUR_KI               =  30;
Uint16 D_CUR_KP               =  30;
Uint16 Q_CUR_KI               =  30;
Uint16 Q_CUR_KP               =  30;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
Uint16 PWM_FREQUENCY          =  160;     // 100 * 0.1kHz 载波频率
Uint16 AVR_MODE               =  1;       // AVR. 0,1: Enable, 2:Disabled

//--------------------- 频率范围参数----------------------/
Uint16 MAX_LIMIT_FREQ         =  3680;   // 300.00Hz
Uint16 MOTOR_RATED_FRE        =  16000;
Uint16 COAST_STOP_FRQ         =  100;    // 1.0HZ,自由停机起始频率

//--------------------- 直流制动参数----------------------/
Uint16 DC_CONTROL_KP          = 3;
Uint16 DC_CONTROL_KI          = 5;
Uint16 DC_BRAKE_CUR           = 2100;

//--------------------- 电流限制百分比参数-------------/
Uint16 TORQUE_CUR_LIMIT       = 180;//125;

//--------------------- 弱磁参数----------------------/
Uint16 FILED_WEEKING_MODE     = 1;       // FE-00, 0 ~ 2
Uint16 FILED_WEEKING_CUR_SCAL = 80;      // FE-01, 0 ~ 200
Uint16 FILED_WEEKING_SCAL     = 4;       // FE-02, 1 ~ 10
Uint16 FIELD_WEAK_REF         = 3950;    // FE-05, 0 ~ 4100
Uint16 IPD_PERIOD_SCALING     = 128;     // 避免更换电机后电感特别小导致运行电流很大
Uint16 IdMaxScaling           = 512;     // 弱磁电流占最大电流百分比，512 就是50%，Q10 格式
Uint16 IqMaxSet               = 400;     // NOT USED

Uint16 SOFT_OC_POINT          = 10000;   // Current limit : 3A,150%
                                         // 9000 * 5 / 4096 = 11A
Uint16 STALL_PR_TIME          = 8;       // 8s, 8000ms

Uint16 SF_LOW_SPEED            = 160;
Uint16 MIN_CUR_LOW_SPEED       = 10;

Uint16 OVER_SPEED_SCALING      = 130;


// #define  THETA_COM_0_1_DEGREE         20   // tmp_swDeltaThetaPu, 182 ~ 1.0Degree...
// 1000Hz, if Coff = 1000, tmp_swDeltaThetaPu = 620, that is 3.4Degree......
// For AC System, 250HZ Max,
// ThetaCoff    Degree
// 4000         3.4
// 10000        8.5
// 15000        12.7
Uint16 ThetaCoff              =  5000;
Uint16 AUTO_RESET_CNT         =  5;


//--------------------- FOC 磁链控制参数（一般不用修改）----------------------/
Uint16 SPEED_FEEDBACK_FILTER   = 56;
Uint16 FILED_WEEKING_OUTVOL_SC = 0;
Uint16 SPECIAL_FACTOR_1        = 0;
Uint16 SPEED_MEASURE_PAR_1     = 20;
Uint16 SPEED_MEASURE_PAR_2     = 20;
Uint16 START_PRESET_CUR        = 0;
Uint16 LOW_LIMIT_FREQUENCY     = 0;
Uint16 INIT_POS_DECTION_MODE   = 1;
Uint16 INIT_POS_DECTION_CUR    = 120;
Uint16 INIT_POS_DECTION_TIME   = 60;
Uint16 LOW_SPEED_FILTER        = 16;
Uint16 PMSM_RS_MIN             = 16;

//--------------------- 单电阻采样  参数----------------------/
Uint16 T_DELAY                = 150; // 1.5us
Uint16 T_SAMPLE               = 150; // 1.5us
//#define  CURR_SAMPLE_DELAY         (T_SAMPLE - T_DELAY)
//#define  MIN_SAMPLE_TIME           (T_SAMPLE + T_DELAY)
//#define  TWO_MIN_SAMPLE_TIME       (MIN_SAMPLE_TIME*2)

Uint16 FirstSamplePointShift  = 100; // 1.0us
Uint16 SecondSamplePointShift = 100; // 1.0us

Uint16 RatioForLowNoisePWM            = 1000;
Uint16 RatiotThresholdForLowNoisePWM  = 250;

#define ID_MIN_FRQ                  5                                  // 5.0HZ.
#define ID_MAX_FRQ                  50                                 // 50.0Hz
Uint32 IdMinInternal =  (int32)ID_MIN_FRQ * 1602;                        // 5 * 1602
Uint32 IdMaxInternal =  (int32)ID_MAX_FRQ * 1602;                        // 50 * 1602

#define SF_INDEPAND_OF_FREQUENCY_MIN    20                             // 20HZ
#define SF_INDEPAND_OF_FREQUENCY_MAX    40                             // 40HZ
Uint32 LowFreqencyOFSF  =  (int32)SF_INDEPAND_OF_FREQUENCY_MIN * 1602;                        // 20 * 1602
Uint32 HighFreqencyOFSF =  (int32)SF_INDEPAND_OF_FREQUENCY_MAX * 1602;                        // 40 * 1602

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
Uint16 PWMZeroLength    = 0;
#else
Uint16 PWMZeroLength    = 260;
#endif
