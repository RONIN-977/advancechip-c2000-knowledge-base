//#############################################################################
//
// FILE:   MotorInclude.h
//
// TITLE:  Motor control shared declarations.
//
// Description:
// This file collects the motor-control macros, runtime data structures, shared
// global variables, and public helper declarations used by the application.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef MOTOR_INCLUDE_H
#define MOTOR_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include "MainInclude.h"
#include "Parameter.h"
#include "SubPrgInclude.h"

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/

#define ERROR_NONE                0      // 0   -- none
#define ERROR_OC_HARDWARE         12     // 12  -- hardware overcurrent
#define ERROR_OV                  13     // 13  -- overvoltage
#define ERROR_UV                  14     // 14  -- undervoltage
#define ERROR_LOSE_PHASE_OUTPUT   15     // 15  -- motor phase
#define ERROR_CURRENT_SAMPLE      17     // 17  -- current detection failure
#define ERROR_STALL               20     // 20  -- Motor stalling fault
#define ERROR_NTC                 21     // 21  -- NTC fault
#define ERROR_NO_LOAD             22     // 22  -- Empty-load fault
#define ERROR_MOTOR_SHORT_TO_GND  23     // 23  -- Ground short circuit fault of motor
#define ERROR_IPD_NO_MOTOR        32     // 21  -- IPD
#define ERROR_OC_SOFTWARE         33     // 21  -- Software Overcurrent
#define ERROR_CBC                 41     // 41  -- Rapid rate limiting, timeout and failure
#define ERROR_ERROR_KE            44     // 44  -- Over-shoot fault
#define ERROR_LIN                 55     // 55  -- Lin communication failure
#define ERROR_DRIVER              66     // 66  -- Pre-drive failure

#define T_PRD  (Uint16)(((unsigned long)DSP_CLOCK60 * 5000) / PWM_FREQUENCY)


#define  ADC_TRIG_SOURCE       13   //set SOC0 start trigger on EPWM1A, due to round-robin SOC0 converts first then SOC1, then SOC2
#define  ACQPS_SYS_CLKS        8    //set SOC0 S/H Window to 9 ADC Clock Cycles, (8 ACQPS plus 1)

#define  ADC_PIN_S0            0    //A0
#define  ADC_PIN_S1            1    //A1
#define  ADC_PIN_S2            2    //A2
#define  ADC_PIN_S3            3    //A3
#define  ADC_PIN_S4            4    //A4
#define  ADC_PIN_S5            5    //A5
#define  ADC_PIN_S6            6    //A6
#define  ADC_PIN_S7            7    //A7
#define  ADC_PIN_S8            8    //B0
#define  ADC_PIN_S9            9    //B1
#define  ADC_PIN_S10           10   //B2
#define  ADC_PIN_S11           11   //B3
#define  ADC_PIN_S12           12   //B4
#define  ADC_PIN_S13           13   //B5
#define  ADC_PIN_S14           14   //B6
#define  ADC_PIN_S15           15   //B7

#define ADCTRIG_SOFT        0
#define ADCTRIG_CPU_TINT0   1
#define ADCTRIG_CPU_TINT1   2
#define ADCTRIG_CPU_TINT2   3
#define ADCTRIG_XINT2       4
#define ADCTRIG_EPWM1_SOCA  5
#define ADCTRIG_EPWM1_SOCB  6
#define ADCTRIG_EPWM2_SOCA  7
#define ADCTRIG_EPWM2_SOCB  8
#define ADCTRIG_EPWM3_SOCA  9
#define ADCTRIG_EPWM3_SOCB  10
#define ADCTRIG_EPWM4_SOCA  11
#define ADCTRIG_EPWM4_SOCB  12
#define ADCTRIG_EPWM5_SOCA  13
#define ADCTRIG_EPWM5_SOCB  14
#define ADCTRIG_EPWM6_SOCA  15
#define ADCTRIG_EPWM6_SOCB  16
#define ADCTRIG_EPWM7_SOCA  17
#define ADCTRIG_EPWM7_SOCB  18

// Definitions of registers related to chips
#define PIE_VECTTABLE_ADCINT    PieVectTable.ADCINT1                    //ADC interrupt vector
#define ADC_CLEAR_INT_FLAG      AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1    //Clear the interrupt flags of the ADC module
#define ADC_RESET_SEQUENCE      AdcRegs.SOCPRICTL.bit.RRPOINTER = 0x20  //reset the sequence
#define ADC_START_CONVERSION   	AdcRegs.ADCSOCFRC1.all = 0xFFFF 		//Start the software and enable AD.
#define ADC_END_CONVERSIN       AdcRegs.ADCINTFLG.bit.ADCINT1           //Completion flag for AD conversion


