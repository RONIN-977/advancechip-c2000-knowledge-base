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
	CLB_init();
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
	//
	// OUTPUTXBAR7 -> myOUTPUTXBAR7 Pinmux
	//
	GPIO_setPinConfig(myOUTPUTXBAR7_OUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
void CLB_init(){
	myTILE1_init();
}

void myTILE1_init(){
	CLB_setOutputMask(myTILE1_BASE,
				(0UL << 0UL), true);
	//
	// myTILE1 CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myTILE1_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(myTILE1_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(myTILE1_BASE, CLB_IN0, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(myTILE1_BASE, CLB_IN0, CLB_FILTER_NONE);
	CLB_setGPREG(myTILE1_BASE,0);

	CLB_enableCLB(myTILE1_BASE);
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
	GPIO_setDirectionMode(myGPIO0, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(myGPIO0, GPIO_CORE_CPU1);
}
void myGPIO1_init(){
	GPIO_setPadConfig(myGPIO1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myGPIO1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO1, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(myGPIO1, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void OUTPUTXBAR_init(){
	myOUTPUTXBAR1_init();
	myOUTPUTXBAR7_init();
}

void myOUTPUTXBAR1_init(){
	XBAR_setOutputLatchMode(myOUTPUTXBAR1, false);
	XBAR_invertOutputSignal(myOUTPUTXBAR1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(myOUTPUTXBAR1, XBAR_OUT_MUX01_CLB1_OUT4);
	XBAR_enableOutputMux(myOUTPUTXBAR1, XBAR_MUX01);
}
void myOUTPUTXBAR7_init(){
	XBAR_setOutputLatchMode(myOUTPUTXBAR7, false);
	XBAR_invertOutputSignal(myOUTPUTXBAR7, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(myOUTPUTXBAR7, XBAR_OUT_MUX03_CLB1_OUT5);
	XBAR_enableOutputMux(myOUTPUTXBAR7, XBAR_MUX03);
}

