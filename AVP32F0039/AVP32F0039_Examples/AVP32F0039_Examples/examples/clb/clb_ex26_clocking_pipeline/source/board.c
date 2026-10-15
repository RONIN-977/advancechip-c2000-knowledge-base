/******************************* Copyright(C) *********************************************
*                          AVP32F0039_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  Board Configurations.
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
	CLB_init();
	CLB_OUTPUTXBAR_init();

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
	// CLB_OUTPUTXBAR2 -> myCLB_OUTPUTXBAR0 Pinmux
	//
	GPIO_setPinConfig(myCLB_OUTPUTXBAR0_CLBOUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
void CLB_init(){
	myCLBForTILE1_init();
	myCLBForTILE2_init();
}

void myCLBForTILE1_init(){
	CLB_setOutputMask(myCLBForTILE1_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(myCLBForTILE1_BASE);
	//
	// myCLBForTILE1 SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(myCLBForTILE1_BASE);
	CLB_configSPIBufferLoadSignal(myCLBForTILE1_BASE, 0);
	CLB_configSPIBufferShift(myCLBForTILE1_BASE, 0);
	CLB_setGPREG(myCLBForTILE1_BASE,0);

	CLB_disableCLB(myCLBForTILE1_BASE);
}
void myCLBForTILE2_init(){
	CLB_setOutputMask(myCLBForTILE2_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(myCLBForTILE2_BASE);
	//
	// myCLBForTILE2 SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(myCLBForTILE2_BASE);
	CLB_configSPIBufferLoadSignal(myCLBForTILE2_BASE, 0);
	CLB_configSPIBufferShift(myCLBForTILE2_BASE, 0);
	CLB_setGPREG(myCLBForTILE2_BASE,0);

	CLB_disableCLB(myCLBForTILE2_BASE);
}

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_OUTPUTXBAR_init(){
	myCLB_OUTPUTXBAR0_init();
}

	
void myCLB_OUTPUTXBAR0_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR0, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR0, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR0, XBAR_OUT_MUX04_CLB1_OUT4);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR0, XBAR_MUX04);
}

