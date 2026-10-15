/******************************* Copyright(C) *********************************************
*                             AVP32F0049_Example_V1.01
*                              湖南进芯电子科技有限公司
*                             http://www.advancechip.com/
*
*                         FILE:  sci_ex1_loopback.c
*                         TITLE: SCI FIFO Digital Loop Back.
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Globals
//
uint16_t loopCount;
uint16_t errorCount;

//
// Function Prototypes
//
void error();

//
// Main
//
void main(void)
{
    uint16_t sendChar;
    uint16_t receivedChar;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Setup GPIO by disabling pin locks and enabling pullups
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board Initialization
    //
    Board_init();

    //
    // Enables CPU interrupts
    //
    Interrupt_enableGlobal();

    //
    // Initialize counts
    //
    loopCount = 0;
    errorCount = 0;

    //
    // Send a character starting with 0
    //
    sendChar = 0;

    //
    // Send Characters forever starting with 0x00 and going through 0xFF.
    // After sending each, check the receive buffer for the correct value.
    //
    for(;;)
    {
        SCI_writeCharNonBlocking(mySCI0_BASE, sendChar);

        //
        // Wait for RRDY/RXFFST = 1 for 1 data available in FIFO
        //
        while(SCI_getRxFIFOStatus(mySCI0_BASE) == SCI_FIFO_RX0)
        {
            ;
        }

        //
        // Check received character
        //
        receivedChar = SCI_readCharBlockingFIFO(mySCI0_BASE);

        //
        // Received character not correct
        //
        if(receivedChar != sendChar)
        {
            errorCount++;
            // asm("     ESTOP0");  // Uncomment to stop the test here
            for (;;);
        }

        //
        // Move to the next character and repeat the test
        //
        sendChar++;

        //
        // Limit the character to 8-bits
        //
        sendChar &= 0x00FF;
        loopCount++;

    }
}

//
// End of file
//
