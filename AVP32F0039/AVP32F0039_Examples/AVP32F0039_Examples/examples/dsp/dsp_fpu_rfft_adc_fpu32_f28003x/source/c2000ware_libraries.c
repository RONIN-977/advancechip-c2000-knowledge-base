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
    myRFFT0_init();
}

RFFT_F32_STRUCT  myRFFT0_obj;
RFFT_F32_STRUCT_Handle myRFFT0_handle = &myRFFT0_obj;
RFFT_ADC_F32_STRUCT myRFFT0_adc_obj;
RFFT_ADC_F32_STRUCT_Handle myRFFT0_adc_handle = &myRFFT0_adc_obj;

void myRFFT0_init()
{
    RFFT_f32_setInputPtr(myRFFT0_handle, inPtr);
    RFFT_f32_setOutputPtr(myRFFT0_handle, outPtr);
	RFFT_f32_setStages(myRFFT0_handle, myRFFT0_RFFT_NUM_STAGES);
	RFFT_f32_setFFTSize(myRFFT0_handle, myRFFT0_RFFT_SIZE);
    RFFT_f32_setMagnitudePtr(myRFFT0_handle, magPtr);
    RFFT_f32_setPhasePtr(myRFFT0_handle, phasPtr);
	RFFT_f32_setTwiddlesPtr(myRFFT0_handle, RFFT_f32_twiddleFactors);
    RFFT_ADC_f32_setInBufPtr(myRFFT0_adc_handle, inADC);
    RFFT_ADC_f32_setTailPtr(myRFFT0_adc_handle, outPtr);
}


