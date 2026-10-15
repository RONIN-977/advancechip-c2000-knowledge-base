
#ifndef LIN_BOOTPROTOCOL_H_
#define LIN_BOOTPROTOCOL_H_
//-----------------------------------------------------------------------------
// Public Enumerated and Structure Definitions

#define nAppStartAddr         0x3F0000                //APP程序起始地址
#define nSectorAStart         0x3F6000                //APP程序终止地址

#define Jump_App        (void  (*)(void)) nAppStartAddr    //0x3F4000是APP程序的入口地址
#define Boot_FlagAdd    0x3F5FFE                      //升级标志地址

#define ProductInfoAdd  0x3F5FCE                      //软件版本地址

#define     SBE_COM_RequstUpdate      0x70        // 表示是请求升级命令
#define     SBE_COM_IntBoot           0x71        // 表示是进入BOOT
#define     SBE_COM_SendFirmware      0x72        // 表示是发送固件
#define     SBE_COM_EndUpdate         0x73        // 表示是升级结束

#define     SBE_COM_ReqVersionRead    0x74        // 请求读取软件版本
#define     SBE_COM_StartReadVersion  0x75        // 开始读取软件版本


//lin interrupt type
#define No_interrupt                                  0
#define Wakeup                                        1
#define Inconsistent_sync_field_error                 2
#define Parity_error                                  3
#define ID                                            4
#define Physical_bus_error                            5
#define Frame_error                                   6
#define Break_detect                                  7
#define Checksum_error                                8
#define Overrun_error                                 9
#define Bit_error                                     10
#define Receive                                       11
#define Transmit                                      12
#define No_response_error                             13
#define Timeout_after_wakeup_signal_150_ms            14
#define Timeout_after_three_wakeup_signals_1_5_s      15
#define Timeout_Bus_Idle_4s                           16

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
#define REMOTE_MSG_BUFF_SIZE            135

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

    unsigned char ReceiveBuf[160];

    Uint16 ProductInfoBuf[48];

    Uint16 ProductInfoNum;

    Uint16 nRxLength;

    Uint16 RxBufferIndex;

    unsigned char SendBuf[64];

    Uint16 TxBufferIndex;

    Uint16 TransmitLength;

    Uint16 SendDataIndex;

    Uint16 nCRCErrorState;

    Uint16 nEraseErrorFlag;

    Uint16 nRecivedErrorFlag;

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

    Uint16 nPackIndex;

    Uint32 nOffsetAddr;

    Uint32 nFlashAPIAddr;

    Uint16 nBootProceed;

    Uint32 nLinTxData0;
    Uint32 nLinTxData1;

}TRemoteModbus;

//-----------------------------------------------------------------------------
// Public Variable Prototypes
extern volatile TRemoteModbus  RemoteModbus;

//-----------------------------------------------------------------------------
// Public Function Prototypes


extern void RemoteModbus_Initialize(void);

extern void RemoteModbus_MsgProcess(void);

//extern void Sci_Initialize(void);

extern void LIN_Boot(void);

#endif /* MAIN_SCI_BOOT_H_ */
