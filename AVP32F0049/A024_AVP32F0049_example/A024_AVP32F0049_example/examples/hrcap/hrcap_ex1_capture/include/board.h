/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  HRCAP Capture and Calibration Example
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
// GPIO2 - GPIO Settings
//
#define myGPIO0_GPIO_PIN_CONFIG GPIO_2_GPIO2

//
// OTHER -> myOTHER0 Pinmux
//
//
// XCLKOUT - GPIO Settings
//
#define GPIO_PIN_XCLKOUT 16
#define myOTHER0_XCLKOUT_GPIO 16
#define myOTHER0_XCLKOUT_PIN_CONFIG GPIO_16_XCLKOUT

//*****************************************************************************
//
// ECAP Configurations
//
//*****************************************************************************
#define myECAP0_BASE ECAP7_BASE
#define myECAP0_HR_BASE HRCAP7_BASE
void myECAP0_init();

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define myGPIO0 2
void myGPIO0_init();

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
#define myINPUTXBARINPUT0_SOURCE 2
#define myINPUTXBARINPUT0_INPUT XBAR_INPUT7
void myINPUTXBARINPUT0_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myECAP0
#define INT_myECAP0 INT_ECAP7
#define INT_myECAP0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP4
extern __interrupt void INT_myECAP0_ISR(void);

// Interrupt Settings for INT_myECAP0_HR
#define INT_myECAP0_HR INT_ECAP7_2
#define INT_myECAP0_HR_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP4
extern __interrupt void INT_myECAP0_HR_ISR(void);

//*****************************************************************************
//
// OTHER Configurations
//
//*****************************************************************************

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
void	ECAP_init();
void	GPIO_init();
void	INPUTXBAR_init();
void	INTERRUPT_init();
void	OTHER_init();
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