#define  ADC_VOLTAGE_08         ( 8L*65535/33)  // The sampled value corresponding to an input of 0.8V on the AD module
#define  ADC_VOLTAGE_10         (10L*65535/33)  // The sampled value corresponding to an input of 1.0V on the AD module
#define  ADC_VOLTAGE_20         (20L*65535/33)  // The sampled value corresponding to an input of 2.0V on the AD module
#define  ADC_VOLTAGE_25         (25L*65535/33)  // The sampled value corresponding to an input of 2.5V on the AD module
 
/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/
#define DisConnectRelay()   	    GpioDataRegs.GPACLEAR.bit.GPIO8  = 1
#define ConnectRelay()    	        GpioDataRegs.GPASET.bit.GPIO8  = 1    //Power-up buffer relay control
#define DRIVE_ENABLE()              GpioDataRegs.GPASET.bit.GPIO31  = 1
#define DRIVE_DISABLE()             GpioDataRegs.GPACLEAR.bit.GPIO31  = 1    //DRRIVER_EN

#define EnableDrive()							\
												\
	EALLOW;										\
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;			\
	EDIS;                                       \

#define DisableDrive()							\
												\
	EALLOW;										\
	GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;			\
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;			\
	EDIS;                                       \

/************************************************************
Definitions of registers related to chips
************************************************************/
#define ADC_GND             (AdcResult.ADCRESULT3<<4)
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define	ADC_IV              (AdcResult.ADCRESULT0)              // Related to hardware
#define ADC_IV1             (AdcResult.ADCRESULT1)
#define	ADC_UDC				(AdcResult.ADCRESULT2<<4)
#else
#define ADC_IU               (AdcResult.ADCRESULT0<<4)
#define ADC_IV               (AdcResult.ADCRESULT1<<4)          // Related to hardware
#define ADC_IW               (AdcResult.ADCRESULT0<<4)
#define ADC_UDC              (AdcResult.ADCRESULT3<<4)
#endif

#define ADCRESULT_UU        (AdcResult.ADCRESULT3)
#define ADCRESULT_UV        (AdcResult.ADCRESULT4)
#define ADCRESULT_UW        (AdcResult.ADCRESULT5)

#define	ADC_VSP				(AdcResult.ADCRESULT6)
#define	ADC_TEMP			(AdcResult.ADCRESULT3)
#define ADC_DSP_TEMP        (AdcResult.ADCRESULT3)

#define ADC_IBUS             (AdcResult.ADCRESULT7)


/*****************Basic information data structure********************/


typedef struct MOTOR_STR_DEF {
	Uint 	Type;					//Motor type
	Uint 	Power;					//Motor power, unit 0.1KW
	Uint 	Votage;					//Motor voltage, unit 1V
	Uint 	CurrentGet;				//Motor current, unit 0.1A
	Uint 	Frequency;				//Motor frequency, unit 0.01Hz
	Uint 	FreqRatedPerUnit;       //Per-unit motor frequency
	Uint 	Current;				//Current base value, unit 0.1A
	Uint	CurBaseCoff;			//Current base value amplification factor
}MOTOR_STR;//Motor basic information structure



typedef struct RUN_STATUS_STR_DEF {
    Uint    RunStep;                //Main step
    Uint    SubStep;                //Sub step
    Uint    VFStatus;               //VF status, defined by bit position
    Uint    VCStatus;               //VC status, defined by bit position
    Uint    ErrorCode;              //Error flag
    Uint    LastErrorCode;          //Last fault code, used for output voltage phase switching
} RUN_STATUS_STR;//Inverter running status structure

typedef struct BASE_COMMAND_STR_DEF {
    union MAIN_COMMAND_UNION_DEF Command;   //Main command word structure
    int     FreqSet;                //Set speed
    int     FreqSetApply;           //Actual speed (synchronous speed)
    long    FreqPreWs;              //Synchronous speed before slip compensation
    long    FreqLast;               //Speed at previous step before zero speed
    long    FreqFeed;               //Feedback frequency
    long    FreqDesired;            //Target speed
    long    VCTorqueLim;            //VC torque limit
    long    FreqReal;               //Frequency in 0.01Hz units
	Uint	SpeedFalg;				//Acceleration/deceleration flag
}BASE_COMMAND_STR;//Real-time modified command structure

/************************************************************/
/**********Data structure for motor control related setting parameters***********/
typedef struct BASE_PAR_STR_DEF {
    long    FullFreq;               //Frequency represented by 32767
    Uint    MaxFreq;                //Maximum frequency
    Uint    UpFreq;                 //Upper limit frequency
    Uint    LowFreq;                //Lower limit frequency
    Uint    FcSet;                  //Set carrier frequency
    Uint    FcSetApply;             //Actual carrier frequency
}BASE_PAR_STR;	//Basic operation information structure

