
#ifndef F_PARAMETER_H
#define F_PARAMETER_H

#include "Define.h"

extern Uint32 MAX_PEAK_CUR_3_3V_410;
extern Uint32 MAX_PEAK_CUR_3_3V_2;
extern Uint16 MAX_DC_VOL_3_3V;

extern Uint16 OVER_DC_VOL_POINT;
extern Uint16 OVER_DC_VOL_LIMIT;
extern Uint16 UNDER_DC_VOL_POINT;
extern Uint16  OVER_DC_VOL_KP;
extern Uint16 MaxAllowUdcLimit;
extern Uint16 MidAllowUdcLimit;

extern Uint16 OverCurPointInternal;   // 硬件过流比较值
extern Uint16 CBCCurPointInternal;    // CBC过流比较值

extern Uint16 DeadTimeInternal;
extern Uint16 DeadTimeCompInternal;

///////////////////////////////////////////////////////////////////////////////////////
// 电机参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern Uint16 MOTOR_RATED_POWER;    //0.1W为单位
extern Uint16 MOTOR_RATED_VOL;
extern Uint16 MOTOR_RATED_CUR;
extern Uint16 LOWER_LIMIT_FREQ;

extern Uint16 PMSM_LD;          // 37uH
extern Uint16 PMSM_LQ;          // 50uH
extern Uint16 PMSM_RS;          // 11mohm

#ifdef KE_WIDE_RANGE
extern Uint16 PMSM_KE;          // Ke = 43
#else
extern Uint16 PMSM_KE;           // ke = 4
#endif

///////////////////////////////////////////////////////////////////////////////////////
// 开环切换闭环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern Uint16 FRQ_OPEN_TO_CLOSE;    // 要计算一下单位，不是0.01HZ
//extern Uint16 FULL_OPEN_LOOP_MODE;       // 开环模式，禁止屏蔽该行
extern Uint16 OPEN_LOOP_TOR_START;
extern Uint16 FRQ_PWM_SHIFT_EN;   // 要计算一下单位，不是0.01HZ

///////////////////////////////////////////////////////////////////////////////////////
// 速度环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern Uint16 F_CHANGING_POINT_1_PI;
extern Uint16 F_CHANGING_POINT_2_PI;
extern Uint16 LOW_SPEED_KI;
extern Uint16 LOW_SPEED_KP;
extern Uint16 HIGH_SPEED_KI_GAIN;
extern Uint16 HIGH_SPEED_KP_GAIN;

///////////////////////////////////////////////////////////////////////////////////////
// 控制参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern Uint16 PWM_FREQUENCY;     // 100 * 0.1kHz 载波频率
extern Uint16 AVR_MODE;

///////////////////////////////////////////////////////////////////////////////////////
// 电流环环参数设置
///////////////////////////////////////////////////////////////////////////////////////
extern Uint16 D_CUR_KI;
extern Uint16 D_CUR_KP;
extern Uint16 Q_CUR_KI;
extern Uint16 Q_CUR_KP;


//--------------------- 频率范围参数----------------------/
extern Uint16 MAX_LIMIT_FREQ;   // 300.00Hz
extern Uint16 MOTOR_RATED_FRE;
extern Uint16 COAST_STOP_FRQ;    // 1.0HZ,自由停机起始频率

//--------------------- 直流制动参数----------------------/
extern Uint16 DC_CONTROL_KP;
extern Uint16 DC_CONTROL_KI;
extern Uint16 DC_BRAKE_CUR;

extern Uint16 TORQUE_CUR_LIMIT;

extern int32 SpinControlInternal;

extern Uint16 FILED_WEEKING_MODE;         // FE-00, 0 ~ 2
extern Uint16 FILED_WEEKING_CUR_SCAL;     // FE-01, 0 ~ 200
extern Uint16 FILED_WEEKING_SCAL;         // FE-02, 1 ~ 10
extern Uint16 FIELD_WEAK_REF;             // FE-05, 0 ~ 4100
extern Uint16 IPD_PERIOD_SCALING;         // 避免更换电机后电感特别小导致运行电流很大
extern Uint16 SOFT_OC_POINT;              // Current limit : 3A,150%
extern Uint16 IdMaxScaling;               // 弱磁电流占最大电流百分比，512 就是50%，Q10 格式
extern Uint16 IqMaxSet;                   // NOT USED

extern Uint16 STALL_PR_TIME;              // 8s, 8000ms

extern Uint16 OVER_SPEED_SCALING;
extern Uint16 SPEED_FEEDBACK_FILTER;
extern Uint16 SF_LOW_SPEED;
extern Uint16 MIN_CUR_LOW_SPEED;
extern Uint16 FILED_WEEKING_OUTVOL_SC;
extern Uint16 SPECIAL_FACTOR_1;
extern Uint16 SPEED_MEASURE_PAR_1;
extern Uint16 SPEED_MEASURE_PAR_2;
extern Uint16 START_PRESET_CUR;
extern Uint16 LOW_LIMIT_FREQUENCY;
extern Uint16 INIT_POS_DECTION_MODE;
extern Uint16 INIT_POS_DECTION_CUR;
extern Uint16 INIT_POS_DECTION_TIME;
extern Uint16 LOW_SPEED_FILTER;
extern Uint16 PMSM_RS_MIN;

extern Uint16 T_DELAY;
extern Uint16 T_SAMPLE;
extern Uint16 FirstSamplePointShift;
extern Uint16 SecondSamplePointShift;

extern Uint16 LowLimit;
extern Uint16 HighLimit;
extern Uint16 BemfOffset;

extern int16 RefSet;
extern Uint16 ACC1;
extern Uint16 ACC2;
extern Uint16 DEC;
extern Uint16 DCBrakeTime;
extern Uint16 AUTO_RESET_CNT;
extern Uint16 ThetaCoff;
extern Uint16 AVR_Mode;

extern Uint16 RatioForLowNoisePWM;
extern Uint16 RatiotThresholdForLowNoisePWM;

extern Uint32 LowFreqencyOFSF;                          // 20 * 1602
extern Uint32 HighFreqencyOFSF;                         // 40 * 1602

extern int16 MTPA_Ld_TableData_H[20];
extern int16 MTPA_Lq_TableData_H[20];

extern Uint32 IdMinInternal;                        // 5 * 1602
extern Uint32 IdMaxInternal;                        // 50 * 1602
extern Uint16 PWMZeroLength;

#define CURRENT_SAMPLE_TYPE             CURRENT_SAMPLE_1SHUNT
#define SINGLE_SHUNT_PWM_MODE           NORMAL_NOISE_PWM_MODE // LOW_NOISE_PWM_MODE //NORMAL_NOISE_PWM_MODE // LOW_NOISE_PWM_MODE
#define ADM32F036_TYPE                  ADM32F036_A3
#define SPI_SCOPE_TEST                  1
//#define TESTCLK                         1

#define SWITCH_FREQUENCY_EMC_TYPE       FIXED_SWICTH_FREQUENCY //RANDOM_SWICTH_FREQUENCY // 固定开关频率还是随机
#define SWITCH_FREQUENCY_AMP            20                      // 上下跳频值

#define SF_INDEPAND_OF_FREQUENCY        SW_RANDOM_DISABLE

#define SPEED_RAMP_LINEAR_MODE          0
#define SPEED_RAMP_S_MODE               1
#define SPEED_RAMP_MODE                 0//0//SPEED_RAMP_S_MODE

// 故障检测时间
#define STALL_FAULT_DETECT_TIME        (4000)    // 4.0S
#define NO_LOAD_FAULT_DETECT_TIME      (5000)    // 5.0S

// 故障恢复时间设置
#define VOLT_FAULT_RECOVER_TIME        (12000)
#define CURRENT_FAULT_RECOVER_TIME     (12000)
#define STALL_FAULT_RECOVER_TIME       (12000)
#define PHASELOSS_FAULT_RECOVER_TIME   (8000)
#define TEMP_FAULT_RECOVER_TIME        (2000)
#define START_FAULT_RECOVER_TIME       (11000)
#define NO_LOAD_FAULT_RECOVER_TIME     (12000)

// 水泵的缺水检测阈值：250HZ, IRMS:1.9A,IDC=1.1A,P=14W...
#define NO_LOAD_FAULT_FREQUENCY        (25000)  // 150.0HZ
#define NO_LOAD_FAULT_IRMS_CUR         (810)    // 410 * 5.0A / 4096 = 0.5A

// 堵转检测阈值：
#define STALL_FAULT_REF                 (6000)  // 150.0HZ
#define STALL_FAULT_OMG                 (4000)  // 40.0HZ


#define YellowLED_ON                GpioDataRegs.GPASET.bit.GPIO18  = 1
#define YellowLED_OFF               GpioDataRegs.GPACLEAR.bit.GPIO18    = 1

#define TRIP_FLASHING_TIME          100
#define WAIT_POWER_ON_TIME          10000       // 进入主程序之后，等待_*0.5ms开始响应欠压处理和运行命令

#define LD_LQ_BASED_ON_CUR          1

#endif // __F_PARAMETER_H__

