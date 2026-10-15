/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   f28003x_otp.h
*                         TITLE:  Definitions for the OTP registers.
*******************************************************************************************/

#ifndef F28003X_OTP_H
#define F28003X_OTP_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// OTP Individual Register Bit Definitions:

struct UID_REGS {
    Uint32                                   UID_PSRAND0;                  // UID Psuedo-random 160 bit number
    Uint32                                   UID_PSRAND1;                  // UID Psuedo-random 160 bit number
    Uint32                                   UID_PSRAND2;                  // UID Psuedo-random 160 bit number
    Uint32                                   UID_PSRAND3;                  // UID Psuedo-random 160 bit number
    Uint32                                   UID_PSRAND4;                  // UID Psuedo-random 160 bit number
    Uint32                                   UID_PSRAND5;                  // UID Psuedo-random 160 bit number
    Uint32                                   UID_UNIQUE;                   // UID Unique 32 bit number
    Uint32                                   UID_CHECKSUM;                 // UID Checksum
};

//---------------------------------------------------------------------------
// OTP External References & Function Declarations:
//
extern volatile struct UID_REGS UidRegs;
#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif

//===========================================================================
// End of file.
//===========================================================================
