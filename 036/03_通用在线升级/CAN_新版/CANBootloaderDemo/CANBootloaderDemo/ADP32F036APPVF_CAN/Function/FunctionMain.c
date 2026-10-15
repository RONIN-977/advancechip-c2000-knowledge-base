//#############################################################################
//
// FILE:   FunctionMain.c
//
// TITLE:  Function-layer application control logic.
//
// Description:
// This file implements the 2 ms application task, run-state transitions,
// reference handling, and frequency ramp calculations used by the function
// layer.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include "Interface.h"
#include "RunSrc.h"
#include "Reference.h"
#include "MotorInclude.h"

/*============================================================================*/
/*                          GLOBAL DATA DEFINITIONS                           */
/*============================================================================*/
Uint16 FrequncyToMotor;
Uint16 FrqencyCurAimToMotor;
union MOTOR_MAIN_COMMAND MotorMainCmd;
enum RUN_STATUS RunMode;
Uint32 AccelerationTime;
Uint32 DecelerationTime;
Uint16 ForwardCmd;
int32 FrqencyCurrent;
int32 FrequencyTemp;
int32 FrqencyAim;
int32 FrqencyAimTemp;
int32 FrqencyCurAim;
Uint32 MaxFrequency;
LINE_RAMP_STR FrequencyLineRamp = LINE_CHANGE_STRTUCT_DEFALUTS;

/*============================================================================*/
/*                           FUNCTION DECLARATIONS                            */
/*============================================================================*/
void StartLogicMode(void);
void RunningLogicMode(void);
void CostStopMode(void);
void UpdateDataFunc2Core(void);

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Run the 2 ms function-layer task.
//!
//! This function updates the reference input, executes the function-layer run
//! logic, and forwards the latest command and frequency state to the motor
//! control layer.
//!
//! \return None.
//
//*****************************************************************************
void SystemLeve2msFunction(void)
{
    ReferenceSet();               // Frequency reference setting
    RunLogicDeal();               // Running logic processing
    UpdateDataFunc2Core();      // Update data from function to core
}

//*****************************************************************************
//
//! Initialize the function-layer application state.
//!
//! This function performs the startup synchronization required before the main
//! control loop begins updating the motor-facing data buffer.
//!
//! \return None.
//
//*****************************************************************************
void InitForFunctionApp(void)
{
	// Initialization
    UpdateDataFunc2Core();
}

//*****************************************************************************
//
//! Update the function-to-motor data exchange buffer.
//!
//! This function scales the current and target frequencies into the motor-side
//! fixed-point format and writes the latest command data into the shared
//! communication buffer.
//!
//! \return None.
//
//*****************************************************************************
void UpdateDataFunc2Core(void)
{
    FrequncyToMotor      = (FrqencyCurrent << 15) / ((signed long)MaxFrequency + 200);
    FrqencyCurAimToMotor = (FrqencyCurAim << 15) / ((signed long)MaxFrequency + 200);

    FunToMotorData[0]  = MotorMainCmd.all;
    FunToMotorData[1]  = FrequncyToMotor;
    FunToMotorData[2]  = 0;
	FunToMotorData[3]  = 0; 	// Frequency default 8KHz
	FunToMotorData[4]  = 0; 	// Frequency
	FunToMotorData[5] = 0; 	// Frequency
	FunToMotorData[6] = 0;	// DC link voltage
	FunToMotorData[7] = 0;
	FunToMotorData[8] = 0;	// DC link frequency
}

//*****************************************************************************
//
//! Execute the main function-layer run logic.
//!
//! This function refreshes the acceleration and deceleration settings, updates
//! the current run mode, applies the current frequency target, and dispatches
//! the active state handler.
//!
//! \return None.
//
//*****************************************************************************
void RunLogicDeal(void)
{
    // Calculate acceleration/deceleration time
    AccDecTimeCalc();

    if (ForwardCmd)
    {
        if (RUN_STATUS_WAIT == RunMode) // Waiting state
        {
            RunMode = RUN_STATUS_START;
        }
    }

    UpdateFrqSetAim();
    FrqencyCurAim = FrqencyAim;      // Directly assign aim frequency before ramp control

    switch (RunMode)
    {
        case RUN_STATUS_START:  // Start state
            StartLogicMode();
            break;
        case RUN_STATUS_NORMAL:
            RunningLogicMode();
            break;
        case RUN_STATUS_STOP:  // Stop state
            CostStopMode();
            break;
        case RUN_STATUS_SHUT_DOWN:
            CostStopMode();
            break;
        default:
            break;
    }
    FrqencyCurrent = FrequencyTemp;
}

//*****************************************************************************
//
//! Handle the start-state logic.
//!
//! This function asserts the run command and selects the next run mode based
//! on the current forward command input.
//!
//! \return None.
//
//*****************************************************************************
void StartLogicMode(void)
{
    MotorMainCmd.bit.run = 1;

    if (!ForwardCmd) {
        RunMode = RUN_STATUS_SHUT_DOWN;
    } else {
        RunMode = RUN_STATUS_NORMAL;
    }
}

