/**
 * File: CommInterface.c
 * Description: LIN communication interface and protocol processing layer.
 *              Includes LIN interrupt handler, data frame parsing, and remote modbus command handling.
 * Engineer: Advancechip
 * Date: 2025-11-07
 * Version: V1.0
 */

/****************************************************************
 * Module: LIN Communication Interface
 * Version: V1.0
 *
 * Description: This file contains the LIN communication driver
 *              and protocol handling functions, including the
 *              main interrupt service routine for LIN frame
 *              processing and command dispatching.
 ****************************************************************/

#include "LinLDF.h"
#include "RemoteModbus.h"

/* Global Variables */
Uint16 flash_IAP_flag = 0;               // Flag for In-Application Programming (Bootloader)
LIN_DATA0 SenddDataA;                    // LIN data structure for transmission (Frame A)
LIN_DATA1 SenddDataB;                    // LIN data structure for transmission (Frame B)
Uint16 ReciveDataIDCorrect[8];           // Buffer for correctly received data frames
Uint16 ReciveDataIDInCorrect[8];         // Buffer for incorrectly received data frames

TRemoteModbus   RemoteModbus;            // Remote Modbus communication structure
Uint16 LinRxFlag = 0;                    // Flag indicating LIN data reception is complete
Uint16 rdataA[8];                        // Temporary buffer for raw received data
Uint32 LinL0IntVect;                     // LIN interrupt vector/status register copy
Uint32 FrameIncorrectCnt;                // Counter for frames with incorrect checksum or error

/******************************************************************************
 * Function Name: LINA_LEVEL_isr
 * Function description: LIN A Level (Frame) Interrupt Service Routine.
 *                       Handles different LIN interrupt events: ID reception,
 *                       data reception, and errors. Processes specific LIN IDs
 *                       for master-slave data exchange and bootloader commands.
 * Engineer: Advancechip
 * Date: 2025-11-07
 * Parameters: None
 * Returns: None
 ******************************************************************************/
