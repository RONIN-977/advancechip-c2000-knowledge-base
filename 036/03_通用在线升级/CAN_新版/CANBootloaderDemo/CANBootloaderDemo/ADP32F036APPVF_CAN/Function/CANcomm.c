//#############################################################################
//
// FILE:   CANcomm.c
//
// TITLE:  eCAN communication support functions.
//
// Description:
// This file implements mailbox interrupt handling, eCAN controller
// initialization, receive queue helpers, and periodic CAN service routines.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include "CANcomm.h"
#include "MotorInclude.h"
#include "Parameter.h"

/*============================================================================*/
/*                          GLOBAL DATA DEFINITIONS                           */
/*============================================================================*/
CanRecv_MsgData_Struct     CanRecv_MsgData_Buffer;
CanMailBox_MsgData_Struct  CanMailBox_MsgData;
Mailbox_ReadyBits          CanMailBoxReadyFlag;
const unsigned int  *VarAddr[28] = {0};

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Handle the eCAN mailbox interrupt.
//!
//! This interrupt service routine processes mailbox receive, transmit-complete,
//! and timeout conditions, clears the corresponding flags, and defers payload
//! processing until after the interrupt is acknowledged.
//!
//! \return None.
//
//*****************************************************************************
interrupt void CanMailBox_IsrSrv(void)
{
    struct ECAN_REGS mCanCtrlStatusShadow;
    //struct MBOX *pBoxEntry;
    volatile struct MBOX *pBoxEntry;

    Uint16 IntMailNo;
    Uint16 mMailBoxRecvFlag = 0x00;

    DINT;

    // Step 1. Read the interrupt status.
    mCanCtrlStatusShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;

    IntMailNo = mCanCtrlStatusShadow.CANGIF1.bit.MIV1;
    pBoxEntry = &ECanaMboxes.MBOX0 + IntMailNo;

    CanMailBoxReadyFlag.all  |= ECanaRegs.CANTA.all;

    mCanCtrlStatusShadow.CANTA.all = ECanaRegs.CANTA.all;
    ECanaRegs.CANTA.all = mCanCtrlStatusShadow.CANTA.all;

    if(mCanCtrlStatusShadow.CANGIF1.bit.GMIF1 == 0x1)
    {
        mCanCtrlStatusShadow.CANRMP.all = ECanaRegs.CANRMP.all;
		mCanCtrlStatusShadow.CANTA.all   = ECanaRegs.CANTA.all;
        if(((mCanCtrlStatusShadow.CANRMP.all >> IntMailNo) & 1) == 0x1)
        {
            mCanCtrlStatusShadow.CANRMP.all |= (1 << IntMailNo);
            ECanaRegs.CANRMP.all = mCanCtrlStatusShadow.CANRMP.all;
            mMailBoxRecvFlag = 1;
        }
    }
    else if(mCanCtrlStatusShadow.CANGIF1.bit.MTOF1 == 0x1)  // ³¬Ê±ÖÐ¶Ï
    {
        mCanCtrlStatusShadow.CANTOS.all = ECanaRegs.CANTOS.all;
        mCanCtrlStatusShadow.CANTOS.all |= (1 << IntMailNo);
        ECanaRegs.CANTOS.all = mCanCtrlStatusShadow.CANTOS.all;
    }

    // Step 4. Clear the global interrupt flag.
    ECanaRegs.CANGIF1.all = mCanCtrlStatusShadow.CANGIF1.all;

    EINT;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    // Step 5. Process the received frame outside the critical interrupt path.
    if(mMailBoxRecvFlag == 1) {
        CanMsgData_Push(pBoxEntry);
        mMailBoxRecvFlag = 0;
    }
}

//*****************************************************************************
//
//! Configure the GPIO pins used by eCAN-A.
//!
//! This function enables the CAN pin pull-ups, selects asynchronous input
//! qualification for CANRX, and maps GPIO30 and GPIO31 to the CAN peripheral.
//!
//! \return None.
//
//*****************************************************************************
void InitECanaGpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;

    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;

    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;
    EDIS;
}

