#include "MotorInclude.h"
#include "Interface.h"
#include "Parameter.h"
#include "FuncUser.h"
#include "PreDriver.h"

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

/******************************************************************************
Function Name : InitForMotorApp
Function description : Initialize motor application variables before main loop
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : ParGetFromFunction
Function description : Get parameters from function table
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void ParGetFromFunction(void)
{
    int m_Index;

    // Note: These parameters are stored in global variables
    for(m_Index = 0;m_Index < 10;m_Index++)
    {
        *gSendToMotorTable[m_Index] = FunToMotorData[m_Index];
    }
}

/******************************************************************************
Function Name : SystemLeve2msMotor
Function description : 2ms level motor control system tasks
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void SystemLeve2msMotor(void)
{
	ParGetFromFunction();
	ParameterChange();

	SwitchFrqUpdate();
	GetStepAngleINSF();
	RunCaseDeal();
	GetDCLinkVol();			// DC link voltage measurement (0.9% execution time)
}

/******************************************************************************
Function Name : ParameterChange
Function description : Parameter conversion and coefficient calculation
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void ParameterChange(void)		
{
	// UDC.Coff = 9500, 3.3V ~ 950V with 0.1 decimal point
	// 2802x directly converts UDC.Coff to a fixed value
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

/******************************************************************************
Function Name : SwitchFrqUpdate
Function description : Switch frequency update calculation
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : RunCaseDeal
Function description : Motor running state machine processing
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : StateModeUnderVol
Function description : Under voltage state handling
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : RunCaseRun
Function description : Motor running state processing
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : RunCaseStop
Function description : Motor stop state processing
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : PrepareParForRun
Function description : Prepare parameters for motor running state
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : MotorControlMainISR
Function description : Main motor control interrupt service routine
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : HardWareErrorDeal
Function description : Hardware error handling routine
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void HardWareErrorDeal()
{
	DisableDrive();

	EALLOW;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EPwm4Regs.TZCLR.bit.OST = 1;
	EDIS;
}

unsigned int FunToMotorData[10];

// Global variable declarations
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

/******************************************************************************
Function Name : NOShifPWM
Function description : Non-shifted PWM output calculation
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : UpdatPWMRegs
Function description : Update PWM module registers to implement PWM output
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : GetStepAngleINSF
Function description : Calculate step angle for SVPWM in RAM function section
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
#pragma CODE_SECTION(GetStepAngleINSF, "ramfuncs");
void GetStepAngleINSF(void)
{
    gSynPWM.FcApply = ((Ulong)gBasePar.FcSetApply) << 9;
    SVPWMPWMAngleCal(gSynPWM.FcApply);

    gPhase.StepPhaseApply = gPhase.StepPhase;
    gMainCmd.FreqSetApply = gMainCmd.FreqSet;
    return;
}

/******************************************************************************
Function Name : SVPWMPWMAngleCal
Function description : SVPWM PWM angle calculation in RAM function section
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : FcApply - Applied frequency value
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : CalOutputPhase
Function description : Calculate output phase angle for motor control
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void CalOutputPhase(void)
{
    gPhase.IMPhase += gPhase.StepPhaseApply;
    gPhase.OutPhase = (int)(gPhase.IMPhase>>16) + 8192;
}

/******************************************************************************
Function Name : SVPWM
Function description : Space Vector PWM calculation in RAM function section
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : DeadTimeCompensation
Function description : Dead time compensation for PWM signals
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : GetRatioFromVot
Function description : Get voltage ratio from output voltage in RAM function section
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
#pragma CODE_SECTION(GetRatioFromVot, "ramfuncs");
void GetRatioFromVot(void)
{
    Uint RatioTemp;
    RatioTemp = OutVolt.VoltApply;
    gRatio = (RatioTemp > 8192) ? 8192 : RatioTemp;
}

/******************************************************************************
Function Name : GetVoltageFromFrqencyInVFMode
Function description : Calculate output voltage based on frequency in V/F mode
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : GetDCLinkVol
Function description : Get DC link voltage measurement in RAM function section
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
#pragma CODE_SECTION(GetDCLinkVol, "ramfuncs");
void GetDCLinkVol(void)
{
    Uint m_uDC;
    long ADCTemp = ADC_UDC;
    m_uDC = ((Uint32)(ADCTemp) * UDC.coefficient)>>16;
    UDC.uDC = (UDC.uDC + m_uDC) >> 1;
}

/******************************************************************************
Function Name : SVPWM_1ShuntGetPhaseCurrent
Function description : Get phase current measurement for single shunt SVPWM in RAM function section
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
#pragma CODE_SECTION(SVPWM_1ShuntGetPhaseCurrent, "ramfuncs");
void SVPWM_1ShuntGetPhaseCurrent(void)
{
    // Single shunt current measurement implementation
    // Reserved for future implementation
}
