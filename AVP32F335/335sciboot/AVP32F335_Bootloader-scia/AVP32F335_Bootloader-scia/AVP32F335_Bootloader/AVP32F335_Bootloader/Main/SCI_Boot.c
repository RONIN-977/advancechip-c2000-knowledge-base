//#############################################################################
/*<FH>*************************************************************************
* Copyright (c) :  2021-2031, Advancechip Co., Ltd. All Rights Reserved
* Original Author: pei.shen

* FileName: SCI_Boot.c
* Current Version: V1.0
* Date: 21-Aug-2021

* ABSTRACT:  the SCI Function Initialize, the device is AVP32F335
* DESCRIPTION:

* Revise:
;   Last rev by:    ***
;   Last rev date:  ***
;   Last rev desc.: ***
**<FH>************************************************************************/

//-----------------------------------------------------------------------------
// Include

#include "MainInclude.h"


//-----------------------------------------------------------------------------
// Public Variable Definitions

TRemoteModbus   RemoteModbus;
FLASH_ST        FlashStatus;


static void SendNormalFrame(void);

static void ReveiveBufferFrame(void);

static Uint16 Boot_CRC16_ByteCalc(const unsigned char *data, Uint16 length);

/*******************************************************************************
Sci_Initialize -This function initializes the SCI(s) to a known state.


Paramters:


Precondition:
-
-
Postcondition:
-
-
-
-

*******************************************************************************/

void Sci_Initialize(void)
{

    EALLOW;
    /* SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   // SCI-B
   GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0; //Enable pull-up for GPIO22 (SCITXDB)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0; //Enable pull-up for GPIO23 (SCIRXDB)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3; //Configure GPIO22 to SCITXDB
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3; //Configure GPIO23 for SCIRXDB*/
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0; //Enable pull-up for GPIO22 (SCITXDB)
     GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0; //Enable pull-up for GPIO23 (SCIRXDB)
     GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO23 (SCIRXDB)
     GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1; //Configure GPIO22 to SCITXDB
     GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1; //Configure GPIO23 for SCIRXDB
    EDIS;


    //配置中断接收
    EALLOW;
    PieVectTable.SCIRXINTA  = &SCI_RXD_isr;         //通讯接收中断
    PieVectTable.SCITXINTA   = &SCI_TXD_isr;         //通讯发送中断
    EDIS;

    IER |= (M_INT9);  //  Enable interrupts:
    // Enable SCIRX, SCITX interrupt...
   /* PieCtrlRegs.PIEIER9.bit.INTx3 = 1;           // PIE Group 9, INT3
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;           // PIE Group 9, INT4*/
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;           // PIE Group 9, INT1
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;           // PIE Group 9, INT2
    EINT;
    ERTM;

    // SCI Register setup... ...
    // 0000 0111
    SciaRegs.SCICCR.all = 0x0007;        // 1 stop bit,  No loopback
                                         // No parity,8 char bits,
                                         // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;       // enable TX, RX, internal SCICLK,
                                         // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    //////////////////////////////////
    // Baud rate, LSPCLK = 150MHZ/4 = 37.5MHz
    // BRR = LSPCLK/(Baudrate*8)-1
    // Baudrate  // BRR      //SCIHBAUD   // SCIHBAUD
    // 2400     // 780      // 0x03      //   0x0c
    // 4800     // 390      // 0x01      //   0x86
    // 9600     // 195      // 0x00      //   0xc3
    // 19200    // 97       // 0x00      //   0x61
    // 38400    // 48       // 0x00      //   0x30
    // 194000   // 9        // 0x00      //   0x09
    // 115200   // 39       // 0x00      //   0x27
    // 921600   // 1        //  0        //   0x01
    //////////////////////////////////
    SciaRegs.SCIHBAUD = 0x00;
    SciaRegs.SCILBAUD = 0x27;

    SciaRegs.SCICTL1.all = 0x0023;          // Relinquish SCI from Reset
    SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}


