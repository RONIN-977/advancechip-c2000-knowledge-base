/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  Device GPIO Interrupt
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
// GPIO28 - GPIO Settings
//
#define myGPIOInputInterrupt0_GPIO_PIN_CONFIG GPIO_28_GPIO28
//
// GPIO47 - GPIO Settings
//
#define myGPIOOutput0_GPIO_PIN_CONFIG GPIO_47_GPIO47

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIOInputInterrupt0 28
void myGPIOInputInterrupt0_init();
#define myGPIOOutput0 47
void myGPIOOutput0_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	GPIO_init();
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
