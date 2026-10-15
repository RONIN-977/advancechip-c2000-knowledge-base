/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hw_aes_ss.h
*                         TITLE:  Definitions for the AES_SS registers.
*******************************************************************************************/

#ifndef HW_AES_SS_H
#define HW_AES_SS_H

//*************************************************************************************************
//
// The following are defines for the AES_SS register offsets
//
//*************************************************************************************************
#define AES_SS_O_AES_GLB_INT_FLG   0x2U   // AES Global Interrupt Flag Register
#define AES_SS_O_AES_GLB_INT_CLR   0x4U   // AES Global Interrupt Clear Register


//*************************************************************************************************
//
// The following are defines for the bit fields in the AES_GLB_INT_FLG register
//
//*************************************************************************************************
#define AES_SS_AES_GLB_INT_FLG_INT_FLG   0x1U   // Global Interrupt Flag for AES Interrupt

//*************************************************************************************************
//
// The following are defines for the bit fields in the AES_GLB_INT_CLR register
//
//*************************************************************************************************
#define AES_SS_AES_GLB_INT_CLR_INT_FLG_CLR   0x1U   // Global Interrupt flag clear for AES
                                                    // Interrupt



#endif
