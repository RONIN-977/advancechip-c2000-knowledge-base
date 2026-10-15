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
// SPIA -> SPIA_peripheral Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 8
#define SPIA_peripheral_SPIPICO_GPIO 8
#define SPIA_peripheral_SPIPICO_PIN_CONFIG GPIO_8_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 10
#define SPIA_peripheral_SPIPOCI_GPIO 10
#define SPIA_peripheral_SPIPOCI_PIN_CONFIG GPIO_10_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 9
#define SPIA_peripheral_SPICLK_GPIO 9
#define SPIA_peripheral_SPICLK_PIN_CONFIG GPIO_9_SPIA_CLK
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 11
#define SPIA_peripheral_SPIPTE_GPIO 11
#define SPIA_peripheral_SPIPTE_PIN_CONFIG GPIO_11_SPIA_STE

//
// SPIB -> SPIB_controller Pinmux
//
//
// SPIB_PICO - GPIO Settings
//
#define GPIO_PIN_SPIB_PICO 40
#define SPIB_controller_SPIPICO_GPIO 40
#define SPIB_controller_SPIPICO_PIN_CONFIG GPIO_40_SPIB_SIMO
//
// SPIB_POCI - GPIO Settings
//
#define GPIO_PIN_SPIB_POCI 41
#define SPIB_controller_SPIPOCI_GPIO 41
#define SPIB_controller_SPIPOCI_PIN_CONFIG GPIO_41_SPIB_SOMI
//
// SPIB_CLK - GPIO Settings
//
#define GPIO_PIN_SPIB_CLK 22
#define SPIB_controller_SPICLK_GPIO 22
#define SPIB_controller_SPICLK_PIN_CONFIG GPIO_22_SPIB_CLK
//
// SPIB_PTE - GPIO Settings
//
#define GPIO_PIN_SPIB_PTE 23
#define SPIB_controller_SPIPTE_GPIO 23
#define SPIB_controller_SPIPTE_PIN_CONFIG GPIO_23_SPIB_STE

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_SPIA_peripheral_RX
#define INT_SPIA_peripheral_RX INT_SPIA_RX
#define INT_SPIA_peripheral_RX_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP6
extern __interrupt void spiaRxFIFOISR(void);

// Interrupt Settings for INT_SPIA_peripheral_TX
#define INT_SPIA_peripheral_TX INT_SPIA_TX
#define INT_SPIA_peripheral_TX_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP6
extern __interrupt void INT_SPIA_peripheral_TX_ISR(void);

// Interrupt Settings for INT_SPIB_controller_RX
#define INT_SPIB_controller_RX INT_SPIB_RX
#define INT_SPIB_controller_RX_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP6
extern __interrupt void INT_SPIB_controller_RX_ISR(void);

// Interrupt Settings for INT_SPIB_controller_TX
#define INT_SPIB_controller_TX INT_SPIB_TX
#define INT_SPIB_controller_TX_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP6
extern __interrupt void spibTxFIFOISR(void);

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define SPIA_peripheral_BASE SPIA_BASE
#define SPIA_peripheral_BITRATE 500000
#define SPIA_peripheral_DATAWIDTH 16
void SPIA_peripheral_init();
#define SPIB_controller_BASE SPIB_BASE
#define SPIB_controller_BITRATE 500000
#define SPIB_controller_DATAWIDTH 16
void SPIB_controller_init();

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
void	SPI_init();
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
