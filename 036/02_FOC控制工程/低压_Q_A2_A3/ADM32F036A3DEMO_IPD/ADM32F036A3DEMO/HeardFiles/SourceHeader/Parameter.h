
#ifndef __F_PARAMETER_H__
#define __F_PARAMETER_H__

extern unsigned long MAX_PEAK_CUR_3_3V_410;
extern unsigned long MAX_PEAK_CUR_3_3V_2;
extern unsigned int MAX_DC_VOL_3_3V;

extern unsigned int OVER_DC_VOL_POINT;
extern unsigned int OVER_DC_VOL_LIMIT;
extern unsigned int UNDER_DC_VOL_POINT;
extern unsigned int  OVER_DC_VOL_KP;
extern unsigned int MaxAllowUdcLimit;
extern unsigned int MidAllowUdcLimit;

extern unsigned int OverCurPointInternal;   // 硬件过流比较值
extern unsigned int CBCCurPointInternal;    // CBC过流比较值

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
extern volatile unsigned int F_CHANGING_POINT_1_PI;
extern volatile unsigned int F_CHANGING_POINT_2_PI;
extern volatile unsigned int LOW_SPEED_KI;
extern volatile unsigned int LOW_SPEED_KP;
extern volatile unsigned int HIGH_SPEED_KI_GAIN;
extern volatile unsigned int HIGH_SPEED_KP_GAIN;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern unsigned int PWM_FREQUENCY;     // 100 * 0.1kHz 载波频率
extern unsigned int AVR_MODE;

///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern volatile unsigned int D_CUR_KI;
extern volatile unsigned int D_CUR_KP;
extern volatile unsigned int Q_CUR_KI;
extern volatile unsigned int Q_CUR_KP;


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
extern unsigned int IdMaxScaling;               // 弱磁电流占最大电流百分比，512 就是50%，Q10 格式
extern unsigned int IqMaxSet;                   // NOT USED

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
extern unsigned int BemfOffset;

extern int RefSet;
extern unsigned int ACC1;
extern unsigned int ACC2;
extern unsigned int DEC;
extern unsigned int DCBrakeTime;
extern unsigned int AUTO_RESET_CNT;
extern unsigned int ThetaCoff;
extern unsigned int AVR_Mode;

extern unsigned int RatioForLowNoisePWM;
extern unsigned int RatiotThresholdForLowNoisePWM;

extern unsigned long LowFreqencyOFSF;                          // 20 * 1602
extern unsigned long HighFreqencyOFSF;                         // 40 * 1602

extern int MTPA_Ld_TableData_H[20];
extern int MTPA_Lq_TableData_H[20];

extern unsigned long IdMinInternal;                        // 5 * 1602
extern unsigned long IdMaxInternal;                        // 50 * 1602

#define CURRENT_SAMPLE_TYPE             CURRENT_SAMPLE_1SHUNT
#define SINGLE_SHUNT_PWM_MODE           LOW_NOISE_PWM_MODE_2    //NORMAL_NOISE_PWM_MODE // LOW_NOISE_PWM_MODE
#define ADM32F036_TYPE                  ADM32F036_A3
//#define SPI_SCOPE_TEST                  1
//#define TESTCLK                         1

#define SWITCH_FREQUENCY_EMC_TYPE       FIXED_SWICTH_FREQUENCY  // 固定开关频率还是随机
#define SWITCH_FREQUENCY_AMP            20                      // 上下跳频值

#define SF_INDEPAND_OF_FREQUENCY        SW_RANDOM_DISABLE

#define SPEED_RAMP_LINEAR_MODE          0
#define SPEED_RAMP_S_MODE               1
#define SPEED_RAMP_MODE                 0//SPEED_RAMP_S_MODE

// 故障恢复时间设置
#define VOLT_FAULT_RECOVER_TIME        (12000)
#define CURRENT_FAULT_RECOVER_TIME     (12000)
#define STALL_FAULT_RECOVER_TIME       (12000)
#define PHASELOSS_FAULT_RECOVER_TIME   (8000)
#define TEMP_FAULT_RECOVER_TIME        (2000)
#define START_FAULT_RECOVER_TIME       (11000)
#define EMPTY_FAULT_RECOVER_TIME       (12000)

#define YellowLED_ON                GpioDataRegs.GPASET.bit.GPIO18  = 1
#define YellowLED_OFF               GpioDataRegs.GPACLEAR.bit.GPIO18    = 1

#define TRIP_FLASHING_TIME          100
#define WAIT_POWER_ON_TIME          10000       // 进入主程序之后，等待_*0.5ms开始响应欠压处理和运行命令

#define LD_LQ_BASED_ON_CUR          1

#endif // __F_PARAMETER_H__

