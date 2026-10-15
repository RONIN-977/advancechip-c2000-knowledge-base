//#############################################################################
//
// FILE:   ADP32F03x_SysCtrl.c
//
// TITLE:  ADP32F03x system control initialization functions.
//
// Description:
// This file initializes system control, watchdog, PLL, oscillator selection,
// and peripheral clock configuration for the ADP32F03x device.
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
#include <ADP32F03x_Device.h>
#include <ADP32F03x_Examples.h>

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

// Functions run from RAM are assigned to a separate section that the linker
// maps to dedicated load and run addresses.

//*****************************************************************************
//
//! Initialize the system control registers.
//!
//! This function disables the watchdog, performs device calibration when the
//! boot ROM path was bypassed, selects the default clock source, initializes
//! the PLL, and enables the peripheral clocks used by the application.
//!
//! \return None.
//
//*****************************************************************************

void InitSysCtrl(void)
{

    // Disable the watchdog
    DisableDog();

    // *IMPORTANT*
    // The Device_cal function, which copies the ADC & oscillator calibration values
    // from AdvChip reserved OTP into the appropriate trim registers, occurs automatically
    // in the Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC and oscillators to function according
    // to specification. The clocks to the ADC MUST be enabled before calling this
    // function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.

	    EALLOW;
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // Enable ADC peripheral clock
		(*Device_cal)();
		SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // Return ADC clock to original state
		EDIS;

   // Select Internal Oscillator 1 as Clock Source (default), and turn off all unused clocks to
   // conserve power.
   IntOsc1Sel();

   // Initialize the PLL control: PLLCR and CLKINDIV
   // DSP28_PLLCR and DSP28_CLKINDIV are defined in ADP32F03x_Examples.h
   InitPll(DSP28_PLLCR,DSP28_DIVSEL);//(6,2);//
   // Initialize the peripheral clocks
   InitPeripheralClocks();
}

//*****************************************************************************
//
//! Initialize the flash control registers.
//!
//! This function must execute from RAM. It enables flash pipeline mode and
//! programs the flash and OTP wait-state settings required for reliable flash
//! execution.
//!
//! \return None.
//
//*****************************************************************************
#pragma CODE_SECTION(InitFlash, "ramfuncs");
void InitFlash(void)
{
    EALLOW;
    //Enable Flash Pipeline mode to improve performance
    //of code executed from Flash.
    FlashRegs.FOPT.bit.ENPIPE = 1;

    //                CAUTION
    //Minimum waitstates required for the flash operating
    //at a given CPU rate must be characterized by AdvChip.
    //Refer to the datasheet for the latest information.

   //Set the Paged Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;//2;

   //Set the Random Waitstate for the Flash
   FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;//2;

   //Set the Waitstate for the OTP
   FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;//2;

   //                CAUTION
   //ONLY THE DEFAULT VALUE FOR THESE 2 REGISTERS SHOULD BE USED
   FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
   FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
   EDIS;

   //Force a pipeline flush to ensure that the write to
   //the last register configured occurs before returning.

   asm(" RPT #7 || NOP");
}

//*****************************************************************************
//
//! Service the watchdog timer.
//!
//! This function writes the watchdog key sequence required to prevent a
//! watchdog reset while the application is running.
//!
//! \return None.
//
//*****************************************************************************

void ServiceDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//*****************************************************************************
//
//! Disable the watchdog timer.
//!
//! This function writes the watchdog control register to stop watchdog reset
//! generation during initialization or long operations.
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
//! This function restores watchdog operation so the device can be reset or
//! supervised after initialization has completed.
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
#if 0
//*****************************************************************************
//
//! Initialize the PLL divider settings.
//!
//! \param val is the PLL multiplier value written to the PLL control register.
//! \param divsel is the requested system clock divider selection.
//!
//! This function updates the PLL configuration after checking limp mode and
//! waiting for the PLL to lock when a divider change is required.
//!
//! \return None.
//
//*****************************************************************************

