/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hw_otp.h
*                         TITLE:  Definitions for the OTP registers.
*******************************************************************************************/

#ifndef HW_OTP_H
#define HW_OTP_H

//*************************************************************************************************
//
// The following are defines for the OTP register offsets
//
//*************************************************************************************************
#define OTP_O_UID_PSRAND0    0x0U   // UID Psuedo-random 160 bit number
#define OTP_O_UID_PSRAND1    0x2U   // UID Psuedo-random 160 bit number
#define OTP_O_UID_PSRAND2    0x4U   // UID Psuedo-random 160 bit number
#define OTP_O_UID_PSRAND3    0x6U   // UID Psuedo-random 160 bit number
#define OTP_O_UID_PSRAND4    0x8U   // UID Psuedo-random 160 bit number
#define OTP_O_UID_PSRAND5    0xAU   // UID Psuedo-random 160 bit number
#define OTP_O_UID_UNIQUE     0xCU   // UID UID Unique 32 bit number
#define OTP_O_UID_CHECKSUM   0xEU   // UID Checksum




#endif
