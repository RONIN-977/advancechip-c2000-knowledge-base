/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   erad_ex3_stackoverflow.c
*                         TITLE:  ERAD Stack Overflow Example
*******************************************************************************************/
//! This example uses 1 HW watchpoint :
//!  - HWBP_1 : Data Write Address Bus = Stack end address + 1
//!
//! \b External \b Connections \n
//!  - None.
//!
//! \b Watch \b Variables \n
//!  - msgCount - A counter for the number of successful messages received
//!  - txMsgData - An array with the data being sent
//!  - rxMsgData - An array with the data that was received
//!  - msgHistoryBuff - An array meant to store the last 50 messages received
//!
//! \b Profiling \b Script \b Output
//!  - "STACK OVERFLOW detected. Halting CPU." will be printed in the scripting
//!    console when a stack overflow occurs (that is, when the watchpoint is hit)
//
//#############################################################################
//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define MSG_DATA_LENGTH    2

//
// Globals
//
volatile unsigned long msgCount = 0;

//
// Stack start and end addresses
//
extern uint16_t _stack;
extern uint16_t _STACK_END;

//
// Main
//
void main(void)
{
    //
    // Setup transfer and receive buffers
    //
    uint16_t txMsgData[2], rxMsgData[2];

    //
    // Create buffer to hold message history up to 50 messages
    //
    volatile uint32_t msgHistoryBuff[50];

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure GPIO pins for CANTX/CANRX
    //
    Device_initGPIO();
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANRXA);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANTXA);

    //
    // Initialize the CAN controller
    //
    CAN_initModule(CANA_BASE);

    //
    // Set up the CAN bus bit rate to 500kHz
    // Refer to the Driver Library User Guide for information on how to set
    // tighter timing control. Additionally, consult the device data sheet
    // for more information about the CAN module clocking.
    //
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 500000, 16);

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
    // Enable CAN test mode with external loopback
    //
    CAN_enableTestMode(CANA_BASE, CAN_TEST_EXL);

    //
    // Initialize the transmit message object used for sending CAN messages.
    // Message Object Parameters:
    //      Message Object ID Number: 1
    //      Message Identifier: 0x1234
    //      Message Frame: Standard
    //      Message Type: Transmit
    //      Message ID Mask: 0x0
    //      Message Object Flags: None
    //      Message Data Length: 2 Bytes
    //
    CAN_setupMessageObject(CANA_BASE, 1, 0x1234, CAN_MSG_FRAME_STD,
                           CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 
                           MSG_DATA_LENGTH);

    //
    // Initialize the receive message object used for receiving CAN messages.
    // Message Object Parameters:
    //      Message Object ID Number: 2
    //      Message Identifier: 0x1234
    //      Message Frame: Standard
    //      Message Type: Receive
    //      Message ID Mask: 0x0
    //      Message Object Flags: None
    //      Message Data Length: 2 Bytes
    //
    CAN_setupMessageObject(CANA_BASE, 2, 0x1234, CAN_MSG_FRAME_STD,
                           CAN_MSG_OBJ_TYPE_RX, 0, CAN_MSG_OBJ_NO_FLAGS, 
                           MSG_DATA_LENGTH);

    //
    // Start CAN module operations
    //
    CAN_startModule(CANA_BASE);    

    //
    // Setup send and receive buffers
    //
    txMsgData[0] = 0x01;
    txMsgData[1] = 0x02;
    *(uint16_t *)rxMsgData = 0;

    //
    // Loop Forever - Send and Receive data continuously
    //
    for(;;)
    {
        //
        // Send CAN message data from message object 1
        //
        CAN_sendMessage(CANA_BASE, 1, MSG_DATA_LENGTH, txMsgData);

        //
        // Delay before receiving the data
        //
        DEVICE_DELAY_US(50000);

        //
        // Read CAN message object 2 and check for new data
        //
        if (CAN_readMessage(CANA_BASE, 2, rxMsgData))
        {
            //
            // Check that received data matches sent data.
            // Device will halt here during debug if data doesn't match.
            //
            if((txMsgData[0] != rxMsgData[0]) ||
               (txMsgData[1] != rxMsgData[1]))
            {
                asm(" ESTOP0");
            }
            else
            {
                //
                // Store message in message history buffer and increment message
                // received counter. Purposely leave out logic to reset counter
                // when buffer is full, eventually causing a stack overflow
                //
                uint32_t message =
                    (uint32_t)rxMsgData[0] | ((uint32_t)rxMsgData[1] << 16);
                msgHistoryBuff[msgCount++] = message;
            }
        }
        else
        {
            //
            // Device will halt here during debug if no new data was received.
            //
            asm(" ESTOP0");            
        }

        //
        // Increment the value in the transmitted message data.
        //
        txMsgData[0] += 0x01;
        txMsgData[1] += 0x01;
        
        //
        // Reset data if exceeds a byte
        //
        if(txMsgData[0] > 0xFF)
        {
            txMsgData[0] = 0;
        }
        if(txMsgData[1] > 0xFF)
        {
            txMsgData[1] = 0;
        }        
    }
}

//
// End of File
//
