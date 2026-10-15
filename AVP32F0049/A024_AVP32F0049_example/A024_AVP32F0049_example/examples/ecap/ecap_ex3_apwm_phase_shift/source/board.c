/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  eCAP APWM Phase-shift Example.
*******************************************************************************************/

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	SYNC_init();
	ECAP_init();
	OUTPUTXBAR_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	//
	// OUTPUTXBAR3 -> myOUTPUTXBAR0 Pinmux
	//
	GPIO_setPinConfig(myOUTPUTXBAR0_OUTPUTXBAR_PIN_CONFIG);
	//
	// OUTPUTXBAR4 -> myOUTPUTXBAR1 Pinmux
	//
	GPIO_setPinConfig(myOUTPUTXBAR1_OUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// ECAP Configurations
//
//*****************************************************************************
void ECAP_init(){
	myECAP0_init();
	myECAP1_init();
}

void myECAP0_init(){
	//
	// Stops Time stamp counter.
	//
	ECAP_stopCounter(myECAP0_BASE);
	//
	// Sets eCAP in APWM mode.
	//
	ECAP_enableAPWMMode(myECAP0_BASE);
	//
	// Set eCAP APWM period.
	//
	ECAP_setAPWMPeriod(myECAP0_BASE,10000U);
	//
	// Set eCAP APWM on or off time count.
	//
	ECAP_setAPWMCompare(myECAP0_BASE,5000U);
	//
	// Set eCAP APWM polarity.
	//
	ECAP_setAPWMPolarity(myECAP0_BASE,ECAP_APWM_ACTIVE_LOW);
	//
	// Sets a phase shift value count.
	//
	ECAP_setPhaseShiftCount(myECAP0_BASE,0U);
	//
	// Disable counter loading with phase shift value.
	//
	ECAP_disableLoadCounter(myECAP0_BASE);
	//
	// Configures Sync out signal mode.
	//
	ECAP_setSyncOutMode(myECAP0_BASE,ECAP_SYNC_OUT_COUNTER_PRD);
	//
	// Configures emulation mode.
	//
	ECAP_setEmulationMode(myECAP0_BASE,ECAP_EMULATION_STOP);
	//
	// Starts Time stamp counter for myECAP0.
	//
	ECAP_startCounter(myECAP0_BASE);
}
void myECAP1_init(){
	//
	// Stops Time stamp counter.
	//
	ECAP_stopCounter(myECAP1_BASE);
	//
	// Sets eCAP in APWM mode.
	//
	ECAP_enableAPWMMode(myECAP1_BASE);
	//
	// Set eCAP APWM period.
	//
	ECAP_setAPWMPeriod(myECAP1_BASE,10000U);
	//
	// Set eCAP APWM on or off time count.
	//
	ECAP_setAPWMCompare(myECAP1_BASE,5000U);
	//
	// Set eCAP APWM polarity.
	//
	ECAP_setAPWMPolarity(myECAP1_BASE,ECAP_APWM_ACTIVE_LOW);
	//
	// Sets a phase shift value count.
	//
	ECAP_setPhaseShiftCount(myECAP1_BASE,3000U);
	//
	// Enable counter loading with phase shift value.
	//
	ECAP_enableLoadCounter(myECAP1_BASE);
	//
	// Configures Sync out signal mode.
	//
	ECAP_setSyncOutMode(myECAP1_BASE,ECAP_SYNC_OUT_SYNCI);
	//
	// Configures emulation mode.
	//
	ECAP_setEmulationMode(myECAP1_BASE,ECAP_EMULATION_STOP);
	//
	// Starts Time stamp counter for myECAP1.
	//
	ECAP_startCounter(myECAP1_BASE);
}

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void OUTPUTXBAR_init(){
	myOUTPUTXBAR0_init();
	myOUTPUTXBAR1_init();
}

void myOUTPUTXBAR0_init(){
	XBAR_setOutputLatchMode(myOUTPUTXBAR0, false);
	XBAR_invertOutputSignal(myOUTPUTXBAR0, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(myOUTPUTXBAR0, XBAR_OUT_MUX00_ECAP1_OUT);
	XBAR_enableOutputMux(myOUTPUTXBAR0, XBAR_MUX00);
}
void myOUTPUTXBAR1_init(){
	XBAR_setOutputLatchMode(myOUTPUTXBAR1, false);
	XBAR_invertOutputSignal(myOUTPUTXBAR1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(myOUTPUTXBAR1, XBAR_OUT_MUX02_ECAP2_OUT);
	XBAR_enableOutputMux(myOUTPUTXBAR1, XBAR_MUX02);
}

//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init(){
	SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
	//
	// For EPWM1, the sync input is: SYSCTL_SYNC_IN_SRC_EXTSYNCIN1
	//
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM7, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP1, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP6, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	SysCtl_enableExtADCSOCSource(0);
}
