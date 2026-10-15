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

//*****************************************************************************
//
// DMA Configurations
//
//*****************************************************************************
extern const void *srcAddr;
extern const void *destAddr;
#define myDMA0_BASE DMA_CH6_BASE 
#define myDMA0_BURSTSIZE 8U
#define myDMA0_TRANSFERSIZE 16U
#define myDMA0_SRC_WRAPSIZE 65535U
#define myDMA0_DEST_WRAPSIZE 65535U
void myDMA0_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myDMA0
#define INT_myDMA0 INT_DMA_CH6
#define INT_myDMA0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP7
extern __interrupt void INT_myDMA0_ISR(void);

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
void	DMA_init();
void	INTERRUPT_init();
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
