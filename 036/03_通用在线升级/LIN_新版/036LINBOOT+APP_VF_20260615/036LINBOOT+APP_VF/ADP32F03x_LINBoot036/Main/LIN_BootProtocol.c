//-----------------------------------------------------------------------------
#include "MainInclude.h"

//-----------------------------------------------------------------------------
// Public Variable Definitions
volatile TRemoteModbus   RemoteModbus;
FLASH_ST        FlashStatus;

static void SendNormalFrame(void);
static void ReveiveBufferFrame(void);   // Note: typo kept for compatibility

// CRC16 table
static Uint16 CRC16Table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

static Uint16 txErrFlag = 0;
static Uint16 crc = 0;

unsigned int CRC16(unsigned char* dataIn, int length)
{
    Uint16 result = 0;
    Uint16 tableNo = 0;
    int i;

    for (i = 0; i < length; i++)
    {
        tableNo = ((result ^ dataIn[i]) & 0xFF);
        result = (result >> 8) ^ CRC16Table[tableNo];
    }
    return result;
}

/*******************************************************************************
* Function Name  : Flash_ReadWord
* Description    : 读取函数,读取固定一位的数据地址,355ns@100MHz
* Input          : 数据地址
* Output         : None
* Return         : 数据内容
*******************************************************************************/
Uint16 Flash_ReadWord(Uint32 *FlashAddr)
{
    return *FlashAddr;  // 直接读取内存映射的Flash
}

void RemoteModbus_Initialize(void)
{
    memset(&RemoteModbus, 0, sizeof(RemoteModbus));

    RemoteModbus.RecvState = RECEIVE_STATE;
    RemoteModbus.nBootVersion = 200;
    RemoteModbus.nAppJumpFlag = 1;
    RemoteModbus.nBootProceed = 0;
}

// Level 0.5ms Call
void LIN_Boot(void)
{
    RemoteModbus_MsgProcess();
}

extern volatile Uint16 LinRxFlag;

void RemoteModbus_MsgProcess(void)
{
    static Uint16 IndexZm1;

    switch (RemoteModbus.RecvState)
    {
        case RECEIVE_STATE:   // receiving state
            if (LinRxFlag == 1)
            {
                RemoteModbus.nAppCmdID = (RemoteModbus.ReceiveBuf[2] & 0x00FF);
                if (RemoteModbus.nAppCmdID == SBE_COM_SendFirmware)
                {
                    IndexZm1 = 135;
                }
                else
                {
                    IndexZm1 = 7;
                }

                if (RemoteModbus.RxBufferIndex >= IndexZm1)
                {
                    RemoteModbus.RxBufferIndex = 0;
                    RemoteModbus.RecvState = FRAME_RECEIVED_STATE;
                    DisableDog();
                }
            }
            break;

        case FRAME_RECEIVED_STATE:
            ReveiveBufferFrame();
            RemoteModbus.RxBufferIndex = 0;
            LinRxFlag = 0;
            RemoteModbus.RecvState = PERPARD_TO_TRANSMIT_STATE;
            break;

        case PERPARD_TO_TRANSMIT_STATE:
            RemoteModbus.RecvState = START_TRANSMIT;
            break;

        case START_TRANSMIT:
            RemoteModbus.RecvState = TRANSMIT_STATE;
            break;

        case TRANSMIT_STATE:
            if (RemoteModbus.TxBufferIndex == 1)
            {
                RemoteModbus.TxBufferIndex = 0;

                if ((RemoteModbus.nAppCmdID == SBE_COM_RequstUpdate) ||
                    (RemoteModbus.nAppCmdID == SBE_COM_IntBoot) ||
                    (RemoteModbus.nAppCmdID == SBE_COM_SendFirmware))
                {
                    // DisableDog();   // intentionally left blank
                    // EnableDog();
                }
                else
                {
                    // EnableDog();
                    DisableDog();
                }

                RemoteModbus.RecvState = CHECK_FOR_LAST_CHARACTER;
            }
            break;

        case CHECK_FOR_LAST_CHARACTER:
            RemoteModbus.RecvState = RECEIVE_STATE;
            break;

        default:
            break;
    }
}