//*****************************************************************************
//
//! Initialize the eCAN-A controller core.
//!
//! This function configures the CAN I/O control bits, clears the mailbox
//! control state, programs the CAN bit timing registers, and leaves all
//! mailboxes disabled until mailbox assignment is completed.
//!
//! \return None.
//
//*****************************************************************************
void InitECana_1(void)
{
    struct ECAN_REGS ECanaShadow;
    volatile struct MBOX *TmpMailbox = (struct MBOX *)0x00;
    //struct MBOX *TmpMailbox = (struct MBOX *)0x00;
    int   iLoop = 0x00;

    EALLOW;
    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SCB = 1;
    ECanaShadow.CANMC.bit.ABO = 0;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaRegs.CANME.all = 0x00;

    TmpMailbox = &ECanaMboxes.MBOX0;
    for (iLoop = 0x00; iLoop < MAILBOX_LENGTH; iLoop++)
    {
        TmpMailbox = TmpMailbox + iLoop;
        TmpMailbox->MSGCTRL.all = 0x00;
    }

    ECanaRegs.CANTA.all   = 0xFFFFFFFF;
    ECanaRegs.CANRMP.all  = 0xFFFFFFFF;
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 1 ;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1);

    ECanaShadow.CANBTC.all = 0;

#ifdef CAN_BAUD_250K

    ECanaShadow.CANBTC.bit.BRPREG = 9;
    ECanaShadow.CANBTC.bit.SJWREG = 3;
    ECanaShadow.CANBTC.bit.TSEG2REG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 14;

#endif

// Bit time = [1 + (TSEG1REG + 1) + (TSEG2REG + 1)] *
//            [(BRPREG + 1) / CAN_CLK_FREQ]
// TSEG1REG must be greater than or equal to TSEG2REG.
// SJWREG is typically set to TSEG2REG or TSEG2REG - 1.

#ifdef CAN_BAUD_500K

    ECanaShadow.CANBTC.bit.BRPREG = 4;
    ECanaShadow.CANBTC.bit.SJWREG = 3;
    ECanaShadow.CANBTC.bit.TSEG2REG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 14;

#endif

    ECanaShadow.CANBTC.bit.SAM = 0;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );
    ECanaRegs.CANME.all = 0;
    EDIS;
}

//*****************************************************************************
//
//! Configure the eCAN interrupt sources.
//!
//! This function routes the ECAN1INTA vector, enables mailbox interrupt lines,
//! and turns on the CAN error and bus-off interrupt sources required by the
//! application.
//!
//! \return None.
//
//*****************************************************************************
void CAN_Init_Interrupt(void)
{
    struct ECAN_REGS ECanaShadow;

    EALLOW;

    PieVectTable.ECAN1INTA= &CanMailBox_IsrSrv;
    EDIS;

    EALLOW;
    ECanaShadow.CANMIL.all = ECanaRegs.CANMIL.all;
    ECanaShadow.CANMIL.all = 0xFFFFFFFF;
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;


    ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all;
    ECanaShadow.CANMIM.all = 0xFFFFFFFF;
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

    ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all;
    ECanaShadow.CANGIM.all =0XFFFFFFFF;
    ECanaShadow.CANGIM.bit.GIL = 0;
    ECanaShadow.CANGIM.bit.BOIM = 1;
    ECanaShadow.CANGIM.bit.EPIM = 1;
    ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;

    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;      // Enable INT 9.6 in the PIE

    EDIS;
}