/************************************************************/
/********************Real-time information data structure******************/
typedef struct UDC_STR_DEF {
	int		DetaUdc;
	Uint	uDCBak;
    Uint    uDC;                //Unit 0.1V
    Uint    uDCFilter;          //Large filter DC bus voltage, unit 0.1V
    Uint    uDCBigFilter;       //DC bus voltage for overvoltage/undervoltage judgment, unit 0.1V
	Uint 	coefficient;		//DC bus voltage calculation coefficient
	Uint 	Power;
}UDC_STR;	//DC bus voltage data

typedef struct UVW_STR_DEF{
	int  	U;					//Q12 format, with motor rated value as per-unit base value
	int  	V;
	int  	W;
}UVW_STR;	//Stator three-phase axis current

typedef struct IUVW_SAMPLING_STR_DEF{
    int     U;                  //Q12 format, with motor rated current as per-unit base value
    int     V;
    int     W;
    int     UErr;               //U-phase glitch filter
    int     VErr;               //V-phase glitch filter
    long    Coff;               //Coefficient for converting sampled current to per-unit current
}IUVW_SAMPLING_STR;	//Current sampling structure

typedef struct ALPHABETA_STR_DEF{
	int  	Alph;				//Q12
	int  	Beta;
}ALPHABETA_STR;//Stator two-phase axis current, voltage structure

typedef struct MT_STR_DEF{
	int  	M;					//Q12
	int  	T;
}MT_STR;	//Current, voltage structure in MT axis system

typedef struct AMPTHETA_STR_DEF{
	Uint  	Amp;				//Q12
	int  	Theta;				//Q15
	int     ThetaFilter;        //Q15 Filtered value of current-voltage phase angle
}AMPTHETA_STR;//Current, voltage structure represented in polar coordinates

typedef struct ANGLE_STR_DEF {
    long    StepPhase;          //Step angle (calculated)
    long    StepPhaseApply;     //Step angle (actually used)
    long    IMPhase;            //M-axis angle
    int     OutPhase;           //PWM angle
    int     CompPhase;          //Phase delay compensation angle
    int     RotorPhase;         //Rotor angle
}ANGLE_STR;

/************************************************************/
/*****************Data structures used by independent modules***************/
typedef struct OUT_VOLT_STR_DEF {
	int  	Volt;				//Q12 Output voltage and phase during intermediate calculation
	int   	VoltPhase;
	int  	VoltApply;			//Q12 Output voltage and phase used for modulation coefficient calculation
	int     VoltDisplay;        //Display output voltage, base value is inverter rated voltage
	int   	VoltPhaseApply;
}OUT_VOLT_STR;

typedef struct PWM_OUT_STR_DEF {
	long	U;
	long	V;
	long	W;				//Do not change parameters before this structure
	Uint	gPWMPrd;		//Calculated carrier period (20ns unit)
	Uint	gPWMPrdActual;	//Actually used carrier period in interrupt
	int    	SoftPWMCoff;
}PWM_OUT_STR;	        //Structure for PWM output



typedef struct SYN_PWM_STR_DEF {
	Ulong   FcApply;
}SYN_PWM_STR;	//


/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern MOTOR_STR 			gMotorInfo;		//Motor information
extern RUN_STATUS_STR 		gMainStatus;	//Main running status
extern BASE_COMMAND_STR		gMainCmd;		//Main command


/************************************************************/
/**********The following are motor control related setting parameter definitions******************/
extern BASE_PAR_STR			gBasePar;	//Basic operation parameters
extern UDC_STR				UDC;		//DC bus voltage data
extern IUVW_SAMPLING_STR    gCurSamp;
extern UVW_STR				gIUVW;		//Stator three-phase current
extern ALPHABETA_STR	    gIAlphBeta;	//Stator two-phase axis current
extern MT_STR				gIMT;		//Current in MT axis system
extern AMPTHETA_STR			gIAmpTheta;	//Current represented in polar coordinates

extern ANGLE_STR		    gPhase;		//Angle structure
//extern DEAD_BAND_STR	    gDeadBand;
extern OUT_VOLT_STR			OutVolt;
extern Uint				    gRatio;		//Modulation coefficient
extern PWM_OUT_STR			gPWM;
//extern FC_CAL_STR			gFcCal;		//Data structure used by carrier frequency calculation
extern SYN_PWM_STR			gSynPWM;

extern Uint * const gSendToMotorTable[];
extern Uint PWMArrayData0[12];
extern Uint PWMArrayData1[12];
extern Uint PWMArrayData2[12];

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
extern void GetVoltageFromFrqencyInVFMode(void);
extern void SVPWM(void);
extern void NOShifPWM(void);
extern void UpdatPWMRegs(void);
extern void DeadTimeCompensation(void);
extern void InitSetPWM(void);
extern void InitSetAdc(void);
extern void ParGetFromFunction(void);		
extern void GetRatioFromVot(void);
extern void GetDCLinkVol(void);
extern void SVPWM_1ShuntGetPhaseCurrent(void);
extern void SwitchFrqUpdate();


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