void InitPll(Uint16 val, Uint16 divsel)
{
   volatile Uint16 iVol;

   // Make sure the PLL is not running in limp mode
   if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
   {
	  EALLOW;
      // OSCCLKSRC1 failure detected. PLL running in limp mode.
      // Re-enable missing clock logic.
      SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
      EDIS;
      // Replace this line with a call to an appropriate
      // SystemShutdown(); function.
      asm("        ESTOP0");     // Uncomment for debugging purposes
   }

   // DIVSEL MUST be 0 before PLLCR can be changed from
   // 0x0000. It is set to 0 by an external reset XRSn
   // This puts us in 1/4
   if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
   {
       EALLOW;
       SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
       EDIS;
   }

   // Change the PLLCR
   if (SysCtrlRegs.PLLCR.bit.DIV != val)
   {

      EALLOW;
      // Before setting PLLCR turn off missing clock detect logic
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
      SysCtrlRegs.PLLCR.bit.DIV = val;
      EDIS;

      // Optional: Wait for PLL to lock.
      // During this time the CPU will switch to OSCCLK/2 until
      // the PLL is stable.  Once the PLL is stable the CPU will
      // switch to the new PLL value.
      //
      // This time-to-lock is monitored by a PLL lock counter.
      //
      // Code is not required to sit and wait for the PLL to lock.
      // However, if the code does anything that is timing critical,
      // and requires the correct clock be locked, then it is best to
      // wait until this switching has completed.

      // Wait for the PLL lock bit to be set.

      // The watchdog should be disabled before this loop, or fed within
      // the loop via ServiceDog().

	  // Uncomment to disable the watchdog
      DisableDog();

      while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
      {
	      // Uncomment to service the watchdog
          // ServiceDog();
      }

      EALLOW;
      SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
      EDIS;
    }

    // If switching to 1/2
	if((divsel == 1)||(divsel == 2))
	{
		EALLOW;
	    SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
	    EDIS;
	}

	// If switching to 1/1
	// * First go to 1/2 and let the power settle
	//   The time required will depend on the system, this is only an example
	// * Then switch to 1/1
	if(divsel == 3)
	{
		EALLOW;
	    SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
	    DELAY_US(50L);
	    SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
	    EDIS;
    }
}
#endif
//*****************************************************************************
//
//! Initialize peripheral module clocks.
//!
//! This function configures the low-speed peripheral prescaler, sets the
//! XCLKOUT ratio, and enables the clocks required by the selected peripherals.
//!
//! \return None.
//
//*****************************************************************************

void InitPeripheralClocks(void)
{
   EALLOW;

// LOSPCP prescale register settings, normally it will be set to default values

   GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // GPIO18 = XCLKOUT
   SysCtrlRegs.LOSPCP.all = 0x0002;

// XCLKOUT to SYSCLKOUT ratio.  By default XCLKOUT = 1/4 SYSCLKOUT
   SysCtrlRegs.XCLK.bit.XCLKOUTDIV=2;

// Peripheral clock enables set for the selected peripherals.
// If you are not using a peripheral leave the clock off
// to save on power.
//
//
// This function is not written to be an example of efficient code.

   SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;      // ADC
   SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 1;    // COMP1
   SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1;    // COMP2
   SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK = 1;    // COMP3
   SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;    // eCAP1
   SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=1;      // eCAN-A
   SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;    // eQEP1
   SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;    // ePWM1
   SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;    // ePWM2
   SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;    // ePWM3
   SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;    // ePWM4
   SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;	  // ePWM5
   SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;	  // ePWM6
   SysCtrlRegs.PCLKCR1.bit.EPWM7ENCLK = 1;	  // ePWM7
   SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK = 1;    // HRPWM
   SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;     // I2C
   SysCtrlRegs.PCLKCR0.bit.LINAENCLK = 1;     // LIN-A
   SysCtrlRegs.PCLKCR3.bit.CLA1ENCLK = 1;     // CLA1
   SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;     // SCI-A
   SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;     // SPI-A
   SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 1;     // SPI-B

   SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;     // Enable TBCLK within the ePWM

   EDIS;
}

