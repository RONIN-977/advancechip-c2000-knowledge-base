//#############################################################################
//
// FILE:   MotorControl.c
//
// TITLE:  Motor control core routines.
//
// Description:
// This file implements the motor-side state machine, PWM update path, V/F
// calculations, fault handling, and the RAM-resident helper routines used by
// the control loop.
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
#include "MotorInclude.h"
#include "Interface.h"
#include "Parameter.h"
#include "FuncUser.h"
#include "PreDriver.h"

/*============================================================================*/
/*                           GLOBAL DATA DEFINITIONS                          */
/*============================================================================*/
unsigned int FunToMotorData[10];

MOTOR_STR           gMotorInfo;
RUN_STATUS_STR      gMainStatus;
BASE_COMMAND_STR    gMainCmd;
BASE_PAR_STR        gBasePar;
UDC_STR             UDC;
ANGLE_STR           gPhase;
PWM_CMP_DEF         gPWM_CMP;
OUT_VOLT_STR        OutVolt;
Uint                gRatio;
PWM_OUT_STR         gPWM;
SYN_PWM_STR         gSynPWM;

Uint RsdData;
Uint * const gSendToMotorTable[10] =
{
    (Uint *)&gMainCmd.Command.all,
    (Uint *)&gMainCmd.FreqSet,          // Frequency setting
    &RsdData,
    &RsdData,
    &RsdData,
    &RsdData,
    &RsdData,
    &RsdData,
    &RsdData,
};

/*============================================================================*/
/*                           FUNCTION DECLARATIONS                            */
/*============================================================================*/
void ParameterChange(void);	
void SwitchFrqUpdate(void);
void RunCaseDeal(void);
void StateModeUnderVol(void);
void RunCaseRun(void);
void RunCaseStop(void);
void PrepareParForRun(void);
void GetStepAngleINSF(void);
void GetRatioFromVot(void);
void CalOutputPhase(void);

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Initialize the motor-layer startup state.
//!
//! This function disables the drive output and initializes the run state,
//! switching frequency, and PWM period values used before the control loop
//! begins running.
//!
//! \return None.
//
//*****************************************************************************
void  InitForMotorApp(void)
{
    DisableDrive();
	gMainStatus.RunStep = STATUS_UNDER_VOL;	// Under voltage state
	gMainStatus.SubStep = 1;				// Sub step initialization
	gBasePar.FcSet = PWM_FREQUENCY;
	gBasePar.FcSetApply = PWM_FREQUENCY;
    gPWM.gPWMPrd = C_INIT_PRD;
    gPWM.gPWMPrdActual = C_INIT_PRD;
}

//*****************************************************************************
//
//! Copy the shared function-layer command table.
//!
//! This function transfers the latest command words from the function-layer
//! buffer into the motor-side target table used by the control logic.
//!
//! \return None.
//
//*****************************************************************************
void ParGetFromFunction(void)
{
    int m_Index;

    // Note: These parameters are stored in global variables
    for(m_Index = 0;m_Index < 10;m_Index++)
    {
        *gSendToMotorTable[m_Index] = FunToMotorData[m_Index];
    }
}

//*****************************************************************************
//
//! Run the 2 ms motor-layer task.
//!
//! This function refreshes shared parameters, updates the switching frequency,
//! advances the motor run-state machine, and refreshes the DC-link estimate.
//!
//! \return None.
//
//*****************************************************************************
void SystemLeve2msMotor(void)
{
	ParGetFromFunction();
	ParameterChange();

	SwitchFrqUpdate();
	GetStepAngleINSF();
	RunCaseDeal();
	GetDCLinkVol();			// DC link voltage measurement (0.9% execution time)
}

