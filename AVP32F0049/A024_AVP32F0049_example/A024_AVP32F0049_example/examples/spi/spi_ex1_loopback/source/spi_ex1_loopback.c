/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   spi_ex1_loopback.c
*                         TITLE:  SPI Digital Loopback
*******************************************************************************************/
//! This program uses the internal loopback test mode of the SPI module. This
//! is a very basic loopback that does not use the FIFOs or interrupts. A
//! stream of data is sent and then compared to the received stream.
//! The pinmux and SPI modules are configure through the sysconfig file.
//!
//! The sent data looks like this: \n
//!  0000 0001 0002 0003 0004 0005 0006 0007 .... FFFE FFFF 0000
//!
//! This pattern is repeated forever.
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
// Main
//
void main(void)
{
    uint16_t sData = 0;                  // Send data
    uint16_t rData = 0;                  // Receive data

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
    // Board initialization
    //
    Board_init();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Loop forever. Suspend or place breakpoints to observe the buffers.
    //
    while(1)
    {
        // Transmit data
        SPI_writeDataNonBlocking(mySPI0_BASE, sData);

        // Block until data is received and then return it
        rData = SPI_readDataBlockingNonFIFO(mySPI0_BASE);

        // Check received data against sent data
        if(rData != sData)
        {
            // Something went wrong. rData doesn't contain expected data.
            ESTOP0;
        }

        sData++;

    }
}

//
// End File
//
