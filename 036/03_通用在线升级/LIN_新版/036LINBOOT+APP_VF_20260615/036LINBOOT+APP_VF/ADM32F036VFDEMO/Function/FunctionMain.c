#include "Interface.h"
#include "RunSrc.h"
#include "Reference.h"
#include "MotorInclude.h"

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

void StartLogicMode(void);
void RunningLogicMode(void);
void CostStopMode(void);
void UpdateDataFunc2Core(void);


/******************************************************************************
Function Name : SystemLeve2msFunction
Function description : 2ms level system function for motor control
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void SystemLeve2msFunction(void)
{
    ReferenceSet();               // Frequency reference setting
    RunLogicDeal();               // Running logic processing
    UpdateDataFunc2Core();        // Update data from function to core
    CommsProtocalLayerTask2ms();  // Process communication protocol layer tasks
}

/******************************************************************************
Function Name : InitForFunctionApp
Function description : Initialize function application variables
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void InitForFunctionApp(void)
{
	// Initialization
    UpdateDataFunc2Core();
}



/******************************************************************************
Function Name : SystemLeve0msFunction
Function description : 0ms level system function for system initialization and
                       error handling. Primarily handles bootloader entry
                       sequence and clears LIN communication error flags.
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
Uint16 error1 = 0;                     // Variable to store LIN error status
Uint16 testERnum = 0;                  // Counter for LIN error occurrences
extern Uint16 flash_IAP_flag;          // External flag for In-Application Programming (Bootloader entry)
void SystemLeve0msFunction(void)
{
    // bootloader entry check
    if (flash_IAP_flag == 1)           // Check if bootloader entry is requested
    {
        DINT;                          // Disable global interrupts
        DisableDrive();                // Disable motor drive outputs for safety

        ResetDSP();                    // Reset the DSP, preparing for bootloader jump
        // (*Jump_Boot)();             // Alternative jump instruction (commented out)
        // asm(" MOVL XAR7, #0x3F7FF6"); // Inline assembly to load bootloader entry address
        // asm(" LB *XAR7");             // Long branch to bootloader
    }

    // LIN error flag monitoring and clearing
    error1 = ((LinaRegs.SCIFLR.all & 0xFF000000)>>24); // Extract error flags from LIN status register
    if(error1 > 0)                      // If any error flag is set
    {
        // Clear all LIN error flags by writing 1 to each bit (typically a clear-on-write-1 operation)
        LinaRegs.SCIFLR.bit.BE = 1;     // Clear Break Error flag
        LinaRegs.SCIFLR.bit.PBE = 1;    // Clear Physical Bus Error flag
        LinaRegs.SCIFLR.bit.CE = 1;     // Clear Checksum Error flag
        LinaRegs.SCIFLR.bit.ISFE = 1;   // Clear Inconsistent Sync Field Error flag
        LinaRegs.SCIFLR.bit.NRE = 1;    // Clear No Response Error flag
        LinaRegs.SCIFLR.bit.FE = 1;     // Clear Framing Error flag
        LinaRegs.SCIFLR.bit.OE = 1;     // Clear Overrun Error flag
        LinaRegs.SCIFLR.bit.PE = 1;     // Clear Parity Error flag

        testERnum++;                    // Increment the error occurrence counter (for debugging/monitoring)
    }
}

/******************************************************************************
Function Name : UpdateDataFunc2Core
Function description : Update data from function module to core module
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : RunLogicDeal
Function description : Main running logic processing for motor control
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : StartLogicMode
Function description : Motor start logic state handling
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void StartLogicMode(void)
{
    MotorMainCmd.bit.run = 1;

    if (!ForwardCmd) {
        RunMode = RUN_STATUS_SHUT_DOWN;
    } else {
        RunMode = RUN_STATUS_NORMAL;
    }
}

/******************************************************************************
Function Name : RunningLogicMode
Function description : Motor running logic state handling
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : CostStopMode
Function description : Motor cost stop logic state handling
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void CostStopMode(void)
{
    MotorMainCmd.bit.run = 0;
    FrequencyTemp = 0;
    FrequencyLineRamp.Remainder = 0;
    RunMode = RUN_STATUS_WAIT;
}

/******************************************************************************
Function Name : AccDecTimeCalc
Function description : Calculate acceleration and deceleration times
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void AccDecTimeCalc(void)
{
    AccelerationTime = ACC1;
    DecelerationTime = ACC2;
}

LINE_RAMP_STR FrequencyLineRamp = LINE_CHANGE_STRTUCT_DEFALUTS;

/******************************************************************************
Function Name : AccAndDecFrqCalc
Function description : Calculate frequency acceleration and deceleration
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : AccTime - Acceleration time, DecTime - Deceleration time, AccRampmode - Ramp mode
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : LineRampCalc
Function description : Linear ramp calculation for frequency change
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : Pointer - Pointer to linear ramp structure
Returns : N/A
******************************************************************************/
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

/******************************************************************************
Function Name : ReferenceSet
Function description : Set frequency reference values
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void ReferenceSet(void)
{
    MaxFrequency = MAX_LIMIT_FREQ;
    FrqencyAimTemp = RefSet;
}

/******************************************************************************
Function Name : UpdateFrqSetAim
Function description : Update frequency aim value
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void UpdateFrqSetAim(void)
{
    // Set value to FrqencyAim
    FrqencyAim = FrqencyAimTemp;
}
