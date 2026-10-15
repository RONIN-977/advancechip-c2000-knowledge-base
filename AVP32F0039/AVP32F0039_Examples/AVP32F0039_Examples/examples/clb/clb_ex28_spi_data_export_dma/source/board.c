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
	
	//
	// SPIA -> mySPIForTILE1 Pinmux
	//
	GPIO_setPinConfig(mySPIForTILE1_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(mySPIForTILE1_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPIForTILE1_SPIPICO_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySPIForTILE1_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(mySPIForTILE1_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPIForTILE1_SPIPOCI_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySPIForTILE1_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(mySPIForTILE1_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPIForTILE1_SPICLK_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySPIForTILE1_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(mySPIForTILE1_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPIForTILE1_SPIPTE_GPIO, GPIO_QUAL_ASYNC);


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
	CLB_enableSPIBufferAccess(myCLBForTILE1_BASE);
	CLB_configSPIBufferLoadSignal(myCLBForTILE1_BASE, 3);
	CLB_configSPIBufferShift(myCLBForTILE1_BASE, 0);
	CLB_setGPREG(myCLBForTILE1_BASE,0);

	CLB_disableCLB(myCLBForTILE1_BASE);
}

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	mySPIForTILE1_init();
}

void mySPIForTILE1_init(){
	SPI_disableModule(mySPIForTILE1_BASE);
	SPI_setConfig(mySPIForTILE1_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
				  SPI_MODE_PERIPHERAL, 1000000, 16);
	SPI_setPTESignalPolarity(mySPIForTILE1_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(mySPIForTILE1_BASE);
	SPI_setFIFOInterruptLevel(mySPIForTILE1_BASE, SPI_FIFO_TXEMPTY, SPI_FIFO_RX8);
	SPI_disableLoopback(mySPIForTILE1_BASE);
	SPI_setEmulationMode(mySPIForTILE1_BASE, SPI_EMULATION_STOP_AFTER_TRANSMIT);
	SPI_enableModule(mySPIForTILE1_BASE);
}

