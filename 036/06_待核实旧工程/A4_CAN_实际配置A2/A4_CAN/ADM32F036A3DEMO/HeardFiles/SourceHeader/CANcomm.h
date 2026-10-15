/*
 * CANcomm.h
 *
 *  Created on: 2016-07-18
 *      Author: BYTEK Lee
 */

#ifndef CANCOMM_H_
#define CANCOMM_H_

#include "ADP32F03x_Device.h"
#include "ADP32F03x_ECan.h"

#define   CAN_BAUD_500K
//#define   CAN_BAUD_250K

#define   MAILBOX_LENGTH  (sizeof(struct MBOX))
#define   CANRECV_BOXDATA_LENGTH  8


typedef  struct  __CanMailBox_MsgData_Struct {
	Uint32   mMailBox_DataL;
	Uint32   mMailBox_DataH;
}CanMailBox_MsgData_Struct;


typedef  struct __CanMsgData_Entry {
	Uint32 mMsgId;
	Uint32 mMsgCtrl;
	Uint32 CanDataLow;
	Uint32 CanDataHigh;
}CanMsgData_Entry;

typedef struct __CanRecv_MsgData_Struct {
	int  mHeader;
	int  mTailer;
	int  mCount;
	CanMsgData_Entry mCanMsg_Entry[CANRECV_BOXDATA_LENGTH];
}CanRecv_MsgData_Struct;

typedef union
{
    Uint32 all;
    struct
    {                           // bit  description
        Uint32  Mailbox0:1;     // 0   RMP for Mailbox 0
        Uint32  Mailbox1:1;     // 1   RMP for Mailbox 1
        Uint32  Mailbox2:1;     // 2   RMP for Mailbox 2
        Uint32  Mailbox3:1;     // 3   RMP for Mailbox 3
        Uint32  Mailbox4:1;     // 4   RMP for Mailbox 4
        Uint32  Mailbox5:1;     // 5   RMP for Mailbox 5
        Uint32  Mailbox6:1;     // 6   RMP for Mailbox 6
        Uint32  Mailbox7:1;     // 7   RMP for Mailbox 7
        Uint32  Mailbox8:1;     // 8   RMP for Mailbox 8
        Uint32  Mailbox9:1;     // 9   RMP for Mailbox 9
        Uint32  Mailbox10:1;    // 10  RMP for Mailbox 10
        Uint32  Mailbox11:1;    // 11  RMP for Mailbox 11
        Uint32  Mailbox12:1;    // 12  RMP for Mailbox 12
        Uint32  Mailbox13:1;    // 13  RMP for Mailbox 13
        Uint32  Mailbox14:1;    // 14  RMP for Mailbox 14
        Uint32  Mailbox15:1;    // 15  RMP for Mailbox 15
        Uint32  Mailbox16:1;    // 16  RMP for Mailbox 16
        Uint32  Mailbox17:1;    // 17  RMP for Mailbox 17
        Uint32  Mailbox18:1;    // 18  RMP for Mailbox 18
        Uint32  Mailbox19:1;    // 19  RMP for Mailbox 19
        Uint32  Mailbox20:1;    // 20  RMP for Mailbox 20
        Uint32  Mailbox21:1;    // 21  RMP for Mailbox 21
        Uint32  Mailbox22:1;    // 22  RMP for Mailbox 22
        Uint32  Mailbox23:1;    // 23  RMP for Mailbox 23
        Uint32  Mailbox24:1;    // 24  RMP for Mailbox 24
        Uint32  Mailbox25:1;    // 25  RMP for Mailbox 25
        Uint32  Mailbox26:1;    // 26  RMP for Mailbox 26
        Uint32  Mailbox27:1;    // 27  RMP for Mailbox 27
        Uint32  Mailbox28:1;    // 28  RMP for Mailbox 28
        Uint32  Mailbox29:1;    // 29  RMP for Mailbox 29
        Uint32  Mailbox30:1;    // 30  RMP for Mailbox 30
        Uint32  Mailbox31:1;    // 31  RMP for Mailbox 31
    }bit;
}Mailbox_ReadyBits;

extern  CanRecv_MsgData_Struct     CanRecv_MsgData_Buffer;
extern  CanMailBox_MsgData_Struct  CanMailBox_MsgData;
extern  Mailbox_ReadyBits          CanMailBoxReadyFlag;

extern const unsigned int  *VarAddr[];
extern unsigned int RealTimeDataSendFlag;
extern unsigned int RunSignal;

extern  void  CANInit(void);
extern  void  Init_CanMsgData(void);
extern  CanMsgData_Entry *CanMsgData_Pop(void);
extern  void  CanMsgData_Push(struct  MBOX *msgBoxData);
extern  void  HandleCanRecveMsg(void);
extern  void  CAN_Init_Interrupt(void);
extern  void  CanTransmit2ms(void);
extern  void  CanRemoteAckDataFill(void);
extern  void  CANTransceiverEnable(void);

#endif /* CANCOMM_H_ */
