/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   board.c
*                         TITLE:  SPI Digital Loopback with DMA
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
	DMA_init();
	SPI_init();
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
	// SPIA -> mySPI0 Pinmux
	//
	GPIO_setPinConfig(mySPI0_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPIPICO_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySPI0_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPIPOCI_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySPI0_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPICLK_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(mySPI0_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPIPTE_GPIO, GPIO_QUAL_ASYNC);


}

//*****************************************************************************
//
// DMA Configurations
//
//*****************************************************************************
void DMA_init(){
    DMA_initController();
	mySPI0_RX_DMA_init();
	mySPI0_TX_DMA_init();
}

void mySPI0_RX_DMA_init(){
    DMA_setEmulationMode(DMA_EMULATION_STOP);
    DMA_configAddresses(mySPI0_RX_DMA_BASE, destAddr, mySPI0_RX_DMA_ADDRESS);
    DMA_configBurst(mySPI0_RX_DMA_BASE, 8U, 0, 1);
    DMA_configTransfer(mySPI0_RX_DMA_BASE, 16U, 0, 1);
    DMA_configWrap(mySPI0_RX_DMA_BASE, 65535U, 0, 65535U, 0);
    DMA_configMode(mySPI0_RX_DMA_BASE, mySPI0_RX_DMA_TRIGGER, DMA_CFG_ONESHOT_DISABLE | DMA_CFG_CONTINUOUS_DISABLE | DMA_CFG_SIZE_16BIT);
    DMA_setInterruptMode(mySPI0_RX_DMA_BASE, DMA_INT_AT_END);
    DMA_enableInterrupt(mySPI0_RX_DMA_BASE);
    DMA_disableOverrunInterrupt(mySPI0_RX_DMA_BASE);
    DMA_enableTrigger(mySPI0_RX_DMA_BASE);
    DMA_stopChannel(mySPI0_RX_DMA_BASE);
}
void mySPI0_TX_DMA_init(){
    DMA_setEmulationMode(DMA_EMULATION_STOP);
    DMA_configAddresses(mySPI0_TX_DMA_BASE, mySPI0_TX_DMA_ADDRESS, srcAddr);
    DMA_configBurst(mySPI0_TX_DMA_BASE, 8U, 1, 0);
    DMA_configTransfer(mySPI0_TX_DMA_BASE, 16U, 1, 0);
    DMA_configWrap(mySPI0_TX_DMA_BASE, 65535U, 0, 65535U, 0);
    DMA_configMode(mySPI0_TX_DMA_BASE, mySPI0_TX_DMA_TRIGGER, DMA_CFG_ONESHOT_DISABLE | DMA_CFG_CONTINUOUS_DISABLE | DMA_CFG_SIZE_16BIT);
    DMA_setInterruptMode(mySPI0_TX_DMA_BASE, DMA_INT_AT_END);
    DMA_enableInterrupt(mySPI0_TX_DMA_BASE);
    DMA_disableOverrunInterrupt(mySPI0_TX_DMA_BASE);
    DMA_enableTrigger(mySPI0_TX_DMA_BASE);
    DMA_stopChannel(mySPI0_TX_DMA_BASE);
}

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************
void INTERRUPT_init(){
	
	// Interrupt Setings for INT_mySPI0_RX_DMA
	Interrupt_register(INT_mySPI0_RX_DMA, &INT_mySPI0_RX_DMA_ISR);
	Interrupt_disable(INT_mySPI0_RX_DMA);
	
	// Interrupt Setings for INT_mySPI0_TX_DMA
	Interrupt_register(INT_mySPI0_TX_DMA, &INT_mySPI0_TX_DMA_ISR);
	Interrupt_disable(INT_mySPI0_TX_DMA);
}
//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	mySPI0_init();
}

void mySPI0_init(){
	SPI_disableModule(mySPI0_BASE);
	SPI_setConfig(mySPI0_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
				  SPI_MODE_CONTROLLER, 500000, 16);
	SPI_setPTESignalPolarity(mySPI0_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_enableFIFO(mySPI0_BASE);
	SPI_setFIFOInterruptLevel(mySPI0_BASE, SPI_FIFO_TX8, SPI_FIFO_RX8);
	SPI_enableLoopback(mySPI0_BASE);
	SPI_setEmulationMode(mySPI0_BASE, SPI_EMULATION_STOP_MIDWAY);
	SPI_enableModule(mySPI0_BASE);
}

