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
	SYNC_init();
	CLB_init();
	CLB_INPUTXBAR_init();
	CLB_OUTPUTXBAR_init();
	CLBXBAR_init();
	EPWM_init();
	GPIO_init();
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
	// CLB_OUTPUTXBAR7 -> CLB_SCLK_1 Pinmux
	//
	GPIO_setPinConfig(CLB_SCLK_1_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// CLB_OUTPUTXBAR8 -> CLB_SCLKX2_1 Pinmux
	//
	GPIO_setPinConfig(CLB_SCLKX2_1_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// CLB_OUTPUTXBAR5 -> CLB_SOUT_1 Pinmux
	//
	GPIO_setPinConfig(CLB_SOUT_1_CLBOUTPUTXBAR_PIN_CONFIG);
	//
	// EPWM1 -> EPWM_CLK Pinmux
	//
	GPIO_setPinConfig(EPWM_CLK_EPWMA_PIN_CONFIG);
	GPIO_setPadConfig(EPWM_CLK_EPWMA_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EPWM_CLK_EPWMA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(EPWM_CLK_EPWMB_PIN_CONFIG);
	GPIO_setPadConfig(EPWM_CLK_EPWMB_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EPWM_CLK_EPWMB_GPIO, GPIO_QUAL_SYNC);

	// GPIO17 -> CLB_SIN_1 Pinmux
	GPIO_setPinConfig(GPIO_17_GPIO17);

}

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
void CLB_init(){
	CCSI1_RX_TILE_init();
	CCSI1_TX_TILE_init();
}

void CCSI1_RX_TILE_init(){
	CLB_setOutputMask(CCSI1_RX_TILE_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(CCSI1_RX_TILE_BASE);
	//
	// CCSI1_RX_TILE SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(CCSI1_RX_TILE_BASE);
	CLB_configSPIBufferLoadSignal(CCSI1_RX_TILE_BASE, 0);
	CLB_configSPIBufferShift(CCSI1_RX_TILE_BASE, 0);
	//
	// CCSI1_RX_TILE CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_INPUT1);
	CLB_configGlobalInputMux(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_RX_TILE_BASE, CLB_IN1);
	CLB_selectInputFilter(CCSI1_RX_TILE_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_RX_TILE_BASE, CLB_IN1);
	//
	// CCSI1_RX_TILE CLB_IN2 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_LOCAL_IN_MUX_INPUT2);
	CLB_configGlobalInputMux(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_RX_TILE_BASE, CLB_IN2);
	CLB_selectInputFilter(CCSI1_RX_TILE_BASE, CLB_IN2, CLB_FILTER_RISING_EDGE);
	CLB_disableInputPipelineMode(CCSI1_RX_TILE_BASE, CLB_IN2);
	//
	// CCSI1_RX_TILE CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_GP_IN_MUX_GP_REG);
	CLB_enableSynchronization(CCSI1_RX_TILE_BASE, CLB_IN3);
	CLB_selectInputFilter(CCSI1_RX_TILE_BASE, CLB_IN3, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_RX_TILE_BASE, CLB_IN3);
	CLB_setGPREG(CCSI1_RX_TILE_BASE,0);

	CLB_disableCLB(CCSI1_RX_TILE_BASE);
}
void CCSI1_TX_TILE_init(){
	CLB_setOutputMask(CCSI1_TX_TILE_BASE,
				(0UL << 0UL), true);
	CLB_enableOutputMaskUpdates(CCSI1_TX_TILE_BASE);
	//
	// CCSI1_TX_TILE SPI Buffer Configuration
	//
	CLB_disableSPIBufferAccess(CCSI1_TX_TILE_BASE);
	CLB_configSPIBufferLoadSignal(CCSI1_TX_TILE_BASE, 0);
	CLB_configSPIBufferShift(CCSI1_TX_TILE_BASE, 0);
	//
	// CCSI1_TX_TILE CLB_IN0 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN0);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN0, CLB_FILTER_FALLING_EDGE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN0);
	//
	// CCSI1_TX_TILE CLB_IN1 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN1, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN1);
	//
	// CCSI1_TX_TILE CLB_IN3 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN3);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN3, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN3);
	//
	// CCSI1_TX_TILE CLB_IN4 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_GLOBAL_IN_MUX_EPWM1B);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN4);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN4, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN4);
	//
	// CCSI1_TX_TILE CLB_IN5 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_GLOBAL_IN_MUX_CLB_AUXSIG0);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_GP_IN_MUX_EXTERNAL);
	CLB_enableSynchronization(CCSI1_TX_TILE_BASE, CLB_IN5);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN5, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN5);
	//
	// CCSI1_TX_TILE CLB_IN6 initialization
	//
	// The following functions configure the CLB input mux and whether the inputs
	// have synchronization or pipeline enabled; check the device manual for more
	// information on when a signal needs to be synchronized or go through a
	// pipeline filter
	//
	CLB_configLocalInputMux(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_LOCAL_IN_MUX_GLOBAL_IN);
	CLB_configGlobalInputMux(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_GLOBAL_IN_MUX_EPWM1A);
	CLB_configGPInputMux(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_GP_IN_MUX_GP_REG);
	CLB_selectInputFilter(CCSI1_TX_TILE_BASE, CLB_IN6, CLB_FILTER_NONE);
	CLB_disableInputPipelineMode(CCSI1_TX_TILE_BASE, CLB_IN6);
	CLB_setGPREG(CCSI1_TX_TILE_BASE,0);

	CLB_disableCLB(CCSI1_TX_TILE_BASE);
}

//*****************************************************************************
//
// CLBINPUTXBAR Configurations
//
//*****************************************************************************
void CLB_INPUTXBAR_init(){
	CLBINPUTXBAR_CLB_SIN_1_init();
	CLBINPUTXBAR_CLB_SCLKX2_init();
}

void CLBINPUTXBAR_CLB_SIN_1_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, CLBINPUTXBAR_CLB_SIN_1_INPUT, CLBINPUTXBAR_CLB_SIN_1_SOURCE);
}
void CLBINPUTXBAR_CLB_SCLKX2_init(){
	XBAR_setInputPin(CLBINPUTXBAR_BASE, CLBINPUTXBAR_CLB_SCLKX2_INPUT, CLBINPUTXBAR_CLB_SCLKX2_SOURCE);
}

