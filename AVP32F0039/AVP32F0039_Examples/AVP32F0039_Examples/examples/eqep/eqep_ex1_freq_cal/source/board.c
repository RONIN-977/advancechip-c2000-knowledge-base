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
	EQEP_init();

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
	// EQEP1 -> myEQEP0 Pinmux
	//
	GPIO_setPinConfig(myEQEP0_EQEPA_PIN_CONFIG);
	GPIO_setPadConfig(myEQEP0_EQEPA_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myEQEP0_EQEPA_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(myEQEP0_EQEPB_PIN_CONFIG);
	// AGPIO -> GPIO mode selected
	// On 100PZ package for F28003x, if "B5, GPIO20" is used in peripheral signal digital mode, 
	// then "B5" can be used for ADC
	// On 100PZ package for F28003x, if "B11, GPIO21" is used in peripheral signal digital mode, 
	// then "B11" can be used for ADC
	GPIO_setAnalogMode(21, GPIO_ANALOG_DISABLED);
	GPIO_setPadConfig(myEQEP0_EQEPB_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myEQEP0_EQEPB_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(myEQEP0_EQEPSTROBE_PIN_CONFIG);
	GPIO_setPadConfig(myEQEP0_EQEPSTROBE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myEQEP0_EQEPSTROBE_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(myEQEP0_EQEPINDEX_PIN_CONFIG);
	GPIO_setPadConfig(myEQEP0_EQEPINDEX_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myEQEP0_EQEPINDEX_GPIO, GPIO_QUAL_SYNC);


}

//*****************************************************************************
//
// EQEP Configurations
//
//*****************************************************************************
void EQEP_init(){
	myEQEP0_init();
}

void myEQEP0_init(){
	EQEP_SourceSelect source_myEQEP0 =
	{
		EQEP_SOURCE_DEVICE_PIN, 		// eQEPA source
		EQEP_SOURCE_DEVICE_PIN,		// eQEPB source
		EQEP_SOURCE_DEVICE_PIN,  	// eQEP Index source 
	};
	//
	// Selects the source for eQEPA/B/I signals
	//
	EQEP_selectSource(myEQEP0_BASE, source_myEQEP0);
	//
	// Set the strobe input source of the eQEP module.
	//
	EQEP_setStrobeSource(myEQEP0_BASE,EQEP_STROBE_FROM_GPIO);
	//
	// Sets the polarity of the eQEP module's input signals.
	//
	EQEP_setInputPolarity(myEQEP0_BASE,false,false,false,false);
	//
	// Configures eQEP module's quadrature decoder unit.
	//
	EQEP_setDecoderConfig(myEQEP0_BASE, (EQEP_CONFIG_UP_COUNT | EQEP_CONFIG_2X_RESOLUTION | EQEP_CONFIG_NO_SWAP | EQEP_CONFIG_IGATE_DISABLE));
	//
	// Set the emulation mode of the eQEP module.
	//
	EQEP_setEmulationMode(myEQEP0_BASE,EQEP_EMULATIONMODE_RUNFREE);
	//
	// Configures eQEP module position counter unit.
	//
	EQEP_setPositionCounterConfig(myEQEP0_BASE,EQEP_POSITION_RESET_1ST_IDX,4294967295U);
	//
	// Sets the current encoder position.
	//
	EQEP_setPosition(myEQEP0_BASE,0U);
	//
	// Enables the eQEP module unit timer.
	//
	EQEP_enableUnitTimer(myEQEP0_BASE,1200000U);
	//
	// Disables the eQEP module watchdog timer.
	//
	EQEP_disableWatchdog(myEQEP0_BASE);
	//
	// Configures the quadrature modes in which the position count can be latched.
	//
	EQEP_setLatchMode(myEQEP0_BASE,(EQEP_LATCH_UNIT_TIME_OUT|EQEP_LATCH_RISING_STROBE|EQEP_LATCH_RISING_INDEX));
	//
	// Set the quadrature mode adapter (QMA) module mode.
	//
	EQEP_setQMAModuleMode(myEQEP0_BASE,EQEP_QMA_MODE_BYPASS);
	//
	// Disable Direction Change During Index
	//
	EQEP_disableDirectionChangeDuringIndex(myEQEP0_BASE);
	//
	// Configures the mode in which the position counter is initialized.
	//
	EQEP_setPositionInitMode(myEQEP0_BASE,(EQEP_INIT_DO_NOTHING));
	//
	// Sets the software initialization of the encoder position counter.
	//
	EQEP_setSWPositionInit(myEQEP0_BASE,true);
	//
	// Sets the init value for the encoder position counter.
	//
	EQEP_setInitialPosition(myEQEP0_BASE,0U);
	//
	// Enables the eQEP module.
	//
	EQEP_enableModule(myEQEP0_BASE);
	//
	// Configures eQEP module edge-capture unit.
	//
	EQEP_setCaptureConfig(myEQEP0_BASE,EQEP_CAPTURE_CLK_DIV_128,EQEP_UNIT_POS_EVNT_DIV_8);
	//
	// Enables the eQEP module edge-capture unit.
	//
	EQEP_enableCapture(myEQEP0_BASE);
}

