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

#include <C28x_FPU_FastRTS.h>
#include <fastrts.h>
#include "math.h"
#include <C28x_FPU_FastRTS.h>

//
//  Functions available for FPU32 configuration:
//
//  float32_t acosf(float32_t X)
//  float32_t asinf(float32_t X)
//  float32_t atanf(float32_t X)
//  float32_t atan2f(float32_t Y, float32_t X)
//  float32_t cosf(float32_t X)
//  float32_t FS$$DIV(float32_t X, float32_t Y); call using '/' operator
//  float32_t expf(float32_t X)
//  float32_t isqrtf(float32_t X)
//  float32_t logf(float32_t X)
//  float32_t powf(float32_t X, float32_t Y)
//  float32_t sinf(float32_t X)
//  float32_t sqrtf(float32_t X)
//
void FFR_init();


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
