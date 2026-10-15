/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   c2000ware_libraries.h
*                         TITLE:  
*******************************************************************************************/


#ifndef C2000WARE_LIBRARIES_H
#define C2000WARE_LIBRARIES_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "board.h"
#include <dsp.h>
#include <fpu32/fpu_filter.h>
#include <fpu32/fpu_rfft.h>

#define myIIR0_IIR_ORDER 6
extern IIR_f32_Handle myIIR0_handle;
extern float32_t *denCoeffs;
extern float32_t *numCoeffs;
extern float32_t *delayBuff;
extern float32_t *inBuff;
extern float32_t *outBuff;
extern float32_t *biquadPtr;

void myIIR0_init();



void FILTER_init();
void C2000Ware_libraries_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif
