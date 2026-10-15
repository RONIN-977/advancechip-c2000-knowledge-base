#ifndef _PWM_COMM_FUNC_H_
#define _PWM_COMM_FUNC_H_

#include "Define.h"
#include "ADP32F03x_Device.h"

#ifdef  _PWM_COMM_FUNC_FILE_
#define _PWM_COMM_FUNC_VAR_
#else
#define _PWM_COMM_FUNC_VAR_  extern
#endif

#define  PULL_DOWN_PWM()     //GpioDataRegs.GPASET.bit.GPIO16 = 1
#define  RELEASE_PWM()       //GpioDataRegs.GPACLEAR.bit.GPIO16 = 1

typedef struct
{
    Uint16 PWMState;
    Uint16 PWMStateBak;

    Uint32 PwmLowCnt;
    Uint32 PwmHighCnt;

    Uint32 PwmLowCntTemp;
    Uint32 PwmHighCntTemp;

    Uint32 PwmLowCntAct;
    Uint32 PwmHighCntAct;

    int16  DutyCycle;
    int16  DutyCycleLast;
    int32  DutyCycleAcc;
    int16  DutyCycleFilter;

    int16  SpeedRef;
    int16  SpeedRPM;
    Uint16 OverTempFlag;

    Uint16 RunCmd;
    Uint16 RunTimeCnt;

    Uint16 OverFreqTimeCnt;

    Uint16 FirstPowerOn;
    Uint16 FirstPowerOnCnt;
    Uint16 PullDownPWMFlag;

    Uint16 LowPowerCnt;

    Uint16 ErrorCodeState;
    Uint16 ErrorCodeRestCnt;
    Uint16 ErrorCodePeriodCnt;
    Uint16 ErrorCodeSendCnt;

    Uint16 HeartBitCnt;
}PWM_COMM_DEF;

_PWM_COMM_FUNC_VAR_ PWM_COMM_DEF gPWM_COMM;


typedef struct
{
    Uint16  pwm_reg_periodCount;       // pwm周期计数值
    Uint16  pwm_reg_high_periodCount;  // pwm高计数值
    Uint16  pwm_reg_low_periodCount;   // pwm低计数值
    Uint16  pwm_int_flag;              // PWM 中断标志
    Uint16  pwm_io_status;             // PWM 电平状态
    Uint16  pwm_last_duty;             // 上次PWM占空比
    Uint16  pwm_cur_duty;              // 当前PWM占空比
    Uint16  pwm_duty_direct;           // 0=不变，1=上升，-1=下降
    Uint16  pwm_freq_err_flag;         // 超频标志
    Uint16  Pwm_freq;                  // pwm频率
}PWM_CAPT_TypeDef;
//extern volatile PWM_CAPT_TypeDef pwm_capt_data;   //pwm捕获处理结果 全局变量
extern PWM_CAPT_TypeDef pwm_capt_data;   //pwm捕获处理结果 全局变量


extern void PWMCommInit(void);
extern interrupt void Xint1_isr(void);

#endif /* _PWM_COMM_FUNC_H_ */