const Uint16 Boot_CRC16Table[256] =
{
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
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,
};

const unsigned char CRC8Boot_TAB[256] =
{
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};


unsigned char Boot_crc8_calc(unsigned char * buf, Uint32 len)
{
    unsigned char i;
    unsigned char crc8 = 0;
    while (len--)
    {
        i = crc8 ^ (*buf++);
        crc8 = CRC8Boot_TAB[i];
    }
    return crc8;
}

Uint16 Boot_CRC16_ByteCalc(const unsigned char *data, Uint16 length)
{
    Uint16 result = 0;
    Uint16 tableNo  = 0;
    Uint16 i;

    for(i=0; i < length; i++)
    {
        tableNo = ((result ^ data[i])& 0xff);
        result = (result >> 8) ^ Boot_CRC16Table[tableNo];
    }

    return result;
}


void RemoteModbus_Initialize(void)
{
    //
    memset(&RemoteModbus, 0, sizeof(RemoteModbus));

    RemoteModbus.RecvState = RECEIVE_STATE;
    RemoteModbus.nBootVersion = 200;
    RemoteModbus.nAppJumpFlag = 1;
}

// SCI TXD Interrupt function
interrupt void SCI_TXD_isr(void)
{
    // Is there still byte to transmit?
    if (RemoteModbus.TxBufferIndex < RemoteModbus.TransmitLength) {

        // Yes. Transmit next byte.
        SciaRegs.SCITXBUF = RemoteModbus.SendBuf[RemoteModbus.TxBufferIndex];

        RemoteModbus.TxBufferIndex++;
    } else {
        // No. End transmission
        // Reset the index.
        RemoteModbus.TxBufferIndex = 0;
    }

    // Clear SCI Interrupt flag
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;  // Clear SCI Interrupt flag

    // Issue PIE ACK
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
}

// Interrupt SCIRXINTA - Interrupt group 9 - Priority 13

interrupt void SCI_RXD_isr(void)
{
    unsigned short Temp;
    Temp = SciaRegs.SCIRXBUF.all;

    // Check the error flag.
    if (SciaRegs.SCIRXST.bit.RXERROR) {

        // There has been a receive error. Reset the periphery to clear the error.
        SciaRegs.SCICTL1.bit.SWRESET = 0;
        SciaRegs.SCICTL1.bit.SWRESET = 1;

    } else {
        RemoteModbus.ReceiveBuf[RemoteModbus.RxBufferIndex] = Temp & 0xFF;

        RemoteModbus.RxBufferIndex++;
        if (RemoteModbus.RxBufferIndex >= REMOTE_MSG_BUFF_SIZE) {
            RemoteModbus.RxBufferIndex = 0;
        }
    }

    // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;

    // Clear Interrupt flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;

    // Issue PIE ack
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
}


void StartTransmit(void)
{
    // Must set TransmitLength and Index BEFORE send byte.
    // If the byte is sent first and an interrupt occurs before setting TransmitLength and Index
    // the wrong number of byte can be sent.
    // Store the transmission length.
    // Set index to 1 as the first byte was transmitted here.
    RemoteModbus.TxBufferIndex  = 1;

    // Start transmission
    // 要使能发送就是对发送寄存器写一个字符即可进入中断。
    SciaRegs.SCITXBUF   = RemoteModbus.SendBuf[0];
}



void SCI_Boot(void)
{
    RemoteModbus_MsgProcess();
}


