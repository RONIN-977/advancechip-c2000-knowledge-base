#ifndef ONCHIP_DRIVER_INC_H_H_
#define ONCHIP_DRIVER_INC_H_H_

#include "Debug_Cfg.h"
#include "ADP32F03x_Project.h"
#include "Delay.h"
#include "timer0_base.h"

#define SYSCLK 60000000L


#define ADP32F036_48PIN  1
#define VERSION1         0
//LED1 低电平亮;
#if ADP32F036_48PIN
#if VERSION1 //V1.0开发板
#define LED1_MUX       GpioCtrlRegs.GPBMUX1.bit.GPIO34
#define LED1_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO34
#define LED1_TOGGLE()  (GpioDataRegs.GPBTOGGLE.bit.GPIO34 =1)
#define LED1_ON()      (GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1)
#define LED1_OFF()     (GpioDataRegs.GPBSET.bit.GPIO34 = 1)
#else //V1.1开发板
#define LED1_MUX       GpioCtrlRegs.GPAMUX1.bit.GPIO3
#define LED1_DIR       GpioCtrlRegs.GPADIR.bit.GPIO3
#define LED1_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO3 =1)
#define LED1_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO3 = 1)
#define LED1_OFF()     (GpioDataRegs.GPASET.bit.GPIO3= 1)
#endif
#else  //56PIN开发板
#define LED1_MUX       GpioCtrlRegs.GPAMUX1.bit.GPIO1
#define LED1_DIR       GpioCtrlRegs.GPADIR.bit.GPIO1
#define LED1_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO1 =1)
#define LED1_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO1 = 1)
#define LED1_OFF()     (GpioDataRegs.GPASET.bit.GPIO1= 1)
#endif



//BEEP蜂鸣器接口
#define BEEP_MUX   GpioCtrlRegs.GPAMUX1.bit.GPIO15
#define BEEP_DIR   GpioCtrlRegs.GPADIR.bit.GPIO15
#define BEEP_PUD   GpioCtrlRegs.GPAPUD.bit.GPIO15
#define BEEP_ON()  (GpioDataRegs.GPASET.bit.GPIO15 =1)
#define BEEP_OFF() (GpioDataRegs.GPACLEAR.bit.GPIO15 =1)

//按键接口
//K1
#define K1_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO17
#define K1_DIR       GpioCtrlRegs.GPADIR.bit.GPIO17
#define K1_PUD       GpioCtrlRegs.GPAPUD.bit.GPIO17
#define rK1_DAT()    GpioDataRegs.GPADAT.bit.GPIO17

//K2
#define K2_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO16
#define K2_DIR       GpioCtrlRegs.GPADIR.bit.GPIO16
#define K2_PUD       GpioCtrlRegs.GPAPUD.bit.GPIO16
#define rK2_DAT()    GpioDataRegs.GPADAT.bit.GPIO16



#endif
