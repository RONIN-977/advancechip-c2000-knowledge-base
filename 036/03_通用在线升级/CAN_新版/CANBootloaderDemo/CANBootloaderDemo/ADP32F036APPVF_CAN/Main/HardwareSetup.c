//#############################################################################
//
// FILE:   HardwareSetup.c
//
// TITLE:  Application hardware setup and system initialization.
//
// Description:
// This file initializes the system clock tree, interrupt framework, flash,
// peripheral clocks, GPIO, PWM, ADC, and related hardware resources used by
// the application.
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
#include "ADP32F03x_EPwm_defines.h"
#include "MotorInclude.h"
#include "PreDriver.h"
#include "Parameter.h"
#include "CANcomm.h"

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#define Device_cal (void   (*)(void))0x3D7C80

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern COPY_TABLE prginRAM;

/*============================================================================*/
/*                           FUNCTION DECLARATIONS                            */
/*============================================================================*/
void 	copy_prg(COPY_TABLE *tp);
void   	DisableDog(void);   
void 	InitPll(Uint val);
void	InitFlash(void);
void	InitPeripheralClocks(void);

void 	InitPieCtrl(void);
void 	InitPieVectTable(void);
void 	InitSetGpio(void);
void    InitSetPWM();
void    InitSetAdc(void);

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Initialize the system control startup sequence.
//!
//! This function disables the watchdog, configures the PLL, copies RAM-resident
//! routines, initializes flash wait states, and enables the required peripheral
//! clocks before the application starts.
//!
//! \return None.
//
//*****************************************************************************
void InitSysCtrl()
{   
    DisableDog();			    // Disable the watchdog
	InitPll(DSP_CLOCK/10);      // Initialize PLL to set system clock frequency
    copy_prg(&prginRAM);		// Move the program from FLASH to RAM
    InitFlash();				// Initialize Flash Control registers
    InitPeripheralClocks();		// Initialize peripheral clocks
}

//*****************************************************************************
//
//! Initialize the interrupt framework.
//!
//! This function clears global interrupts, resets the PIE control state, and
//! installs the ADC, trip-zone, and PWM interrupt handlers used by the
//! application.
//!
//! \return None.
//
//*****************************************************************************
void InitInterrupt()
{
   DINT;							// Clear all interrupts and initialize PIE vector table
   InitPieCtrl();  					// Disable PIE
   IER = 0x0000;
   //IFR = 0x0000;

   InitPieVectTable();				// Enable PIE

   EALLOW;  						// Enable protected register access

   PieVectTable.ADCINT1     = &EndOfADCISR;		    // ADC interrupt
   PieVectTable.EPWM2_TZINT = &OneShotTZOfEPWMISR;	// Protection interrupt
   PieVectTable.EPWM2_INT 	= &ZeroOfEPWMISR;		// PWM interrupt

   EDIS;
}

//*****************************************************************************
//
//! Enable the application interrupt sources.
//!
//! This function enables the CPU interrupt groups and the PIE channels required
//! for ADC conversion completion and EPWM protection/control events.
//!
//! \return None.
//
//*****************************************************************************
void SetInterruptEnable()
{
   IER |= (M_INT1 | M_INT2 | M_INT3 | M_INT5);	// Enable interrupts
   IER |= M_INT9;

   PieCtrlRegs.PIEIER1.bit.INTx1 = 1;           // ADC1 interrupt
   PieCtrlRegs.PIEIER2.bit.INTx2 = 1;           // EPWM1 protection interrupt
   PieCtrlRegs.PIEIER3.bit.INTx2 = 1;           // EPWM1 cycle interrupt
}

//*****************************************************************************
//
//! Initialize the application peripherals.
//!
//! This function configures GPIO, PWM, ADC, CAN, optional SPI resources,
//! predriver support, and the CPU timer base used by the application.
//!
//! \return None.
//
//*****************************************************************************
void InitPeripherals(void)
{
   	InitSetGpio();
   	InitSetPWM();
   	InitSetAdc();
   	CANInit();

#ifdef SPI_SCOPE_TEST
   	InitSpiIO();
   	InitSpi();
#endif

#if (ADM32F036_TYPE == ADM32F036_A3)
   	PreDriverInit();
   	DriverClearFault();
#endif

   	InitCpuTimers();
   	StartCpuTimer1();				     // Prepare for timer
}

