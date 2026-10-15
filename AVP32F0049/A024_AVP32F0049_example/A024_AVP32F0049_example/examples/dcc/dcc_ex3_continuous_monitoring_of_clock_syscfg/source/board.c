/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  DCC Continuous clock monitoring with sysconfig
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
	DCC_init();
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
// DCC Configurations
//
//*****************************************************************************
void DCC_init(){
	myDCC0_init();
}

void myDCC0_init(){
	DCC_disableModule(myDCC0_BASE);
	DCC_disableErrorSignal(myDCC0_BASE);
	DCC_disableDoneSignal(myDCC0_BASE);
	DCC_setCounter0ClkSource(myDCC0_BASE, DCC_COUNT0SRC_INTOSC2);
	DCC_setCounter1ClkSource(myDCC0_BASE, DCC_COUNT1SRC_PLL);
	DCC_setCounterSeeds(myDCC0_BASE, 2880, 240, 30000);	
	DCC_disableSingleShotMode(myDCC0_BASE);
	DCC_enableErrorSignal(myDCC0_BASE);
	//
	// This DCC is not started in this function.
	// The application SW must call the DCC_enableModule when needed.
	// DCC_enableModule(myDCC0_BASE);
	//
}	

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_myDCC0
	Interrupt_register(INT_myDCC0, &INT_myDCC0_ISR);
	Interrupt_enable(INT_myDCC0);
}
