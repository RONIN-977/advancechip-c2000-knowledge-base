#ifndef PWM_COMM_FUNC_H
#define PWM_COMM_FUNC_H

#include "Define.h"
#include "ADP32F03x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    Uint64  pwm_reg_periodCount;
    Uint64  pwm_reg_high_periodCount;
    Uint64  pwm_reg_low_periodCount;
    Uint32  pwm_int_flag;
    Uint32  pwm_cur_duty;
    Uint32  pwm_final_duty;
    Uint32  pwm_freq_err_flag;
    Uint32  Pwm_freq;
} PWM_CAPT_TypeDef;

extern volatile PWM_CAPT_TypeDef pwm_capt_data;

void PWMCommInit(void);
interrupt void Xint1_isr(void);
void PwmInDeal(void);
void ReportError(void);

#ifdef __cplusplus
}
#endif

#endif /* PWM_COMM_FUNC_H */
