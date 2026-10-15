/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  clb_ex27_spi_data_export.c
*                         TITLE: CLB SPI Data Export
*******************************************************************************************/



//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "clb_config.h"

//
// Count for SPI RX FIFO INT
//
uint16_t spiFifoRxIntCount = 0;

//
// Destination buffer for exported CLB data
//
uint16_t clbExportedData[16] = {0};

__interrupt void mySPIForTILE1_RxFIFOISR(void)
{
    uint16_t dataIndex = 0;
    uint16_t rxFIFOLevel = 0;
    uint16_t txFIFOLevel = 0;

    //
    // Read the interrupt status
    //
    uint32_t spiIntStatus = SPI_getInterruptStatus(mySPIForTILE1_BASE);

    //
    // Read the current FIFO level to know how much data to read
    // from the buffer
    //
    SPI_getFIFOInterruptLevel(mySPIForTILE1_BASE, &txFIFOLevel, &rxFIFOLevel);

    if (spiIntStatus & SPI_INT_RXFF)
    {
        for (dataIndex = 0; dataIndex < rxFIFOLevel; dataIndex++)
        {
            clbExportedData [dataIndex] = SPI_readDataNonBlocking(mySPIForTILE1_BASE);
        }
    }
    if (spiIntStatus & SPI_INT_RXFF_OVERFLOW)
    {
        ESTOP0;

        //
        // Error! Data was not read in time
        //
        while(1);
    }

    //
    // Increment number of interrupt counts.
    //
    spiFifoRxIntCount++;

    //
    // Clear interrupt flag and issue ACK
    //
    SPI_clearInterruptStatus(mySPIForTILE1_BASE, SPI_INT_RXFF | SPI_INT_RXFF_OVERFLOW);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP6);
}

//
// Main
//
void main(void)
{
    Device_init();
    Device_initGPIO();

    Interrupt_initModule();
    Interrupt_initVectorTable();

    Interrupt_register(INT_SPIA_RX, &mySPIForTILE1_RxFIFOISR);
    Interrupt_enable(INT_SPIA_RX);

    //
    // Configures SPI to enable RX FIFO and RX FIFO interrupts
    // for CLB data export
    //
    Board_init();

    //
    // The tile configuration is:
    // TILE1:
    // Non PIPELINE MODE
    // COUNTER0 set to generate a match event every 125000 clocks then reset
    // COUNTER0 match1 event will:
    // 1. Trigger HLC to increment R0 by 5
    // 2. Trigger SPI Export event to push R0 to the SPIRX buffer
    //
    initTILE1(myCLBForTILE1_BASE);


    CLB_enableCLB(myCLBForTILE1_BASE);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    while(1)
    {

    }
}


//
// End of File
//
