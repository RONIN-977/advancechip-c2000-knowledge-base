/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  SPI Digital Loopback with FIFO Interrupts
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
// SPIA -> SPIA_peripheral Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 16
#define SPIA_peripheral_SPIPICO_GPIO 16
#define SPIA_peripheral_SPIPICO_PIN_CONFIG GPIO_16_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 17
#define SPIA_peripheral_SPIPOCI_GPIO 17
#define SPIA_peripheral_SPIPOCI_PIN_CONFIG GPIO_17_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 56
#define SPIA_peripheral_SPICLK_GPIO 56
#define SPIA_peripheral_SPICLK_PIN_CONFIG GPIO_56_SPIA_CLK
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 57
#define SPIA_peripheral_SPIPTE_GPIO 57
#define SPIA_peripheral_SPIPTE_PIN_CONFIG GPIO_57_SPIA_STE

//
// SPIB -> SPIB_controller Pinmux
//
//
// SPIB_PICO - GPIO Settings
//
#define GPIO_PIN_SPIB_PICO 24
#define SPIB_controller_SPIPICO_GPIO 24
#define SPIB_controller_SPIPICO_PIN_CONFIG GPIO_24_SPIB_SIMO
//
// SPIB_POCI - GPIO Settings
//
#define GPIO_PIN_SPIB_POCI 31
#define SPIB_controller_SPIPOCI_GPIO 31
#define SPIB_controller_SPIPOCI_PIN_CONFIG GPIO_31_SPIB_SOMI
//
// SPIB_CLK - GPIO Settings
//
#define GPIO_PIN_SPIB_CLK 22
#define SPIB_controller_SPICLK_GPIO 22
#define SPIB_controller_SPICLK_PIN_CONFIG GPIO_22_SPIB_CLK
//
// SPIB_PTE - GPIO Settings
//
#define GPIO_PIN_SPIB_PTE 27
#define SPIB_controller_SPIPTE_GPIO 27
#define SPIB_controller_SPIPTE_PIN_CONFIG GPIO_27_SPIB_STE

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define SPIA_peripheral_BASE SPIA_BASE
#define SPIA_peripheral_BITRATE 500000
void SPIA_peripheral_init();
#define SPIB_controller_BASE SPIB_BASE
#define SPIB_controller_BITRATE 500000
void SPIB_controller_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
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
