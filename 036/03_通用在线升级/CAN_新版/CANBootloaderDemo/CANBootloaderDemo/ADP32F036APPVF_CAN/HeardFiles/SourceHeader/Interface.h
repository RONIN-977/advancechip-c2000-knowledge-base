//#############################################################################
//
// FILE:   Interface.h
//
// TITLE:  Function-to-motor interface declarations.
//
// Description:
// This file declares the shared command buffer and function-layer entry points
// used by the motor control side of the application.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef __F_INTERFACE_H__
#define __F_INTERFACE_H__

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern unsigned int FunToMotorData[10];

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
void InitForFunctionApp(void);                // Function code initialization
void SystemLeve2msFunction(void);             // Function processing function, 2ms

#endif // __F_INTERFACE_H__

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