//*****************************************************************************
//
//! Update converted control parameters.
//!
//! This function refreshes the DC-bus scaling coefficient, rated current,
//! dead-time registers, and the derived frequency scaling constants used by the
//! motor control loop.
//!
//! \return None.
//
//*****************************************************************************
void ParameterChange(void)		
{
	// UDC.Coff = 9500, 3.3V ~ 950V with 0.1 decimal point
    // 443.3V, then 4433/
	UDC.coefficient = MAX_DC_VOL_3_3V;

	gMotorInfo.Current = MOTOR_RATED_CUR;

	// Dead time setting, fixed at 2us in practice
	EALLOW;									// Enable protected register access
	EPwm2Regs.DBFED = DeadTimeInternal;
	EPwm2Regs.DBRED = DeadTimeInternal;
	EPwm3Regs.DBFED = DeadTimeInternal;
	EPwm3Regs.DBRED = DeadTimeInternal;
	EPwm4Regs.DBFED = DeadTimeInternal;
	EPwm4Regs.DBRED = DeadTimeInternal;
	EDIS;
	
	// 24.75A current limit calculation
    // MAX_PEAK_CUR_3_3V = 414,  4.14A Peak. Rated is 2.0A
    // (4.14 / 2.0) * 2^12 = 8448,
    gBasePar.FullFreq = ((long)MAX_LIMIT_FREQ * 10) + 2000;
	gMotorInfo.FreqRatedPerUnit = ((Ulong)MOTOR_RATED_FRE <<15) / gBasePar.FullFreq;
}

//*****************************************************************************
//
//! Update the applied switching frequency.
//!
//! This function clamps the requested switching frequency and converts it into
//! the active PWM period value used by the EPWM modules.
//!
//! \return None.
//
//*****************************************************************************
void SwitchFrqUpdate(void)
{
    unsigned long data;
    unsigned long data1;

    data1 = __IQsat(gBasePar.FcSet,200,20);
    gBasePar.FcSetApply = data1;

    data = DSP_CLOCK;
    data *= 5000;
    DINT;
    gPWM.gPWMPrdActual  = (Uint)(data/data1);
    EINT;
}

//*****************************************************************************
//
//! Process the motor run-state machine.
//!
//! This function dispatches the control flow to the under-voltage, run, or
//! stop-state handlers according to the current run-step state.
//!
//! \return None.
//
//*****************************************************************************
void RunCaseDeal(void)
{
	switch(gMainStatus.RunStep)
	{
		case STATUS_UNDER_VOL:	// Under voltage/standby state
			StateModeUnderVol();
			break;

		case STATUS_RUN:		// Running state (VF/FVC/SVC modes)
			RunCaseRun();
			break;

		default:				// Stop state or transition to other states
			RunCaseStop();
			break;
	}	
}

//*****************************************************************************
//
//! Handle the under-voltage state.
//!
//! This function forces the controller back to the stop state and clears the
//! EPWM trip flags required before normal operation can resume.
//!
//! \return None.
//
//*****************************************************************************
void StateModeUnderVol(void)
{
    gMainStatus.RunStep = STATUS_STOP;
    gMainStatus.SubStep = 1;

    EALLOW;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm3Regs.TZCLR.bit.OST = 1;
    EPwm4Regs.TZCLR.bit.OST = 1;

    EPwm2Regs.TZCLR.bit.CBC = 1;
    EPwm3Regs.TZCLR.bit.CBC = 1;
    EPwm4Regs.TZCLR.bit.CBC = 1;

    EPwm2Regs.TZCLR.bit.INT = 1;
    EDIS;
}

//*****************************************************************************
//
//! Handle the active run state.
//!
//! This function checks for stop or fault conditions and otherwise updates the
//! applied V/F voltage command used by the PWM path.
//!
//! \return None.
//
//*****************************************************************************
void RunCaseRun(void)
{
    if((gMainStatus.ErrorCode != ERROR_NONE) || (gMainCmd.Command.bit.Start == FALSE))
    {
        DisableDrive();
        gMainStatus.RunStep = STATUS_STOP;
        gMainStatus.SubStep = 1;
        return;
    }
    GetVoltageFromFrqencyInVFMode();
    OutVolt.VoltApply = OutVolt.Volt;
}

//*****************************************************************************
//
//! Handle the stop state.
//!
//! This function disables the drive, prepares the restart parameters, and
//! re-enters the run state when the command and fault conditions allow it.
//!
//! \return None.
//
//*****************************************************************************
void RunCaseStop(void)
{
    DisableDrive();
	PrepareParForRun();
	gMainStatus.SubStep = 1;

	if ((gMainStatus.ErrorCode == ERROR_NONE) && (gMainCmd.Command.bit.Start == 1))
	{
        gMainStatus.RunStep = STATUS_RUN;
        gMainStatus.SubStep = 1;
        EnableDrive();
	}
}

