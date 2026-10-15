
/****************************************************************
文件功能：变频部分与主机接口函数
文件版本：V1.0
作者        ：徐中领
最新更新： 
****************************************************************/

#include "Define.h"
#include "DSP2803x_Device.h"
#include "Main.h"
#include "MotorInclude.h"

#include "RunSrc.h"
#include "LinLDF.h"
#include "RemoteModbus.h"

///////////////////////////////////////////////////////////////////////////////////////
// File scope Definition and constant
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// File scope function.
///////////////////////////////////////////////////////////////////////////////////////
#ifndef LIN_CONTROL_MODE
void SendNormalFrame(void);
void ReveiveData(void);
void StartTransmit(void);
#endif

///////////////////////////////////////////////////////////////////////////////////////
// File scope data.
///////////////////////////////////////////////////////////////////////////////////////
#ifndef LIN_CONTROL_MODE
enum T_STATES {
	RECEIVE_STATE,
	FRAME_RECEIVED_STATE,
	TRANSMIT_STATE,
	CHECK_FOR_LAST_CHARACTER,
	PERPARD_TO_TRANSMIT_STATE,
	START_TRANSMIT
};

int LENGTH_TRANSMIT_FRAME = 8;
int LENGTH_RECEIVE_FRAME  = 20;

unsigned short TxBufferIndex = 0;
unsigned short RxBufferIndex = 0;
unsigned short TransmitLength;
unsigned short ReciveDataIndex;
unsigned short SendDataIndex;
unsigned int ReceiveBuf[19];
unsigned int SendBuf[19];
#endif

//#define LIN_DATA_TEST    1
#ifdef LIN_DATA_TEST
int InterruptCnt[11];
#endif
TRemoteModbus   RemoteModbus;
Uint16 LinRxFlag = 0;
Uint16 rdataA[8];
Uint32 LinL0IntVect;
Uint32 FrameIncorrectCnt;
interrupt void LINA_LEVEL_isr(void)
{
    // OFFSET                           FUNCTION
    // 4                                ID
    // 11                               Receive
    // 12                               Send
    // 13、10、9、8、7、6、5、3、2          error
    ////////////////////////////////////////////////////////
    // ID       PID         Length      FUNCTION
    // 0x36     0x76        7           Send TO Master
    // 0x31     0xB1        7           Receive From Master
    // 无论主机是发送数据和接收数据，都会有前面一段同步+ID 帧，这段结束都会产生一个中断。
    // 如果是发送数据会紧接着收到另外一个中断....
    /////////////////////////////////////////////////////////////////////////////

    LinL0IntVect = LinaRegs.SCIINTVECT0.all;
    if (LinL0IntVect == 4)                              // ID Data
    {
        if (LinaRegs.LINID.bit.RECEIVEDID == 0x76)      // ID:0x36
        {
            UpdateLinDataToSent();
            LinaRegs.SCIFORMAT.bit.LENGTH   = 7;
            LinaRegs.LINTD0.all             = SenddDataA.all;
            LinaRegs.LINTD1.all             = SenddDataB.all;
        }
        //boot
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0x64) //ID:0x24
        {
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
            LinaRegs.LINTD1.all = 0;
            LinaRegs.LINTD0.all = 0;
            RemoteModbus.TxBufferIndex = 1;
        }
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0xB1)  //ID:0x31
        {
            LinaRegs.SCIFORMAT.bit.LENGTH   = 2;
        }
    }
    else if (LinL0IntVect == 11)
    {
        if (LinaRegs.LINID.bit.RECEIVEDID == 0xB1)      //ID:0x31
        {
            ReciveDataIDCorrect[0]       = LinaRegs.LINRD0.bit.RD0;
            ReciveDataIDCorrect[1]       = LinaRegs.LINRD0.bit.RD1;
            ReciveDataIDCorrect[2]       = LinaRegs.LINRD0.bit.RD2;
            UpdateRecivedLinDataToVars();
        }
        //boot
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0xA3) //ID:0x23
        {
            rdataA[0] = LinaRegs.LINRD0.bit.RD0;
            rdataA[1] = LinaRegs.LINRD0.bit.RD1;
            rdataA[2] = LinaRegs.LINRD0.bit.RD2;
            rdataA[3] = LinaRegs.LINRD0.bit.RD3;
            rdataA[4] = LinaRegs.LINRD1.bit.RD4;
            rdataA[5] = LinaRegs.LINRD1.bit.RD5;
            rdataA[6] = LinaRegs.LINRD1.bit.RD6;
            rdataA[7] = LinaRegs.LINRD1.bit.RD7;

            RemoteModbus.ReceiveBuf[0] = rdataA[0] & 0xFF;
            RemoteModbus.ReceiveBuf[1] = rdataA[1] & 0xFF;
            RemoteModbus.ReceiveBuf[2] = rdataA[2] & 0xFF;
            RemoteModbus.ReceiveBuf[3] = rdataA[3] & 0xFF;
            RemoteModbus.ReceiveBuf[4] = rdataA[4] & 0xFF;
            RemoteModbus.ReceiveBuf[5] = rdataA[5] & 0xFF;
            RemoteModbus.ReceiveBuf[6] = rdataA[6] & 0xFF;
            RemoteModbus.ReceiveBuf[7] = rdataA[7] & 0xFF;

            if(RemoteModbus.ReceiveBuf[0] == 0xCC)
            {
                RemoteModbus.RxBufferIndex+=8;
                LinRxFlag= 1;
            }
            else
            {
                RemoteModbus.RxBufferIndex = 0;
            }

        }
        else
        {
            ReciveDataIDInCorrect[0]     = LinaRegs.LINRD0.bit.RD0;
            ReciveDataIDInCorrect[1]     = LinaRegs.LINRD0.bit.RD1;
            ReciveDataIDInCorrect[2]     = LinaRegs.LINRD0.bit.RD2;
            ReciveDataIDInCorrect[3]     = LinaRegs.LINRD0.bit.RD3;
            ReciveDataIDInCorrect[4]     = LinaRegs.LINRD1.bit.RD4;
            ReciveDataIDInCorrect[5]     = LinaRegs.LINRD1.bit.RD5;
            ReciveDataIDInCorrect[6]     = LinaRegs.LINRD1.bit.RD6;
            ReciveDataIDInCorrect[7]     = LinaRegs.LINRD1.bit.RD7;
        }
