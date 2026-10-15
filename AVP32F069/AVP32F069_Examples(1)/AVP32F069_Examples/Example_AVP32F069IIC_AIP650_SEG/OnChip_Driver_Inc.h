#ifndef ONCHIP_DRIVER_INC_H_H_
#define ONCHIP_DRIVER_INC_H_H_

#include "Debug_Cfg.h"
#include "AVP32F069_Project.h"
#include "Delay.h"
#include "timer0_base.h"
#include "segscan.h"

#define SYSCLK 90000000L

//LED1 低电平亮;
#define LED1_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO25
#define LED1_DIR       GpioCtrlRegs.GPADIR.bit.GPIO25
#define LED1_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO25 =1)
#define LED1_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO25 = 1)
#define LED1_OFF()     (GpioDataRegs.GPASET.bit.GPIO25 = 1)

//LED2 低电平亮;
#define LED2_MUX       GpioCtrlRegs.GPBMUX2.bit.GPIO50
#define LED2_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO50
#define LED2_TOGGLE()  (GpioDataRegs.GPBTOGGLE.bit.GPIO50 =1)
#define LED2_ON()      (GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1)
#define LED2_OFF()     (GpioDataRegs.GPBSET.bit.GPIO50 = 1)

//LED3 低电平亮;
#define LED3_MUX       GpioCtrlRegs.GPAMUX1.bit.GPIO12
#define LED3_DIR       GpioCtrlRegs.GPADIR.bit.GPIO12
#define LED3_TOGGLE()  (GpioDataRegs.GPATOGGLE.bit.GPIO12 =1)
#define LED3_ON()      (GpioDataRegs.GPACLEAR.bit.GPIO12 = 1)
#define LED3_OFF()     (GpioDataRegs.GPASET.bit.GPIO12 = 1)

//LED3 低电平亮;
#define LED4_MUX       GpioCtrlRegs.GPBMUX2.bit.GPIO51
#define LED4_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO51
#define LED4_TOGGLE()  (GpioDataRegs.GPBTOGGLE.bit.GPIO51 =1)
#define LED4_ON()      (GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1)
#define LED4_OFF()     (GpioDataRegs.GPBSET.bit.GPIO51 = 1)

//BEEP蜂鸣器接口
#define BEEP_MUX   GpioCtrlRegs.GPBMUX2.bit.GPIO56
#define BEEP_DIR   GpioCtrlRegs.GPBDIR.bit.GPIO56
#define BEEP_PUD   GpioCtrlRegs.GPBPUD.bit.GPIO56
#define BEEP_ON()  (GpioDataRegs.GPBSET.bit.GPIO56 =1)
#define BEEP_OFF() (GpioDataRegs.GPBCLEAR.bit.GPIO56 =1)

//按键接口
//K1
#define K1_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO20
#define K1_DIR       GpioCtrlRegs.GPADIR.bit.GPIO20
#define K1_PUD       GpioCtrlRegs.GPAPUD.bit.GPIO20
#define rK1_DAT()    GpioDataRegs.GPADAT.bit.GPIO20

//K2
#define K2_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO21
#define K2_DIR       GpioCtrlRegs.GPADIR.bit.GPIO21
#define K2_PUD       GpioCtrlRegs.GPAPUD.bit.GPIO21
#define rK2_DAT()    GpioDataRegs.GPADAT.bit.GPIO21

//K3
#define K3_MUX       GpioCtrlRegs.GPBMUX1.bit.GPIO39
#define K3_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO39
#define K3_PUD       GpioCtrlRegs.GPBPUD.bit.GPIO39
#define rK3_DAT()    GpioDataRegs.GPBDAT.bit.GPIO39

//K4
#define K4_MUX       GpioCtrlRegs.GPBMUX2.bit.GPIO53
#define K4_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO53
#define K4_PUD       GpioCtrlRegs.GPBPUD.bit.GPIO53
#define rK4_DAT()    GpioDataRegs.GPBDAT.bit.GPIO53

#endif