//*****************************************************************************
//
//! Prepare the PWM state for the next run.
//!
//! This function clears the applied command state and restores the EPWM period
//! and compare registers to a centered startup condition.
//!
//! \return None.
//
//*****************************************************************************
void PrepareParForRun(void)
{
	gMainCmd.FreqSetApply = 0;
	OutVolt.Volt = 0;
	OutVolt.VoltApply = 0;
	gPhase.OutPhase = (int)(gPhase.IMPhase>>16) + 8192;
	gRatio = 0;

	EALLOW;
	EPwm2Regs.TBPRD = gPWM.gPWMPrdActual;
	EPwm3Regs.TBPRD = gPWM.gPWMPrdActual;
	EPwm4Regs.TBPRD = gPWM.gPWMPrdActual;
    EPwm5Regs.TBPRD = gPWM.gPWMPrdActual;

	EPwm2Regs.CMPA.half.CMPA = gPWM.gPWMPrdActual/2;
	EPwm3Regs.CMPA.half.CMPA = gPWM.gPWMPrdActual/2;
	EPwm4Regs.CMPA.half.CMPA = gPWM.gPWMPrdActual/2;
    EPwm5Regs.CMPA.half.CMPA = gPWM.gPWMPrdActual/2;
    EPwm5Regs.CMPB = gPWM.gPWMPrdActual / 2;

    gPWM_CMP.U_CMP_U = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.U_CMP_V = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.U_CMP_W = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.D_CMP_U = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.D_CMP_V = EPwm2Regs.CMPA.half.CMPA;
    gPWM_CMP.D_CMP_W = EPwm2Regs.CMPA.half.CMPA;

    gPWM_CMP.T1CMP = 100;
    gPWM_CMP.T2CMP = 500;
    EPwm5Regs.CMPA.half.CMPA = gPWM_CMP.T1CMP;
    EPwm5Regs.CMPB = gPWM_CMP.T2CMP;
	EDIS;
}

//*****************************************************************************
//
//! Execute the main motor control interrupt routine.
//!
//! This function performs current sampling, voltage feedback, phase update,
//! SVPWM calculation, and hardware register updates for each control cycle.
//!
//! \return None.
//
//*****************************************************************************
void MotorControlMainISR()
{
    SVPWM_1ShuntGetPhaseCurrent();
    GetDCLinkVol();
    GetRatioFromVot();
    CalOutputPhase();
    SVPWM();
    NOShifPWM();
    UpdatPWMRegs();
}

//*****************************************************************************
//
//! Handle a hardware fault event.
//!
//! This function disables the gate drive and clears the one-shot EPWM trip
//! flags after a hardware protection event is detected.
//!
//! \return None.
//
//*****************************************************************************
void HardWareErrorDeal()
{
	DisableDrive();

	EALLOW;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EPwm4Regs.TZCLR.bit.OST = 1;
	EDIS;
}

//*****************************************************************************
//
//! Generate the non-shifted PWM compare outputs.
//!
//! This function copies the calculated phase compare values into the buffered
//! up-count and down-count PWM compare fields.
//!
//! \return None.
//
//*****************************************************************************
void SVPWMPWMAngleCal(Ulong);
void NOShifPWM(void)
{
    gPWM_CMP.U_CMP_U = gPWM.U;
    gPWM_CMP.D_CMP_U = gPWM.U;

    gPWM_CMP.U_CMP_V = gPWM.V;
    gPWM_CMP.D_CMP_V = gPWM.V;

    gPWM_CMP.U_CMP_W = gPWM.W;
    gPWM_CMP.D_CMP_W = gPWM.W;
}

//*****************************************************************************
//
//! Update the EPWM hardware registers.
//!
//! This function writes the latest PWM period and compare values into the EPWM
//! register set used to drive the motor phases.
//!
//! \return None.
//
//*****************************************************************************
void UpdatPWMRegs()
{
    // Update registers: TBPRD, CMPA, CMPB need EALLOW protection
    EPwm2Regs.TBPRD = gPWM.gPWMPrdActual;
    EPwm3Regs.TBPRD = gPWM.gPWMPrdActual;
    EPwm4Regs.TBPRD = gPWM.gPWMPrdActual;
    EPwm5Regs.TBPRD = gPWM.gPWMPrdActual;

    EPwm2Regs.CMPA.half.CMPA = gPWM_CMP.U_CMP_U;
    EPwm3Regs.CMPA.half.CMPA = gPWM_CMP.U_CMP_V;
    EPwm4Regs.CMPA.half.CMPA = gPWM_CMP.U_CMP_W;
    EPwm2Regs.CMPB           = gPWM_CMP.D_CMP_U;
    EPwm3Regs.CMPB           = gPWM_CMP.D_CMP_V;
    EPwm4Regs.CMPB           = gPWM_CMP.D_CMP_W;

    EPwm5Regs.CMPA.half.CMPA = gPWM_CMP.T1CMP;
    EPwm5Regs.CMPB           = gPWM_CMP.T2CMP;
}

