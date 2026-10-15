/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                              湖南进芯电子科技有限公司
*                           http://www.advancechip.com/
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
	INPUTXBAR_init();
	CLB_init();
	CLBXBAR_init();
	GPIO_init();
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
	
	// GPIO1 -> myGPIO1Input Pinmux
	GPIO_setPinConfig(GPIO_1_GPIO1);
	// GPIO0 -> myGPIO0Input Pinmux
	GPIO_setPinConfig(GPIO_0_GPIO0);
	//
	// OUTPUTXBAR1 -> myOUTPUTXBAR0 Pinmux
	//
	GPIO_setPinConfig(myOUTPUTXBAR0_OUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
void CLB_init(){
	myCLBForTILE1_init();
}

void myCLBForTILE1_init(){
	CLB_setOutputMask(myCLBForTILE1_BASE,
				(0UL << 0UL), true);
	//
	// myCLBForTILE1 CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_CLB_AUXSIG0);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN0);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN0, CLB_FILTER_NONE);
	//
	// myCLBForTILE1 CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_CLB_AUXSIG1);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN1, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN1);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_setGPREG(myCLBForTILE1_BASE,0);

	CLB_disableCLB(myCLBForTILE1_BASE);
}

//*****************************************************************************
//
// CLBXBAR Configurations
//
//*****************************************************************************
void CLBXBAR_init(){
	myCLBXBAR0_init();
	myCLBXBAR1_init();
}

void myCLBXBAR0_init(){
		
	XBAR_setCLBMuxConfig(myCLBXBAR0, XBAR_CLB_MUX01_INPUTXBAR1);
	XBAR_enableCLBMux(myCLBXBAR0, myCLBXBAR0_ENABLED_MUXES);
}
void myCLBXBAR1_init(){
		
	XBAR_setCLBMuxConfig(myCLBXBAR1, XBAR_CLB_MUX03_INPUTXBAR2);
	XBAR_enableCLBMux(myCLBXBAR1, myCLBXBAR1_ENABLED_MUXES);
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	myGPIO1Input_init();
	myGPIO0Input_init();
}

void myGPIO1Input_init(){
	GPIO_setPadConfig(myGPIO1Input, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(myGPIO1Input, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO1Input, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO1Input, GPIO_CORE_CPU1);
}
void myGPIO0Input_init(){
	GPIO_setPadConfig(myGPIO0Input, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(myGPIO0Input, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO0Input, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO0Input, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
void INPUTXBAR_init(){
	myINPUTXBARINPUT0_init();
	myINPUTXBARINPUT1_init();
}

void myINPUTXBARINPUT0_init(){
	XBAR_setInputPin(myINPUTXBARINPUT0_INPUT, myINPUTXBARINPUT0_SOURCE);
}
void myINPUTXBARINPUT1_init(){
	XBAR_setInputPin(myINPUTXBARINPUT1_INPUT, myINPUTXBARINPUT1_SOURCE);
}

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void OUTPUTXBAR_init(){
	myOUTPUTXBAR0_init();
}

void myOUTPUTXBAR0_init(){
	XBAR_setOutputLatchMode(myOUTPUTXBAR0, false);
	XBAR_invertOutputSignal(myOUTPUTXBAR0, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(myOUTPUTXBAR0, XBAR_OUT_MUX01_CLB1_OUT4);
	XBAR_enableOutputMux(myOUTPUTXBAR0, XBAR_MUX01);
}

