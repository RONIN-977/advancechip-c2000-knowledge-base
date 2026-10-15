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
	CLB_INPUTXBAR_init();
	CLB_OUTPUTXBAR_init();
	GPIO_init();

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
	// CLB_OUTPUTXBAR2 -> myCLB_OUTPUTXBAR1 Pinmux
	//
	GPIO_setPinConfig(myCLB_OUTPUTXBAR1_CLBOUTPUTXBAR_PIN_CONFIG);
	// GPIO0 -> myGPIO0 Pinmux
	GPIO_setPinConfig(GPIO_0_GPIO0);

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
	//
	// myCLBForTILE1 CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_INPUT1);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN0);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN0, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(myCLBForTILE1_BASE, CLB_IN0);
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
	//
	// myCLBForTILE2 CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE2_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(myCLBForTILE2_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_CLB1_OUT16);
	CLB_configGPInputMux(myCLBForTILE2_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_selectInputFilter(myCLBForTILE2_BASE, CLB_IN0, CLB_FILTER_NONE);
	CLB_enableInputPipelineMode(myCLBForTILE2_BASE, CLB_IN0);
	CLB_setGPREG(myCLBForTILE2_BASE,0);

	CLB_disableCLB(myCLBForTILE2_BASE);
}

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
void CLB_INPUTXBAR_init(){
	myCLBINPUTXBARINPUT0_init();
}

void myCLBINPUTXBARINPUT0_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, myCLBINPUTXBARINPUT0_INPUT, myCLBINPUTXBARINPUT0_SOURCE);
}

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_OUTPUTXBAR_init(){
	myCLB_OUTPUTXBAR1_init();
}

	
void myCLB_OUTPUTXBAR1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR1, XBAR_OUT_MUX08_CLB2_OUT0);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, myCLB_OUTPUTXBAR1, XBAR_MUX08);
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	myGPIO0_init();
}

void myGPIO0_init(){
	GPIO_setPadConfig(myGPIO0, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(myGPIO0, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO0, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO0, GPIO_CORE_CPU1);
}

