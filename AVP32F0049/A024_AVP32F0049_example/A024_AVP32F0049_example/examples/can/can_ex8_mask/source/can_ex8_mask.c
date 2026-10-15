/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:     can_ex8_mask.c
*                         TITLE:   CAN Acceptance Mask Example
*******************************************************************************************/

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define MSG_DATA_LENGTH    0    // "Don't care" for a Receive mailbox
#define RX_MSG_OBJ_ID      1    // Use mailbox 1

//
// Globals
//
uint16_t rxMsgData[8];          // Buffer for received data
uint16_t Toggle_ctr;            // GPIO toggle counter
volatile uint32_t rxMsgCount = 0;

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO
    //
    Device_initGPIO();

    //
	// Configure GPIO pins for CANTX/CANRX
	//
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANRXA);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANTXA);

    //
	// Configure GPIO pin which is toggled upon message reception
	//
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);

    //
    // Initialize the receive buffer to a known value
    //
    rxMsgData[0] = 0xDEAD;
    rxMsgData[1] = 0xDEAD;
    rxMsgData[2] = 0xDEAD;
    rxMsgData[3] = 0xDEAD;
    rxMsgData[4] = 0xDEAD;
    rxMsgData[5] = 0xDEAD;
    rxMsgData[6] = 0xDEAD;
    rxMsgData[7] = 0xDEAD;

    //
    // Initialize the CAN controller
    //
    CAN_initModule(CANA_BASE);

    //
    // Set up the CAN bus bit rate to 500kbps for each module
    // Refer to the Driver Library User Guide for information on how to set
    // tighter timing control. Additionally, consult the device data sheet
    // for more information about the CAN module clocking.
    //
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 500000, 16);

    //
    // Initialize the receive message object used for receiving CAN messages.
	// Possible flags: CAN_MSG_OBJ_NO_FLAGS, CAN_MSG_OBJ_USE_EXT_FILTER,
	//                 CAN_MSG_OBJ_USE_DIR_FILTER
    // Message Object Parameters:
    //      CAN Module: A
    //      Message Object ID Number: 1
    //      Message Identifier: 0x1F9FFFFA
    //      Message Frame: Extended
    //      Message Type: Receive
    //      Message ID Mask: 0x1F000000
    //      Message Object Flags: UMask, MXtd, MDir
    //      Message Object flag CAN_MSG_OBJ_USE_ID_FILTER enables usage 
    //       of msgIDMask parameter for Message Identifier based filtering
    //      Message Object flag CAN_MSG_OBJ_USE_EXT_FILTER enables usage 
    //       of frameType parameter for Message Frame based filtering
    //      Message Data Length: "Don't care" for a Receive mailbox
    CAN_setupMessageObject(CANA_BASE, RX_MSG_OBJ_ID, 0x1F9FFFFA,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_RX, 0x1F000000,
                           (CAN_MSG_OBJ_USE_ID_FILTER | CAN_MSG_OBJ_NO_FLAGS |
                           CAN_MSG_OBJ_USE_EXT_FILTER), MSG_DATA_LENGTH);

    //
    // Start CAN module A operations
    //
    CAN_startModule(CANA_BASE);

    //
	// Start reception - Just wait for data from another node
    //
    while(1)
    {
        //
        // Poll bit0 in CAN_NDAT_21 register to check completion of Reception
        //
        if((HWREG_BP(CANA_BASE + CAN_O_NDAT_21))  == 0x00000001)
        {
            //
            // Get the received message
            //
            asm("  NOP");
            CAN_readMessage(CANA_BASE, RX_MSG_OBJ_ID, rxMsgData);

            rxMsgCount++;
            for(Toggle_ctr=0; Toggle_ctr<100; Toggle_ctr++)
            {
                GPIO_togglePin(DEVICE_GPIO_PIN_LED1);
                DEVICE_DELAY_US(100000);
            }
        }
    }
}

//
// End of File
//