//*****************************************************************************
//
//! Initialize the complete CAN communication module.
//!
//! This function configures the CAN GPIO pins, initializes the controller,
//! assigns the mailbox identifiers and modes, enables the selected mailboxes,
//! and then enables CAN interrupt handling.
//!
//! \return None.
//
//*****************************************************************************
void CANInit()
{
    struct  ECAN_REGS  ECanaShadow;

    InitECanaGpio();
    InitECana_1();

    // Configure the transmit mailboxes.
    ECanaMboxes.MBOX3.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX3.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX3.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX3.MSGID.bit.STDMSGID = 0x492;

    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX3.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX3.MSGCTRL.bit.TPL = 2;

    ECanaMboxes.MBOX31.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX31.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX31.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX31.MSGID.bit.STDMSGID = 0x481;

    ECanaMboxes.MBOX31.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX31.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX31.MSGCTRL.bit.TPL = 3;

    // Configure the receive mailboxes.
    ECanaMboxes.MBOX12.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX12.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX12.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX12.MSGID.bit.STDMSGID = 0x491;

    ECanaMboxes.MBOX15.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX15.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX15.MSGID.bit.IDE  = 1;
    ECanaMboxes.MBOX15.MSGID.bit.STDMSGID   = (0x1F1F1F1F >> 18) & 0x7FF;
    ECanaMboxes.MBOX15.MSGID.bit.EXTMSGID_H = (0x1F1F1F1F >> 16) & 0x3;
    ECanaMboxes.MBOX15.MSGID.bit.EXTMSGID_L = (0x1F1F1F1F & 0xFFFF);


    ECanaMboxes.MBOX16.MSGID.bit.AAM = 1;
    ECanaMboxes.MBOX16.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX16.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX16.MSGID.bit.STDMSGID = 0x181;

    ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 1;
    ECanaMboxes.MBOX16.MSGCTRL.bit.TPL = 0;

    ECanaMboxes.MBOX17.MSGID.bit.AAM = 1;
    ECanaMboxes.MBOX17.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX17.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX17.MSGID.bit.STDMSGID = 0x281;

    ECanaMboxes.MBOX17.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX17.MSGCTRL.bit.RTR = 1;
    ECanaMboxes.MBOX17.MSGCTRL.bit.TPL = 0;

    ECanaMboxes.MBOX18.MSGID.bit.AAM = 1;
    ECanaMboxes.MBOX18.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX18.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX18.MSGID.bit.STDMSGID = 0x381;

    ECanaMboxes.MBOX18.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX18.MSGCTRL.bit.RTR = 1;
    ECanaMboxes.MBOX18.MSGCTRL.bit.TPL = 0;
 
    ECanaShadow.CANME.all = 0x80079008;
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    ECanaShadow.CANMD.all = 0x00009000;
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    CanMailBoxReadyFlag.all = 0xFFFFFFFF;

    CAN_Init_Interrupt();  // Configure CAN receive interrupts
    Init_CanMsgData();
}

//*****************************************************************************
//
//! Initialize the CAN receive ring buffer.
//!
//! This function clears the receive queue count and resets both the head and
//! tail indexes to the start of the buffer.
//!
//! \return None.
//
//*****************************************************************************
void Init_CanMsgData(void)
{
    CanRecv_MsgData_Buffer.mCount = 0x00;
    CanRecv_MsgData_Buffer.mHeader = 0x00;
    CanRecv_MsgData_Buffer.mTailer = 0x00;
}

//*****************************************************************************
//
//! Push a received CAN frame into the ring buffer.
//!
//! \param msgBoxData points to the source mailbox register image containing
//! the received identifier, control, and payload words.
//!
//! This function stores one received mailbox entry into the software queue
//! when free space is available.
//!
//! \return None.
//
//*****************************************************************************
//void CanMsgData_Push(struct  MBOX *msgBoxData)
void CanMsgData_Push(volatile  struct  MBOX *msgBoxData)
{
    if(CanRecv_MsgData_Buffer.mCount < CANRECV_BOXDATA_LENGTH)
    {
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].mMsgId = msgBoxData->MSGID.all;
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].mMsgCtrl = msgBoxData->MSGCTRL.all;
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].CanDataLow = msgBoxData->MDL.all;
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].CanDataHigh = msgBoxData->MDH.all;

        CanRecv_MsgData_Buffer.mTailer = CanRecv_MsgData_Buffer.mTailer + 1;
        if(CanRecv_MsgData_Buffer.mTailer >= CANRECV_BOXDATA_LENGTH)
        {
            CanRecv_MsgData_Buffer.mTailer = 0;
        }
        CanRecv_MsgData_Buffer.mCount = CanRecv_MsgData_Buffer.mCount + 1;
    }
}

//*****************************************************************************
//
//! Pop one CAN frame from the ring buffer.
//!
//! This function returns the next queued CAN message entry when data is
//! available, or a null pointer when the queue is empty.
//!
//! \return A pointer to the next queued message entry, or null when empty.
//
//*****************************************************************************
CanMsgData_Entry* CanMsgData_Pop(void)
{
    CanMsgData_Entry *pMsgEntry;
    if (CanRecv_MsgData_Buffer.mCount <= 0x00)
    {
        return (CanMsgData_Entry *)0x00;
    }
    pMsgEntry = (CanMsgData_Entry *)&(CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mHeader]);
    CanRecv_MsgData_Buffer.mHeader = CanRecv_MsgData_Buffer.mHeader + 1;
    if(CanRecv_MsgData_Buffer.mHeader >= CANRECV_BOXDATA_LENGTH)
    {
        CanRecv_MsgData_Buffer.mHeader = 0;
    }
    CanRecv_MsgData_Buffer.mCount = (CanRecv_MsgData_Buffer.mCount - 1);
    return pMsgEntry;
}

