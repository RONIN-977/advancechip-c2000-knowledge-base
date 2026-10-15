/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  DCC Continuous clock monitoring with sysconfig
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
// DCC Configurations
//
//*****************************************************************************
#define myDCC0_BASE DCC0_BASE
#define myDCC0_COUNTER0SEED 2880
#define myDCC0_VALIDCOUNTER0SEED 240
#define myDCC0_COUNTER1SEED 30000
#define myDCC0_FSYSCLK 100
#define myDCC0_FCLK0 10
#define myDCC0_FCLK1 100
#define myDCC0_DCCERRTOL 1
#define myDCC0_FREQERRTOL 3
void myDCC0_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myDCC0
#define INT_myDCC0 INT_DCC
#define INT_myDCC0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP7
extern __interrupt void INT_myDCC0_ISR(void);

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	DCC_init();
void	INTERRUPT_init();
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
