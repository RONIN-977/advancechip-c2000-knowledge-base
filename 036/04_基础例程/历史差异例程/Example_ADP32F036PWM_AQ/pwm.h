#ifndef PWM_H_
#define PWM_H_

#include "BspInc.h"

//10K调制，PWM中间对其模式，PWMCLK = 60M/2/载波频率 - 1
#define frequency (30000000/10000 - 1)

#define STOP 0
#define UP   1
#define DOWM 2

typedef struct pwm{
	Uint16 dutyDir;    //方向
	Uint16 dutyCycle;  //PWM占空比
	Uint16 PWMstep;    //改变占空比的单位大小
}pwm;

extern pwm PWM5B;
void InitEPwm5(void);
void ChangePWMDutyCycle(void);
void InitEPwm1_AQ_DB(void);

#endif /* PWM_H_ */
