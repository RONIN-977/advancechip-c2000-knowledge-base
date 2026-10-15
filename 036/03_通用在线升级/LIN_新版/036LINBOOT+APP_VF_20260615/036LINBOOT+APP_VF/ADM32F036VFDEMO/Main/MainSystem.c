/****************************************************************
 * System Framework
 * Version: V1.0
 *        
 * Author: Advancechip
 ****************************************************************/

#include "MainInclude.h"
#include "MotorInclude.h"
#include "Parameter.h"

#pragma DATA_SECTION(Bootflag, "BOOTFLAG");//Places the variable 'Bootflag' into a dedicated memory section named "BOOTFLAG".
const unsigned int Bootflag = 0xAA;

#pragma DATA_SECTION(productInfo, "productInfo")
/*软件版本读取格式(最长48个字节) ：       “ 产品名称，库版本，作者，软件版本，附加信息（没有写0）\0 ”*/
const unsigned char productInfo[48] = "AdvancechipMCS,SVC_v1.03,HULAODA,V0.1,SUAVS921\0";
/******************************************************************************
Function Name : main
Function description : Main application entry point and system control loop
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
void main(void)
{
	Ulong m_BaseTime,m_DetaTime;
	Uint  Level500usCount;

   	InitSysCtrl();						// Step 1. Initialize System Control
   	InitInterrupt();					// Step 2. Initialize Interrupt service program
   	InitPeripherals(); 					// Step 3. Initialize all the Device Peripherals
   	InitForMotorApp();					// Step 4. User specific code for motor application
   	InitForFunctionApp();				// Step 4. User specific code for function application
	EnableDog();						// Enable watchdog timer
	SetInterruptEnable();				// Step 5. Enable interrupts
	EINT;								// Enable global interrupts
   	ERTM;								// Enable real-time interrupt mode

   	Level500usCount = 0;
	m_BaseTime = GetTime();				// Initialize base time for timing control

  	while(1)							// Step 6. Main application loop
   	{
		m_DetaTime = m_BaseTime - GetTime();
		if(m_DetaTime >= C_TIME_05MS)	// Check if 0.5ms time interval has elapsed
        {

		    Level500usCount ++;			// Increment 500us counter
			m_BaseTime -= C_TIME_05MS;	// Update base time

 			KickDog();					// Reset watchdog timer

			if((Level500usCount & 0x03) == 0)	// Every 2ms with offset
			{
				SystemLeve2msMotor();	// Execute 2ms level motor control tasks
			}
			if((Level500usCount & 0x03) == 2)	// Every 2ms with offset
			{
				SystemLeve2msFunction();	// Execute 2ms level function tasks
			}
        }
		SystemLeve0msFunction();         // Execute 0ms level function tasks
   	}
}

/******************************************************************************
Function Name : EndOfADCISR
Function description : ADC conversion complete interrupt service routine
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
interrupt void EndOfADCISR(void)
{
    EALLOW;								// Enable protected register write access
    ADC_CLEAR_INT_FLAG;					// Clear ADC interrupt flag
    EDIS;								// Disable protected register write access

    EALLOW;
    ADC_RESET_SEQUENCE;					// Reset ADC conversion sequence
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;	// Acknowledge PIE group 1 interrupt
    EDIS;
} 

/******************************************************************************
Function Name : ZeroOfEPWMISR
Function description : EPWM zero crossing interrupt service routine for motor control
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
interrupt void ZeroOfEPWMISR(void)
{
   	EPwm2Regs.ETCLR.bit.INT = 1;		// Clear EPWM interrupt flag
	EINT;								// Enable global interrupts
	MotorControlMainISR();				// Execute main motor control algorithm
	DINT;								// Disable global interrupts
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge PIE group 3 interrupt
}

/******************************************************************************
Function Name : OneShotTZOfEPWMISR
Function description : EPWM trip zone one-shot interrupt service routine for hardware protection
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
interrupt void OneShotTZOfEPWMISR(void)
{
	HardWareErrorDeal();				// Handle hardware error condition
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	// Acknowledge PIE group 2 interrupt
}
