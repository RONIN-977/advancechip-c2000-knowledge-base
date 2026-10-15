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
// GPIO11 - GPIO Settings
//
#define SPI_CS_GPIO_GPIO_PIN_CONFIG GPIO_11_GPIO11

//
// SPIA -> SPI_EEPROM Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 8
#define SPI_EEPROM_SPIPICO_GPIO 8
#define SPI_EEPROM_SPIPICO_PIN_CONFIG GPIO_8_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 10
#define SPI_EEPROM_SPIPOCI_GPIO 10
#define SPI_EEPROM_SPIPOCI_PIN_CONFIG GPIO_10_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 9
#define SPI_EEPROM_SPICLK_GPIO 9
#define SPI_EEPROM_SPICLK_PIN_CONFIG GPIO_9_SPIA_CLK

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define SPI_CS_GPIO 11
void SPI_CS_GPIO_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define SPI_EEPROM_BASE SPIA_BASE
#define SPI_EEPROM_BITRATE 1000000
#define SPI_EEPROM_DATAWIDTH 8
void SPI_EEPROM_init();

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
