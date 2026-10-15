//#############################################################################
//
// FILE:   Reference.h
//
// TITLE:  Frequency-reference interface declarations.
//
// Description:
// This file declares the public helpers that update the application frequency
// reference and target command state.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef __F_FRQSRC_H__
#define __F_FRQSRC_H__

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include <ADP32F03x_Device.h>

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
void ReferenceSet(void);
void UpdateFrqSetAim(void);

#endif // __F_FRQSRC_H__

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
