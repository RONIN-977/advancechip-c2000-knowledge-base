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
	
	// GPIO0 -> myGPIO0 Pinmux
	GPIO_setPinConfig(GPIO_0_GPIO0);
	// GPIO1 -> myGPIO1 Pinmux
	GPIO_setPinConfig(GPIO_1_GPIO1);
	//
	// OUTPUTXBAR1 -> myOUTPUTXBAR1 Pinmux
	//
	GPIO_setPinConfig(myOUTPUTXBAR1_OUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
void CLB_init(){
	myCLBTILE1_init();
}

void myCLBTILE1_init(){
	CLB_setOutputMask(myCLBTILE1_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(myCLBTILE1_BASE);
	//
	// myCLBTILE1 SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(myCLBTILE1_BASE);
	CLB_configSPIBufferLoadSignal(myCLBTILE1_BASE, 0);
	CLB_configSPIBufferShift(myCLBTILE1_BASE, 0);
	//
	// myCLBTILE1 CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBTILE1_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(myCLBTILE1_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_CLB_AUXSIG0);
	CLB_configGPInputMux(myCLBTILE1_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBTILE1_BASE, CLB_IN0);
	CLB_selectInputFilter(myCLBTILE1_BASE, CLB_IN0, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(myCLBTILE1_BASE, CLB_IN0);
	//
	// myCLBTILE1 CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBTILE1_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(myCLBTILE1_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_CLB_AUXSIG1);
	CLB_configGPInputMux(myCLBTILE1_BASE, CLB_IN1, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBTILE1_BASE, CLB_IN1);
	CLB_selectInputFilter(myCLBTILE1_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(myCLBTILE1_BASE, CLB_IN1);
	CLB_setGPREG(myCLBTILE1_BASE,0);

	CLB_disableCLB(myCLBTILE1_BASE);
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
	myGPIO0_init();
	myGPIO1_init();
}

void myGPIO0_init(){
	GPIO_setPadConfig(myGPIO0, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myGPIO0, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO0, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO0, GPIO_CORE_CPU1);
}
void myGPIO1_init(){
	GPIO_setPadConfig(myGPIO1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myGPIO1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO1, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO1, GPIO_CORE_CPU1);
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
	XBAR_setInputPin(INPUTXBAR_BASE, myINPUTXBARINPUT0_INPUT, myINPUTXBARINPUT0_SOURCE);
}
void myINPUTXBARINPUT1_init(){
	XBAR_setInputPin(INPUTXBAR_BASE, myINPUTXBARINPUT1_INPUT, myINPUTXBARINPUT1_SOURCE);
}

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void OUTPUTXBAR_init(){
	myOUTPUTXBAR1_init();
}

void myOUTPUTXBAR1_init(){
	XBAR_setOutputLatchMode(OUTPUTXBAR_BASE, myOUTPUTXBAR1, false);
	XBAR_invertOutputSignal(OUTPUTXBAR_BASE, myOUTPUTXBAR1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(OUTPUTXBAR_BASE, myOUTPUTXBAR1, XBAR_OUT_MUX01_CLB1_OUT4);
	XBAR_enableOutputMux(OUTPUTXBAR_BASE, myOUTPUTXBAR1, XBAR_MUX01);
}

