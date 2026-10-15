/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
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
// GPIO4 - GPIO Settings
//
#define myGPIO_INPUT_GPIO_PIN_CONFIG GPIO_4_GPIO4

//
// OUTPUTXBAR3 -> myOUTPUTXBAR0 Pinmux
//
//
// OUTPUTXBAR3 - GPIO Settings
//
#define GPIO_PIN_OUTPUTXBAR3 5
#define myOUTPUTXBAR0_OUTPUTXBAR_GPIO 5
#define myOUTPUTXBAR0_OUTPUTXBAR_PIN_CONFIG GPIO_5_OUTPUTXBAR3

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO_INPUT 4
void myGPIO_INPUT_init();

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
#define myINPUTXBARINPUT1_SOURCE 4
#define myINPUTXBARINPUT1_INPUT XBAR_INPUT4
void myINPUTXBARINPUT1_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myGPIO_INPUT_XINT
#define INT_myGPIO_INPUT_XINT INT_XINT1
#define INT_myGPIO_INPUT_XINT_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP1
extern __interrupt void INT_myGPIO_INPUT_XINT_ISR(void);

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void myOUTPUTXBAR0_init();
#define myOUTPUTXBAR0 XBAR_OUTPUT3
#define myOUTPUTXBAR0_ENABLED_MUXES (XBAR_MUX07)

//*****************************************************************************
//
// XINT Configurations
//
//*****************************************************************************
#define myGPIO_INPUT_XINT GPIO_INT_XINT1
#define myGPIO_INPUT_XINT_TYPE GPIO_INT_TYPE_FALLING_EDGE
void myGPIO_INPUT_XINT_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	GPIO_init();
void	INPUTXBAR_init();
void	INTERRUPT_init();
void	OUTPUTXBAR_init();
void	XINT_init();
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
