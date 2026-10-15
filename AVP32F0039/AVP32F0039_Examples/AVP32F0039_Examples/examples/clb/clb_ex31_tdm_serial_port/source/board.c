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
	OUTPUTXBAR_init();
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
	
	//
	// CLB_OUTPUTXBAR6 -> CLB_FSYNC_OUT Pinmux
	//
	GPIO_setPinConfig(CLB_FSYNC_OUT_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// CLB_OUTPUTXBAR8 -> CLB_DATA1_OUT Pinmux
	//
	GPIO_setPinConfig(CLB_DATA1_OUT_CLBOUTPUTXBAR_PIN_CONFIG);
	// GPIO1 -> CLB_BCLK_IN Pinmux
	GPIO_setPinConfig(GPIO_1_GPIO1);
	// GPIO0 -> CLB_FSYNC_IN Pinmux
	GPIO_setPinConfig(GPIO_0_GPIO0);
	// GPIO2 -> CLB_DATA1_IN Pinmux
	GPIO_setPinConfig(GPIO_2_GPIO2);
	//
	// OUTPUTXBAR3 -> CLB_BCLK_OUT Pinmux
	//
	GPIO_setPinConfig(CLB_BCLK_OUT_OUTPUTXBAR_PIN_CONFIG);

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
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN0, CLB_FILTER_FALLING_EDGE);
	CLB_disableInputPipelineMode(myCLBForTILE1_BASE, CLB_IN0);
	//
	// myCLBForTILE1 CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_INPUT2);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN1, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN1);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(myCLBForTILE1_BASE, CLB_IN1);
	//
	// myCLBForTILE1 CLB_IN2 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN2, CLB_LOCAL_IN_MUX_INPUT3);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN2, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN2, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN2);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN2, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(myCLBForTILE1_BASE, CLB_IN2);
	//
	// myCLBForTILE1 CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_INPUT1);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN3, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN3);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN3, CLB_FILTER_RISING_EDGE);
	CLB_disableInputPipelineMode(myCLBForTILE1_BASE, CLB_IN3);
	//
	// myCLBForTILE1 CLB_IN5 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(myCLBForTILE1_BASE, CLB_IN5, CLB_LOCAL_IN_MUX_INPUT1);
	CLB_configGlobalInputMux(myCLBForTILE1_BASE, CLB_IN5, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(myCLBForTILE1_BASE, CLB_IN5, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(myCLBForTILE1_BASE, CLB_IN5);
	CLB_selectInputFilter(myCLBForTILE1_BASE, CLB_IN5, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(myCLBForTILE1_BASE, CLB_IN5);
	CLB_setGPREG(myCLBForTILE1_BASE,0);

	CLB_disableCLB(myCLBForTILE1_BASE);
}

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
void CLB_INPUTXBAR_init(){
	CLBINPUTXBAR_BCLK_IN_init();
	CLBINPUTXBAR_FSYNC_IN_init();
	CLBINPUTXBAR_DATA1_IN_init();
}

void CLBINPUTXBAR_BCLK_IN_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, CLBINPUTXBAR_BCLK_IN_INPUT, CLBINPUTXBAR_BCLK_IN_SOURCE);
}
void CLBINPUTXBAR_FSYNC_IN_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, CLBINPUTXBAR_FSYNC_IN_INPUT, CLBINPUTXBAR_FSYNC_IN_SOURCE);
}
void CLBINPUTXBAR_DATA1_IN_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, CLBINPUTXBAR_DATA1_IN_INPUT, CLBINPUTXBAR_DATA1_IN_SOURCE);
}

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_OUTPUTXBAR_init(){
	CLB_FSYNC_OUT_init();
	CLB_DATA1_OUT_init();
}

	
void CLB_FSYNC_OUT_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_FSYNC_OUT, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_FSYNC_OUT, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_FSYNC_OUT, XBAR_OUT_MUX04_CLB1_OUT4);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_FSYNC_OUT, XBAR_MUX04);
}
	
void CLB_DATA1_OUT_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_DATA1_OUT, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_DATA1_OUT, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_DATA1_OUT, XBAR_OUT_MUX00_CLB1_OUT0);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_DATA1_OUT, XBAR_MUX00);
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	CLB_BCLK_IN_init();
	CLB_FSYNC_IN_init();
	CLB_DATA1_IN_init();
}

void CLB_BCLK_IN_init(){
	GPIO_setPadConfig(CLB_BCLK_IN, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP | GPIO_PIN_TYPE_INVERT);
	GPIO_setQualificationMode(CLB_BCLK_IN, GPIO_QUAL_ASYNC);
	GPIO_setDirectionMode(CLB_BCLK_IN, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(CLB_BCLK_IN, GPIO_CORE_CPU1);
}
void CLB_FSYNC_IN_init(){
	GPIO_setPadConfig(CLB_FSYNC_IN, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(CLB_FSYNC_IN, GPIO_QUAL_ASYNC);
	GPIO_setDirectionMode(CLB_FSYNC_IN, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(CLB_FSYNC_IN, GPIO_CORE_CPU1);
}
void CLB_DATA1_IN_init(){
	GPIO_setPadConfig(CLB_DATA1_IN, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(CLB_DATA1_IN, GPIO_QUAL_ASYNC);
	GPIO_setDirectionMode(CLB_DATA1_IN, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(CLB_DATA1_IN, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_myCLBForTILE1
	Interrupt_register(INT_myCLBForTILE1, &clbISR);
	Interrupt_disable(INT_myCLBForTILE1);
}
//*****************************************************************************
//
// OUTPUTXBAR Configurations
//
//*****************************************************************************
void OUTPUTXBAR_init(){
	CLB_BCLK_OUT_init();
}

void CLB_BCLK_OUT_init(){
	XBAR_setOutputLatchMode(OUTPUTXBAR_BASE, CLB_BCLK_OUT, false);
	XBAR_invertOutputSignal(OUTPUTXBAR_BASE, CLB_BCLK_OUT, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(OUTPUTXBAR_BASE, CLB_BCLK_OUT, XBAR_OUT_MUX03_CLB1_OUT5);
	XBAR_enableOutputMux(OUTPUTXBAR_BASE, CLB_BCLK_OUT, XBAR_MUX03);
}

