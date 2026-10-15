/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                              湖南进芯电子科技有限公司
*                           http://www.advancechip.com/
*
*                           FILE:   board.h
*                           TITLE:  Board Configurations.
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
// EQEP1 -> myEQEP0 Pinmux
//
//
// EQEP1_A - GPIO Settings
//
#define GPIO_PIN_EQEP1_A 6
#define myEQEP0_EQEPA_GPIO 6
#define myEQEP0_EQEPA_PIN_CONFIG GPIO_6_EQEP1_A
//
// EQEP1_B - GPIO Settings
//
#define GPIO_PIN_EQEP1_B 11
#define myEQEP0_EQEPB_GPIO 11
#define myEQEP0_EQEPB_PIN_CONFIG GPIO_11_EQEP1_B
//
// EQEP1_STROBE - GPIO Settings
//
#define GPIO_PIN_EQEP1_STROBE 12
#define myEQEP0_EQEPSTROBE_GPIO 12
#define myEQEP0_EQEPSTROBE_PIN_CONFIG GPIO_12_EQEP1_STROBE
//
// EQEP1_INDEX - GPIO Settings
//
#define GPIO_PIN_EQEP1_INDEX 13
#define myEQEP0_EQEPINDEX_GPIO 13
#define myEQEP0_EQEPINDEX_PIN_CONFIG GPIO_13_EQEP1_INDEX

//*****************************************************************************
//
// EQEP Configurations
//
//*****************************************************************************
#define myEQEP0_BASE EQEP1_BASE
void myEQEP0_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myEQEP0
#define INT_myEQEP0 INT_EQEP1
#define INT_myEQEP0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP5
extern __interrupt void INT_myEQEP0_ISR(void);

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	EQEP_init();
void	INTERRUPT_init();
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
