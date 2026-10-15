/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  Software Sync.
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
	SYNC_init();
	ECAP_init();
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
	
	// GPIO16 -> myGPIO0 Pinmux
	GPIO_setPinConfig(GPIO_16_GPIO16);

}

//*****************************************************************************
//
// ECAP Configurations
//
//*****************************************************************************
void ECAP_init(){
	myECAP1_init();
	myECAP2_init();
	myECAP3_init();
}

void myECAP1_init(){
	//
	// Disables time stamp capture.
	//
	ECAP_disableTimeStampCapture(myECAP1_BASE);
	//
	// Stops Time stamp counter.
	//
	ECAP_stopCounter(myECAP1_BASE);
	//
	// Sets eCAP in Capture mode.
	//
	ECAP_enableCaptureMode(myECAP1_BASE);
	//
	// Sets the capture mode.
	//
	ECAP_setCaptureMode(myECAP1_BASE,ECAP_ONE_SHOT_CAPTURE_MODE,ECAP_EVENT_4);
	//
	// Sets the Capture event prescaler.
	//
	ECAP_setEventPrescaler(myECAP1_BASE, 0U);
	//
	// Sets the Capture event polarity.
	//
	ECAP_setEventPolarity(myECAP1_BASE,ECAP_EVENT_1,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP1_BASE,ECAP_EVENT_2,ECAP_EVNT_RISING_EDGE);
	ECAP_setEventPolarity(myECAP1_BASE,ECAP_EVENT_3,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP1_BASE,ECAP_EVENT_4,ECAP_EVNT_RISING_EDGE);
	//
	// Configure counter reset on events
	//
	ECAP_enableCounterResetOnEvent(myECAP1_BASE,ECAP_EVENT_1);	
	ECAP_enableCounterResetOnEvent(myECAP1_BASE,ECAP_EVENT_2);	
	ECAP_enableCounterResetOnEvent(myECAP1_BASE,ECAP_EVENT_3);	
	ECAP_enableCounterResetOnEvent(myECAP1_BASE,ECAP_EVENT_4);	
	//
	// Select eCAP input.
	//
	ECAP_selectECAPInput(myECAP1_BASE,ECAP_INPUT_INPUTXBAR7);
	//
	// Sets a phase shift value count.
	//
	ECAP_setPhaseShiftCount(myECAP1_BASE,0U);
	//
	// Enable counter loading with phase shift value.
	//
	ECAP_enableLoadCounter(myECAP1_BASE);
	//
	// Load time stamp counter.
	//
	ECAP_loadCounter(myECAP1_BASE);
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
	//
	// Enables time stamp capture for myECAP1.
	//
	ECAP_enableTimeStampCapture(myECAP1_BASE);
	//
	// Re-arms the eCAP module for myECAP1.
	//
	ECAP_reArm(myECAP1_BASE);
}
void myECAP2_init(){
	//
	// Disables time stamp capture.
	//
	ECAP_disableTimeStampCapture(myECAP2_BASE);
	//
	// Stops Time stamp counter.
	//
	ECAP_stopCounter(myECAP2_BASE);
	//
	// Sets eCAP in Capture mode.
	//
	ECAP_enableCaptureMode(myECAP2_BASE);
	//
	// Sets the capture mode.
	//
	ECAP_setCaptureMode(myECAP2_BASE,ECAP_ONE_SHOT_CAPTURE_MODE,ECAP_EVENT_4);
	//
	// Sets the Capture event prescaler.
	//
	ECAP_setEventPrescaler(myECAP2_BASE, 0U);
	//
	// Sets the Capture event polarity.
	//
	ECAP_setEventPolarity(myECAP2_BASE,ECAP_EVENT_1,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP2_BASE,ECAP_EVENT_2,ECAP_EVNT_RISING_EDGE);
	ECAP_setEventPolarity(myECAP2_BASE,ECAP_EVENT_3,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP2_BASE,ECAP_EVENT_4,ECAP_EVNT_RISING_EDGE);
	//
	// Configure counter reset on events
	//
	ECAP_enableCounterResetOnEvent(myECAP2_BASE,ECAP_EVENT_1);	
	ECAP_enableCounterResetOnEvent(myECAP2_BASE,ECAP_EVENT_2);	
	ECAP_enableCounterResetOnEvent(myECAP2_BASE,ECAP_EVENT_3);	
	ECAP_enableCounterResetOnEvent(myECAP2_BASE,ECAP_EVENT_4);	
	//
	// Select eCAP input.
	//
	ECAP_selectECAPInput(myECAP2_BASE,ECAP_INPUT_INPUTXBAR7);
	//
	// Sets a phase shift value count.
	//
	ECAP_setPhaseShiftCount(myECAP2_BASE,0U);
	//
	// Enable counter loading with phase shift value.
	//
	ECAP_enableLoadCounter(myECAP2_BASE);
	//
	// Configures Sync out signal mode.
	//
	ECAP_setSyncOutMode(myECAP2_BASE,ECAP_SYNC_OUT_SYNCI);
	//
	// Configures emulation mode.
	//
	ECAP_setEmulationMode(myECAP2_BASE,ECAP_EMULATION_STOP);
	//
	// Starts Time stamp counter for myECAP2.
	//
	ECAP_startCounter(myECAP2_BASE);
	//
	// Enables time stamp capture for myECAP2.
	//
	ECAP_enableTimeStampCapture(myECAP2_BASE);
	//
	// Re-arms the eCAP module for myECAP2.
	//
	ECAP_reArm(myECAP2_BASE);
}
void myECAP3_init(){
	//
	// Disable ,clear all capture flags and interrupts
	//
	ECAP_disableInterrupt(myECAP3_BASE,
		(ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
		ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
		ECAP_ISR_SOURCE_COUNTER_PERIOD   |
		ECAP_ISR_SOURCE_COUNTER_COMPARE));
	ECAP_clearInterrupt(myECAP3_BASE,
		(ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
		ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
		ECAP_ISR_SOURCE_COUNTER_PERIOD   |
		ECAP_ISR_SOURCE_COUNTER_COMPARE));
	//
	// Disables time stamp capture.
	//
	ECAP_disableTimeStampCapture(myECAP3_BASE);
	//
	// Stops Time stamp counter.
	//
	ECAP_stopCounter(myECAP3_BASE);
	//
	// Sets eCAP in Capture mode.
	//
	ECAP_enableCaptureMode(myECAP3_BASE);
	//
	// Sets the capture mode.
	//
	ECAP_setCaptureMode(myECAP3_BASE,ECAP_ONE_SHOT_CAPTURE_MODE,ECAP_EVENT_4);
	//
	// Sets the Capture event prescaler.
	//
	ECAP_setEventPrescaler(myECAP3_BASE, 0U);
	//
	// Sets the Capture event polarity.
	//
	ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_1,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_2,ECAP_EVNT_RISING_EDGE);
	ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_3,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP3_BASE,ECAP_EVENT_4,ECAP_EVNT_RISING_EDGE);
	//
	// Configure counter reset on events
	//
	ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_1);	
	ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_2);	
	ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_3);	
	ECAP_enableCounterResetOnEvent(myECAP3_BASE,ECAP_EVENT_4);	
	//
	// Select eCAP input.
	//
	ECAP_selectECAPInput(myECAP3_BASE,ECAP_INPUT_INPUTXBAR7);
	//
	// Sets a phase shift value count.
	//
	ECAP_setPhaseShiftCount(myECAP3_BASE,0U);
	//
	// Enable counter loading with phase shift value.
	//
	ECAP_enableLoadCounter(myECAP3_BASE);
	//
	// Configures Sync out signal mode.
	//
	ECAP_setSyncOutMode(myECAP3_BASE,ECAP_SYNC_OUT_SYNCI);
	//
	// Configures emulation mode.
	//
	ECAP_setEmulationMode(myECAP3_BASE,ECAP_EMULATION_STOP);
	//
	// Starts Time stamp counter for myECAP3.
	//
	ECAP_startCounter(myECAP3_BASE);
	//
	// Enables time stamp capture for myECAP3.
	//
	ECAP_enableTimeStampCapture(myECAP3_BASE);
	//
	// Re-arms the eCAP module for myECAP3.
	//
	ECAP_reArm(myECAP3_BASE);
	//
	// Enables interrupt source for myECAP3.
	//
	ECAP_enableInterrupt(myECAP3_BASE,(ECAP_ISR_SOURCE_CAPTURE_EVENT_4));
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
	GPIO_setPadConfig(myGPIO0, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myGPIO0, GPIO_QUAL_ASYNC);
	GPIO_setDirectionMode(myGPIO0, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO0, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
void INPUTXBAR_init(){
	myINPUTXBARINPUT0_init();
}

void myINPUTXBARINPUT0_init(){
	XBAR_setInputPin(myINPUTXBARINPUT0_INPUT, myINPUTXBARINPUT0_SOURCE);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_myECAP3
	Interrupt_register(INT_myECAP3, &INT_myECAP3_ISR);
	Interrupt_enable(INT_myECAP3);
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
