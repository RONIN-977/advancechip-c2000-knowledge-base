/************************************************************************
 Beginning of File, do not put anything above here except notes
 Compiler Directives
*************************************************************************/

#include "Define.h"
#include "Main.h"

///////////////////////////////////////////////////////////////////////////////////////
// 控制板参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MAX_PEAK_CUR_3_3V  = 5000;//4000;//10000;//2000;//4000;        // 0.01A
unsigned int MAX_DC_VOL_3_3V    =  603;//471;//471;//480;         // 0.1vdc

unsigned int OVER_DC_VOL_POINT    =  320;//160;//160;         // 0.1vdc
unsigned int OVER_DC_VOL_LIMIT    =  320;//150;         // 0.1vdc
unsigned int UNDER_DC_VOL_POINT   =  120;//60;//75;     // 0.1vdc

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int MOTOR_RATED_POWER  = 120;//3000;    //0.1W为单位
unsigned int MOTOR_RATED_VOL    =  17;//9;//14;
unsigned int MOTOR_RATED_CUR    =  5;//10;//30;      //0.1A
unsigned int LOWER_LIMIT_FREQ   =  0;

unsigned int PMSM_LD            =  4645;//41;//150;//260;//120;//30;//432;          // 80uH
unsigned int PMSM_LQ            =  5077;//48;//175;//265;////135;//33;//464;          // 94uH
unsigned int PMSM_RS            =  4051;//17;//4;//1529;//50;//155;//47;//25;//251;          // 65mohm
unsigned int PMS_L_R_UNIT       =  0;           //
#ifdef KE_WIDE_RANGE
unsigned int PMSM_KE            =  160;//28;//40;//24;//60;    //0.1
#else
unsigned int PMSM_KE            =  4;//6;           // ke = 4
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
volatile int RefSet                 =  800;   // 120.00HZ
unsigned int ACC1                   =  500;     // 低速段加速时间 5.0S
unsigned int ACC2                   =  500;     // 高速段加速时间 2.0S
unsigned int DCBrakeTime            =  200;      // 直流制动时间     1.0S

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int F_CHANGING_POINT_1_PI  =  200;
unsigned int F_CHANGING_POINT_2_PI  =  3000;
volatile unsigned int LOW_SPEED_KI           =  30;//30;
volatile unsigned int LOW_SPEED_KP           =  20;//30;
volatile unsigned int HIGH_SPEED_KI_GAIN     =  1;//20;
volatile unsigned int HIGH_SPEED_KP_GAIN     =  1;


///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
volatile unsigned int D_CUR_KI               =  10;//25;//20;//60;
volatile unsigned int D_CUR_KP               =  10;//25;//20;//60;
volatile unsigned int Q_CUR_KI               =  34;//20;//60;
volatile unsigned int Q_CUR_KP               =  34;//20;//60;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
unsigned int PWM_FREQUENCY          =  100;//200;     // 100 * 0.1kHz 载波频率
unsigned int AVR_MODE               =  1;       // AVR. 0,1: Enable, 2:Disabled

//--------------------- 频率范围参数----------------------/
unsigned int MAX_LIMIT_FREQ         =  3000;   // 300.00Hz
unsigned int MOTOR_RATED_FRE        =  10000;  //40000;
unsigned int COAST_STOP_FRQ         =  50;     // 1.0HZ,自由停机起始频率

//--------------------- 直流制动参数----------------------/
unsigned int DC_CONTROL_KP          = 20;//20;
unsigned int DC_CONTROL_KI          = 20;//20;
unsigned int DC_BRAKE_CUR           = 4000;//4100;

//--------------------- 电流限制百分比参数-------------/
unsigned int TORQUE_CUR_LIMIT       = 200;//100;

//--------------------- 弱磁参数----------------------/
unsigned int FILED_WEEKING_MODE     = 2;       // FE-00, 0 ~ 2
unsigned int FILED_WEEKING_CUR_SCAL = 80;      // FE-01, 0 ~ 200
unsigned int FILED_WEEKING_SCAL     = 4;       // FE-02, 1 ~ 10
unsigned int FIELD_WEAK_REF         = 3950;    // FE-05, 0 ~ 4100
unsigned int IPD_PERIOD_SCALING     = 128;     // 避免更换电机后电感特别小导致运行电流很大

unsigned int SOFT_OC_POINT          = 15000;   // Current limit : 3A,150%
                                               // 9000 * 5 / 4096 = 11A
unsigned int STALL_PR_TIME          = 8;       // 8s, 8000ms

unsigned int SF_LOW_SPEED            = 180;
unsigned int MIN_CUR_LOW_SPEED       = 15;

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
