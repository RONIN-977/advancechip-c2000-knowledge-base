
#ifndef __F_PARAMETER_H__
#define __F_PARAMETER_H__

#define YellowLED_ON               // GpioDataRegs.GPASET.bit.GPIO18      = 1
#define YellowLED_OFF              // GpioDataRegs.GPACLEAR.bit.GPIO18    = 1

#define TRIP_FLASHING_TIME          250
#define TRIP_FLASHING_TIMEX2        250*2
#define TRIP_FLASHING_TIMEX4        250*6

#define WAIT_POWER_ON_TIME          10000       // 进入主程序之后，等待_*0.5ms开始响应欠压处理和运行命令

///////////////////////////////////////////////////////////////////////////////////////
// 控制板参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned long MAX_PEAK_CUR_3_3V_410;
extern unsigned int MAX_DC_VOL_3_3V;

extern unsigned int OVER_DC_VOL_POINT;
extern unsigned int OVER_DC_VOL_LIMIT;
extern unsigned int UNDER_DC_VOL_POINT;

extern unsigned int OverCurPointInternal;
extern unsigned int CBCCurPointInternal;

extern unsigned int DeadTimeInternal;
extern unsigned int DeadTimeCompInternal;

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int MOTOR_RATED_POWER;    //0.1W为单位
extern unsigned int MOTOR_RATED_VOL;
extern unsigned int MOTOR_RATED_CUR;
extern unsigned int LOWER_LIMIT_FREQ;

extern unsigned int PMSM_LD;          // 37uH
extern unsigned int PMSM_LQ;          // 50uH
extern unsigned int PMSM_RS;          // 11mohm
extern unsigned int PMS_L_R_UNIT;           //
#ifdef KE_WIDE_RANGE
extern unsigned int PMSM_KE;          // Ke = 43
#else
extern unsigned int PMSM_KE;           // ke = 4
#endif

///////////////////////////////////////////////////////////////////////////////////////
// 开环切换闭环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int FRQ_OPEN_TO_CLOSE;    // 要计算一下单位，不是0.01HZ
//extern unsigned int FULL_OPEN_LOOP_MODE;       // 开环模式，禁止屏蔽该行
extern unsigned int OPEN_LOOP_TOR_START;
extern unsigned int FRQ_PWM_SHIFT_EN;   // 要计算一下单位，不是0.01HZ

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int F_CHANGING_POINT_1_PI;
extern unsigned int F_CHANGING_POINT_2_PI;
extern unsigned int LOW_SPEED_KI;
extern unsigned int LOW_SPEED_KP;
extern unsigned int HIGH_SPEED_KI_GAIN;
extern unsigned int HIGH_SPEED_KP_GAIN;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int PWM_FREQUENCY;     // 100 * 0.1kHz 载波频率
extern unsigned int AVR_MODE;

///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int D_CUR_KI;
extern unsigned int D_CUR_KP;
extern unsigned int Q_CUR_KI;
extern unsigned int Q_CUR_KP;


//--------------------- 频率范围参数----------------------/
extern unsigned int MAX_LIMIT_FREQ;   // 300.00Hz
extern unsigned int MOTOR_RATED_FRE;
extern unsigned int COAST_STOP_FRQ;    // 1.0HZ,自由停机起始频率

//--------------------- 直流制动参数----------------------/
extern unsigned int DC_CONTROL_KP;
extern unsigned int DC_CONTROL_KI;
extern unsigned int DC_BRAKE_CUR;

extern unsigned int TORQUE_CUR_LIMIT;

extern long SpinControlInternal;

extern unsigned int FILED_WEEKING_MODE;         // FE-00, 0 ~ 2
extern unsigned int FILED_WEEKING_CUR_SCAL;     // FE-01, 0 ~ 200
extern unsigned int FILED_WEEKING_SCAL;         // FE-02, 1 ~ 10
extern unsigned int FIELD_WEAK_REF;             // FE-05, 0 ~ 4100
extern unsigned int IPD_PERIOD_SCALING;         // 避免更换电机后电感特别小导致运行电流很大
extern unsigned int SOFT_OC_POINT;              // Current limit : 3A,150%

extern unsigned int STALL_PR_TIME;              // 8s, 8000ms

extern unsigned int OVER_SPEED_SCALING;
extern unsigned int SPEED_FEEDBACK_FILTER;
extern unsigned int SF_LOW_SPEED;
extern unsigned int MIN_CUR_LOW_SPEED;
extern unsigned int FILED_WEEKING_OUTVOL_SC;
extern unsigned int SPECIAL_FACTOR_1;
extern unsigned int SPEED_MEASURE_PAR_1;
extern unsigned int SPEED_MEASURE_PAR_2;
extern unsigned int START_PRESET_CUR;
extern unsigned int LOW_LIMIT_FREQUENCY;
extern unsigned int INIT_POS_DECTION_MODE;
extern unsigned int INIT_POS_DECTION_CUR;
extern unsigned int INIT_POS_DECTION_TIME;
extern unsigned int LOW_SPEED_FILTER;
extern unsigned int PMSM_RS_MIN;

extern unsigned int T_DELAY;
extern unsigned int T_SAMPLE;
extern unsigned int FirstSamplePointShift;
extern unsigned int SecondSamplePointShift;

extern unsigned int LowLimit;
extern unsigned int HighLimit;

extern int RefSet;
extern unsigned int ACC1;
extern unsigned int ACC2;
extern unsigned int DEC;
extern unsigned int DCBrakeTime;

extern unsigned int ThetaCoff;
extern unsigned int AUTO_RESET_CNT;
extern unsigned int AVR_Mode;
extern unsigned int TS_L_VDC_SCALING;

extern const Uint16 PMSM_CONTROL_MODE;

//--------------------- 过热降频参数----------------------/
extern const Uint16 ReducedFreqTemp;
extern const Uint16 FreqReducePeriod;
extern const Uint16 ReducedFreqDelta;

#endif // __F_PARAMETER_H__

