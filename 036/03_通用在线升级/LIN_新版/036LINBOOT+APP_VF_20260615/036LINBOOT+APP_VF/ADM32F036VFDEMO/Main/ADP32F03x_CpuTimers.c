//###########################################################################
//###########################################################################

#include "ADP32F03x_Device.h"     // Headerfile Include File

struct CPUTIMER_VARS CpuTimer0;

// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
struct CPUTIMER_VARS CpuTimer1;
//struct CPUTIMER_VARS CpuTimer2;

/******************************************************************************
Function Name : InitCpuTimers
Function description : Initialize all CPU timers to a known state
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : N/A
Returns : N/A
******************************************************************************/
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
	
/******************************************************************************
Function Name : ConfigCpuTimer
Function description : Initialize the selected timer to the specified period
Engineer  : Advancechip
Date : 2025-11-07
Parameters  : Timer - Pointer to timer structure, Freq - Frequency in MHz, Period - Period in microseconds
Returns : N/A
******************************************************************************/
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

//===========================================================================
// End of file.
//===========================================================================