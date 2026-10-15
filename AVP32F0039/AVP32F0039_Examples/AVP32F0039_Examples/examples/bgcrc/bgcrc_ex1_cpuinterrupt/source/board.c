/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
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
	BGCRC_init();
	INTERRUPT_init();

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
	

}

//*****************************************************************************
//
// BGCRC Configurations
//
//*****************************************************************************
void BGCRC_init(){
	myBGCRC0_init();
}

void myBGCRC0_init(){
    BGCRC_enableWatchdog(myBGCRC0_BASE);
    //
    //config watchdog settings
    //
    BGCRC_setWatchdogWindow(myBGCRC0_BASE, myBGCRC0_WDMIN, myBGCRC0_WDMAX);
    //
    //run BGCRC_setConfig
    //
    BGCRC_setConfig(myBGCRC0_BASE, myBGCRC0_NMIMODE, myBGCRC0_EMUCTRL);
    //
    //run BGCRC_setRegion
    //
    BGCRC_setRegion(myBGCRC0_BASE, myBGCRC0_STARTADDR, BGCRC_SIZE_KBYTES(1), myBGCRC0_MODE);
    //
    //call BGCRC_setGoldenCRCValue
    //
    BGCRC_setGoldenCRCValue(myBGCRC0_BASE, myBGCRC0_GOLDENVALUE);
    //
    //call BGCRC_setSeedValue
    //
    BGCRC_setSeedValue(myBGCRC0_BASE, myBGCRC0_SEEDVALUE);
    //
    //handle interrupt flags
    //
	BGCRC_enableInterrupt(myBGCRC0_BASE, myBGCRC0_INTFLAGS);
    //
    //start BGCRC calculation
    //
    BGCRC_start(myBGCRC0_BASE);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Settings for INT_myBGCRC0
	Interrupt_register(INT_myBGCRC0, &bgcrcISR);
	Interrupt_enable(INT_myBGCRC0);
}
