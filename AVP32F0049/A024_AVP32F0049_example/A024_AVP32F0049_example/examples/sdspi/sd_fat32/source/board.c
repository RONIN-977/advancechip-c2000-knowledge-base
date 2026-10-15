/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  SD FATFS Library Example.
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
	SPI_init();

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
	
	// GPIO28 -> mySDCardCS Pinmux
	GPIO_setPinConfig(GPIO_28_GPIO28);
	//
	// SPIB -> mySDCardSPI Pinmux
	//

}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	mySDCardCS_init();
}

void mySDCardCS_init(){
	GPIO_writePin(mySDCardCS, 1);
	GPIO_setPadConfig(mySDCardCS, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySDCardCS, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(mySDCardCS, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(mySDCardCS, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	mySDCardSPI_init();
}

void mySDCardSPI_init(){
	SPI_disableModule(mySDCardSPI_BASE);
	SPI_setConfig(mySDCardSPI_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA1,
				  SPI_MODE_CONTROLLER, 1000000, 8);
	SPI_setPTESignalPolarity(mySDCardSPI_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(mySDCardSPI_BASE);
	SPI_disableLoopback(mySDCardSPI_BASE);
	SPI_setEmulationMode(mySDCardSPI_BASE, SPI_EMULATION_STOP_AFTER_TRANSMIT);
	SPI_enableModule(mySDCardSPI_BASE);
}

