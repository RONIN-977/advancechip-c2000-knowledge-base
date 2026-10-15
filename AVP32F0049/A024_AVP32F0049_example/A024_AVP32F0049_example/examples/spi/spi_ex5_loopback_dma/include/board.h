/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  SPI Digital Loopback with DMA
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
// SPIA -> mySPI0 Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 16
#define mySPI0_SPIPICO_GPIO 16
#define mySPI0_SPIPICO_PIN_CONFIG GPIO_16_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 10
#define mySPI0_SPIPOCI_GPIO 10
#define mySPI0_SPIPOCI_PIN_CONFIG GPIO_10_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 18
#define mySPI0_SPICLK_GPIO 18
#define mySPI0_SPICLK_PIN_CONFIG GPIO_18_SPIA_CLK
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 11
#define mySPI0_SPIPTE_GPIO 11
#define mySPI0_SPIPTE_PIN_CONFIG GPIO_11_SPIA_STE

//*****************************************************************************
//
// DMA Configurations
//
//*****************************************************************************
extern const void *destAddr;
#define mySPI0_RX_DMA_BASE DMA_CH6_BASE 
#define mySPI0_RX_DMA_BURSTSIZE 8U
#define mySPI0_RX_DMA_TRANSFERSIZE 16U
void mySPI0_RX_DMA_init();
extern const void *srcAddr;
#define mySPI0_TX_DMA_BASE DMA_CH5_BASE 
#define mySPI0_TX_DMA_BURSTSIZE 8U
#define mySPI0_TX_DMA_TRANSFERSIZE 16U
void mySPI0_TX_DMA_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_mySPI0_RX_DMA
#define INT_mySPI0_RX_DMA INT_DMA_CH6
#define INT_mySPI0_RX_DMA_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP7
extern __interrupt void INT_mySPI0_RX_DMA_ISR(void);

// Interrupt Settings for INT_mySPI0_TX_DMA
#define INT_mySPI0_TX_DMA INT_DMA_CH5
#define INT_mySPI0_TX_DMA_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP7
extern __interrupt void INT_mySPI0_TX_DMA_ISR(void);

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define mySPI0_BASE SPIA_BASE
#define mySPI0_BITRATE 500000
#define mySPI0_RX_DMA_TRIGGER DMA_TRIGGER_SPIARX           
#define mySPI0_RX_DMA_ADDRESS ((uint16_t *)(SPIA_BASE + SPI_O_RXBUF))
#define mySPI0_TX_DMA_TRIGGER DMA_TRIGGER_SPIATX  
#define mySPI0_TX_DMA_ADDRESS ((uint16_t *)(SPIA_BASE + SPI_O_TXBUF))
void mySPI0_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	DMA_init();
void	INTERRUPT_init();
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