interrupt void LINA_LEVEL_isr(void)
{
    // OFFSET                           FUNCTION
    // 4                                ID Reception Event
    // 11                               Data Reception Event
    // 12                               Data Transmission Event
    // 13109876532                      Error Event

    ///////////////////////////////////////////////////////////////////////////
    // ID       PID         Length      FUNCTION
    // 0x36     0x76        7           Send Data TO Master
    // 0x31     0xB1        7           Receive Data FROM Master
    // Data frame integrity check: Frame data must be consistent with ID.
    // Data reception completion check...
    ///////////////////////////////////////////////////////////////////////////

    LinL0IntVect = LinaRegs.SCIINTVECT0.all; // Read LIN interrupt vector

    if (LinL0IntVect == 4)                     // ID Reception Event
    {
        if (LinaRegs.LINID.bit.RECEIVEDID == 0x76)      // ID: 0x36 (Normal data to Master)
        {
            LinaRegs.SCIFORMAT.bit.LENGTH   = 7;        // Set response data length
            LinaRegs.LINTD0.all             = SenddDataA.all; // Load data to TX buffer
            LinaRegs.LINTD1.all             = SenddDataB.all;
        }
        // Bootloader related frame
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0x64) // ID: 0x24 (Bootloader response)
        {
            LinaRegs.SCIFORMAT.bit.LENGTH   = 7;
            LinaRegs.LINTD0.all             = SenddDataA.all;
            LinaRegs.LINTD1.all             = 0x11111111; // Placeholder data
            RemoteModbus.TxBufferIndex = 1; // Prepare for transmission
        }
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0xB1)  // ID: 0x31 (Data from Master)
        {
            LinaRegs.SCIFORMAT.bit.LENGTH   = 2;        // Set expected reception length
        }
    }
    else if (LinL0IntVect == 11)               // Data Reception Event
    {
        if (LinaRegs.LINID.bit.RECEIVEDID == 0xB1)      // ID: 0x31 (Data from Master)
        {
            // Store data bytes for correct ID reception
            ReciveDataIDCorrect[0]       = LinaRegs.LINRD0.bit.RD0;
            ReciveDataIDCorrect[1]       = LinaRegs.LINRD0.bit.RD1;
            ReciveDataIDCorrect[2]       = LinaRegs.LINRD0.bit.RD2;
        }
        // Bootloader data reception
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0xA3) // ID: 0x23 (Bootloader data from Master)
        {
            // Store all 8 bytes of received data
            rdataA[0] = LinaRegs.LINRD0.bit.RD0;
            rdataA[1] = LinaRegs.LINRD0.bit.RD1;
            rdataA[2] = LinaRegs.LINRD0.bit.RD2;
            rdataA[3] = LinaRegs.LINRD0.bit.RD3;
            rdataA[4] = LinaRegs.LINRD1.bit.RD4;
            rdataA[5] = LinaRegs.LINRD1.bit.RD5;
            rdataA[6] = LinaRegs.LINRD1.bit.RD6;
            rdataA[7] = LinaRegs.LINRD1.bit.RD7;

            // Copy raw data to Modbus receive buffer
            RemoteModbus.ReceiveBuf[0] = rdataA[0] & 0xFF;
            RemoteModbus.ReceiveBuf[1] = rdataA[1] & 0xFF;
            RemoteModbus.ReceiveBuf[2] = rdataA[2] & 0xFF;
            RemoteModbus.ReceiveBuf[3] = rdataA[3] & 0xFF;
            RemoteModbus.ReceiveBuf[4] = rdataA[4] & 0xFF;
            RemoteModbus.ReceiveBuf[5] = rdataA[5] & 0xFF;
            RemoteModbus.ReceiveBuf[6] = rdataA[6] & 0xFF;
            RemoteModbus.ReceiveBuf[7] = rdataA[7] & 0xFF;

            // Check for bootloader command header (0xCC)
            if(RemoteModbus.ReceiveBuf[0] == 0xCC)
            {
                RemoteModbus.RxBufferIndex += 8; // Advance buffer index
                LinRxFlag = 1;                   // Set reception complete flag
            }
            else
            {
                RemoteModbus.RxBufferIndex = 0;  // Reset index on invalid header
            }
        }
        else // Reception for other/incorrect IDs
        {
            // Store all 8 bytes for logging/debugging
            ReciveDataIDInCorrect[0] = LinaRegs.LINRD0.bit.RD0;
            ReciveDataIDInCorrect[1] = LinaRegs.LINRD0.bit.RD1;
            ReciveDataIDInCorrect[2] = LinaRegs.LINRD0.bit.RD2;
            ReciveDataIDInCorrect[3] = LinaRegs.LINRD0.bit.RD3;
            ReciveDataIDInCorrect[4] = LinaRegs.LINRD1.bit.RD4;
            ReciveDataIDInCorrect[5] = LinaRegs.LINRD1.bit.RD5;
            ReciveDataIDInCorrect[6] = LinaRegs.LINRD1.bit.RD6;
            ReciveDataIDInCorrect[7] = LinaRegs.LINRD1.bit.RD7;
        }
    }
    else // Other/Error Interrupt Events
    {
        FrameIncorrectCnt++; // Increment error frame counter
    }

    // Acknowledge the interrupt in PIE controller
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/******************************************************************************
 * Function Name: CommsProtocalLayerTaskBG
 * Function description: Background task for communication protocol layer.
 *                       (Currently empty - placeholder for future expansion).
 * Engineer: Advancechip
 * Date: 2025-11-07
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void CommsProtocalLayerTaskBG(void)
{
    // Background processing can be added here if needed.
}

/******************************************************************************
 * Function Name: SendNormalFrame
 * Function description: Prepares and loads data from the Modbus receive buffer
 *                       into the LIN transmission data registers for sending
 *                       a normal response frame.
 * Engineer: Advancechip
 * Date: 2025-11-07
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void SendNormalFrame(void)
{
    // Pack bytes 0-3 from ReceiveBuf into a 32-bit word for LIN TX
    RemoteModbus.nLinTxData0 = ((Uint32)RemoteModbus.ReceiveBuf[0] << 24
                               | (Uint32)RemoteModbus.ReceiveBuf[1] << 16
                               | RemoteModbus.ReceiveBuf[2] << 8
                               | RemoteModbus.ReceiveBuf[3]);
    // Pack bytes 4-7 from ReceiveBuf into a 32-bit word for LIN TX
    RemoteModbus.nLinTxData1 = ((Uint32)RemoteModbus.ReceiveBuf[4] << 24
                               | (Uint32) RemoteModbus.ReceiveBuf[5] << 16
                               | RemoteModbus.ReceiveBuf[6] << 8
                               | RemoteModbus.ReceiveBuf[7]);
}

/******************************************************************************
 * Function Name: ReveiveBufferFrame
 * Function description: Parses the received frame buffer, extracts the command ID
 *                       and boot sequence number, and dispatches commands
 *                       (e.g., bootloader update request).
 * Engineer: Advancechip
 * Date: 2025-11-07
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void ReveiveBufferFrame(void)
{
    // Extract Command ID from byte 2
    RemoteModbus.nAppCmdID = (RemoteModbus.ReceiveBuf[2] & 0x00ff);
    // Extract Boot Sequence Number from bytes 3-4 (little-endian)
    RemoteModbus.nBootSeqNum = (((RemoteModbus.ReceiveBuf[4] << 8) & 0xff00) | (RemoteModbus.ReceiveBuf[3] & 0x00ff));

    // GPIO toggle for debugging (commented out)
    // GpioDataRegs.GPATOGGLE.bit.GPIO19 = 1;

    switch(RemoteModbus.nAppCmdID)
    {
       // Command: Request Firmware Update
       case SBE_COM_RequstUpdate:
           SendNormalFrame();        // Prepare and send response frame
           flash_IAP_flag = 1;       // Set flag to enter bootloader/IAP mode
       break;

       default: // Other commands can be handled here
       break;
    }
}

/******************************************************************************
 * Function Name: CommsProtocalLayerTask2ms
 * Function description: 2ms periodic task for the communication protocol layer.
 *                       Manages the state machine for receiving LIN frames,
 *                       validates buffer index, and triggers frame processing.
 *                       Called from the main 2ms loop to reduce ISR execution time.
 * Engineer: Advancechip
 * Date: 2025-11-07
 * Parameters: None
 * Returns: None
 ******************************************************************************/
void CommsProtocalLayerTask2ms(void)
{
    static unsigned char IndexZm1; // Static index threshold

    switch (RemoteModbus.RecvState) {
        case RECEIVE_STATE:
            if(LinRxFlag == 1) // A complete frame has been received in the ISR
            {
                RemoteModbus.nAppCmdID = (RemoteModbus.ReceiveBuf[2] & 0x00ff);
                IndexZm1 = 7; // Expected minimum data index for a valid frame
                if (RemoteModbus.RxBufferIndex >= IndexZm1)
                {
                    RemoteModbus.RxBufferIndex = 0; // Reset index
                    RemoteModbus.RecvState = FRAME_RECEIVED_STATE; // Proceed to processing
                }
            }
        break;

        case FRAME_RECEIVED_STATE:
        {
            ReveiveBufferFrame(); // Parse and handle the received frame
            RemoteModbus.RxBufferIndex = 0; // Clear buffer index
            LinRxFlag = 0; // Clear reception flag
            RemoteModbus.RecvState = RECEIVE_STATE; // Return to listening state
        }
        break;

        default:
        break;
    }
}
