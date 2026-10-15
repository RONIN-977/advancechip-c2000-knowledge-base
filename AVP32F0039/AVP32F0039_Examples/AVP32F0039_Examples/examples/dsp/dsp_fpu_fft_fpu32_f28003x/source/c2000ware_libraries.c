/******************************* Copyright(C) *********************************************
*                          AVP32F0039_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   c2000ware_libraries.c
*                         TITLE:
*******************************************************************************************/

#include "c2000ware_libraries.h"


void C2000Ware_libraries_init()
{
    FFT_init();
}

void FFT_init()
{
    myCFFT0_init();
}

CFFT_F32_STRUCT myCFFT0_obj;
CFFT_F32_STRUCT_Handle myCFFT0_handle = &myCFFT0_obj;

void myCFFT0_init()
{
    CFFT_f32_setInputPtr(myCFFT0_handle, inPtr);
    CFFT_f32_setOutputPtr(myCFFT0_handle, outPtr);
	CFFT_f32_setStages(myCFFT0_handle, myCFFT0_CFFT_NUM_STAGES);
	CFFT_f32_setFFTSize(myCFFT0_handle, myCFFT0_CFFT_SIZE);
	CFFT_f32_setTwiddlesPtr(myCFFT0_handle, CFFT_f32_twiddleFactors);
}

