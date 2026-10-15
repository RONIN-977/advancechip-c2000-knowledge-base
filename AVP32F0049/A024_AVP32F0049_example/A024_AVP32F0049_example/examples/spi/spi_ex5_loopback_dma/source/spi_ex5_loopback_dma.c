/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   spi_ex5_loopback_dma.c
*                         TITLE:  SPI Digital Loopback with DMA
*******************************************************************************************/
//! This program uses the internal loopback test mode of the SPI module. Both
//! DMA interrupts and the SPI FIFOs are used. When the SPI transmit FIFO has
//! enough space (as indicated by its FIFO level interrupt signal), the DMA
//! will transfer data from global variable sData into the FIFO. This will be
//! transmitted to the receive FIFO via the internal loopback.
//!
//! When enough data has been placed in the receive FIFO (as indicated by its
//! FIFO level interrupt signal), the DMA will transfer the data from the FIFO
//! into global variable rData.
//!
//! When all data has been placed into rData, a check of the validity of the
//! data will be performed in one of the DMA channels' ISRs.
//!
//! \b External \b Connections \n
//!  - None
//!
//! \b Watch \b Variables \n
//!  - \b sData - Data to send
//!  - \b rData - Received data
//!
//

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Globals
//
uint16_t sData[128];                // Send data buffer
uint16_t rData[128];                // Receive data buffer

const void *destAddr = (const void *)rData;
const void *srcAddr = (const void *)sData;

// Place buffers in GSRAM
#pragma DATA_SECTION(sData, "ramgs0");
#pragma DATA_SECTION(rData, "ramgs1");

volatile uint16_t done = 0;         // Flag to set when all data transferred

//
// Function Prototypes
//
__interrupt void INT_mySPI0_RX_DMA_ISR(void);
__interrupt void INT_mySPI0_TX_DMA_ISR(void);

//
// Main
//
void main(void)
{
    uint16_t i;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board initialization from SysConfig
    //
    Board_init();


    //
    // Initialize the data buffers
    //
    for(i = 0; i < 128; i++)
    {
        sData[i] = i;
        rData[i]= 0;
    }

    //
    // Enable interrupts required for this example
    //
    Interrupt_enable(INT_mySPI0_RX_DMA);
    Interrupt_enable(INT_mySPI0_TX_DMA);

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Start the DMA channels
    //
    DMA_startChannel(mySPI0_TX_DMA_BASE);
    DMA_startChannel(mySPI0_RX_DMA_BASE);

    //
    // Wait until the DMA transfer is complete
    //
    while(!done);

    //
    // When the DMA transfer is complete the program will stop here
    //
    ESTOP0;
}

//
// DMA Channel 5 ISR
//
__interrupt void INT_mySPI0_TX_DMA_ISR(void)
{
    DMA_stopChannel(mySPI0_TX_DMA_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
    return;
}

//
// DMA Channel 6 ISR
//
 __interrupt void INT_mySPI0_RX_DMA_ISR(void)
{
    uint16_t i;

    DMA_stopChannel(mySPI0_RX_DMA_BASE);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);

    //
    // Check for data integrity
    //
    for(i = 0; i < 128; i++)
    {
        if (rData[i] != i)
        {
            // Something went wrong. rData doesn't contain expected data.
            ESTOP0;
        }
    }

    done = 1;
    return;
}
