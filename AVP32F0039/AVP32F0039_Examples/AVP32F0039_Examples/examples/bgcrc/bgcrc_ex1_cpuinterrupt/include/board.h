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
// BGCRC Configurations
//
//*****************************************************************************
#define myBGCRC0_BASE BGCRC_CPU_BASE
#define myBGCRC0_EMUCTRL BGCRC_EMUCTRL_FREE
#define myBGCRC0_NMIMODE BGCRC_NMI_DISABLE
#define myBGCRC0_MODE BGCRC_CRC_MODE
#define myBGCRC0_WDMIN 0x0
#define myBGCRC0_WDMAX 0xFFFFFFFF
#define myBGCRC0_BLOCKSIZE 1024
#define myBGCRC0_GOLDENVALUE 0x8474ca32
#define myBGCRC0_SEEDVALUE 0
#define myBGCRC0_INTFLAGS (0|BGCRC_TEST_DONE)
#define myBGCRC0_LOCKREGS (0)
extern const void *td_gs0;
#define myBGCRC0_STARTADDR (uint32_t)td_gs0      
void myBGCRC0_init();


//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myBGCRC0
#define INT_myBGCRC0 INT_BGCRC
#define INT_myBGCRC0_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP9
extern __interrupt void bgcrcISR(void);

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	BGCRC_init();
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