//*****************************************************************************
//
// CLBOUTPUTXBAR Configurations
//
//*****************************************************************************
void CLB_OUTPUTXBAR_init(){
	CLB_SCLK_1_init();
	CLB_SCLKX2_1_init();
	CLB_SOUT_1_init();
}

	
void CLB_SCLK_1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, XBAR_OUT_MUX09_CLB2_OUT1);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SCLK_1, XBAR_MUX09);
}
	
void CLB_SCLKX2_1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SCLKX2_1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SCLKX2_1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SCLKX2_1, XBAR_OUT_MUX08_CLB2_OUT0);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SCLKX2_1, XBAR_MUX08);
}
	
void CLB_SOUT_1_init(){
	XBAR_setOutputLatchMode(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, false);
	XBAR_invertOutputSignal(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, false);
		
	//
	//Mux configuration
	//
	XBAR_setOutputMuxConfig(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, XBAR_OUT_MUX12_CLB2_OUT4);
	XBAR_enableOutputMux(CLBOUTPUTXBAR_BASE, CLB_SOUT_1, XBAR_MUX12);
}

//*****************************************************************************
//
// CLBXBAR Configurations
//
//*****************************************************************************
void CLBXBAR_init(){
	myCLBXBAR_TRANSFER_START_SYNC_init();
}

void myCLBXBAR_TRANSFER_START_SYNC_init(){
		
	XBAR_setCLBMuxConfig(myCLBXBAR_TRANSFER_START_SYNC, XBAR_CLB_MUX07_CLB2_OUT5);
	XBAR_enableCLBMux(myCLBXBAR_TRANSFER_START_SYNC, myCLBXBAR_TRANSFER_START_SYNC_ENABLED_MUXES);
}

//*****************************************************************************
//
// EPWM Configurations
//
//*****************************************************************************
void EPWM_init(){
    EPWM_setClockPrescaler(EPWM_CLK_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_2);	
    EPWM_setTimeBasePeriod(EPWM_CLK_BASE, 0);	
    EPWM_setTimeBaseCounter(EPWM_CLK_BASE, 0);	
    EPWM_setTimeBaseCounterMode(EPWM_CLK_BASE, EPWM_COUNTER_MODE_STOP_FREEZE);	
    EPWM_disablePhaseShiftLoad(EPWM_CLK_BASE);	
    EPWM_setPhaseShift(EPWM_CLK_BASE, 0);	
    EPWM_setCounterCompareValue(EPWM_CLK_BASE, EPWM_COUNTER_COMPARE_A, 0);	
    EPWM_setCounterCompareShadowLoadMode(EPWM_CLK_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setCounterCompareValue(EPWM_CLK_BASE, EPWM_COUNTER_COMPARE_B, 0);	
    EPWM_setCounterCompareShadowLoadMode(EPWM_CLK_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(EPWM_CLK_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	
    EPWM_setRisingEdgeDelayCountShadowLoadMode(EPWM_CLK_BASE, EPWM_RED_LOAD_ON_CNTR_ZERO);	
    EPWM_setFallingEdgeDelayCountShadowLoadMode(EPWM_CLK_BASE, EPWM_FED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableRisingEdgeDelayCountShadowLoadMode(EPWM_CLK_BASE);	
    EPWM_disableFallingEdgeDelayCountShadowLoadMode(EPWM_CLK_BASE);	
}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	CLB_SIN_1_init();
}

void CLB_SIN_1_init(){
	GPIO_setPadConfig(CLB_SIN_1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(CLB_SIN_1, GPIO_QUAL_ASYNC);
	GPIO_setDirectionMode(CLB_SIN_1, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(CLB_SIN_1, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_CCSI1_RX_TILE
	Interrupt_register(INT_CCSI1_RX_TILE, &INT_CCSI1_RX_TILE_ISR);
	Interrupt_disable(INT_CCSI1_RX_TILE);
	
	// Interrupt Setings for INT_CCSI1_TX_TILE
	Interrupt_register(INT_CCSI1_TX_TILE, &INT_CCSI1_TX_TILE_ISR);
	Interrupt_disable(INT_CCSI1_TX_TILE);
}
//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init(){
	SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	SysCtl_enableExtADCSOCSource(0);
}