void RemoteModbus_MsgProcess(void)
{
    static Uint16 IndexZm1;
    static Uint16 Counter;

    switch (RemoteModbus.RecvState) {
        // In the receive state the software look for the silent period define by TIMEOUT
        case RECEIVE_STATE:
            // 4.8K baud rate, so 9 characters and 1-1-8, 9*10*/4.8 = 18.7ms frame,
            // LENGTH_RECEIVE_FRAME = 14,
            // one byte ~ 2ms
            // 0.5 tick, then 0.5*4 = 2 bytes
            if (RemoteModbus.RxBufferIndex > 0) {

                // If the current index and last next are the same then no more characters have been
                // received.
                if (RemoteModbus.RxBufferIndex == IndexZm1) {
                    Counter++;
                    if (Counter >= 2) {

                        // The TIMEOUT has been reach therefore a complete frame has arrived.
                        RemoteModbus.RecvState = FRAME_RECEIVED_STATE;
                        Counter = 0;
                    }
                } else {
                    Counter = 0;
                }
            } else {
                Counter = 0;
            }
            IndexZm1 = RemoteModbus.RxBufferIndex;

        break;

        case FRAME_RECEIVED_STATE: {
            // Reveive data from the buffer and convert it to internal units and then put to trammit state
            ReveiveBufferFrame();

            // reset the index after frame is processed. this level 2ms, after reseting then c
            RemoteModbus.RxBufferIndex = 0;
            RemoteModbus.RecvState  = PERPARD_TO_TRANSMIT_STATE;
        }
        break;

        case PERPARD_TO_TRANSMIT_STATE:
            // Enable transmit so disable recive data
            // ScibRegs.SCICTL1.bit.RXENA = 0;
            RemoteModbus.RecvState = START_TRANSMIT;
        break;

        case START_TRANSMIT:
            //Modbus_Tx_Enable;
            SendNormalFrame();
            RemoteModbus.RecvState = TRANSMIT_STATE;
        break;

        case TRANSMIT_STATE:
            // No. End transmission
            // Reset the index.
            if (RemoteModbus.TxBufferIndex == 0) {

                // Return to receive state
                RemoteModbus.RecvState  = CHECK_FOR_LAST_CHARACTER;
            }
        break;

        case CHECK_FOR_LAST_CHARACTER:
            Counter++;
            if (Counter >= 2) {
                //Modbus_Rx_Enable;
                //ScibRegs.SCICTL1.bit.RXENA = 1;
                Counter = 0;
                memset(&RemoteModbus.ReceiveBuf,0,sizeof(RemoteModbus.ReceiveBuf));
                RemoteModbus.RecvState = RECEIVE_STATE;
            }
        break;

        default:
        break;
    }
}



