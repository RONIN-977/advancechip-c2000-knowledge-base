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
    FILTER_init();
}

void FILTER_init()
{
    myIIR0_init();
}

IIR_f32  myIIR0_obj;
IIR_f32_Handle myIIR0_handle = &myIIR0_obj;

void myIIR0_init()
{
    IIR_f32_setCoefficientsAPtr(myIIR0_handle, denCoeffs);
    IIR_f32_setCoefficientsBPtr(myIIR0_handle, numCoeffs);
	IIR_f32_setDelayLinePtr(myIIR0_handle, delayBuff);
	IIR_f32_setInputPtr(myIIR0_handle, inBuff);
    IIR_f32_setOutputPtr(myIIR0_handle, outBuff);
    IIR_f32_setScalePtr(myIIR0_handle, biquadPtr);
	IIR_f32_setOrder(myIIR0_handle, myIIR0_IIR_ORDER);
	IIR_f32_setInitFunction(myIIR0_handle, (v_pfn_v)IIR_f32_init);
	IIR_f32_setCalcFunction(myIIR0_handle, (v_pfn_v)IIR_f32_calc);

    myIIR0_handle->init(myIIR0_handle);
}


