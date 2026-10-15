/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  SPI Digital Loopback with FIFO Interrupts
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
	// SPIA -> SPIA_peripheral Pinmux
	//
	GPIO_setPinConfig(SPIA_peripheral_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_peripheral_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_peripheral_SPIPICO_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIA_peripheral_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_peripheral_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_peripheral_SPIPOCI_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIA_peripheral_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_peripheral_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_peripheral_SPICLK_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIA_peripheral_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(SPIA_peripheral_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIA_peripheral_SPIPTE_GPIO, GPIO_QUAL_ASYNC);

	//
	// SPIB -> SPIB_controller Pinmux
	//
	GPIO_setPinConfig(SPIB_controller_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(SPIB_controller_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIB_controller_SPIPICO_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIB_controller_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(SPIB_controller_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIB_controller_SPIPOCI_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIB_controller_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(SPIB_controller_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIB_controller_SPICLK_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(SPIB_controller_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(SPIB_controller_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(SPIB_controller_SPIPTE_GPIO, GPIO_QUAL_ASYNC);


}

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	SPIA_peripheral_init();
	SPIB_controller_init();
}

void SPIA_peripheral_init(){
	SPI_disableModule(SPIA_peripheral_BASE);
	SPI_setConfig(SPIA_peripheral_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
				  SPI_MODE_PERIPHERAL, 500000, 16);
	SPI_setPTESignalPolarity(SPIA_peripheral_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(SPIA_peripheral_BASE);
	SPI_setFIFOInterruptLevel(SPIA_peripheral_BASE, SPI_FIFO_TX2, SPI_FIFO_RX2);
	SPI_clearInterruptStatus(SPIA_peripheral_BASE, SPI_INT_RXFF);
	SPI_enableInterrupt(SPIA_peripheral_BASE, SPI_INT_RXFF);
	SPI_disableLoopback(SPIA_peripheral_BASE);
	SPI_setEmulationMode(SPIA_peripheral_BASE, SPI_EMULATION_FREE_RUN);
	SPI_enableModule(SPIA_peripheral_BASE);
}
void SPIB_controller_init(){
	SPI_disableModule(SPIB_controller_BASE);
	SPI_setConfig(SPIB_controller_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
				  SPI_MODE_CONTROLLER, 500000, 16);
	SPI_setPTESignalPolarity(SPIB_controller_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(SPIB_controller_BASE);
	SPI_setFIFOInterruptLevel(SPIB_controller_BASE, SPI_FIFO_TX2, SPI_FIFO_RX2);
	SPI_clearInterruptStatus(SPIB_controller_BASE, SPI_INT_TXFF);
	SPI_enableInterrupt(SPIB_controller_BASE, SPI_INT_TXFF);
	SPI_disableLoopback(SPIB_controller_BASE);
	SPI_setEmulationMode(SPIB_controller_BASE, SPI_EMULATION_FREE_RUN);
	SPI_enableModule(SPIB_controller_BASE);
}

