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
// SPIA -> mySPIForTILE1 Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 11
#define mySPIForTILE1_SPIPICO_GPIO 11
#define mySPIForTILE1_SPIPICO_PIN_CONFIG GPIO_11_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 1
#define mySPIForTILE1_SPIPOCI_GPIO 1
#define mySPIForTILE1_SPIPOCI_PIN_CONFIG GPIO_1_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 12
#define mySPIForTILE1_SPICLK_GPIO 12
#define mySPIForTILE1_SPICLK_PIN_CONFIG GPIO_12_SPIA_CLK
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 0
#define mySPIForTILE1_SPIPTE_GPIO 0
#define mySPIForTILE1_SPIPTE_PIN_CONFIG GPIO_0_SPIA_STE

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myCLBForTILE1_BASE CLB1_BASE
void myCLBForTILE1_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define mySPIForTILE1_BASE SPIA_BASE
#define mySPIForTILE1_BITRATE 1000000
void mySPIForTILE1_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	CLB_init();
void	SPI_init();
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
