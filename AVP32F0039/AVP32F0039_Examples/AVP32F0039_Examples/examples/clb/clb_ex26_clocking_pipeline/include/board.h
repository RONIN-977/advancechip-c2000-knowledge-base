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
// CLB_OUTPUTXBAR2 -> myCLB_OUTPUTXBAR0 Pinmux
//
//
// CLB_OUTPUTXBAR2 - GPIO Settings
//
#define GPIO_PIN_CLB_OUTPUTXBAR2 47
#define myCLB_OUTPUTXBAR0_CLBOUTPUTXBAR_GPIO 47
#define myCLB_OUTPUTXBAR0_CLBOUTPUTXBAR_PIN_CONFIG GPIO_47_CLB_OUTPUTXBAR2

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myCLBForTILE1_BASE CLB1_BASE
void myCLBForTILE1_init();
#define myCLBForTILE2_BASE CLB2_BASE
void myCLBForTILE2_init();

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void myCLB_OUTPUTXBAR0_init();
#define myCLB_OUTPUTXBAR0 XBAR_OUTPUT2
#define myCLB_OUTPUTXBAR0_ENABLED_MUXES (XBAR_MUX04)

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CLB_init();
void	CLB_OUTPUTXBAR_init();
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
