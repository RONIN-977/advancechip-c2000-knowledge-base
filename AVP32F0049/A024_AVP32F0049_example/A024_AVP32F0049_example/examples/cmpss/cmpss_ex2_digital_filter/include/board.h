/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
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
// OUTPUTXBAR3 -> myOUTPUTXBAR0 Pinmux
//
//
// OUTPUTXBAR3 - GPIO Settings
//
#define GPIO_PIN_OUTPUTXBAR3 14
#define myOUTPUTXBAR0_OUTPUTXBAR_GPIO 14
#define myOUTPUTXBAR0_OUTPUTXBAR_PIN_CONFIG GPIO_14_OUTPUTXBAR3

//*****************************************************************************
//
// CMPSS Configurations
//
//*****************************************************************************
#define myCMPSS0_BASE CMPSS1_BASE
#define myCMPSS0_HIGH_COMP_BASE CMPSS1_BASE    
#define myCMPSS0_LOW_COMP_BASE CMPSS1_BASE    
void myCMPSS0_init();

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void myOUTPUTXBAR0_init();
#define myOUTPUTXBAR0 XBAR_OUTPUT3
#define myOUTPUTXBAR0_ENABLED_MUXES (XBAR_MUX00)

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CMPSS_init();
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