//*****************************************************************************
//
//! Run the 2 ms CAN service task.
//!
//! This function handles bus-off recovery, dequeues one received CAN frame,
//! interprets the application-specific identifiers, and executes the matching
//! response or boot jump action.
//!
//! \return None.
//
//*****************************************************************************
void CanTransmit2ms(void)
{
    CanMsgData_Entry *mCanMsgEntry;
    struct ECAN_REGS  ECanaShadow;
    struct MBOX RxCanMassage;
    //struct MBOX TxCanMassage;

    if( ECanaRegs.CANGIF0.bit.BOIF0 == 1)
    {
        ECanaRegs.CANTRR.bit.TRR1 = 1;
        ECanaRegs.CANTRR.bit.TRR2 = 1;
        ECanaRegs.CANGIF0.bit.BOIF0 = 1;

        EALLOW;
        ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
        ECanaShadow.CANMC.bit.CCR = 0;
        EDIS;
    }

    if (CanRecv_MsgData_Buffer.mCount <= 0 )
    {
        return;
    }
    mCanMsgEntry = CanMsgData_Pop();

    RxCanMassage.MSGID.all   = mCanMsgEntry->mMsgId;
    RxCanMassage.MDL.all = mCanMsgEntry->CanDataLow;
    RxCanMassage.MDH.all = mCanMsgEntry->CanDataHigh;


    if(RxCanMassage.MSGID.bit.STDMSGID == 0x491)
    {

    }
    else if((RxCanMassage.MSGID.bit.STDMSGID == ((0x1F1F1F1F >> 18) & 0x7FF))
        &&(RxCanMassage.MSGID.bit.EXTMSGID_H == ((0x1F1F1F1F >> 16) & 0x3))
        &&(RxCanMassage.MSGID.bit.EXTMSGID_L == (0x1F1F1F1F & 0xFFFF)))
    {
        if((ECanaMboxes.MBOX15.MDL.all == 0x00AA00AA) && (ECanaMboxes.MBOX15.MDH.all == 0x00AA00AA))
        {
            DisableDrive();
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            DINT;
            asm(" MOVL XAR7, #0x3F7F7E ");
            asm(" LB *XAR7 ");
            //EnableDog();
            //while(1);
        }
    }
}

//*****************************************************************************
//
//! Fill and transmit the remote acknowledge frame.
//!
//! This function prepares the fixed debug or telemetry payload for mailbox 31
//! and requests transmission when the mailbox is available.
//!
//! \return None.
//
//*****************************************************************************
void CanRemoteAckDataFill(void)
{
    struct MBOX TxCanMassage;
    struct ECAN_REGS  ECanaShadow;
    Uint16 CHData1, CHData2, CHData3, CHData4;

    if(CanMailBoxReadyFlag.bit.Mailbox31 == 1)
    {
        CHData1 = 0x1111;
        CHData2 = 0x2222;
        CHData3 = 0xabcd;
        CHData4 = 0xaaaa;
        TxCanMassage.MDL.byte.BYTE0 = (CHData1 >> 8) & 0xFF;
        TxCanMassage.MDL.byte.BYTE1 = CHData1 & 0xFF;
        TxCanMassage.MDL.byte.BYTE2 = (CHData2 >> 8) & 0xFF;
        TxCanMassage.MDL.byte.BYTE3 = CHData2 & 0xFF;

        TxCanMassage.MDH.byte.BYTE4 = (CHData3 >> 8) & 0xFF;
        TxCanMassage.MDH.byte.BYTE5 = CHData3 & 0xFF;
        TxCanMassage.MDH.byte.BYTE6 = (CHData4 >> 8) & 0xFF;
        TxCanMassage.MDH.byte.BYTE7 = CHData4 & 0xFF;

        ECanaMboxes.MBOX31.MDL.all = TxCanMassage.MDL.all;
        ECanaMboxes.MBOX31.MDH.all = TxCanMassage.MDH.all;

        ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
        ECanaShadow.CANTRS.bit.TRS31 = 1;
        ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
        CanMailBoxReadyFlag.bit.Mailbox31 = 0;
    }
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
