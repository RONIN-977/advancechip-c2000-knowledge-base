/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   f28003x_aes_ss.h
*                         TITLE:  Definitions for the AES_SS registers.
*******************************************************************************************/

#ifndef F28003X_AES_SS_H
#define F28003X_AES_SS_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// AES_SS Individual Register Bit Definitions:

struct AES_GLB_INT_FLG_BITS {           // bits description
    Uint16 INT_FLG:1;                   // 0 Global Interrupt Flag for AES Interrupt
    Uint16 rsvd1:15;                    // 15:1 Reserved
    Uint16 rsvd2:16;                    // 31:16 Reserved
};

union AES_GLB_INT_FLG_REG {
    Uint32  all;
    struct  AES_GLB_INT_FLG_BITS  bit;
};

struct AES_GLB_INT_CLR_BITS {           // bits description
    Uint16 INT_FLG_CLR:1;               // 0 Global Interrupt flag clear for AES Interrupt
    Uint16 rsvd1:15;                    // 15:1 Reserved
    Uint16 rsvd2:16;                    // 31:16 Reserved
};

union AES_GLB_INT_CLR_REG {
    Uint32  all;
    struct  AES_GLB_INT_CLR_BITS  bit;
};

struct AES_SS_REGS {
    Uint16                                   rsvd1[2];                     // Reserved
    union   AES_GLB_INT_FLG_REG              AES_GLB_INT_FLG;              // AES Global Interrupt Flag Register
    union   AES_GLB_INT_CLR_REG              AES_GLB_INT_CLR;              // AES Global Interrupt Clear Register
};

//---------------------------------------------------------------------------
// AES_SS External References & Function Declarations:
//
#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif

//===========================================================================
// End of file.
//===========================================================================