void ReveiveBufferFrame(void)
{
    unsigned char CRCLSB;
    unsigned char CRCMSB;
    unsigned char CheckCRC8Sum;
    Uint16 nCRCCheckSum,Status,i,txErrFlag;
    Uint16 Status_Verify;


    // start with 0xCC
    if (RemoteModbus.ReceiveBuf[0] != REMOTE_ADDR) {
        RemoteModbus.nCRCErrorState = 1;
        return;
    }

    // length
    RemoteModbus.nRxLength = (((RemoteModbus.ReceiveBuf[2]<<8) & 0xFF00) | (RemoteModbus.ReceiveBuf[1] & 0x00FF))>>3;
    //HeaderCRC8 SOF和Version/Length三个字节的头校验
    CheckCRC8Sum = Boot_crc8_calc(RemoteModbus.ReceiveBuf, 3);
    if(CheckCRC8Sum != RemoteModbus.ReceiveBuf[3]){
        RemoteModbus.nCRCErrorState = 2;
        return;
    }

    //LSB在前 MSB在后 CRC return : 0xD56A, then MSB = 0XD5, LSB = 0x6A
    nCRCCheckSum = Boot_CRC16_ByteCalc(RemoteModbus.ReceiveBuf, RemoteModbus.nRxLength-2);
    CRCLSB =  (unsigned char)(nCRCCheckSum & 0x00FF);
    CRCMSB =  (unsigned char)(nCRCCheckSum >> 8);

    if ((CRCMSB != RemoteModbus.ReceiveBuf[RemoteModbus.nRxLength-1]) || (CRCLSB != RemoteModbus.ReceiveBuf[RemoteModbus.nRxLength-2])) {
        RemoteModbus.nCRCErrorState = 3;
        return;
    }

    // Devicetype
    //RemoteModbus.DeviceType = boot_rec_buf[4];
    // CMD
    RemoteModbus.nAppCmdID = (((RemoteModbus.ReceiveBuf[6]<<8) & 0xff00) | (RemoteModbus.ReceiveBuf[5] & 0x00ff));
    //序列号
    RemoteModbus.nBootSeqNum = (((RemoteModbus.ReceiveBuf[8]<<8) & 0xff00) | (RemoteModbus.ReceiveBuf[7] & 0x00ff));

    switch(RemoteModbus.nAppCmdID)
    {
       case SBE_COM_GetVersion:

           RemoteModbus.SendDataIndex = 9;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;      //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;      //协议版本号

           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;


           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=(RemoteModbus.nBootVersion&0x000000FF);        //BOOT 版本
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=((RemoteModbus.nBootVersion>>8)&0x000000FF);
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=((RemoteModbus.nBootVersion>>16)&0x000000FF);
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=((RemoteModbus.nBootVersion>>24)&0x000000FF);


           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=0;        //BOOT 版本
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=0;

           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;    //12 bite

       break;

       case SBE_COM_RebootDevice:

           //tx data
           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;    //12 bite
       break;

       case SBE_COM_SetSN:

           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;    //12 bite

       break;

       //升级程序
       case SBE_COM_RequstUpdate:

           RemoteModbus.nFirmwareSize = ((Uint32)RemoteModbus.ReceiveBuf[12]<<24|(Uint32)RemoteModbus.ReceiveBuf[11]<<16|(Uint32)RemoteModbus.ReceiveBuf[10]<<8|(Uint32)RemoteModbus.ReceiveBuf[9]);

           //tx data
           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           if((RemoteModbus.nFirmwareSize < 100000)&&(RemoteModbus.nBootFlashErase == 0)){

               DINT;DRTM;
               Status = Flash_Erase((SECTORH),&FlashStatus);

               if(Status == STATUS_SUCCESS)
               {
                  RemoteModbus.nBootFlashErase = 1;
                  RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
               }else{
                  RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
               }
               EINT;ERTM;
           }else{
               RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           }

           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0x80;   //
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex + 2;    //12 bite

       break;

       case SBE_COM_IntBoot:  //通用回答

           //tx data
           RemoteModbus.nAppJumpFlag = 0;
           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;    //12 bite

       break;

       case SBE_COM_CheckState:

           //tx data
           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 1;   //0-app NO Update   1-boot YES Update
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;         //14 bite

       break;

       case SBE_COM_SendFirmware:

           RemoteModbus.nPackIndex = ((Uint32)RemoteModbus.ReceiveBuf[12]<<24 | (Uint32) RemoteModbus.ReceiveBuf[11]<<16 | RemoteModbus.ReceiveBuf[10]<<8 | RemoteModbus.ReceiveBuf[9]);

           RemoteModbus.nFirmwareLen = (((RemoteModbus.ReceiveBuf[14]<<8) & 0xFF00) | (RemoteModbus.ReceiveBuf[13] & 0x00FF))/2;

           RemoteModbus.nOffsetAddr = (unsigned long)RemoteModbus.nPackIndex * RemoteModbus.nFirmwareLen;

           RemoteModbus.nFlashAPIAddr =  RemoteModbus.nOffsetAddr + nAppStartAddr;

           if(RemoteModbus.nFlashAPIAddr < nSectorAStart)
           {
              for(i = 0; i < RemoteModbus.nFirmwareLen; i++)
              {
                  RemoteModbus.nFlashBuffer[i]=(RemoteModbus.ReceiveBuf[2*i+16]<<8)+(RemoteModbus.ReceiveBuf[2*i+15]);
              }
              Status = Flash_Program((Uint16*)(RemoteModbus.nFlashAPIAddr),(Uint16*)RemoteModbus.nFlashBuffer, (Uint32)RemoteModbus.nFirmwareLen, &FlashStatus);

              Status_Verify = Flash_Verify((Uint16*)RemoteModbus.nFlashAPIAddr,(Uint16*)RemoteModbus.nFlashBuffer,(Uint32)RemoteModbus.nFirmwareLen, &FlashStatus);

              if((Status == STATUS_SUCCESS)||(Status_Verify == STATUS_SUCCESS))
              {
                  txErrFlag = 0;
              }
              else{
                  txErrFlag = 1;
              }
           }
           else{
               txErrFlag = 0;     //超过APP的程序不做跟新处理
           }

           //tx data
           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = txErrFlag;   //0-app NO Update   1-boot YES Update

           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=(RemoteModbus.nPackIndex&0x000000FF);          //包序列号
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=((RemoteModbus.nPackIndex>>8)&0x000000FF);
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=((RemoteModbus.nPackIndex>>16)&0x000000FF);
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++]=((RemoteModbus.nPackIndex>>24)&0x000000FF);

           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;         //14 bite

       break;

       case SBE_COM_EndUpdate:

           // MD5 固件校验码[0-15]

           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;    //0-success 3-md5 fail 5-other
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;           //12 bite

       break;

       case SBE_COM_RestartDevice:

           //tx data

           RemoteModbus.nAppJumpFlag = 1;

           RemoteModbus.SendDataIndex = 9;
           //0-success  1-fail
           RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = 0;    //0-success 1-fail
           RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex+2;           //12 bite

       break;

       default: //01 收包有错
       break;
    }

    // Reset the counter
    RemoteModbus.nCRCErrorState = 0;
}


