
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
unsigned int MAX_DC_VOL_3_3V      =  4370;        // 0.1vdc
#define  MAX_PEAK_CUR_3_3V           4400         // 0.01A
unsigned long MAX_PEAK_CUR_3_3V_410 =  ((long)MAX_PEAK_CUR_3_3V << 12) / 10;

unsigned int OVER_DC_VOL_POINT    =  3600;         // 0.1vdc
unsigned int OVER_DC_VOL_LIMIT    =  3600;         // 0.1vdc
unsigned int UNDER_DC_VOL_POINT   =  1800;//2400;         // 0.1vdc

#define DEAD_TIME                    200                                 // 死区时间，2.00US
#define DEAD_COMPENATION_TIME        70                                  // 死区补偿时间，0.70US
unsigned int DeadTimeInternal     =  ((long)DEAD_TIME * DSP_CLOCK) / 100;               // 死区时间
unsigned int DeadTimeCompInternal =  ((long)DEAD_COMPENATION_TIME * DSP_CLOCK) / 100;   // 死区补偿时间

#define HARDWARE_OVER_CUR            400                                 // 硬件过流比较值，10.00A
#define HARDWARE_CBC_CUR             350                                 // CBC过流比较值, 8.00A
unsigned int OverCurPointInternal =  ((long)HARDWARE_OVER_CUR << 9) / MAX_PEAK_CUR_3_3V + 512;   // 硬件过流比较值
unsigned int CBCCurPointInternal  =  ((long)HARDWARE_CBC_CUR  << 9) / MAX_PEAK_CUR_3_3V + 512;   // CBC过流比较值

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MOTOR_RATED_POWER  =  2000;    //0.1W为单位
unsigned int MOTOR_RATED_VOL    =  220;
unsigned int MOTOR_RATED_CUR    =  160;

unsigned int PMSM_LD            =  538;//18600;       // 37uH
unsigned int PMSM_LQ            =  973;//22500;       // 50uH
unsigned int PMSM_RS            =  271;//2820;        // 11mohm
unsigned int PMS_L_R_UNIT       =  0;           //
#ifdef KE_WIDE_RANGE
unsigned int PMSM_KE            =  43;          // Ke = 43
#else
unsigned int PMSM_KE            =  46;//110;         // ke = 4
#endif

///////////////////////////////////////////////////////////////////////////////////////
// 开环切换闭环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int FRQ_OPEN_TO_CLOSE      =  1800;    // 要计算一下单位，不是0.01HZ
unsigned int OPEN_LOOP_TOR_START    =  1000;
unsigned int FRQ_PWM_SHIFT_EN       =  10000;   // 要计算一下单位，不是0.01HZ

unsigned int LowLimit               =  450;     // 开环启动低速 调制比系数
unsigned int HighLimit              =  1000;    // 开环启动高速 调制比系数

#define SpinControlHZ                  5625              // 顺风启动阈值点, 0.01HZ
long SpinControlInternal   =  (long)SpinControlHZ << 4;  // 顺风启动阈值点, 内部单位。OMG,must be long as pm_est_lpf_omg1 is long

///////////////////////////////////////////////////////////////////////////////////////
// 频率和加减速参数设置
///////////////////////////////////////////////////////////////////////////////////////
         int RefSet                 =  4000;    // 200.0HZ
unsigned int ACC1                   =  500;     // 低速段加速时间 5.0S
unsigned int ACC2                   =  600;     // 高速段加速时间 2.0S
unsigned int DEC                    =  4000;    // 减速时间 10.0S
unsigned int DCBrakeTime            =  10;      // 直流制动时间     1.0S

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int F_CHANGING_POINT_1_PI  =  800;
unsigned int F_CHANGING_POINT_2_PI  =  2000;
unsigned int LOW_SPEED_KI           =  20;//40;
unsigned int LOW_SPEED_KP           =  20;//40;
unsigned int HIGH_SPEED_KI_GAIN     =  30;
unsigned int HIGH_SPEED_KP_GAIN     =  10;//15;


///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int D_CUR_KI               =  50;//15;//40;//100;
unsigned int D_CUR_KP               =  50;//18;//45;//120;
unsigned int Q_CUR_KI               =  50;//15;//40;//100;
unsigned int Q_CUR_KP               =  50;//20;//55;//120;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int PWM_FREQUENCY          =  160;     // 100 * 0.1kHz 载波频率
unsigned int AVR_MODE               =  1;       // AVR. 0,1: Enable, 2:Disabled

//--------------------- 频率范围参数----------------------/
unsigned int MAX_LIMIT_FREQ         =  6000;    // 600.0Hz
unsigned int MOTOR_RATED_FRE        =  5000;    // 333.3Hz
unsigned int COAST_STOP_FRQ         =   800;    // 90.0HZ,自由停机起始频率
unsigned int LOWER_LIMIT_FREQ       =   800;

//--------------------- 直流制动参数----------------------/
unsigned int DC_CONTROL_KP          = 3;
unsigned int DC_CONTROL_KI          = 4;
unsigned int DC_BRAKE_CUR           = 3024;

//--------------------- 电流限制百分比参数-------------/
unsigned int TORQUE_CUR_LIMIT       = 100;

//--------------------- 弱磁参数----------------------/
unsigned int FILED_WEEKING_MODE     = 1;       // FE-00, 0 ~ 2
unsigned int FILED_WEEKING_CUR_SCAL = 80;      // FE-01, 0 ~ 200
unsigned int FILED_WEEKING_SCAL     = 4;       // FE-02, 1 ~ 10
unsigned int FIELD_WEAK_REF         = 3950;    // FE-05, 0 ~ 4100
unsigned int IPD_PERIOD_SCALING     = 128;     // 避免更换电机后电感特别小导致运行电流很大

unsigned int SOFT_OC_POINT          = 10000;   // Current limit : 10000 / 4096 = 250%
                                               // 9000 * 5 / 4096 = 11A
unsigned int STALL_PR_TIME          = 8;       // 8s, 8000ms

unsigned int SF_LOW_SPEED            = 160;
unsigned int MIN_CUR_LOW_SPEED       = 30;

unsigned int OVER_SPEED_SCALING      = 130;

//--------------------- FOC 磁链控制参数----------------------/
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

// #define  THETA_COM_0_1_DEGREE         20   // tmp_swDeltaThetaPu, 182 ~ 1.0Degree...
// 1000Hz, if Coff = 1000, tmp_swDeltaThetaPu = 620, that is 3.4Degree......
// For AC System, 250HZ Max,
// ThetaCoff    Degree
// 4000         3.4
// 10000        8.5
// 15000        12.7
unsigned int ThetaCoff              =  5000;
unsigned int AUTO_RESET_CNT         =  5;
unsigned int AVR_Mode               =  AVR_MODE_DEFAULT_1;

// TS_L_VDC
// 100MHZ, CLKDIV = 7, TS_L_VDC * 128 * 1 / 100MHZ = 75... ... ... ...
// Toggle EPWMxA Output: at Period time, so 75 * 2 = 150US ... ... ...
// U = L*I/T, SO I = U*T/(Ld+Lq) = 310.0V * 0.150 / 40MH = 1.2A... ...
// T = ((l_ld + l_lq) * gMotorInfo.Current) / gUDC.uDCFilter = 40000 * 20 / 3100 = 258
// 258 * 60 / 150 = 100
///////////////////////////////////////////////////////////////////////
unsigned int TS_L_VDC_SCALING       =  1024;

const Uint16 PMSM_CONTROL_MODE      = PMSM_CONTROL_MODE_FOC;    //PMSM_CONTROL_MODE_VF


