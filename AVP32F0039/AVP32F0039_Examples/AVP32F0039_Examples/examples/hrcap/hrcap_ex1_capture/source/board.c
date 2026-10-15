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
	INPUTXBAR_init();
	ECAP_init();
	GPIO_init();
	OTHER_init();
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
	
	// GPIO2 -> myGPIO0 Pinmux
	GPIO_setPinConfig(GPIO_2_GPIO2);
	//
	// OTHER -> myOTHER0 Pinmux
	//
	GPIO_setPinConfig(myOTHER0_XCLKOUT_PIN_CONFIG);

}

//*****************************************************************************
//
// ECAP Configurations
//
//*****************************************************************************
void ECAP_init(){
	myECAP0_init();
}

void myECAP0_init(){
	//
	// HRCAP module configuration for myECAP0_HR_BASE (part of myECAP0_BASE).
	//
	// Enable High resolution clock
	//
	HRCAP_enableHighResolutionClock(myECAP0_HR_BASE);
	DEVICE_DELAY_US(1); // 1us delay as mentioned in HRCAP chapter of TRM
	//
	// Enable High Resolution module
	//
	HRCAP_enableHighResolution(myECAP0_HR_BASE);
	DEVICE_DELAY_US(1); // 1us delay as mentioned in HRCAP chapter of TRM
	//
	// Disable ,clear all capture flags and interrupts
	//
	ECAP_disableInterrupt(myECAP0_BASE,
		(ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
		ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
		ECAP_ISR_SOURCE_COUNTER_PERIOD   |
		ECAP_ISR_SOURCE_COUNTER_COMPARE));
	ECAP_clearInterrupt(myECAP0_BASE,
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
	ECAP_disableTimeStampCapture(myECAP0_BASE);
	//
	// Stops Time stamp counter.
	//
	ECAP_stopCounter(myECAP0_BASE);
	//
	// Sets eCAP in Capture mode.
	//
	ECAP_enableCaptureMode(myECAP0_BASE);
	//
	// Sets the capture mode.
	//
	ECAP_setCaptureMode(myECAP0_BASE,ECAP_ONE_SHOT_CAPTURE_MODE,ECAP_EVENT_4);
	//
	// Sets the Capture event prescaler.
	//
	ECAP_setEventPrescaler(myECAP0_BASE, 0U);
	//
	// Sets the Capture event polarity.
	//
	ECAP_setEventPolarity(myECAP0_BASE,ECAP_EVENT_1,ECAP_EVNT_RISING_EDGE);
	ECAP_setEventPolarity(myECAP0_BASE,ECAP_EVENT_2,ECAP_EVNT_FALLING_EDGE);
	ECAP_setEventPolarity(myECAP0_BASE,ECAP_EVENT_3,ECAP_EVNT_RISING_EDGE);
	ECAP_setEventPolarity(myECAP0_BASE,ECAP_EVENT_4,ECAP_EVNT_FALLING_EDGE);
	//
	// Configure counter reset on events
	//
	ECAP_disableCounterResetOnEvent(myECAP0_BASE,ECAP_EVENT_1);
	ECAP_disableCounterResetOnEvent(myECAP0_BASE,ECAP_EVENT_2);
	ECAP_disableCounterResetOnEvent(myECAP0_BASE,ECAP_EVENT_3);
	ECAP_disableCounterResetOnEvent(myECAP0_BASE,ECAP_EVENT_4);
	//
	// Select eCAP input.
	//
	ECAP_selectECAPInput(myECAP0_BASE,ECAP_INPUT_INPUTXBAR7);
	//
	// Sets a phase shift value count.
	//
	ECAP_setPhaseShiftCount(myECAP0_BASE,0U);
	//
	// Enable counter loading with phase shift value.
	//
	ECAP_enableLoadCounter(myECAP0_BASE);
	//
	// Configures Sync out signal mode.
	//
	ECAP_setSyncOutMode(myECAP0_BASE,ECAP_SYNC_OUT_SYNCI);
	//
	// Resets eCAP counters and flags.
	//
	ECAP_resetCounters(myECAP0_BASE);
	//
	// Configures emulation mode.
	//
	ECAP_setEmulationMode(myECAP0_BASE,ECAP_EMULATION_STOP);
	//
	// Set up the source for sync-in pulse..
	//
	ECAP_setSyncInPulseSource(myECAP0_BASE,ECAP_SYNC_IN_PULSE_SRC_DISABLE);
	//
	// Starts Time stamp counter for myECAP0.
	//
	ECAP_startCounter(myECAP0_BASE);
	//
	// Enables time stamp capture for myECAP0.
	//
	ECAP_enableTimeStampCapture(myECAP0_BASE);
	//
	// Re-arms the eCAP module for myECAP0.
	//
	ECAP_reArm(myECAP0_BASE);
	//
	// Enables interrupt source for myECAP0.
	//
	ECAP_enableInterrupt(myECAP0_BASE,(ECAP_ISR_SOURCE_CAPTURE_EVENT_4));
	//
	// myECAP0_HR_BASE Calibration Settings
	//
	// Set calibration period (register is in SYSCLK cycles), 1.6ms recommended
	//
	HWREG(myECAP0_HR_BASE + HRCAP_O_HRCALPRD) = (uint32_t) (120e9 * (1.6/1000));
	//
	// Configure in continuous calibration mode
	//
	HRCAP_setCalibrationMode(myECAP0_HR_BASE);
	//
	// Enable HRCAP calibration interrupts
	//
	HRCAP_enableCalibrationInterrupt(myECAP0_HR_BASE, (HRCAP_CALIBRATION_DONE
		 | HRCAP_CALIBRATION_PERIOD_OVERFLOW));
	//
	// Start HR calibration
	//
	HRCAP_startCalibration(myECAP0_HR_BASE);

    //-----------------Signal Monitoring--------------------//
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
	XBAR_setInputPin(INPUTXBAR_BASE, myINPUTXBARINPUT0_INPUT, myINPUTXBARINPUT0_SOURCE);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Settings for INT_myECAP0
	Interrupt_register(INT_myECAP0, &INT_myECAP0_ISR);
	Interrupt_enable(INT_myECAP0);
	
	// Interrupt Settings for INT_myECAP0_HR
	Interrupt_register(INT_myECAP0_HR, &INT_myECAP0_HR_ISR);
	Interrupt_enable(INT_myECAP0_HR);
}
//*****************************************************************************
//
// OTHER Configurations
//
//*****************************************************************************
void OTHER_init(){
	//myOTHER0 initialization
	    
}
