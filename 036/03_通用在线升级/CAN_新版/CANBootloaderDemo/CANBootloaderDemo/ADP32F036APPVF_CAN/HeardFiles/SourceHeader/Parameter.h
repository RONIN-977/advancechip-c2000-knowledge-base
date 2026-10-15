//#############################################################################
//
// FILE:   Parameter.h
//
// TITLE:  Application parameter declarations.
//
// Description:
// This file declares the runtime parameter symbols shared between the control,
// peripheral, and communication modules.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef __F_PARAMETER_H__
#define __F_PARAMETER_H__

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#define CURRENT_SAMPLE_TYPE             CURRENT_SAMPLE_1SHUNT
#define ADM32F036_TYPE                  ADM32F036_A2

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern unsigned int MAX_DC_VOL_3_3V;

extern unsigned int OverCurPointInternal;   // Hardware overcurrent comparison value
extern unsigned int CBCCurPointInternal;    // CBC overcurrent comparison value
extern unsigned int DeadTimeInternal;

extern unsigned int MOTOR_RATED_CUR;

extern unsigned int PWM_FREQUENCY;          // 100 * 0.1kHz PWM frequency
extern unsigned int MAX_LIMIT_FREQ;         // 300.00Hz
extern unsigned int MOTOR_RATED_FRE;

extern int RefSet;
extern unsigned int ACC1;
extern unsigned int ACC2;

extern unsigned int AUTO_RESET_CNT;
extern unsigned int ThetaCoff;
extern unsigned int AVR_Mode;

#endif // __F_PARAMETER_H__

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

