/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   ecap_ex1_apwm.c
*                         TITLE:  eCAP APWM Example.
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
#define GPIO_PIN_OUTPUTXBAR3 5
#define myOUTPUTXBAR0_OUTPUTXBAR_GPIO 5
#define myOUTPUTXBAR0_OUTPUTXBAR_PIN_CONFIG GPIO_5_OUTPUTXBAR3

//*****************************************************************************
//
// ECAP Configurations
//
//*****************************************************************************
#define myECAP0_BASE ECAP1_BASE
void myECAP0_init();

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
void	OUTPUTXBAR_init();
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
