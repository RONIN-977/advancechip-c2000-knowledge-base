/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   C28x_FPU_FastRTS.h
*                         TITLE:  Prototypes and Definitions for the C28x Fast RTS Library
*******************************************************************************************/

#ifndef C28X_FPU_FAST_RTS_H
#define C28X_FPU_FAST_RTS_H


#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Standard C28x Data Types
//-----------------------------------------------------------------------------


#ifndef DSP28_DATA_TYPES
#define DSP28_DATA_TYPES
typedef int                 int16;
typedef long                int32;
typedef long long           int64;
typedef unsigned int        Uint16;
typedef unsigned long       Uint32;
typedef unsigned long long  Uint64;
typedef float               float32;
typedef long double         float64;
#endif


//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

float32 isqrtf(float32 X);
void sincos(float32 radian, float32* PtrSin, float32* PtrCos);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of C28X_FPU_FAST_RTS_H

//===========================================================================
// End of file.
//===========================================================================