void ReveiveBufferFrame(void)
{
    Uint16 Status, i;
    Uint16 Status_Verify;
    Uint16 nCRC;

    RemoteModbus.nAppCmdID = (RemoteModbus.ReceiveBuf[2] & 0x00FF);

    // Sequence number
    RemoteModbus.nBootSeqNum = (((RemoteModbus.ReceiveBuf[4] << 8) & 0xFF00) |
                                (RemoteModbus.ReceiveBuf[3] & 0x00FF));

    switch (RemoteModbus.nAppCmdID)
    {
        case SBE_COM_RequstUpdate:   // 0x70: request update
            RemoteModbus.nAppJumpFlag = 0;

            DINT;
            DRTM;
            EALLOW;
            LinaRegs.SCIGCR1.bit.RXENA = 0;
            LinaRegs.SCIGCR1.bit.TXENA = 0;

            DisableDog();
            Status = Flash_Erase((SECTORB | SECTORC | SECTORD), &FlashStatus);
            EnableDog();

            if (Status == STATUS_SUCCESS)
            {
                RemoteModbus.nEraseErrorFlag = 0;
                RemoteModbus.nRecivedErrorFlag = 0;
            }
            else
            {
                RemoteModbus.nEraseErrorFlag = 1;
            }
            EINT;
            ERTM;

            RemoteModbus.SendBuf[0] = RemoteModbus.ReceiveBuf[0];
            RemoteModbus.SendBuf[1] = 0x01;
            RemoteModbus.SendBuf[2] = RemoteModbus.nAppCmdID;
            RemoteModbus.SendBuf[3] = 0x00;
            RemoteModbus.SendBuf[4] = 0x00;
            RemoteModbus.SendBuf[5] = RemoteModbus.nEraseErrorFlag & 0x00FF;
            RemoteModbus.SendBuf[6] = 0x00;
            RemoteModbus.SendBuf[7] = 0x00;
            SendNormalFrame();
            LinaRegs.SCIGCR1.bit.RXENA = 1;
            LinaRegs.SCIGCR1.bit.TXENA = 1;
            EDIS;
            break;

        case SBE_COM_IntBoot:        // 0x71: general answer
            RemoteModbus.nAppJumpFlag = 0;

            RemoteModbus.SendBuf[0] = RemoteModbus.ReceiveBuf[0];
            RemoteModbus.SendBuf[1] = 0x01;
            RemoteModbus.SendBuf[2] = RemoteModbus.nAppCmdID;
            RemoteModbus.SendBuf[3] = 0x00;
            RemoteModbus.SendBuf[4] = 0x00;
            RemoteModbus.SendBuf[5] = RemoteModbus.nCRCErrorState & 0x00FF;
            RemoteModbus.SendBuf[6] = 0x00;
            RemoteModbus.SendBuf[7] = 0x00;
            SendNormalFrame();
            break;

        case SBE_COM_SendFirmware:   // 0x72: send firmware
            DisableDog();
            EALLOW;
            LinaRegs.SCIGCR1.bit.RXENA = 0;
            LinaRegs.SCIGCR1.bit.TXENA = 0;
            EDIS;

            crc = (((RemoteModbus.ReceiveBuf[134] << 8) & 0xFF00) |
                   (RemoteModbus.ReceiveBuf[133] & 0x00FF));
            RemoteModbus.nCRC = CRC16(RemoteModbus.ReceiveBuf, 133);

            if ((RemoteModbus.nRecivedErrorFlag != 0) || (RemoteModbus.nCRC != crc))
            {
                RemoteModbus.nRecivedErrorFlag = 0;
                txErrFlag = 1;
                RemoteModbus.RxBufferIndex = 0;
            }
            else
            {
                RemoteModbus.nBootProceed = 1;
                RemoteModbus.nPackIndex = (Uint16)((RemoteModbus.ReceiveBuf[4] << 8) +
                                                   RemoteModbus.ReceiveBuf[3]);
                RemoteModbus.nFirmwareLen = 64;   // fixed length
                RemoteModbus.nOffsetAddr = (Uint32)(RemoteModbus.nPackIndex *
                                                    RemoteModbus.nFirmwareLen);
                RemoteModbus.nFlashAPIAddr = RemoteModbus.nOffsetAddr + nAppStartAddr;

                if (RemoteModbus.nFlashAPIAddr < nSectorAStart)
                {
                    for (i = 0; i < RemoteModbus.nFirmwareLen; i++)
                    {
                        RemoteModbus.nFlashBuffer[i] = (RemoteModbus.ReceiveBuf[2 * i + 6] << 8) +
                                                       (RemoteModbus.ReceiveBuf[2 * i + 5]);
                    }

                    Status = Flash_Program((Uint16*)(RemoteModbus.nFlashAPIAddr),
                                           (Uint16*)RemoteModbus.nFlashBuffer,
                                           (Uint32)RemoteModbus.nFirmwareLen,
                                           &FlashStatus);
                    Status_Verify = Flash_Verify((Uint16*)RemoteModbus.nFlashAPIAddr,
                                                 (Uint16*)RemoteModbus.nFlashBuffer,
                                                 (Uint32)RemoteModbus.nFirmwareLen,
                                                 &FlashStatus);

                    if ((Status == STATUS_SUCCESS) && (Status_Verify == STATUS_SUCCESS))
                    {
                        txErrFlag = 0;
                    }
                    else
                    {
                        txErrFlag = 2;   // write error, abort programming
                    }
                }
                else
                {
                    txErrFlag = 0;       // address out of range, ignore
                }
            }

            RemoteModbus.SendBuf[0] = RemoteModbus.ReceiveBuf[0];
            RemoteModbus.SendBuf[1] = 0x01;
            RemoteModbus.SendBuf[2] = RemoteModbus.nAppCmdID;
            RemoteModbus.SendBuf[3] = (RemoteModbus.nPackIndex & 0x00FF);
            RemoteModbus.SendBuf[4] = (RemoteModbus.nPackIndex >> 8) & 0x00FF;
            RemoteModbus.SendBuf[5] = txErrFlag;
            RemoteModbus.SendBuf[6] = 0x00;
            RemoteModbus.SendBuf[7] = 0x00;
            SendNormalFrame();

            EALLOW;
            LinaRegs.SCIGCR1.bit.RXENA = 1;
            LinaRegs.SCIGCR1.bit.TXENA = 1;
            EDIS;
            EnableDog();
            break;

        case SBE_COM_EndUpdate:      // 0x73: end of update
            RemoteModbus.SendBuf[0] = RemoteModbus.ReceiveBuf[0];
            RemoteModbus.SendBuf[1] = 0x01;
            RemoteModbus.SendBuf[2] = RemoteModbus.nAppCmdID;
            RemoteModbus.SendBuf[3] = 0x00;
            RemoteModbus.SendBuf[4] = 0x00;
            RemoteModbus.SendBuf[5] = 0;   // txErrFlag
            RemoteModbus.SendBuf[6] = 0x00;
            RemoteModbus.SendBuf[7] = 0x00;
            SendNormalFrame();

            if (RemoteModbus.ReceiveBuf[4] == 1)
            {
                RemoteModbus.nAppJumpFlag = 1;
            }
            break;

        case SBE_COM_ReqVersionRead:
            RemoteModbus.nAppJumpFlag = 0;

            DINT;
            DRTM;
            EALLOW;
            LinaRegs.SCIGCR1.bit.RXENA = 0;
            LinaRegs.SCIGCR1.bit.TXENA = 0;

            DisableDog();
            for(i = 0; i < 48; i++)
            {
                RemoteModbus.ProductInfoBuf[i] = *(Uint16 *)( ProductInfoAdd + i );
            }
            RemoteModbus.nCRC = CRC16(RemoteModbus.ProductInfoBuf, 48);

            EnableDog();

            RemoteModbus.nEraseErrorFlag = 0;
            RemoteModbus.nRecivedErrorFlag = 0;

            EINT;
            ERTM;

            RemoteModbus.SendBuf[0] = RemoteModbus.ReceiveBuf[0];
            RemoteModbus.SendBuf[1] = 0x01;
            RemoteModbus.SendBuf[2] = RemoteModbus.nAppCmdID;
            RemoteModbus.SendBuf[3] = 0x00;
            RemoteModbus.SendBuf[4] = 0x00;
            RemoteModbus.SendBuf[5] = RemoteModbus.nEraseErrorFlag & 0x00FF;
            RemoteModbus.SendBuf[6] = 0x00;
            RemoteModbus.SendBuf[7] = 0x00;
            SendNormalFrame();
            LinaRegs.SCIGCR1.bit.RXENA = 1;
            LinaRegs.SCIGCR1.bit.TXENA = 1;
            EDIS;

            for(i = 0; i < 48; i++)
            {
                RemoteModbus.SendBuf[ i + 3 ] = RemoteModbus.ProductInfoBuf[i];
            }
            RemoteModbus.SendBuf[51] = RemoteModbus.nCRC  & 0x00FF;
            RemoteModbus.SendBuf[52] = ( RemoteModbus.nCRC >> 8 )  & 0x00FF;

            RemoteModbus.ProductInfoNum = 0;
            break;

        case SBE_COM_StartReadVersion:  //0x75
            // Frame 0: 3 header bytes (PID, 0x01, cmdID) + 5 data bytes at SendBuf[3..7]
            // Frame 1+: 8 data bytes in SendBuf[0..7], header positions overwritten with data
            if(RemoteModbus.ProductInfoNum == 0)
            {
                RemoteModbus.SendBuf[0] = RemoteModbus.ReceiveBuf[0];
                RemoteModbus.SendBuf[1] = 0x01;
                RemoteModbus.SendBuf[2] = RemoteModbus.nAppCmdID;

                i = 0;  // data starts at offset 0
                if(i < 50) RemoteModbus.SendBuf[3] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[3] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[4] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[4] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[5] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[5] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[6] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[6] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[7] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[7] = 0; i++;
            }
            else
            {
                // Frames 1+: 8 data bytes, overwrite SendBuf[0..2] (header area)
                // Data offset = 5 + (ProductInfoNum-1)*8 = ProductInfoNum*8 - 3
                i = RemoteModbus.ProductInfoNum * 8 - 3;
                if(i < 50) RemoteModbus.SendBuf[0] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[0] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[1] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[1] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[2] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[2] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[3] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[3] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[4] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[4] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[5] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[5] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[6] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[6] = 0; i++;
                if(i < 50) RemoteModbus.SendBuf[7] = RemoteModbus.SendBuf[3 + i]; else RemoteModbus.SendBuf[7] = 0; i++;
            }

            RemoteModbus.ProductInfoNum++;
            SendNormalFrame();
            break;

        default:
            break;
    }

    RemoteModbus.nCRCErrorState = 0;
}

void SendNormalFrame(void)
{
    RemoteModbus.nLinTxData0 = ((Uint32)RemoteModbus.SendBuf[0] << 24) |
                               ((Uint32)RemoteModbus.SendBuf[1] << 16) |
                               ((Uint32)RemoteModbus.SendBuf[2] << 8)  |
                               ((Uint32)RemoteModbus.SendBuf[3]);
    RemoteModbus.nLinTxData1 = ((Uint32)RemoteModbus.SendBuf[4] << 24) |
                               ((Uint32)RemoteModbus.SendBuf[5] << 16) |
                               ((Uint32)RemoteModbus.SendBuf[6] << 8)  |
                               ((Uint32)RemoteModbus.SendBuf[7]);
}
