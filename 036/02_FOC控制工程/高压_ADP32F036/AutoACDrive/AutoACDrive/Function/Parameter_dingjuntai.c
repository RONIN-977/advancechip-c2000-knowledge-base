
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
unsigned int MAX_DC_VOL_3_3V      =  10443;       // 0.1vdc
#define  MAX_PEAK_CUR_3_3V           3148        // 0.01A
unsigned long MAX_PEAK_CUR_3_3V_410 =  ((long)MAX_PEAK_CUR_3_3V << 12) / 10;

unsigned int OVER_DC_VOL_POINT    =  9000;         // 0.1vdc
unsigned int OVER_DC_VOL_LIMIT    =  9000;         // 0.1vdc
unsigned int UNDER_DC_VOL_POINT   =  2500;            // 0.1vdc

#define DEAD_TIME                    100                                 // 死区时间，0.50US
#define DEAD_COMPENATION_TIME        0                                  // 死区补偿时间，0.25US
unsigned int DeadTimeInternal     =  ((long)DEAD_TIME * DSP_CLOCK) / 100;               // 死区时间
unsigned int DeadTimeCompInternal =  ((long)DEAD_COMPENATION_TIME * DSP_CLOCK) / 100;   // 死区补偿时间

#define HARDWARE_OVER_CUR            2000                                 // 硬件过流比较值，10.00A
#define HARDWARE_CBC_CUR             200                                 // CBC过流比较值, 8.00A
unsigned int OverCurPointInternal =  ((long)HARDWARE_OVER_CUR << 9) / MAX_PEAK_CUR_3_3V + 512;   // 硬件过流比较值
unsigned int CBCCurPointInternal  =  ((long)HARDWARE_CBC_CUR  << 9) / MAX_PEAK_CUR_3_3V + 512;   // CBC过流比较值

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MOTOR_RATED_POWER  =  1805;    //0.1W为单位
unsigned int MOTOR_RATED_VOL    =  382;
unsigned int MOTOR_RATED_CUR    =  100;
unsigned int LOWER_LIMIT_FREQ   =  0;
unsigned int COAST_STOP_FRQ     =  200;     // 1.0HZ,自由停机起始频率

unsigned int PMSM_PN            =  4;          //极对数
unsigned int PMSM_LD            =  10639;//5478;       // 37uH
unsigned int PMSM_LQ            =  16035;//6180;       // 50uH
unsigned int PMSM_RS            =  2811;//1220;       // 11mohm   1.5
unsigned int PMS_L_R_UNIT       =  0;          //
unsigned int PMSM_KE            =  86;        // ke = 52v/kRpm

//unsigned int PMSM_LD            =  11920/2;       // 37uH
//unsigned int PMSM_LQ            =  21880/2;       // 50uH
//unsigned int PMSM_RS            =  1220;       // 11mohm
//unsigned int PMS_L_R_UNIT       =  0;          //
//unsigned int PMSM_KE            =  102;        // ke = 4

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
         int RefSet                 =  2000;    // 120.00HZ
unsigned int ACC1                   =  500;      // 低速段RPM/Min
unsigned int ACC2                   =  500;     // 高速段RPM/Min
unsigned int DEC                    =  1000;    // 减速RPM/Min
unsigned int DCBrakeTime            =  20;      // 直流制动时间  1.0S

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int F_CHANGING_POINT_1_PI  =  500;
unsigned int F_CHANGING_POINT_2_PI  =  3000;
unsigned int LOW_SPEED_KI           =  30;
unsigned int LOW_SPEED_KP           =  30;
unsigned int HIGH_SPEED_KI_GAIN     =  20;
unsigned int HIGH_SPEED_KP_GAIN     =  20;


///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
#ifdef ENABLE_NEW_STATIC_OBS
unsigned int D_CUR_KI               =  200;
unsigned int D_CUR_KP               =  400;
unsigned int Q_CUR_KI               =  200;
unsigned int Q_CUR_KP               =  220;
#else
unsigned int D_CUR_KI               =  50;
unsigned int D_CUR_KP               =  50;
unsigned int Q_CUR_KI               =  50;
unsigned int Q_CUR_KP               =  50;
#endif

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int PWM_FREQUENCY          =  120;     // 100 * 0.1kHz 载波频率
unsigned int AVR_MODE               =  1;       // AVR. 0,1: Enable, 2:Disabled

