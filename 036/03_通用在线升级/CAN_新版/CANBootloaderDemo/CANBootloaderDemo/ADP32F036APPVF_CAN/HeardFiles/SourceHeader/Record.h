//#############################################################################
//
// FILE:   Record.h
//
// TITLE:  Runtime recording helper declarations.
//
// Description:
// This file declares the helpers used to manage internal sample recording and
// read back logged values.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef RECORD_HPP_
#define RECORD_HPP_

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include <ADP32F03x_Device.h>

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
void MoveToNextRow(void);
void ControlAllowRecord(int32 Value, int32 SampleRateInput);
void ChangeSampleRate(int32 SampleRateInput);
void ResetIndex(void);
void IsRecordThisSample(int32 Data0, int32 Data1, int32 Data2, int32 Data3,
						int32 Data4, int32 Data5, int32 Data6);
int32 GetResult(int16 Channal, int16 Index);

#endif

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
