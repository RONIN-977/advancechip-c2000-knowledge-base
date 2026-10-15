//#############################################################################
//
// FILE:   CANFunc.h
//
// TITLE:  eCAN mailbox helper interface.
//
// Description:
// This file declares the mailbox data structure, bootloader upgrade flag
// constants, and helper APIs used by the eCAN bootloader implementation.
//
//#############################################################################
// 
// $Copyright:
// Copyright (C) 2026 AdvChip Inc. All Rights Reserved
// ALL RIGHTS RESERVED
// $
//#############################################################################

#ifndef CANFUNC_H_INCLUDED
#define CANFUNC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                              INCLUDED MODULES                              */
/*============================================================================*/
#include <ADP32F03x_Device.h>
#include <ADP32F03x_ECan.h>

/*============================================================================*/
/*                         MACROS & CONSTANT DEFINES                          */
/*============================================================================*/
/* eCAN bit-rate selection */
#define CAN_BAUD_500K
//#define CAN_BAUD_250K
//#define CAN_BAUD_125K

/* Bootloader upgrade flag addresses */
#define upTureFlgAdd1  0x3F5F70
#define upTureFlgAdd2  0x3F5F71
#define upTureFlgAdd3  0x3F5F78
#define upTureFlgAdd4  0x3F5F79

/* Bootloader upgrade flag values */
#define upTureFlgVal1  0x55AA
#define upTureFlgVal2  0xAA55
#define upTureFlgVal3  0x1234
#define upTureFlgVal4  0x9876

/*============================================================================*/
/*                        REGISTERS & DATA TYPES                              */
/*============================================================================*/
//! Mailbox payload container used by the transmit helper API.
typedef struct __CanMailBox_MsgData_Struct {
	Uint32 mMailBox_DataL;
	Uint32 mMailBox_DataH;
} CanMailBox_MsgData_Struct;

/*============================================================================*/
/*                         GLOBAL DATA DECLARATIONS                           */
/*============================================================================*/
extern CanMailBox_MsgData_Struct CanMailBox_MsgData;

/*============================================================================*/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/*============================================================================*/

//*****************************************************************************
//
//! Initialize one eCAN receive mailbox.
//!
//! \param mailNo is the mailbox index to configure.
//! \param filterNo is the standard or extended identifier filter value.
//! \param filterType selects the identifier format. Use 0 for standard and
//! 1 for extended frames.
//! \param mTimout is the receive timeout reload value.
//!
//! \return None.
//
//*****************************************************************************
extern void CanReceive_MailBoxInit(Uint16 mailNo, Uint32 filterNo,
                                   Uint16 filterType, Uint32 mTimout);

//*****************************************************************************
//
//! Initialize one eCAN transmit mailbox.
//!
//! \param mailNo is the mailbox index to configure.
//! \param filterNo is the standard or extended identifier used by the
//! transmit mailbox.
//! \param filterType selects the identifier format. Use 0 for standard and
//! 1 for extended frames.
//! \param msgNum is the number of data bytes configured for the mailbox.
//!
//! \return None.
//
//*****************************************************************************
extern void CanTransmit_MailBoxInit(Uint16 mailNo, Uint32 filterNo,
                                    Uint16 filterType, Uint16 msgNum);

//*****************************************************************************
//
//! Load data into one transmit mailbox and request transmission.
//!
//! \param mChanNo is the mailbox index used for transmission.
//! \param msgData points to the mailbox payload container.
//!
//! \return None.
//
//*****************************************************************************
extern void CanTranMsg(Uint16 mChanNo, CanMailBox_MsgData_Struct *msgData);

//*****************************************************************************
//
//! Clear the payload data of one mailbox.
//!
//! \param mailNo is the mailbox index to clear.
//!
//! \return None.
//
//*****************************************************************************
extern void CanMsgDataClr(Uint16 mailNo);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* CANFUNC_H_INCLUDED */

/*============================================================================*/
/*                                 END OF FILE                                */
/*============================================================================*/
