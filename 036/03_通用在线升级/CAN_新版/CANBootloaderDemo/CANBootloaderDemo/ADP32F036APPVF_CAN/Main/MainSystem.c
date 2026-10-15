//#############################################################################
//
// FILE:   MainSystem.c
//
// TITLE:  Application main loop and interrupt service hooks.
//
// Description:
// This file provides the application entry point, periodic control scheduler,
// and the ADC, ePWM, and protection interrupt service routines.
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
#include "MainInclude.h"
#include "MotorInclude.h"
#include "Parameter.h"
#include "CANcomm.h"

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Run the main application control loop.
//!
//! This function initializes the system control, interrupt framework,
//! peripherals, motor application, and function application before entering
//! the 0.5 ms scheduler loop.
//!
//! \return None.
//
//*****************************************************************************
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
				CanTransmit2ms();
			}
        }

		CanRemoteAckDataFill();

   	}
}

//*****************************************************************************
//
//! Handle the ADC end-of-conversion interrupt.
//!
//! This interrupt service routine clears the ADC interrupt source, resets the
//! ADC sequence, and acknowledges PIE group 1.
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Handle the ePWM zero event interrupt.
//!
//! This interrupt service routine clears the ePWM interrupt flag, briefly
//! enables nested interrupts, executes the motor control ISR, and then
//! acknowledges PIE group 3.
//!
//! \return None.
//
//*****************************************************************************
interrupt void ZeroOfEPWMISR(void)
{
   	EPwm2Regs.ETCLR.bit.INT = 1;		// Clear EPWM interrupt flag
	EINT;								// Enable global interrupts
	MotorControlMainISR();				// Execute main motor control algorithm
	DINT;								// Disable global interrupts
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;	// Acknowledge PIE group 3 interrupt
}

//*****************************************************************************
//
//! Handle the one-shot ePWM trip-zone interrupt.
//!
//! This interrupt service routine processes the hardware protection event and
//! acknowledges PIE group 2.
//!
//! \return None.
//
//*****************************************************************************
interrupt void OneShotTZOfEPWMISR(void)
{
	HardWareErrorDeal();				// Handle hardware error condition
   	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;	// Acknowledge PIE group 2 interrupt
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
