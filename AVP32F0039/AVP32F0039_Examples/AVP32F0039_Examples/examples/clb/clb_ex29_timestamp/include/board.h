/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  Board Configurations.
*******************************************************************************************/

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// EPWM1 -> myEPWM1 Pinmux
//
//
// EPWM1_A - GPIO Settings
//
#define GPIO_PIN_EPWM1_A 0
#define myEPWM1_EPWMA_GPIO 0
#define myEPWM1_EPWMA_PIN_CONFIG GPIO_0_EPWM1_A
//
// GPIO11 - GPIO Settings
//
#define myGPIO11_GPIO_PIN_CONFIG GPIO_11_GPIO11
//
// GPIO12 - GPIO Settings
//
#define myGPIO12_GPIO_PIN_CONFIG GPIO_12_GPIO12
//
// GPIO13 - GPIO Settings
//
#define myGPIO13_GPIO_PIN_CONFIG GPIO_13_GPIO13

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myTILE1_BASE CLB1_BASE
void myTILE1_init();

//*****************************************************************************
//
// CLBXBAR Configurations
//
//*****************************************************************************
void myCLBXBAR0_init();
#define myCLBXBAR0 XBAR_AUXSIG0

#define myCLBXBAR0_ENABLED_MUXES (XBAR_MUX01)
void myCLBXBAR1_init();
#define myCLBXBAR1 XBAR_AUXSIG1

#define myCLBXBAR1_ENABLED_MUXES (XBAR_MUX03)

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
#define myEPWM1_BASE EPWM1_BASE
#define myEPWM1_TBPRD 0
#define myEPWM1_COUNTER_MODE EPWM_COUNTER_MODE_STOP_FREEZE
#define myEPWM1_TBPHS 0
#define myEPWM1_CMPA 0
#define myEPWM1_CMPB 0
#define myEPWM1_CMPC 0
#define myEPWM1_CMPD 0
#define myEPWM1_DBRED 0
#define myEPWM1_DBFED 0
#define myEPWM1_TZA_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM1_TZB_ACTION EPWM_TZ_ACTION_HIGH_Z
#define myEPWM1_INTERRUPT_SOURCE EPWM_INT_TBCTR_DISABLED

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO11 11
void myGPIO11_init();
#define myGPIO12 12
void myGPIO12_init();
#define myGPIO13 13
void myGPIO13_init();

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
#define myINPUTXBARINPUT0_SOURCE 12
#define myINPUTXBARINPUT0_INPUT XBAR_INPUT1
void myINPUTXBARINPUT0_init();
#define myINPUTXBARINPUT1_SOURCE 13
#define myINPUTXBARINPUT1_INPUT XBAR_INPUT2
void myINPUTXBARINPUT1_init();

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CLB_init();
void	CLBXBAR_init();
void	EPWM_init();
void	GPIO_init();
void	INPUTXBAR_init();
void	SYNC_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
