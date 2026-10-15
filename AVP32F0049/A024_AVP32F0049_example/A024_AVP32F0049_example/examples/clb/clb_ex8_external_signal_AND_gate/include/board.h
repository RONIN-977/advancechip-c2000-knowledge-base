/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                              湖南进芯电子科技有限公司
*                           http://www.advancechip.com/
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
// GPIO0 - GPIO Settings
//
#define myGPIO0_GPIO_PIN_CONFIG GPIO_0_GPIO0
//
// GPIO1 - GPIO Settings
//
#define myGPIO1_GPIO_PIN_CONFIG GPIO_1_GPIO1

//
// OUTPUTXBAR1 -> myOUTPUTXBAR1 Pinmux
//
//
// OUTPUTXBAR1 - GPIO Settings
//
#define GPIO_PIN_OUTPUTXBAR1 24
#define myOUTPUTXBAR1_OUTPUTXBAR_GPIO 24
#define myOUTPUTXBAR1_OUTPUTXBAR_PIN_CONFIG GPIO_24_OUTPUTXBAR1

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myCLBTILE1_BASE CLB1_BASE
void myCLBTILE1_init();

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
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO0 0
void myGPIO0_init();
#define myGPIO1 1
void myGPIO1_init();

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
#define myINPUTXBARINPUT0_SOURCE 0
#define myINPUTXBARINPUT0_INPUT XBAR_INPUT1
void myINPUTXBARINPUT0_init();
#define myINPUTXBARINPUT1_SOURCE 1
#define myINPUTXBARINPUT1_INPUT XBAR_INPUT2
void myINPUTXBARINPUT1_init();

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void myOUTPUTXBAR1_init();
#define myOUTPUTXBAR1 XBAR_OUTPUT1
#define myOUTPUTXBAR1_ENABLED_MUXES (XBAR_MUX01)

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CLB_init();
void	CLBXBAR_init();
void	GPIO_init();
void	INPUTXBAR_init();
void	OUTPUTXBAR_init();
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
