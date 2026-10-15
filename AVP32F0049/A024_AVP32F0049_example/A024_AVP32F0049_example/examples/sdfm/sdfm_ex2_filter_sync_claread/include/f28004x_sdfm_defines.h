/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   f28004x_sdfm_defines.h
*                         TITLE:  #defines used in sdfm examples
*******************************************************************************************/
#ifndef F28004X_SDFM_DEFINES_H
#define F28004X_SDFM_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Macro to read the SDFM1 filter data in 16 bit format
//
#define SDFM1_READ_FILTER1_DATA_16BIT     (*(volatile int16 *)((Uint16)0x5E17))
#define SDFM1_READ_FILTER2_DATA_16BIT     (*(volatile int16 *)((Uint16)0x5E27))
#define SDFM1_READ_FILTER3_DATA_16BIT     (*(volatile int16 *)((Uint16)0x5E37))
#define SDFM1_READ_FILTER4_DATA_16BIT     (*(volatile int16 *)((Uint16)0x5E47))

//
// Macro to read the SDFM1 filter data in 32 bit format
//
#define SDFM1_READ_FILTER1_DATA_32BIT    (*(volatile int32 *)((Uint16)0x5E16))
#define SDFM1_READ_FILTER2_DATA_32BIT    (*(volatile int32 *)((Uint16)0x5E26))
#define SDFM1_READ_FILTER3_DATA_32BIT    (*(volatile int32 *)((Uint16)0x5E36))
#define SDFM1_READ_FILTER4_DATA_32BIT    (*(volatile int32 *)((Uint16)0x5E46))

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of F28004X_SDFM_DEFINES_H

//
// End of file
//
