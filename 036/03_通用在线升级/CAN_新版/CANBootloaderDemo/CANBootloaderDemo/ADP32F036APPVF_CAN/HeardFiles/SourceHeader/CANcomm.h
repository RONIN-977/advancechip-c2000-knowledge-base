//#############################################################################
//
// FILE:   CANcomm.h
//
// TITLE:  CAN communication shared declarations.
//
// Description:
// This file declares the mailbox data structures, shared buffers, and public
// CAN communication interfaces used by the application.
//
//#############################################################################
//
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef CANCOMM_H_
#define CANCOMM_H_

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include "ADP32F03x_Device.h"
#include "ADP32F03x_ECan.h"

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
#define CAN_BAUD_500K
//#define CAN_BAUD_250K

#define MAILBOX_LENGTH              (sizeof(struct MBOX))
#define CANRECV_BOXDATA_LENGTH      8

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/
typedef struct __CanMailBox_MsgData_Struct {
    Uint32   mMailBox_DataL;
    Uint32   mMailBox_DataH;
} CanMailBox_MsgData_Struct;

typedef struct __CanMsgData_Entry {
    Uint32 mMsgId;
    Uint32 mMsgCtrl;
    Uint32 CanDataLow;
    Uint32 CanDataHigh;
} CanMsgData_Entry;

typedef struct __CanRecv_MsgData_Struct {
    int  mHeader;
    int  mTailer;
    int  mCount;
    CanMsgData_Entry mCanMsg_Entry[CANRECV_BOXDATA_LENGTH];
} CanRecv_MsgData_Struct;

typedef union
{
    Uint32 all;
    struct
    {
        Uint32  Mailbox0:1;
        Uint32  Mailbox1:1;
        Uint32  Mailbox2:1;
        Uint32  Mailbox3:1;
        Uint32  Mailbox4:1;
        Uint32  Mailbox5:1;
        Uint32  Mailbox6:1;
        Uint32  Mailbox7:1;
        Uint32  Mailbox8:1;
        Uint32  Mailbox9:1;
        Uint32  Mailbox10:1;
        Uint32  Mailbox11:1;
        Uint32  Mailbox12:1;
        Uint32  Mailbox13:1;
        Uint32  Mailbox14:1;
        Uint32  Mailbox15:1;
        Uint32  Mailbox16:1;
        Uint32  Mailbox17:1;
        Uint32  Mailbox18:1;
        Uint32  Mailbox19:1;
        Uint32  Mailbox20:1;
        Uint32  Mailbox21:1;
        Uint32  Mailbox22:1;
        Uint32  Mailbox23:1;
        Uint32  Mailbox24:1;
        Uint32  Mailbox25:1;
        Uint32  Mailbox26:1;
        Uint32  Mailbox27:1;
        Uint32  Mailbox28:1;
        Uint32  Mailbox29:1;
        Uint32  Mailbox30:1;
        Uint32  Mailbox31:1;
    } bit;
} Mailbox_ReadyBits;

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern CanRecv_MsgData_Struct     CanRecv_MsgData_Buffer;
extern CanMailBox_MsgData_Struct  CanMailBox_MsgData;
extern Mailbox_ReadyBits          CanMailBoxReadyFlag;

extern const unsigned int *VarAddr[];
extern unsigned int RealTimeDataSendFlag;
extern unsigned int RunSignal;

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/
extern void CANInit(void);
extern void Init_CanMsgData(void);
extern CanMsgData_Entry *CanMsgData_Pop(void);
//extern void CanMsgData_Push(struct MBOX *msgBoxData);
extern void CanMsgData_Push(volatile struct MBOX *msgBoxData);
extern void HandleCanRecveMsg(void);
extern void CAN_Init_Interrupt(void);
extern void CanTransmit2ms(void);
extern void CanRemoteAckDataFill(void);

#endif /* CANCOMM_H_ */

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
