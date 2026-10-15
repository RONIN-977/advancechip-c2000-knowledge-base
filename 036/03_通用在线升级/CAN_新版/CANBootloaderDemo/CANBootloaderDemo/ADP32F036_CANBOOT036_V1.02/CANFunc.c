//#############################################################################
//
// FILE:   CANFunc.c
//
// TITLE:  eCAN mailbox helper functions.
//
// Description:
// This file contains helper functions used to initialize eCAN mailboxes,
// send mailbox payloads, and clear mailbox data for the bootloader flow.
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
#include "CANFunc.h"

/*============================================================================*/
/*                            CODE IMPLEMENTATION                             */
/*============================================================================*/

//*****************************************************************************
//
//! Initialize one eCAN receive mailbox.
//!
//! \param mailNo is the mailbox index to configure.
//! \param filterNo is the standard or extended identifier filter value.
//! \param filterType selects the identifier format. Use 0 for standard and
//! 1 for extended frames.
//! \param mTimout is the receive timeout value written to the mailbox timeout
//! register.
//!
//! This function disables the selected mailbox, configures the receive ID
//! filter, enables timeout supervision, and then re-enables mailbox access.
//!
//! \return None.
//
//*****************************************************************************
void CanReceive_MailBoxInit(Uint16 mailNo, Uint32 filterNo, Uint16 filterType,
                            Uint32 mTimout)
{
    struct MBOX *pMailbox;
    struct ECAN_REGS ECanaShadow;
    struct MBOX mTmpMailBox;
    Uint32 *pMailMoTo;

    pMailMoTo = &ECanaMOTORegs.MOTO0 + mailNo;
    pMailbox = &ECanaMboxes.MBOX0 + mailNo;

    //
    // Disable the mailbox before updating its identifier and mode fields.
    //
    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= ~(1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    //
    // Enable timeout monitoring and program the timeout reload value.
    //
    ECanaShadow.CANTOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.all |= (1 << mailNo);
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTOC.all;
    *pMailMoTo = mTimout;

    //
    // Program the receive identifier for either standard or extended frames.
    //
    mTmpMailBox.MSGID.bit.AAM = 0;
    mTmpMailBox.MSGID.bit.AME = 0;
    if(filterType == 0)
    {
        mTmpMailBox.MSGID.bit.IDE = 0;
        mTmpMailBox.MSGID.bit.STDMSGID = filterNo;
    }
    else
    {
        mTmpMailBox.MSGID.bit.IDE = 1;
        mTmpMailBox.MSGID.bit.STDMSGID = (filterNo >> 18) & 0x7FF;
        mTmpMailBox.MSGID.bit.EXTMSGID_H = (filterNo >> 16) & 0x3;
        mTmpMailBox.MSGID.bit.EXTMSGID_L = filterNo & 0xFFFF;
    }
    pMailbox->MSGID.all = mTmpMailBox.MSGID.all;

    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all |= (1 << mailNo);
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    ECanaShadow.CANOPC.all = ECanaRegs.CANOPC.all;
    ECanaShadow.CANOPC.all |= (1 << mailNo);
    ECanaRegs.CANOPC.all = ECanaShadow.CANOPC.all;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= (1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}

//*****************************************************************************
//
//! Initialize one eCAN transmit mailbox.
//!
//! \param mailNo is the mailbox index to configure.
//! \param filterNo is the standard or extended identifier used by the
//! transmit mailbox.
//! \param filterType selects the identifier format. Use 0 for standard and
//! 1 for extended frames.
//! \param msgNum is the number of data bytes that will be transmitted.
//!
//! This function clears any pending transmit request, programs the mailbox
//! identifier and data length, and then enables the mailbox as a transmitter.
//!
//! \return None.
//
//*****************************************************************************
void CanTransmit_MailBoxInit(Uint16 mailNo, Uint32 filterNo, Uint16 filterType,
                             Uint16 msgNum)
{
    struct MBOX *pMailbox;
    struct ECAN_REGS ECanaShadow;
    struct MBOX mTmpMailBox;

    pMailbox = &ECanaMboxes.MBOX0 + mailNo;

    //
    // Abort any pending transfer request before reconfiguring the mailbox.
    //
    ECanaShadow.CANTRR.all = ECanaRegs.CANTRR.all;
    ECanaShadow.CANTRR.all |= (1 << mailNo);
    ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all;

    do
    {
        ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    } while(((ECanaShadow.CANTRS.all >> mailNo) & 1) == 1);

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= ~(1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    //
    // Program the transmit identifier and message control fields.
    //
    mTmpMailBox.MSGID.bit.AAM = 0;
    mTmpMailBox.MSGID.bit.AME = 0;
    if(filterType == 0)
    {
        mTmpMailBox.MSGID.bit.IDE = 0;
        mTmpMailBox.MSGID.bit.STDMSGID = filterNo;
    }
    else
    {
        mTmpMailBox.MSGID.bit.IDE = 1;
        mTmpMailBox.MSGID.bit.STDMSGID = (filterNo >> 18) & 0x7FF;
        mTmpMailBox.MSGID.bit.EXTMSGID_H = (filterNo >> 16) & 0x3;
        mTmpMailBox.MSGID.bit.EXTMSGID_L = filterNo & 0xFFFF;
    }
    mTmpMailBox.MSGCTRL.bit.DLC = msgNum;
    mTmpMailBox.MSGCTRL.bit.RTR = 0;
    mTmpMailBox.MSGCTRL.bit.TPL = 0;
    pMailbox->MSGID.all = mTmpMailBox.MSGID.all;
    pMailbox->MSGCTRL.all = mTmpMailBox.MSGCTRL.all;

    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all &= ~(1 << mailNo);
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= (1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}

//*****************************************************************************
//
//! Load data into one transmit mailbox and request transmission.
//!
//! \param mMailNo is the mailbox index used for transmission.
//! \param msgData points to the low and high 32-bit mailbox payload words.
//!
//! This function copies the payload words into the selected mailbox and then
//! sets the transmit request and acknowledge bits.
//!
//! \return None.
//
//*****************************************************************************
void CanTranMsg(Uint16 mMailNo, CanMailBox_MsgData_Struct *msgData)
{
    struct MBOX *Mailbox;
    struct ECAN_REGS ECanaShadow;

    Mailbox = &ECanaMboxes.MBOX0 + mMailNo;

    Mailbox->MDL.all = msgData->mMailBox_DataL;
    Mailbox->MDH.all = msgData->mMailBox_DataH;

    ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    ECanaShadow.CANTRS.all |= (1 << mMailNo);
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

    ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
    ECanaShadow.CANTA.all |= (1 << mMailNo);
    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
}

//*****************************************************************************
//
//! Clear the payload data of one mailbox.
//!
//! \param mailNo is the mailbox index to clear.
//!
//! This function temporarily disables the mailbox, clears both data words,
//! and then restores mailbox access.
//!
//! \return None.
//
//*****************************************************************************
void CanMsgDataClr(Uint16 mailNo)
{
    struct MBOX *pMailbox;
    struct MBOX MboxShadow;
    struct ECAN_REGS ECanaShadow;

    pMailbox = &ECanaMboxes.MBOX0 + mailNo;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= ~(1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    MboxShadow.MDH.all = 0x00000000;
    MboxShadow.MDL.all = 0x00000000;

    pMailbox->MDH.all = MboxShadow.MDH.all;
    pMailbox->MDL.all = MboxShadow.MDL.all;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= (1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/