//*****************************************************************************
//
//! Update the instantaneous phase-step command.
//!
//! This RAM-resident function derives the applied switching-frequency term,
//! updates the PWM angle helper, and latches the current phase-step state used
//! by the control loop.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(GetStepAngleINSF, "ramfuncs");
void GetStepAngleINSF(void)
{
    gSynPWM.FcApply = ((Ulong)gBasePar.FcSetApply) << 9;
    SVPWMPWMAngleCal(gSynPWM.FcApply);

    gPhase.StepPhaseApply = gPhase.StepPhase;
    gMainCmd.FreqSetApply = gMainCmd.FreqSet;
    return;
}

//*****************************************************************************
//
//! Calculate the PWM phase-step angle.
//!
//! \param FcApply is the applied switching-frequency term.
//!
//! This RAM-resident function converts the applied switching-frequency term and
//! requested output frequency into the electrical phase step and active PWM
//! period.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(SVPWMPWMAngleCal, "ramfuncs");
void SVPWMPWMAngleCal(Ulong FcApply)
{
    Uint  m_HalfTc,m_AbsFreq,m_Fc;
    Ulong m_LData = (500000L * 512);
    if(FcApply > 61440)
    {
        m_Fc = FcApply>>1;
        m_LData = m_LData>>1;
    } else {
        m_Fc = FcApply;
    }

    // Calculate step angle based on switching frequency
    m_HalfTc = (Uint)(m_LData/m_Fc);
    m_AbsFreq  = abs(gMainCmd.FreqSetApply);
    m_LData = ((Ullong)((Ulong)m_HalfTc * (Ulong)m_AbsFreq) * (Ullong)((Ullong)gBasePar.FullFreq * 112590))>>32;

    DINT;
    if (gMainCmd.FreqSetApply >= 0) {
        gPhase.StepPhase = m_LData;
    } else {
        gPhase.StepPhase = -m_LData;
    }
    gPWM.gPWMPrd = m_HalfTc;
    EINT;
}

//*****************************************************************************
//
//! Update the output electrical phase.
//!
//! This function accumulates the instantaneous phase step and converts it into
//! the phase value consumed by the SVPWM routine.
//!
//! \return None.
//
//*****************************************************************************
void CalOutputPhase(void)
{
    gPhase.IMPhase += gPhase.StepPhaseApply;
    gPhase.OutPhase = (int)(gPhase.IMPhase>>16) + 8192;
}

//*****************************************************************************
//
//! Calculate the space-vector PWM timing.
//!
//! This RAM-resident function resolves the present electrical sector, computes
//! the active and zero vector durations, writes the phase timing outputs, and
//! then applies dead-time compensation.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(SVPWM, "ramfuncs");
void SVPWM(void)
{
    Uint Sector30Degree;
    Uint Sector60Degree;

    Uint    RemainPhaseSec;
    Ulong*  PwmPointUVW;

    Uint    Sin30DegreeAlfaPart,Sin60DegreeAlfaPart;
    Uint    VTaLength,VTbLength,VTzeroLength,m_TotalLength;
    Uint    PWMVectorTemp1,PWMVectorTemp2;
    Uint    phase;

    phase = gPhase.OutPhase;
    phase = (phase > 65531)?65531:phase;

    Sector30Degree = phase / 5461;
    if (Sector30Degree > 11) {
        Sector30Degree = 0;
    }
    Sector60Degree = Sector30Degree >> 1;

    RemainPhaseSec = phase - (Sector30Degree * 5461);
    if ((Sector30Degree & 0x01)  == 1) //judge it's odd or even
    {
        RemainPhaseSec = 5461 + RemainPhaseSec;
    }

    if((Sector60Degree & 0x01) == 1)  RemainPhaseSec = 10922 - RemainPhaseSec;

    Sin60DegreeAlfaPart = qsin(10922 - RemainPhaseSec);
    Sin30DegreeAlfaPart = qsin(RemainPhaseSec);

    VTaLength = ((long)Sin60DegreeAlfaPart * (long)gRatio)>>15;
    VTaLength = ((long)VTaLength * (long)gPWM.gPWMPrdActual)>>12;

    VTbLength = ((long)Sin30DegreeAlfaPart * (long)gRatio)>>15;
    VTbLength = ((long)VTbLength * (long)gPWM.gPWMPrdActual)>>12;

    m_TotalLength = VTaLength + VTbLength;
    if (m_TotalLength > gPWM.gPWMPrdActual)
    {
        VTaLength = ((Ulong)VTaLength * (Ulong)gPWM.gPWMPrdActual)/m_TotalLength;
        VTbLength = gPWM.gPWMPrdActual - VTaLength;
        VTzeroLength = 0;
    }

    VTzeroLength = (Uint)((gPWM.gPWMPrdActual - VTaLength - VTbLength)>>1);
    PWMVectorTemp1 = PWMArrayData0[Sector30Degree];
    PWMVectorTemp2 = PWMArrayData2[Sector30Degree];

    VTaLength = VTzeroLength + VTaLength;
    VTbLength = VTaLength + VTbLength;

    // Initialize UVW phase compare values
    PwmPointUVW = (Ulong *)&gPWM.U;
    *(PwmPointUVW + PWMVectorTemp1) = VTzeroLength;
    *(PwmPointUVW + PWMArrayData1[Sector30Degree]) = VTaLength;
    *(PwmPointUVW + PWMVectorTemp2) = VTbLength;

    DeadTimeCompensation();
}

