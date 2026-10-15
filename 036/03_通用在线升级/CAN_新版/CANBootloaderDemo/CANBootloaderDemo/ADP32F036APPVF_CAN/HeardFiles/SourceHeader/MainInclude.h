//#############################################################################
//
// FILE:   MainInclude.h
//
// TITLE:  Main-layer shared declarations.
//
// Description:
// This file exposes the main application entry-point helpers, scheduler timing
// macros, interrupt declarations, and shared startup interfaces.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef MAIN_INCLUDE_H
#define MAIN_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include "Define.h"
#include <ADP32F03x_Device.h>

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#define ABS_INT16(a) (((a) >= 0) ? (a) : (-(a)))
#define ABS_INT32(a) (((a) >= 0L) ? (a) : (-(a)))

#define GetTime() (CpuTimer1.RegsAddr->TIM.all)

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern Uint16 FrequncyToMotor;
extern Uint16 FrqencyCurAimToMotor;

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
extern void InitSysCtrl(void);
extern void InitInterrupt(void);
extern void InitPeripherals(void);
extern void InitForMotorApp(void);
extern void InitForFunctionApp(void);
extern void SetInterruptEnable(void);
extern void ADP32F03x_usDelay(Uint32 Count);

extern void MotorControlMainISR(void);
extern void HardWareErrorDeal(void);

extern void SystemLeve2msMotor(void);
extern void SystemLeve2msFunction(void);

extern void EnableDog(void);
extern void DisableDog(void);
extern void KickDog(void);
extern void ResetDSP(void);

extern void PreDriverTest(void);

extern interrupt void EndOfADCISR(void);
extern interrupt void OneShotTZOfEPWMISR(void);
extern interrupt void ZeroOfEPWMISR(void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of definition

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
