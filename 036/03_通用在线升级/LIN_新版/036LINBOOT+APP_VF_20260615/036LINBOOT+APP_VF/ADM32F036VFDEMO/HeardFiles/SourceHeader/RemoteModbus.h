
#ifndef REMOTEMODBUS_H_
#define REMOTEMODBUS_H_


#define     SBE_COM_GetVersion        0x01        // Get version
#define     SBE_COM_RebootDevice      0x02        // Device reboot
#define     SBE_COM_SetSN             0x03        // Set serial number
#define     SBE_COM_GetSN             0x04        // Get serial number
#define     SBE_COM_GetActState       0x05        // Get activation status
#define     SBE_COM_SetActState       0x06        // Set activation status
#define     SBE_COM_RevActState       0x07        // Deactivation status

#define     SBE_COM_GetRuntime        0x10        // Get runtime
#define     SBE_COM_SetRPM            0x11        // Set rotation speed
#define     SBE_COM_SetSensorCmd      0x12        // Set Hall sensor calibration command
#define     SBE_COM_ReadSensorCmd     0x13        // Read Hall sensor calibration command

#define     SBE_COM_RequstUpdate      0x70        // Request update
#define     SBE_COM_IntBoot           0x72        // Enter bootloader
#define     SBE_COM_CheckState        0x71        // Check device update status
#define     SBE_COM_SendFirmware      0x73        // Send firmware
#define     SBE_COM_EndUpdate         0x74        // End update
#define     SBE_COM_RestartDevice     0x75        // Restart device after update


//-----------------------------------------------------------------------------
// Public Enumerated and Structure Definitions

// the version of protocol
#define REMOTE_PRO_VER          0x01


// the header of Protocol
#define REMOTE_ADDR             0xCC


#define nSectorAStart         0x330000
#define nAppStartAddr         0x300000



// the Offset of Protocol
#define REMOTE_OFFSET_OF_ADDR           0
#define REMOTE_OFFSET_OF_FUNCODE        1
#define REMOTE_OFFSET_OF_DATA           2
#define REMOTE_OFFSET_OF_REG_HI         2
#define REMOTE_OFFSET_OF_REG_LOW        3
#define REMOTE_OFFSET_OF_NUM            4
#define REMOTE_OFFSET_OF_MODCRC         6


//Code
#define REMOTE_READREGISTERS           0x03    ///< Modbus function code defines
#define REMOTE_WRITEREGISTER           0x10    ///< Modbus function code defines



// the size of Protocol Buffer
#define REMOTE_MSG_BUFF_SIZE            128


/**/
typedef enum
{
    RECEIVE_STATE,
    FRAME_RECEIVED_STATE,
    TRANSMIT_STATE,
    CHECK_FOR_LAST_CHARACTER,
    PERPARD_TO_TRANSMIT_STATE,
    START_TRANSMIT
}eRemoteModbusState;


//
typedef struct
{
    // the receive state machine
    eRemoteModbusState  RecvState;

    // the Communication status
    Uint16 CommStatus;

    // the CRC Err Counter
    Uint16 nCRCErrCounter;

    unsigned char ReceiveBuf[128];

    Uint16 nRxLength;

    Uint16 RxBufferIndex;

    unsigned char SendBuf[32];

    Uint16 TxBufferIndex;

    Uint16 TransmitLength;

    Uint16 SendDataIndex;

    Uint16 nCRCErrorState;

    Uint16 nStartAddress;

    Uint16 nCRC;

    Uint16 nAppCmdID;

    Uint16 nBootSeqNum;

    Uint16 nFirmwareSize;

    Uint32 nBootVersion;

    Uint16 nAppJumpFlag;

    Uint16 nBootFlashErase;

    Uint16 nFlashBuffer[64];

    Uint16 nFirmwareLen;

    Uint32 nPackIndex;

    Uint32 nOffsetAddr;

    Uint32 nFlashAPIAddr;

    Uint32 nLinTxData0;
    Uint32 nLinTxData1;

}TRemoteModbus;



//-----------------------------------------------------------------------------
// Public Variable Prototypes

extern  TRemoteModbus  RemoteModbus;


//-----------------------------------------------------------------------------
// Public Function Prototypes


extern void RemoteModbus_Initialize(void);

extern void RemoteModbus_MsgProcess(void);

//extern void Sci_Initialize(void);

extern void SCI_Boot(void);


#endif /* REMOTEMODBUS_H_ */
