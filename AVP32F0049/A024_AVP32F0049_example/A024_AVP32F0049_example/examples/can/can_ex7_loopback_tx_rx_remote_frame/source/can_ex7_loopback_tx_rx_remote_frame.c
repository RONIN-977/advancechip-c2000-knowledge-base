/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:     can_ex7_loopback_tx_rx_remote_frame.c
*                         TITLE:   CAN Remote Request Loopback Example
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Defines
//
#define MSG_DATA_LENGTH    2

//
// Globals
//
volatile unsigned long msgCount = 0;

//
// Main
//
void main(void)
{
    uint16_t rxMsgData[8];

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure GPIO pins for CANTX/CANRX
    //
    Device_initGPIO();

    //
    // Board initialization
    //
    Board_init();

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
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Start CAN module operations
    //
    CAN_startModule(myCAN0_BASE);

    //
    // Setup receive buffers
    //
    *(uint16_t *)rxMsgData = 0;

    while((HWREGH(myCAN0_BASE + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY) ==
          CAN_IF1CMD_BUSY)
    {
    }

    //
    // Write to IF1DATA & IF1DATB registers send buffer
    //
    HWREG_BP(myCAN0_BASE + CAN_O_IF1DATA)  =  0x76543210UL;
    HWREG_BP(myCAN0_BASE + CAN_O_IF1DATB)  =  0xFEDCBA98UL;

    //
    // Transfer to MBX RAM (refer to IFxCMD bit field for explanation).
    // Configuring message object 2 to respond to a remote frame with
    // a data length 8 bytes.
    //
    HWREG_BP(myCAN0_BASE + CAN_O_IF1CMD)  =  0x00830002UL;

    //
    // Wait for busy bit to clear
    //
    while((HWREGH(myCAN0_BASE + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY) ==
           CAN_IF1CMD_BUSY)
    {
    }
    //
    // Loop Forever - Send and Receive data continuously
    //
    for(;;)
    {
        //
        // Send CAN remote request frame from message object 3
        //
        CAN_sendRemoteRequestMessage(myCAN0_BASE, 3);

        //
        // Delay before receiving the data
        //
        DEVICE_DELAY_US(1000000);

        //
        // Read CAN message object 1 and check for new data
        // received by the response to the remote request from
        // message object 2
        //
        if (CAN_readMessage(myCAN0_BASE, 1, rxMsgData))
        {
            //
            // Check that received data matches sent data.
            // Device will halt here during debug .
            //
            asm(" ESTOP0");
        }
        else
        {
            //
            // Device will halt here during debug if no new data was received.
            //
            asm(" ESTOP0");
        }
    }
}

//
// End of File
//