//*****************************************************************************
//
//! Copy one linker-generated program section from flash to RAM.
//!
//! \param tp points to the copy-table structure that describes the RAM load.
//!
//! This function reads the first record in the linker copy table and moves the
//! corresponding program image from its flash load address to the runtime RAM
//! address.
//!
//! \return None.
//
//*****************************************************************************
extern MovePrgFrFlashToRam(long src_addr, long dst_addr, Uint16 size);
void copy_prg(COPY_TABLE *tp)
{
	Uint size;
	COPY_RECORD *crp = &tp->recs[0];

	size = (Uint)crp->size - 1;

	MovePrgFrFlashToRam(crp->src_addr, crp->dst_addr, size);
}

//*****************************************************************************
//
//! Initialize the flash control registers.
//!
//! This RAM-resident function configures flash pipeline mode and wait-state
//! values before flash accesses are used during normal application execution.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(InitFlash, "ramfuncs");
void InitFlash(void)
{
   EALLOW;

   // Enable Flash Pipeline mode to improve performance
   FlashRegs.FOPT.all = 0x01;
   // Set the Random Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = READ_RAND_FLASH_WAITE;
   // Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = READ_PAGE_FLASH_WAITE;
   // Set the Waitstate for the OTP
   FlashRegs.FOTPWAIT.bit.OTPWAIT = READ_OTP_WAITE;
   
   EDIS;

   asm(" RPT #7 || NOP");	// Short delay
}	

//*****************************************************************************
//
//! Service the watchdog timer.
//!
//! This function writes the watchdog key sequence so the watchdog does not time
//! out while the application is running.
//!
//! \return None.
//
//*****************************************************************************
void KickDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//*****************************************************************************
//
//! Trigger a DSP reset through the watchdog.
//!
//! This function programs the watchdog control register to force a device reset
//! when a controlled restart is required.
//!
//! \return None.
//
//*****************************************************************************
void ResetDSP(void)
{
    EALLOW;
    SysCtrlRegs.WDCR = 0x0002;
    EDIS;
}

//*****************************************************************************
//
//! Disable the watchdog timer.
//!
//! This function unlocks the watchdog control register and switches the
//! watchdog into the disabled state.
//!
//! \return None.
//
//*****************************************************************************
void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x0068;
    EDIS;
}

//*****************************************************************************
//
//! Enable the watchdog timer.
//!
//! This function restores the watchdog control configuration used during normal
//! application execution.
//!
//! \return None.
//
//*****************************************************************************
void EnableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR= 0x002A;
    EDIS;
}

