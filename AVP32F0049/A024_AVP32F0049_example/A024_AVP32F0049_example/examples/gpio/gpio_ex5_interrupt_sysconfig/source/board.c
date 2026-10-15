/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  XINT/XBAR Example
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
	GPIO_init();
	OUTPUTXBAR_init();
	XINT_init();
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
	
	// GPIO4 -> myGPIO_INPUT Pinmux
	GPIO_setPinConfig(GPIO_4_GPIO4);
	//
	// OUTPUTXBAR3 -> myOUTPUTXBAR0 Pinmux
	//
	GPIO_setPinConfig(myOUTPUTXBAR0_OUTPUTXBAR_PIN_CONFIG);

}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	myGPIO_INPUT_init();
}

void myGPIO_INPUT_init(){
	GPIO_setPadConfig(myGPIO_INPUT, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(myGPIO_INPUT, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(myGPIO_INPUT, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(myGPIO_INPUT, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// INPUTXBAR Configurations
//
//*****************************************************************************
void INPUTXBAR_init(){
	myINPUTXBARINPUT1_init();
}

void myINPUTXBARINPUT1_init(){
	XBAR_setInputPin(myINPUTXBARINPUT1_INPUT, myINPUTXBARINPUT1_SOURCE);
	XBAR_lockInput(myINPUTXBARINPUT1_INPUT);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_myGPIO_INPUT_XINT
	Interrupt_register(INT_myGPIO_INPUT_XINT, &INT_myGPIO_INPUT_XINT_ISR);
	Interrupt_enable(INT_myGPIO_INPUT_XINT);
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
	XBAR_setOutputMuxConfig(myOUTPUTXBAR0, XBAR_OUT_MUX07_INPUTXBAR4);
	XBAR_enableOutputMux(myOUTPUTXBAR0, XBAR_MUX07);
}

//*****************************************************************************
//
// XINT Configurations
//
//*****************************************************************************
void XINT_init(){
	myGPIO_INPUT_XINT_init();
}

void myGPIO_INPUT_XINT_init(){
	GPIO_setInterruptType(myGPIO_INPUT_XINT, GPIO_INT_TYPE_FALLING_EDGE);
	GPIO_setInterruptPin(myGPIO_INPUT, myGPIO_INPUT_XINT);
	GPIO_enableInterrupt(myGPIO_INPUT_XINT);
}