//--------------------- 频率范围参数----------------------/
unsigned int MAX_LIMIT_FREQ         =  43333;   // 8500/60*4 = 566
unsigned int MOTOR_RATED_FRE        =  40000;

//--------------------- 直流制动参数----------------------/
unsigned int DC_CONTROL_KP          = 10;
unsigned int DC_CONTROL_KI          = 10;
unsigned int DC_BRAKE_CUR           = 3100;

//--------------------- 电流限制百分比参数-------------/
unsigned int TORQUE_CUR_LIMIT       = 1000;    // 180.0%

//--------------------- 弱磁参数----------------------/
unsigned int FILED_WEEKING_MODE     = 2;       // FE-00, 0 ~ 2
unsigned int FILED_WEEKING_CUR_SCAL = 80;      // FE-01, 0 ~ 200
unsigned int FILED_WEEKING_SCAL     = 4;       // FE-02, 1 ~ 10
unsigned int FIELD_WEAK_REF         = 3950;    // FE-05, 0 ~ 4100
unsigned int IPD_PERIOD_SCALING     = 128;     // 避免更换电机后电感特别小导致运行电流很大
unsigned int IdMaxScaling           = 400;     // 弱磁电流占最大电流百分比，512 就是50%，Q10 格式

unsigned int SOFT_OC_POINT          = 10000;   // Current limit : 10000 / 4096 = 250%
                                               // 9000 * 5 / 4096 = 11A
unsigned int STALL_PR_TIME          = 8;       // 8s, 8000ms

unsigned int SF_LOW_SPEED            = 160;
unsigned int MIN_CUR_LOW_SPEED       = 15;

unsigned int OVER_SPEED_SCALING      = 130;

//--------------------- FOC 磁链控制参数-----------------/
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

//--------------------- SVPWM相关调试  参数----------------------/
// 若五段式使能没有屏蔽，以频率大小（设定值）来切换。
unsigned int FIVEPWM_FRQ_SET         = 40000;       // 100.00Hz
unsigned int DPWM_FRQ_LEVEL          = 100;         // 5.00Hz

// 一种噪音抑制算法，默认关闭。需要时候开启，启动时候要避开
unsigned int RandomSezeorVectorModeEn= 0;           // 0:禁止
unsigned int RandomZero_FRQ_SET      = 10000;       // 100HZ

// 最小零矢量限制。注意：
// Zero_Length_Min -- 应用于幅度的线性减小，Zero_Length_Min2 用于最大最小值限制。
// 一般建议：
// 双电阻：Zero_Length_Min         = 0
//        Zero_Length_Min2        = 120;
// 三电阻：Zero_Length_Min         = 0
//        Zero_Length_Min2        = 0;
unsigned int Zero_Length_Min         = 0;
unsigned int Zero_Length_Min2        = 0;

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
unsigned int ThetaCoff              =  5000;//5000 15000 20000
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

// BYD.PUMB:
// WITH KP_D = 300, KP_Q = 400, KI = 200
// Then Alfa            : 500 ~ 5000 OK
//      LamdaValueForOBS: 0   ~ 10
//      set to 2 as with high eff
unsigned int AlfaValueForOBS        = 3000;         // Alfa: 1000 ~ 3000
unsigned int LamdaValueForOBS       = 10;           // Lamda:0 ~ 16
unsigned int FilterCntLowForOBS     = 8;            // LOW:  8
unsigned int FilterCntHighForOBS    = 6;            // High: 8

#define ID_MIN_FRQ                  5                                  // 5.0HZ.
#define ID_MAX_FRQ                  50                                 // 50.0Hz
unsigned long IdMinInternal =  (long)ID_MIN_FRQ * 1602;                        // 5 * 1602
unsigned long IdMaxInternal =  (long)ID_MAX_FRQ * 1602;                        // 50 * 1602
