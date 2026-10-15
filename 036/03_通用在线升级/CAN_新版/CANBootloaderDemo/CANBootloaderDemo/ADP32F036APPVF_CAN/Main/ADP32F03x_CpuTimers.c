//#############################################################################
//
// FILE:   ADP32F03x_CpuTimers.c
//
// TITLE:  CPU timer initialization helpers.
//
// Description:
// This file initializes the device CPU timer objects and provides the common
// timer configuration helper used by the application.
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
#include "ADP32F03x_Device.h"     // Headerfile Include File

/*============================================================================*/
/*                          GLOBAL DATA DEFINITIONS                           */
/*============================================================================*/
struct CPUTIMER_VARS CpuTimer0;

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS.
struct CPUTIMER_VARS CpuTimer1;
//struct CPUTIMER_VARS CpuTimer2;

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Initialize the CPU timer objects to a known state.
//!
//! This function initializes CpuTimer0 and CpuTimer1, sets their period
//! registers to the maximum count, clears the prescalers, and reloads the
//! counters while keeping the timers stopped.
//!
//! \return None.
//
//*****************************************************************************
void InitCpuTimers(void)
{
    // CPU Timer 0
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;

	// Initialize timer period to maximum:	
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;

	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):	
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;

	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;

	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	//CpuTimer0.InterruptCount = 0;	             	
	// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
	// Do not use these two timers if you ever plan on integrating
	// DSP-BIOS or another realtime OS.
	// For this reason, the code to manipulate these two timers is
	// commented out and not used in these examples.
    // Initialize address pointers to respective timer registers:
	CpuTimer1.RegsAddr = &CpuTimer1Regs;


	// Initialize timer period to maximum:
	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;


	// Make sure timers are stopped:
	CpuTimer1Regs.TCR.bit.TSS = 1;             


	CpuTimer1Regs.TCR.bit.TRB = 1;             


}	
	
//*****************************************************************************
//
//! Configure one CPU timer period.
//!
//! \param Timer points to the CPU timer descriptor to configure.
//! \param Freq is the CPU clock frequency in MHz.
//! \param Period is the requested timer period in microseconds.
//!
//! This function calculates the timer period register value, programs the
//! prescaler and control fields, and clears the timer interrupt counter.
//!
//! \return None.
//
//*****************************************************************************
void ConfigCpuTimer(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;
	
	// Initialize timer period:	
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):	
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;
	
	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer 
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 1;
	Timer->RegsAddr->TCR.bit.FREE = 1;     // Timer Free Run
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt
	
	// Reset interrupt counter:
	Timer->InterruptCount = 0;
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/