#ifdef LIN_DATA_TEST
    } else if ((LinL0IntVect == 13) |
               (LinL0IntVect == 10) |
               (LinL0IntVect == 9 ) |
               (LinL0IntVect == 8 ) |
               (LinL0IntVect == 6 ) |
               (LinL0IntVect == 5 ) |
               (LinL0IntVect == 3 ) |
               (LinL0IntVect == 2 ))
    {
        InterruptCnt[0]                  = LinL0IntVect;
#else
    } else {
#endif
        FrameIncorrectCnt++;
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

#ifndef LIN_CONTROL_MODE
// SCI TXD Interrupt function
interrupt void SCI_TXD_isr(void)
{
	// Is there still byte to transmit?
	if (TxBufferIndex < TransmitLength) {

		// Yes. Transmit next byte.
		SciaRegs.SCITXBUF 	= SendBuf[TxBufferIndex];

		TxBufferIndex++;
	} else {
		// No. End transmission
		// Reset the index.
		TxBufferIndex		= 0;
	}

	// Clear SCI Interrupt flag
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1;

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
		ReceiveBuf[RxBufferIndex] = Temp & 0xFF;

		RxBufferIndex++;
		if (RxBufferIndex >= LENGTH_RECEIVE_FRAME) {
			RxBufferIndex = 0;
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
	TransmitLength		= LENGTH_TRANSMIT_FRAME;

	// Set index to 1 as the first byte was transmitted here.
	TxBufferIndex		= 1;

	// Start transmission
	// 要使能发送就是对发送寄存器写一个字符即可进入中断。
	SciaRegs.SCITXBUF	= SendBuf[0];
}
#endif

void CommsProtocalLayerTaskBG(void)
{

}
void SendNormalFrame(void)
{
    RemoteModbus.nLinTxData0 = ((Uint32)RemoteModbus.ReceiveBuf[0]<<24
                               | (Uint32)RemoteModbus.ReceiveBuf[1]<<16
                               | RemoteModbus.ReceiveBuf[2]<<8
                               | RemoteModbus.ReceiveBuf[3]);
    RemoteModbus.nLinTxData1 = ((Uint32)RemoteModbus.ReceiveBuf[4]<<24
                               | (Uint32) RemoteModbus.ReceiveBuf[5]<<16
                               | RemoteModbus.ReceiveBuf[6]<<8
                               | RemoteModbus.ReceiveBuf[7]);
}
void ReveiveBufferFrame(void)
{
    // CMD
    RemoteModbus.nAppCmdID = (RemoteModbus.ReceiveBuf[2] & 0x00ff);
    //序列号
    RemoteModbus.nBootSeqNum = (((RemoteModbus.ReceiveBuf[4]<<8) & 0xff00) | (RemoteModbus.ReceiveBuf[3] & 0x00ff));
//GpioDataRegs.GPATOGGLE.bit.GPIO19  = 1;
    switch(RemoteModbus.nAppCmdID)
    {
       //升级程序
       case SBE_COM_RequstUpdate:
           SendNormalFrame();
           gDSPActiveTime.flash_IAP_flag = 1;

       break;

       default: //01 收包有错
       break;
    }
}
/***************************************************************
 Function: CommsProtocalLayerTask2ms;
 Description: Update data recived from the buffer and prepare the data
              to sent. Put it to the background to Reduce interrupt
              execution time
 Call by: functions CommsProtocalLayerTask2ms which is in 2ms main loop;
 Input Variables: N/A
 Output/Return Variables: N/A
 Subroutine Call: N/A;
 Reference: N/A
****************************************************************/
void CommsProtocalLayerTask2ms(void)
{
    static unsigned char IndexZm1;

    switch (RemoteModbus.RecvState) {
        case RECEIVE_STATE:
            if(LinRxFlag == 1){

                RemoteModbus.nAppCmdID = (RemoteModbus.ReceiveBuf[2] & 0x00ff);
                IndexZm1 = 7;
                if (RemoteModbus.RxBufferIndex >= IndexZm1)
                {
                    RemoteModbus.RxBufferIndex = 0;
                    RemoteModbus.RecvState = FRAME_RECEIVED_STATE;
                }
            }
        break;

        case FRAME_RECEIVED_STATE: {
            ReveiveBufferFrame();
            RemoteModbus.RxBufferIndex = 0;
            LinRxFlag = 0;
            RemoteModbus.RecvState = RECEIVE_STATE;
        }
        break;

        default:
        break;
    }
}