//*****************************************************************************
//
//! Configure the oscillator source and PLL divider.
//!
//! \param val is the target PLL divider value.
//!
//! This function selects the active oscillator source, updates the PLL divider,
//! waits for lock, and applies the final clock divide setting used by the
//! system controller.
//!
//! \return None.
//
//*****************************************************************************
void InitPll(Uint16 val)
{
    // Select oscillator source based on configuration
#ifndef INTER_OSC
    // External crystal oscillator configuration
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;     // Enable external crystal oscillator
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF  = 1;     // Disable external clock input
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0;  // Select oscillator source
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // Switch to external clock
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // Watchdog clock to external
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;     // Disable internal OSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // Disable internal OSC1
    EDIS;
#else
    // Internal oscillator configuration
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;     // Enable internal OSC1
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 0;   // Select internal OSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // Disable external clock
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;     // Disable crystal oscillator
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;     // Disable internal OSC2
    EDIS;
#endif

    // Configure PLL if not in limp mode
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
    {
        // Set DIVSEL to 0 before changing PLL
        if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
        {
            EALLOW;
            SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
            EDIS;
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;    // Disable missing clock detect
        SysCtrlRegs.PLLCR.bit.DIV = val;        // Set PLL divider value
        EDIS;

        // Disable watchdog during PLL lock
        DisableDog();

        // Wait for PLL to lock
        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
            // Wait for lock status
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;    // Enable missing clock detect
        EDIS;

        // Set final clock divider
        EALLOW;
#ifndef INTER_OSC
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;//2;     // Set divider for external clock
#else
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;     // Set divider for internal clock
#endif
        EDIS;
    }
}

//*****************************************************************************
//
//! Enable the peripheral module clocks.
//!
//! This function programs the peripheral clock prescalers and enables the clock
//! gates required by the application's communication, PWM, ADC, and comparator
//! resources.
//!
//! \return None.
//
//*****************************************************************************
void InitPeripheralClocks(void)
{
   EALLOW;
   SysCtrlRegs.LOSPCP.all = 0x0002;        // Set low-speed peripheral clock prescaler
   SysCtrlRegs.XCLK.all = 0x02;            // Configure XCLKOUT settings
   SysCtrlRegs.PCLKCR0.all = 0x051C;       // Enable clocks for multiple peripherals
   SysCtrlRegs.PCLKCR0.bit.LINAENCLK = 1;  // Enable LIN-A peripheral clock
   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;
   SysCtrlRegs.PCLKCR1.all = 0x011F;       // Enable clocks for SCI, SPI, I2C
   SysCtrlRegs.PCLKCR3.all = 0x6701;       // Enable clocks for DMA, CPU pipeline, etc.
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1; // Enable Comparator 2 clock
   EDIS;
}

//*****************************************************************************
//
//! Reset the PIE control registers.
//!
//! This function disables the PIE block and clears all PIE interrupt enable and
//! flag registers before the vector table is initialized.
//!
//! \return None.
//
//*****************************************************************************
void InitPieCtrl(void)
{
	Uint * m_Point1,* m_Point2;
	Uint   m_Index;

    DINT;    
    
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	// Disable the PIE
	// Clear all PIEIER registers:
	m_Point1 = (Uint *)&PieCtrlRegs.PIEIER1.all;
	m_Point2 = (Uint *)&PieCtrlRegs.PIEIFR1.all;
	for(m_Index = 0;m_Index<12;m_Index++)
	{
		*(m_Point1++) = 0;
		*(m_Point2++) = 0;
	}
}	

//*****************************************************************************
//
//! Handle unexpected PIE vector hits.
//!
//! This reserved interrupt service routine increments an error counter so the
//! firmware can detect unexpected interrupt routing during bring-up.
//!
//! \return None.
//
//*****************************************************************************
int gErrorIntCnt;
interrupt void rsvd_ISR(void)      
{
	gErrorIntCnt++;				// Check for unhandled interrupts
}

//*****************************************************************************
//
//! Initialize the PIE vector table.
//!
//! This function fills the entire PIE vector table with the reserved interrupt
//! handler and then enables PIE dispatching.
//!
//! \return None.
//
//*****************************************************************************
void InitPieVectTable(void)
{
	int16	i;
	PINT *pPieTable = (void *) &PieVectTable;
		
	EALLOW;	
	for(i=0; i < 128; i++)
	{
		*pPieTable++ = rsvd_ISR;	// Set all vectors to default handler
	}
	EDIS;

	// Enable the PIE Vector Table
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}

//*****************************************************************************
//
//! Configure the application GPIO pins.
//!
//! This function sets the pull-ups, mux selection, output defaults, and GPIO
//! directions required by the application hardware.
//!
//! \return None.
//
//*****************************************************************************
void InitSetGpio(void)
{
    EALLOW;

    // STEP1: GPIO pull-up configuration
    GpioCtrlRegs.GPAPUD.all = 0x00000000L;
    GpioCtrlRegs.GPBPUD.all = 0x0;

    // STEP2: GPIO function selection for used pins
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;

    // STEP3: GPIO data register initialization
    GpioDataRegs.GPADAT.all = 0x00000000L;
    GpioDataRegs.GPBDAT.all = 0x00000000L;

    // STEP4: GPIO direction configuration (1 = output)
    GpioCtrlRegs.GPADIR.all = 0x00;
    GpioCtrlRegs.GPBDIR.all = 0x00;
    GpioCtrlRegs.GPADIR.bit.GPIO0  = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO1  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO3  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO4  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7  = 1;

    GpioDataRegs.GPACLEAR.bit.GPIO2  = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO3  = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO4  = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO5  = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO6  = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO7  = 1;
#if (ADM32F036_TYPE == ADM32F036_A3)
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
    GpioDataRegs.GPASET.bit.GPIO9  = 1; // LIN_EN
#endif

    EDIS;

}

//*****************************************************************************
//
//! Configure the EPWM modules for motor control.
//!
//! This function initializes the PWM timing base, compare logic, trip-zone
//! protection, dead-band control, and current-sampling trigger behavior used by
//! the motor drive.
//!
//! \return None.
//
//*****************************************************************************
void InitSetPWM(void)
{
	EALLOW;

	///////////// PWM1 (EPWM2) Configuration //////////////
	EPwm2Regs.TBPRD = C_INIT_PRD;              // Set PWM period
	EPwm2Regs.TBPHS.all = 0;                   // Set phase to 0
	EPwm2Regs.TBCTL.all = 0x2012;              // Time-base control

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
	EPwm2Regs.CMPCTL.all = 0x0000;             // Compare control for single shunt
#else
	EPwm2Regs.CMPCTL.all = 0x0100;             // Compare control for multi-shunt
#endif

	EPwm2Regs.CMPA.half.CMPA = C_INIT_PRD/2;   // Set compare A to 50% duty cycle
	EPwm2Regs.CMPB = C_INIT_PRD/2;             // Set compare B to 50% duty cycle

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef CUT_DOWN_VER_HARDWARE
	EPwm2Regs.AQCTLA.all = 0x0810;             // Action qualifier for full hardware
#else
	EPwm2Regs.AQCTLA.all = 0x0420;             // Action qualifier for cut-down hardware
#endif
#else
#ifndef CUT_DOWN_VER_HARDWARE
    EPwm2Regs.AQCTLA.all = 0x0090;             // Action qualifier for multi-shunt full hardware
#else
    EPwm2Regs.AQCTLA.all = 0x0060;             // Action qualifier for multi-shunt cut-down
#endif
#endif

#ifndef CUT_DOWN_VER_HARDWARE
	EPwm2Regs.DBCTL.all = 0x0007;              // Dead-band control for full hardware
#else
	EPwm2Regs.DBCTL.all = 0x000B;              // Dead-band control for cut-down hardware
#endif

	EPwm2Regs.DBFED = C_MAX_DB;                // Set falling edge delay
	EPwm2Regs.DBRED = C_MAX_DB;                // Set rising edge delay

	EPwm2Regs.TZCLR.all = 0x07;                // Clear trip zone flags

    // Configure digital compare and trip zone protection
    EPwm2Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;           // DCA event 1 source selection
    EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;  // Asynchronous event

    EPwm2Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;         // Trip on DCAH low
    EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;    // DCAH from comparator 1
    EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;         // DCAL from trip zone 2

    EPwm2Regs.TZSEL.bit.DCAEVT1 = 1;                     // Enable DCA event 1 trip
    EPwm2Regs.TZSEL.bit.OSHT2 = 1;                       // Enable TZ2 one-shot trip

    // Configure DCB for cycle-by-cycle current limiting
    EPwm2Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;
    EPwm2Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm2Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCBH_LOW;
    EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;
    EPwm2Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;
    EPwm2Regs.TZSEL.bit.DCBEVT2 = 1;

#ifndef CUT_DOWN_VER_HARDWARE
	EPwm2Regs.TZCTL.all = 0x0005;              // Trip output control for full hardware
#else
	EPwm2Regs.TZCTL.all = 0x00A0;              // Trip output control for cut-down hardware
#endif

	EPwm2Regs.TZEINT.all = 0x0004;             // Enable digital compare interrupt

#if (CURRENT_SAMPLE_TYPE != CURRENT_SAMPLE_1SHUNT)
	EPwm2Regs.ETPS.all = 0x0101;               // Event trigger for multi-shunt
#endif
    EPwm2Regs.ETPS.bit.INTPRD = 1;             // Set interrupt period

	///////////// PWM2 (EPWM3) Configuration //////////////
	EPwm3Regs.TBPRD = C_INIT_PRD;              // Set PWM period
	EPwm3Regs.TBPHS.all = 0;                   // Set phase to 0
	EPwm3Regs.TBCTL.all = 0x2006;              // Time-base control

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    EPwm3Regs.CMPCTL.all = 0x0000;             // Compare control for single shunt
#else
    EPwm3Regs.CMPCTL.all = 0x0100;             // Compare control for multi-shunt
#endif

	EPwm3Regs.CMPA.half.CMPA = C_INIT_PRD/2;   // Set compare A to 50% duty cycle

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef CUT_DOWN_VER_HARDWARE
    EPwm3Regs.AQCTLA.all = 0x0810;             // Action qualifier for full hardware
#else
    EPwm3Regs.AQCTLA.all = 0x0420;             // Action qualifier for cut-down hardware
#endif
#else
#ifndef CUT_DOWN_VER_HARDWARE
    EPwm3Regs.AQCTLA.all = 0x0090;             // Action qualifier for multi-shunt full hardware
#else
    EPwm3Regs.AQCTLA.all = 0x0060;             // Action qualifier for multi-shunt cut-down
#endif
#endif

#ifndef CUT_DOWN_VER_HARDWARE
	EPwm3Regs.DBCTL.all = 0x0007;              // Dead-band control for full hardware
#else
	EPwm3Regs.DBCTL.all = 0x000B;              // Dead-band control for cut-down hardware
#endif

	EPwm3Regs.DBFED = C_MAX_DB;                // Set falling edge delay
	EPwm3Regs.DBRED = C_MAX_DB;                // Set rising edge delay

    //DCAEVT1 as one-shot
    //Set the Trip-zone (TZ) Module

    EPwm3Regs.DCACTL.bit.EVT1SRCSEL = 0;

    EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;

    EPwm3Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;

    EPwm3Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;
    EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm3Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm3Regs.TZSEL.bit.OSHT2 = 1;

    // Configure DCB for EPWM3
    EPwm3Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;
    EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm3Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCAH_LOW;
    EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;
    EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;
    EPwm3Regs.TZSEL.bit.DCBEVT2 = 1;

#ifndef CUT_DOWN_VER_HARDWARE
    EPwm3Regs.TZCTL.all = 0x0005;              // Trip output control for full hardware
#else
    EPwm3Regs.TZCTL.all = 0x00A0;              // Trip output control for cut-down hardware
#endif

	///////////// PWM3 (EPWM4) Configuration //////////////
	EPwm4Regs.TBPRD = C_INIT_PRD;              // Set PWM period
	EPwm4Regs.TBPHS.all = 0;                   // Set phase to 0
	EPwm4Regs.TBCTL.all = 0x2006;              // Time-base control

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    EPwm4Regs.CMPCTL.all = 0x0000;             // Compare control for single shunt
#else
    EPwm4Regs.CMPCTL.all = 0x0100;             // Compare control for multi-shunt
#endif

	EPwm4Regs.CMPA.half.CMPA = C_INIT_PRD/2;   // Set compare A to 50% duty cycle

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#ifndef CUT_DOWN_VER_HARDWARE
    EPwm4Regs.AQCTLA.all = 0x0810;             // Action qualifier for full hardware
#else
    EPwm4Regs.AQCTLA.all = 0x0420;             // Action qualifier for cut-down hardware
#endif
#else
#ifndef CUT_DOWN_VER_HARDWARE
    EPwm4Regs.AQCTLA.all = 0x0090;             // Action qualifier for multi-shunt full hardware
#else
    EPwm4Regs.AQCTLA.all = 0x0060;             // Action qualifier for multi-shunt cut-down
#endif
#endif

#ifndef CUT_DOWN_VER_HARDWARE
	EPwm4Regs.DBCTL.all = 0x0007;              // Dead-band control for full hardware
#else
	EPwm4Regs.DBCTL.all = 0x000B;              // Dead-band control for cut-down hardware
#endif

	EPwm4Regs.DBFED = C_MAX_DB;                // Set falling edge delay
	EPwm4Regs.DBRED = C_MAX_DB;                // Set rising edge delay

    // DCAEVT1 as one-shot
    // Set the Trip-zone (TZ) Module
    // EPwm4Regs.DCACTL.bit.EVT1SYNCE = 0;//1;
    EPwm4Regs.DCACTL.bit.EVT1SRCSEL = 0;

    EPwm4Regs.DCTRIPSEL.bit.DCAHCOMPSEL = DC_COMP1OUT;
    EPwm4Regs.DCTRIPSEL.bit.DCALCOMPSEL = DC_TZ2;
    EPwm4Regs.TZDCSEL.bit.DCAEVT1 = TZ_DCAH_LOW;

    EPwm4Regs.DCACTL.bit.EVT1SRCSEL = DC_EVT1;
    EPwm4Regs.DCACTL.bit.EVT1FRCSYNCSEL = DC_EVT_ASYNC;

    EPwm4Regs.TZSEL.bit.DCAEVT1 = 1;
    EPwm4Regs.TZSEL.bit.OSHT2 = 1;

    // Configure DCB for EPWM4
    EPwm4Regs.DCBCTL.bit.EVT2SRCSEL = DC_EVT2;
    EPwm4Regs.DCBCTL.bit.EVT2FRCSYNCSEL = DC_EVT_ASYNC;
    EPwm4Regs.TZDCSEL.bit.DCBEVT2 = TZ_DCAH_LOW;
    EPwm4Regs.DCTRIPSEL.bit.DCBHCOMPSEL = DC_COMP2OUT;
    EPwm4Regs.DCTRIPSEL.bit.DCBLCOMPSEL = DC_TZ2;
    EPwm4Regs.TZSEL.bit.DCBEVT2 = 1;

#ifndef CUT_DOWN_VER_HARDWARE
    EPwm4Regs.TZCTL.all = 0x0005;              // Trip output control for full hardware
#else
    EPwm4Regs.TZCTL.all = 0x00A0;              // Trip output control for cut-down hardware
#endif

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    ///////////// PWM4 (EPWM5) Configuration for Single Shunt //////////////
    EPwm5Regs.TBPRD = C_INIT_PRD;              // Set PWM period
    EPwm5Regs.TBPHS.all = 0;                   // Set phase to 0
    EPwm5Regs.TBCTL.all = 0x2006;              // Time-base control

    EPwm5Regs.CMPA.half.CMPA = C_INIT_PRD/2;   // Set compare A to 50% duty cycle
    EPwm5Regs.CMPB = C_INIT_PRD/2;             // Set compare B to 50% duty cycle

    EPwm5Regs.ETSEL.all = 0xFC00;               // Trigger on CMPA rising and CMPB falling edges
    EPwm5Regs.ETPS.all = 0x1100;                // Event trigger prescale

    // Configure EPWM2 interrupt for single shunt
    EPwm2Regs.ETCLR.bit.INT = 1;               // Clear interrupt flag
    EPwm2Regs.ETSEL.all = 0x0009;              // Enable underflow interrupt
#else
    // Configure EPWM2 for multi-shunt current sampling
    EPwm2Regs.ETCLR.bit.INT = 1;               // Clear interrupt flag
    EPwm2Regs.ETSEL.bit.SOCAEN = 1;            // Enable start-of-conversion A
    EPwm2Regs.ETSEL.bit.SOCASEL = 1;           // Trigger on valley (counter=0)
    EPwm2Regs.ETSEL.bit.INTEN = 1;             // Enable interrupt
    EPwm2Regs.ETSEL.bit.INTSEL = 1;            // Interrupt on period match
#endif

	EDIS;
}

//*****************************************************************************
//
//! Configure the ADC and comparator front end.
//!
//! This function powers up the ADC path, initializes comparator thresholds,
//! configures the sampling trigger sources, and selects the application input
//! channels used by the control loop.
//!
//! \return None.
//
//*****************************************************************************
extern void ADP32F03x_usDelay(Uint32 Count);
void InitSetAdc(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    EDIS;

    asm(" RPT #28 || NOP");                 // Delay
    asm(" RPT #28 || NOP");

    EALLOW;
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    PagOpaRegs.PAGOPACTL.bit.OPAEN = 1;     // Enable internal operational amplifier
#else
    PagOpaRegs.PAGOPACTL.bit.OPAEN = 1;     // Enable internal operational amplifier

    PagOpaRegs.PAGOPACTL.bit.PGA1_GAIN = 4; // Gain 16x
    PagOpaRegs.PAGOPACTL.bit.PGA2_GAIN = 4;
    PagOpaRegs.PAGOPACTL.bit.PGA3_GAIN = 4;

    PagOpaRegs.PAGOPACTL.bit.PGA1EN = 1;
    PagOpaRegs.PAGOPACTL.bit.PGA2EN = 1;
    PagOpaRegs.PAGOPACTL.bit.PGA3EN = 1;
#endif
    EDIS;

    EALLOW;
    Comp1Regs.DACVAL.bit.DACVAL = OverCurPointInternal;
                                        // Hardware comparison point setting

    Comp1Regs.COMPCTL.bit.SYNCSEL       = 0;  // Asynchronous version of comparator output
    Comp1Regs.COMPCTL.bit.CMPINV        = 1;  // Inverted output of comparator
    Comp1Regs.COMPCTL.bit.COMPSOURCE    = 0;  // Input connected to internal DAC
    Comp1Regs.COMPCTL.bit.QUALSEL       = 10; // Qualification period for synchronized output
    Comp1Regs.COMPCTL.bit.COMPDACEN     = 1;  // Comparator/DAC Enable

    Comp2Regs.DACVAL.bit.DACVAL = CBCCurPointInternal;
                                        // Hardware comparison point setting

    Comp2Regs.COMPCTL.bit.SYNCSEL       = 0;  // Asynchronous version of comparator output
    Comp2Regs.COMPCTL.bit.CMPINV        = 1;  // Inverted output of comparator
    Comp2Regs.COMPCTL.bit.COMPSOURCE    = 0;  // Input connected to internal DAC
    Comp2Regs.COMPCTL.bit.QUALSEL       = 10; // Qualification period for synchronized output
    Comp2Regs.COMPCTL.bit.COMPDACEN     = 1;  // Comparator/DAC Enable
    EDIS;

    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC background
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    EDIS;

    ADP32F03x_usDelay(20000L);                 // 1ms delay

    //----------- Start ADC target sampling and conversion channel selection -----------
    EALLOW;
    AdcRegs.ADCSAMPLEMODE.all = 0;           // Sampling mode: sequential (simultaneous)
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;     // ADCINT1 trips after ADC results latch
    AdcRegs.ADCCTL1.bit.TEMPCONV    = 1;

    AdcRegs.ADCCTL2.bit.CLKDIV2EN = 1;
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    // SOCx Trigger Select
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCA;
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;
    AdcRegs.ADCSOC6CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;
    AdcRegs.ADCSOC7CTL.bit.TRIGSEL  = ADCTRIG_EPWM5_SOCB;

    // Acquisition Pulse Size
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS_SYS_CLKS;

    // SOCx Channel Select
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S2;
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S2;
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S12;
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S5;  // Internal Temperature Sensor
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S3;  // A1
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S3;  // A2
    AdcRegs.ADCSOC6CTL.bit.CHSEL  = ADC_PIN_S3;  // A0
    AdcRegs.ADCSOC7CTL.bit.CHSEL  = ADC_PIN_S6;  // IBUS
#else
    AdcRegs.INTSEL1N2.all     = 0x0024;

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADCTRIG_EPWM4_SOCB;
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL  = ADCTRIG_EPWM2_SOCA;//ADC_TRIG_SOURCE;

    //Acquisition Pulse Size
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_SYS_CLKS;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_SYS_CLKS;

    // SOCx Channel Select
    // Dual Motor Demo
    AdcRegs.ADCSOC0CTL.bit.CHSEL  = ADC_PIN_S3; // IU
    AdcRegs.ADCSOC1CTL.bit.CHSEL  = ADC_PIN_S7; // IV
    AdcRegs.ADCSOC2CTL.bit.CHSEL  = ADC_PIN_S12;// ADC_PIN_S12;//13;//ADC_PIN_S0;
    AdcRegs.ADCSOC3CTL.bit.CHSEL  = ADC_PIN_S6; // vdc
    AdcRegs.ADCSOC4CTL.bit.CHSEL  = ADC_PIN_S5; // Internal Temperature Sensor
    AdcRegs.ADCSOC5CTL.bit.CHSEL  = ADC_PIN_S3; // A2
#endif

    ADC_CLEAR_INT_FLAG;

    EDIS;
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
