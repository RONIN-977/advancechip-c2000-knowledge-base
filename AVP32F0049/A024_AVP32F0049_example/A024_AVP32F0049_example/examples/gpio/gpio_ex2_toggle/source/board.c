/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  Device GPIO Toggle
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
	
	// GPIO28 -> myGPIOOutput0 Pinmux
	GPIO_setPinConfig(GPIO_28_GPIO28);

}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	myGPIOOutput0_init();
}

void myGPIOOutput0_init(){
	GPIO_setPadConfig(myGPIOOutput0, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myGPIOOutput0, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIOOutput0, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(myGPIOOutput0, GPIO_CORE_CPU1);
}