//*****************************************************************************
//
//! Unlock the code security module.
//!
//! This function writes the configured key values and performs the password
//! read sequence required to unlock the CSM protection logic.
//!
//! \return STATUS_SUCCESS if the CSM unlock succeeds; otherwise STATUS_FAIL.
//
//*****************************************************************************

#define STATUS_FAIL          0
#define STATUS_SUCCESS       1

Uint16 CsmUnlock()
{
    volatile Uint16 temp;

    // Load the key registers with the current password. The 0xFFFF's are dummy
	// passwords.  User should replace them with the correct password for the DSP.

    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;

    // Perform a dummy read of the password locations
    // if they match the key values, the CSM will unlock

    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;

    // If the CSM unlocked, return succes, otherwise return
    // failure.
    if (CsmRegs.CSMSCR.bit.SECURE == 0) return STATUS_SUCCESS;
    else return STATUS_FAIL;

}

//*****************************************************************************
//
//! Select internal oscillator 1 as the clock source.
//!
//! This function switches the device clocking to internal oscillator 1 and
//! disables the other oscillator sources to minimize power consumption.
//!
//! \return None.
//
//*****************************************************************************

void IntOsc1Sel (void) {
    EALLOW;
	SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL=0;  // Clk Src = INTOSC1
	SysCtrlRegs.CLKCTL.bit.XCLKINOFF=1;     // Turn off XCLKIN
	SysCtrlRegs.CLKCTL.bit.XTALOSCOFF=1;    // Turn off XTALOSC
	SysCtrlRegs.CLKCTL.bit.INTOSC2OFF=1;    // Turn off INTOSC2
    EDIS;
}

//*****************************************************************************
//
//! Select internal oscillator 2 as the clock source.
//!
//! This function switches the device clocking to internal oscillator 2,
//! updates the watchdog clock source, and disables the unused oscillator
//! sources to reduce power consumption.
//!
//! \return None.
//
//*****************************************************************************

void IntOsc2Sel (void) {
    EALLOW;
	SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 0;     // Turn on INTOSC2
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 1;  // Switch to INTOSC2
	SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // Turn off XCLKIN
	SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;     // Turn off XTALOSC
	SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // Switch to Internal Oscillator 2
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // Switch Watchdog Clk Src to INTOSC2
	SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // Turn off INTOSC1
    EDIS;
}

//*****************************************************************************
//
//! Select the external crystal oscillator as the clock source.
//!
//! This function enables the crystal oscillator path, switches the system and
//! watchdog clocks to that source, and disables the unused internal oscillators.
//!
//! \return None.
//
//*****************************************************************************

void XtalOscSel (void)  {
     EALLOW;
     SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;     // Turn on XTALOSC
     SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // Turn off XCLKIN
     SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0;  // Switch to external clock
     SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // Switch from INTOSC1 to INTOSC2/ext clk
     SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // Switch Watchdog Clk Src to external clock
     SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;		// Turn off INTOSC2
     SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;		// Turn off INTOSC1
     EDIS;

}


//*****************************************************************************
//
//! Select the external clock input as the clock source.
//!
//! This function enables XCLKIN-based clocking, switches the system and
//! watchdog clocks to the external source, and disables the internal oscillators.
//!
//! \return None.
//
//*****************************************************************************

void ExtOscSel (void)  {
     EALLOW;
     SysCtrlRegs.XCLK.bit.XCLKINSEL = 1;       // 1-GPIO19 = XCLKIN, 0-GPIO38 = XCLKIN
     SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;	   // Turn on XTALOSC
     SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 0; 	   // Turn on XCLKIN
     SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0; // Switch to external clock
     SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;  // Switch from INTOSC1 to INTOSC2/ext clk
     SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;   // Switch Watchdog Clk Src to external clock
     SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;	   // Turn off INTOSC2
     SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;	   // Turn off INTOSC1
     EDIS;
}



/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
