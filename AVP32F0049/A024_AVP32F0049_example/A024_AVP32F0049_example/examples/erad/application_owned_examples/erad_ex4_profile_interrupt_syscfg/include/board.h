/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.h
*                         TITLE:  ERAD Profiling Interrupts Example
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
// ERAD Configurations
//
//*****************************************************************************
#define myERAD0_COUNTER1_BASE      ERAD_COUNTER1_BASE
//*****************************************************************************
//
// ERAD Configurations
//
//*****************************************************************************
#define myERAD0_COUNTER2_BASE      ERAD_COUNTER2_BASE
//*****************************************************************************
//
// ERAD Configurations
//
//*****************************************************************************
#define myERAD0_COUNTER3_BASE      ERAD_COUNTER3_BASE
//*****************************************************************************
//
// ERAD Configurations
//
//*****************************************************************************
#define myERAD0_COUNTER4_BASE      ERAD_COUNTER4_BASE
extern void* addr_cpuTimer1ISR;
#define myERAD0_BUSCOMP1_BASE     ERAD_HWBP1_BASE
extern void* addr_cpuTimer1IntCount;
#define myERAD0_BUSCOMP2_BASE     ERAD_HWBP2_BASE

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	ERAD_init();
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
