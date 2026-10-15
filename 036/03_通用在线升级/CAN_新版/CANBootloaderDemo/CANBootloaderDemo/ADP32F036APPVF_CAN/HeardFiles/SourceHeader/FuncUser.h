//#############################################################################
//
// FILE:   FuncUser.h
//
// TITLE:  PWM compare helper declarations.
//
// Description:
// This file declares the buffered PWM compare structure shared by the motor
// control implementation.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef  _FUNC_USER_H_
#define  _FUNC_USER_H_

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#ifdef   _FUNC_USER_VAR_
#define  _FUNC_USER_DEF_
#else
#define  _FUNC_USER_DEF_  extern
#endif

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/

typedef struct
{
    unsigned short U_CMP_U;
	unsigned short U_CMP_V;
	unsigned short U_CMP_W;

	unsigned short D_CMP_U;
	unsigned short D_CMP_V;
	unsigned short D_CMP_W;

	unsigned short D_CMP_U_BAK;
	unsigned short D_CMP_V_BAK;
	unsigned short D_CMP_W_BAK;

	unsigned short T1CMP;
	unsigned short T2CMP;

	int  CurrSampleNumAct;
	int  CurrSampleNextNum;
	int  CurrSampleNextNum_bak;
} PWM_CMP_DEF;

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern PWM_CMP_DEF    gPWM_CMP;

#endif

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

