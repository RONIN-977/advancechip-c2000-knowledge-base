//#############################################################################
//
// FILE:   RunSrc.h
//
// TITLE:  Function-layer run logic declarations.
//
// Description:
// This file declares the command structures, run-state types, and ramp helpers
// used by the function-layer speed and acceleration control logic.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef __F_COMMAND_H__
#define __F_COMMAND_H__

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include <ADP32F03x_Device.h>

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#define FUNCCODE_accDecSpdCurve_LINE        0   // Linear acceleration and deceleration

#define LINE_CHANGE_STRTUCT_DEFALUTS       \
{                                          \
    (void (*)(void *))LineRampCalc         \
}

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/
struct MOTOR_MAIN_COMMAND_BITS
{                               // bits  description
    Uint16 run:1;               // 0,    1:run 0:stop
};

union MOTOR_MAIN_COMMAND
{
    Uint16                         all;
    struct MOTOR_MAIN_COMMAND_BITS bit;
};

enum RUN_STATUS
{
    RUN_STATUS_WAIT,
    RUN_STATUS_START,
    RUN_STATUS_NORMAL,
    RUN_STATUS_STOP,
    RUN_STATUS_SHUT_DOWN
};

typedef struct
{
    void (*calc)(void *);

    int32 MaxValue;
    int32 AimValue;
    int32 CurValue;

    Uint32 TickerAll;
    int32 Remainder;
} LINE_RAMP_STR;

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern union MOTOR_MAIN_COMMAND MotorMainCmd;
extern enum RUN_STATUS RunMode;
extern LINE_RAMP_STR FrequencyLineRamp;

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
void RunLogicDeal(void);
void AccDecTimeCalc(void);
void LineRampCalc(LINE_RAMP_STR *p);
void AccAndDecFrqCalc(int32 accTime, int32 decTime, Uint16 AccRampmode);

#endif  // __F_COMMAND_H__

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