void SendNormalFrame(void)
{
    Uint16  nCRCTxCheckSum;

    RemoteModbus.SendBuf[0] = REMOTE_ADDR;

    //txlengh
    RemoteModbus.SendBuf[1] = (unsigned char)((RemoteModbus.TransmitLength<<3) & 0x00FF);
    RemoteModbus.SendBuf[2] = (unsigned char)((RemoteModbus.TransmitLength<<3) >> 8);
    //Header CRC-8
    RemoteModbus.SendBuf[3] = Boot_crc8_calc(RemoteModbus.SendBuf,3);
    //[7]:1 [3]:1 [0]:1  ACK bump
    RemoteModbus.SendBuf[4] = 0x89;

    //Cmd  AppCmdID
    RemoteModbus.SendBuf[5] = (unsigned char)(RemoteModbus.nAppCmdID & 0x00FF);
    RemoteModbus.SendBuf[6] = (unsigned char)(RemoteModbus.nAppCmdID >> 8);

    //SeqNum   BootSeqNum
    RemoteModbus.SendBuf[7] = (unsigned char)(RemoteModbus.nBootSeqNum & 0x00FF);
    RemoteModbus.SendBuf[8] = (unsigned char)(RemoteModbus.nBootSeqNum >> 8);

    if(RemoteModbus.nCRCErrorState != 0){
        RemoteModbus.SendDataIndex = 9;
        //0-success  1-fail
        RemoteModbus.SendBuf[RemoteModbus.SendDataIndex ++] = RemoteModbus.nCRCErrorState;
        RemoteModbus.TransmitLength = RemoteModbus.SendDataIndex +2;    //12 bite
    }
    //crc-16 IBM
    nCRCTxCheckSum = Boot_CRC16_ByteCalc(RemoteModbus.SendBuf, RemoteModbus.TransmitLength-2);
    RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = (unsigned char)(nCRCTxCheckSum & 0x00FF);
    RemoteModbus.SendBuf[RemoteModbus.SendDataIndex++] = (unsigned char)(nCRCTxCheckSum >> 8);

    // Start the transmission.

    // Set index to 1 as the first byte was transmitted here.
    RemoteModbus.TxBufferIndex  = 1;

    // Start transmission
    // 要使能发送就是对发送寄存器写一个字符即可进入中断。
    SciaRegs.SCITXBUF = RemoteModbus.SendBuf[0];
}

