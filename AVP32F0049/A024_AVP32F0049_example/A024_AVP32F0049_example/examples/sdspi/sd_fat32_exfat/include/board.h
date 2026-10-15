/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  SD FATFS Library Example.
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
#define mySDCardCS_GPIO_PIN_CONFIG GPIO_28_GPIO28

//
// SPIB -> mySDCardSPI Pinmux
//

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define mySDCardCS 28
void mySDCardCS_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define mySDCardSPI_BASE SPIB_BASE
#define mySDCardSPI_BITRATE 1000000
void mySDCardSPI_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	GPIO_init();
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
