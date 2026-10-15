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
// GPIO0 - GPIO Settings
//
#define myGPIO0_GPIO_PIN_CONFIG GPIO_0_GPIO0

//
// OUTPUTXBAR1 -> myOUTPUTXBAR1 Pinmux
//
//
// OUTPUTXBAR1 - GPIO Settings
//
#define GPIO_PIN_OUTPUTXBAR1 2
#define myOUTPUTXBAR1_OUTPUTXBAR_GPIO 2
#define myOUTPUTXBAR1_OUTPUTXBAR_PIN_CONFIG GPIO_2_OUTPUTXBAR1

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myTILE1_BASE CLB1_BASE
void myTILE1_init();

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO0 0
void myGPIO0_init();

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
void	GPIO_init();
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