//*****************************************************************************
//
//! Handle the normal running-state logic.
//!
//! This function checks for a stop request and otherwise runs the active
//! acceleration or deceleration profile update.
//!
//! \return None.
//
//*****************************************************************************
void RunningLogicMode(void)
{
    if (!ForwardCmd) // Stop command
    {
        RunMode = RUN_STATUS_STOP;
        return;
    }

    // Calculate acceleration/deceleration
    AccAndDecFrqCalc(AccelerationTime, DecelerationTime, FUNCCODE_accDecSpdCurve_LINE);
}

//*****************************************************************************
//
//! Handle the coast-stop state logic.
//!
//! This function clears the run command, resets the ramp state, and returns
//! the system to the wait mode.
//!
//! \return None.
//
//*****************************************************************************
void CostStopMode(void)
{
    MotorMainCmd.bit.run = 0;
    FrequencyTemp = 0;
    FrequencyLineRamp.Remainder = 0;
    RunMode = RUN_STATUS_WAIT;
}

//*****************************************************************************
//
//! Refresh the configured acceleration and deceleration times.
//!
//! This function copies the active parameter values into the local timing
//! state used by the frequency ramp logic.
//!
//! \return None.
//
//*****************************************************************************
void AccDecTimeCalc(void)
{
    AccelerationTime = ACC1;
    DecelerationTime = ACC2;
}

//*****************************************************************************
//
//! Update the frequency ramp output.
//!
//! \param AccTime is the configured acceleration time.
//! \param DecTime is the configured deceleration time.
//! \param AccRampmode is the active ramp profile selector.
//!
//! This function selects the appropriate ramp timing and updates the current
//! frequency command through the shared line-ramp state structure.
//!
//! \return None.
//
//*****************************************************************************
void AccAndDecFrqCalc(int32 AccTime, int32 DecTime, Uint16 AccRampmode)
{
    int32 AccAndDecRampTime;
    int32 Frquency0 = FrequencyTemp;
    static int32 MaxValue;

    if (FrqencyCurAim == Frquency0)                             // No change needed
    {

    }
    else
    {
        if (((Frquency0 >= 0) && (FrqencyCurAim > Frquency0)) ||
            ((Frquency0 <= 0) && (FrqencyCurAim < Frquency0)))  // Acceleration
        {
            AccAndDecRampTime = AccTime;
            MaxValue = ABS_INT32(FrqencyCurAim);
        }
        else                                                    // Deceleration
        {
            AccAndDecRampTime = DecTime;
        }

        AccAndDecRampTime = AccAndDecRampTime * 5; // Convert to ticker
        MaxValue = MaxFrequency;
        FrequencyLineRamp.AimValue = FrqencyCurAim;
        FrequencyLineRamp.TickerAll = AccAndDecRampTime;
        FrequencyLineRamp.MaxValue = MaxValue;
        FrequencyLineRamp.CurValue = Frquency0;
        FrequencyLineRamp.calc(&FrequencyLineRamp);
        Frquency0 = FrequencyLineRamp.CurValue;
    }

    FrequencyTemp = Frquency0;
}

//*****************************************************************************
//
//! Calculate one step of the linear ramp.
//!
//! \param Pointer points to the line-ramp state structure to update.
//!
//! This function advances the current value toward the target value using the
//! configured ticker count, maximum magnitude, and accumulated remainder.
//!
//! \return None.
//
//*****************************************************************************
void LineRampCalc(LINE_RAMP_STR *Pointer)
{
    int32 DeltaValue;

    if (!Pointer->TickerAll) {
        Pointer->CurValue = Pointer->AimValue;
    } else {
        DeltaValue = ((int32)Pointer->MaxValue + Pointer->Remainder) / Pointer->TickerAll;
        Pointer->Remainder = ((int32)Pointer->MaxValue + Pointer->Remainder) % Pointer->TickerAll;

        if (Pointer->AimValue > Pointer->CurValue)
        {
            Pointer->CurValue += DeltaValue;
            if (Pointer->CurValue > Pointer->AimValue) {
                Pointer->CurValue = Pointer->AimValue;
            }
        } else {
            Pointer->CurValue -= DeltaValue;
            if (Pointer->CurValue < Pointer->AimValue) {
                Pointer->CurValue = Pointer->AimValue;
            }
        }
    }
}

//*****************************************************************************
//
//! Update the active reference source.
//!
//! This function refreshes the maximum frequency limit and copies the latest
//! external reference setting into the temporary aim variable.
//!
//! \return None.
//
//*****************************************************************************
void ReferenceSet(void)
{
    MaxFrequency = MAX_LIMIT_FREQ;
    FrqencyAimTemp = RefSet;
}

//*****************************************************************************
//
//! Commit the current frequency aim value.
//!
//! This function copies the temporary reference target into the active
//! frequency aim state used by the run logic.
//!
//! \return None.
//
//*****************************************************************************
void UpdateFrqSetAim(void)
{
    // Set value to FrqencyAim
    FrqencyAim = FrqencyAimTemp;
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
