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

#include <fpu32/fpu_cfft.h>
#include <fpu32/fpu_rfft.h>

#define myCFFT0_CFFT_NUM_STAGES 7
#define myCFFT0_CFFT_SIZE 128

extern CFFT_F32_STRUCT_Handle myCFFT0_handle;
extern float32_t *inPtr;
extern float32_t *outPtr;

void myCFFT0_init();




void FFT_init();
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
