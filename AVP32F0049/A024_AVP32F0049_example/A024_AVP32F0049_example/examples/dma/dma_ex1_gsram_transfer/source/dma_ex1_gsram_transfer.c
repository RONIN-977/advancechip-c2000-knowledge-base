/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   dma_ex1_gsram_transfer.c
*                         TITLE:  DMA GSRAM Transfer
*******************************************************************************************/
//!  This example uses one DMA channel to transfer data from a buffer in
//!  RAMGS0 to a buffer in RAMGS1. The example sets the DMA channel
//!  PERINTFRC bit repeatedly until the transfer of 16 bursts (where each
//!  burst is 8 16-bit words) has been completed. When the whole transfer is
//!  complete, it will trigger the DMA interrupt.
//!
//!  \b Watch \b Variables \n
//!  - \b sData - Data to send
//!  - \b rData - Received data
//!

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// DMA data sections
//
#pragma DATA_SECTION(sData, "ramgs0");  // map the TX data to memory
#pragma DATA_SECTION(rData, "ramgs1");  // map the RX data to memory

//
// Globals
//
uint16_t sData[128];   // Send data buffer
uint16_t rData[128];   // Receive data buffer
volatile uint16_t done;

const void *destAddr = (const void *)rData;
const void *srcAddr = (const void *)sData;

//
// Function Prototypes
//
void error();
__interrupt void INT_myDMA0_ISR(void);

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
    //Initialize PIE and clear PIE registers. Disables CPU interrupts
    //
    Interrupt_initModule();

    //
    //Initialize the PIE vector table with pointers to the shell Interrupt
    //Service Routines (ISR)
    //
    Interrupt_initVectorTable();
    
    Board_init();


    //
    // User specific code, enable interrupts:
    // Initialize the data buffers
    //
    for(i = 0; i < 128; i++)
    {
        sData[i] = i;
        rData[i] = 0;
    }

    //
    // Enable interrupts required for this example
    //
    Interrupt_enable(INT_myDMA0);
    EINT;                                // Enable Global Interrupts
    // Start DMA channel
    DMA_startChannel(myDMA0_BASE);

    done = 0;           // Test is not done yet

    while(!done)        // wait until the DMA transfer is complete
    {
       DMA_forceTrigger(myDMA0_BASE);

       asm(" RPT #255 || NOP");
    }

    //
    // When the DMA transfer is complete the program will stop here
    //
    ESTOP0;
}

//
// error - Error Function which will halt the debugger
//
void error(void)
{
    ESTOP0;  //Test failed!! Stop!
    for (;;);
}

//
// local_D_INTCH6_ISR - DMA Channel6 ISR
//
__interrupt void INT_myDMA0_ISR(void)
{
    uint16_t i;

    DMA_stopChannel(myDMA0_BASE);
    // ACK to receive more interrupts from this PIE group
    EALLOW;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP7);
    EDIS;

    for( i = 0; i < 128; i++ )
    {
        //
        // check for data integrity
        //
        if (rData[i] != i)
        {
            error();
        }
    }

    done = 1; // Test done.
    return;
}

//
// End of file
//
