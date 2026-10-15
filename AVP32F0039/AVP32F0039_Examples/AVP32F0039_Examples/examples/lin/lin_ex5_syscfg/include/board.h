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
// LINB -> myLIN0 Pinmux
//
//
// LINB_RX - GPIO Settings
//
#define GPIO_PIN_LINB_RX 11
#define myLIN0_LINRX_GPIO 11
#define myLIN0_LINRX_PIN_CONFIG GPIO_11_LINB_RX
//
// LINB_TX - GPIO Settings
//
#define GPIO_PIN_LINB_TX 10
#define myLIN0_LINTX_GPIO 10
#define myLIN0_LINTX_PIN_CONFIG GPIO_10_LINB_TX

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myLIN0_LINE0
#define INT_myLIN0_LINE0 INT_LINB_0
#define INT_myLIN0_LINE0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP8
extern __interrupt void level0ISR(void);

//*****************************************************************************
//
// LIN Configurations
//
//*****************************************************************************
#define myLIN0_BASE LINB_BASE
void myLIN0_init();

//*****************************************************************************
//
// SYSCTL Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	INTERRUPT_init();
void	LIN_init();
void	SYSCTL_init();
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
