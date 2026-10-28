/*
 * uC_hal.h
 *
 *  Created on: Feb 11, 2022
 *      Author: rand C
 */

#ifndef UC_HAL_H_
#define UC_HAL_H_


#include "uC_Inc.h"


#define  SysTick()          (CpuTimer1Regs.TIM.all)
#define  SYS_TICK_05MS            (100 * 500L)
/* short delay, reused from F28002x_Examples.h */
#define HAL_DELAY_US(A)     DELAY_US(A)

/* set duty cycle */
//#define HAL_SET_DUTY(x)     EPwm1Regs.CMPA.half.CMPA = x

/* control analog multiplex */
/* INH pin */
#define HAL_INH_SET         GpioDataRegs.GPASET.bit.GPIO8 = 1
#define HAL_INH_CLR         GpioDataRegs.GPACLEAR.bit.GPIO8 = 1
#define HAL_INH_TGL         GpioDataRegs.GPATOGGLE.bit.GPIO8 = 1

/* CTRL pin */
#define HAL_CTRL_SET        GpioDataRegs.GPBSET.bit.GPIO39 = 1
#define HAL_CTRL_CLR        GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1
#define HAL_CTRL_TGL        GpioDataRegs.GPBTOGGLE.bit.GPIO39 = 1
#define HAL_CTRL_STAT       GpioDataRegs.GPBDAT.bit.GPIO39

/* cpu load test */
#define HAL_BEEP_SET         GpioDataRegs.GPASET.bit.GPIO10 = 1
#define HAL_BEEP_CLR         GpioDataRegs.GPACLEAR.bit.GPIO10 = 1
#define HAL_BEEP_TGL         GpioDataRegs.GPATOGGLE.bit.GPIO10 = 1
#define FO_FaultOc()         (GpioDataRegs.GPADAT.bit.GPIO30 == 0)
/* ADC registers */
#define HAL_ADC_RESULT_COS  AdccResultRegs.ADCRESULT0
#define HAL_ADC_RESULT_SIN  AdcaResultRegs.ADCRESULT0
#define HAL_ADC_RESULT_EXC  AdcaResultRegs.ADCRESULT1
#define HAL_ADC_RESULT_VDC  AdccResultRegs.ADCRESULT1
#define HAL_ADC_RESULT_IU   AdcaResultRegs.ADCRESULT2
#define HAL_ADC_RESULT_IV   AdccResultRegs.ADCRESULT2
#define HAL_ADC_RESULT_ACC2 AdcaResultRegs.ADCRESULT3
#define HAL_ADC_RESULT_ACC1 AdccResultRegs.ADCRESULT3
#define HAL_ADC_RESULT_TEMPIGBT  AdcaResultRegs.ADCRESULT4
#define HAL_ADC_RESULT_V12DC     AdccResultRegs.ADCRESULT4
#define HAL_ADC_RESULT_TEMPMOTOR AdcaResultRegs.ADCRESULT5

#define PWM_COUNT_DOWN              (0 == EPwm1Regs.TBSTS.bit.CTRDIR) // down
#define PWM_COUNT_UP             (1 == EPwm1Regs.TBSTS.bit.CTRDIR) // up

/* system ctrl global variables */
extern u16  TaskCycle;
#endif /* 01_SYSINIT_UC_HAL_H_ */