//*****************************************************************************
//
//! Clamp the PWM outputs after dead-time compensation.
//!
//! This RAM-resident function limits the phase compare values so they remain
//! within the valid PWM period range.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(DeadTimeCompensation, "ramfuncs");
void DeadTimeCompensation(void)
{
    int PeriodMaxTemp;
    PeriodMaxTemp = gPWM.gPWMPrdActual - 1;
    gPWM.U = __IQsat(gPWM.U,PeriodMaxTemp,1);
    gPWM.V = __IQsat(gPWM.V,PeriodMaxTemp,1);
    gPWM.W = __IQsat(gPWM.W,PeriodMaxTemp,1);
    return;
}

//*****************************************************************************
//
//! Update the normalized voltage ratio.
//!
//! This RAM-resident function clamps the applied output-voltage request to the
//! normalized modulation range used by the SVPWM path.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(GetRatioFromVot, "ramfuncs");
void GetRatioFromVot(void)
{
    Uint RatioTemp;
    RatioTemp = OutVolt.VoltApply;
    gRatio = (RatioTemp > 8192) ? 8192 : RatioTemp;
}

//*****************************************************************************
//
//! Calculate the V/F output voltage command.
//!
//! This function converts the applied frequency command into the corresponding
//! normalized output-voltage request for V/F control.
//!
//! \return None.
//
//*****************************************************************************
void GetVoltageFromFrqencyInVFMode(void)
{
    int AbsFrequencyInput;
    int VolInVFMode;
    AbsFrequencyInput = abs(gMainCmd.FreqSetApply);

    if (AbsFrequencyInput >= gMotorInfo.FreqRatedPerUnit) {
        VolInVFMode = 4096;
    } else if (AbsFrequencyInput == 0) {
        VolInVFMode = 4096;
    } else {
        VolInVFMode = ((Ulong)AbsFrequencyInput * 4096L)/gMotorInfo.FreqRatedPerUnit;
    }
    OutVolt.Volt = VolInVFMode;
}

//*****************************************************************************
//
//! Update the DC-link voltage estimate.
//!
//! This RAM-resident function scales the ADC DC-link sample and filters it into
//! the runtime DC-bus estimate used by the control logic.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(GetDCLinkVol, "ramfuncs");
void GetDCLinkVol(void)
{
    Uint m_uDC;
    long ADCTemp = ADC_UDC;
    m_uDC = ((Uint32)(ADCTemp) * UDC.coefficient)>>16;
    UDC.uDC = (UDC.uDC + m_uDC) >> 1;
}

//*****************************************************************************
//
//! Capture the single-shunt phase-current sample.
//!
//! This RAM-resident placeholder provides the entry point for the single-shunt
//! current reconstruction step used by the motor control interrupt flow.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(SVPWM_1ShuntGetPhaseCurrent, "ramfuncs");
void SVPWM_1ShuntGetPhaseCurrent(void)
{
    // Single shunt current measurement implementation
    // Reserved for future implementation
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
