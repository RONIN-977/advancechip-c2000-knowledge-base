#ifndef ONCHIP_DRIVER_INC_H_H_
#define ONCHIP_DRIVER_INC_H_H_

#include "Debug_Cfg.h"
#include "ADP32F03x_Project.h"
#include "Delay.h"
#include "timer0_base.h"
#include "AD.h"

#define SYSCLK 60000000L

//LED1 低电平亮;
#define LED1_MUX       GpioCtrlRegs.GPAMUX1.bit.GPIO13
#define LED1_DIR       GpioCtrlRegs.GPADIR.bit.GPIO13
#define LED1_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO13 =1)
#define LED1_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO13 = 1)
#define LED1_OFF()     (GpioDataRegs.GPASET.bit.GPIO13 = 1)

//LED2 低电平亮;
#define LED2_MUX       GpioCtrlRegs.GPAMUX1.bit.GPIO14
#define LED2_DIR       GpioCtrlRegs.GPADIR.bit.GPIO14
#define LED2_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO14 =1)
#define LED2_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO14 = 1)
#define LED2_OFF()     (GpioDataRegs.GPASET.bit.GPIO14 = 1)

//LED3 低电平亮;
#define LED3_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO20
#define LED3_DIR       GpioCtrlRegs.GPADIR.bit.GPIO20
#define LED3_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO20 =1)
#define LED3_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO20 = 1)
#define LED3_OFF()     (GpioDataRegs.GPASET.bit.GPIO20 = 1)

//LED3 低电平亮;
#define LED4_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO21
#define LED4_DIR       GpioCtrlRegs.GPADIR.bit.GPIO21
#define LED4_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO21 =1)
#define LED4_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO21 = 1)
#define LED4_OFF()     (GpioDataRegs.GPASET.bit.GPIO21 = 1)

//BEEP蜂鸣器接口
#define BEEP_MUX   GpioCtrlRegs.GPAMUX1.bit.GPIO15
#define BEEP_DIR   GpioCtrlRegs.GPADIR.bit.GPIO15
#define BEEP_PUD   GpioCtrlRegs.GPAPUD.bit.GPIO15
#define BEEP_ON()  (GpioDataRegs.GPASET.bit.GPIO15 =1)
#define BEEP_OFF() (GpioDataRegs.GPACLEAR.bit.GPIO15 =1)

//按键接口
//K1
#define K1_MUX       GpioCtrlRegs.GPBMUX1.bit.GPIO39
#define K1_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO39
#define K1_PUD       GpioCtrlRegs.GPBPUD.bit.GPIO39
#define rK1_DAT()    GpioDataRegs.GPBDAT.bit.GPIO39

//K2
#define K2_MUX       GpioCtrlRegs.GPAMUX1.bit.GPIO12
#define K2_DIR       GpioCtrlRegs.GPADIR.bit.GPIO12
#define K2_PUD       GpioCtrlRegs.GPAPUD.bit.GPIO12
#define rK2_DAT()    GpioDataRegs.GPADAT.bit.GPIO12

//K3
#define K3_MUX       GpioCtrlRegs.GPBMUX1.bit.GPIO44
#define K3_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO44
#define K3_PUD       GpioCtrlRegs.GPBPUD.bit.GPIO44
#define rK3_DAT()    GpioDataRegs.GPBDAT.bit.GPIO44

//K4
#define K4_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO25
#define K4_DIR       GpioCtrlRegs.GPADIR.bit.GPIO25
#define K4_PUD       GpioCtrlRegs.GPAPUD.bit.GPIO25
#define rK4_DAT()    GpioDataRegs.GPADAT.bit.GPIO25

#endif
