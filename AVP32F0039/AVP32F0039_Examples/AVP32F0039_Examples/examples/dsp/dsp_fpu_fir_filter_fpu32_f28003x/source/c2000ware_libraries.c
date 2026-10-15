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
    myFIR0_init();
}

FIR_f32 myFIR0_obj;
FIR_f32_Handle myFIR0_handle = &myFIR0_obj;

void myFIR0_init()
{
    FIR_f32_setCoefficientsPtr(myFIR0_handle, filPtr);
    FIR_f32_setDelayLinePtr(myFIR0_handle, delPtr);
	FIR_f32_setOrder(myFIR0_handle, myFIR0_FIR_ORDER);
	FIR_f32_setInitFunction(myFIR0_handle, (v_pfn_v)FIR_f32_init);
	FIR_f32_setCalcFunction(myFIR0_handle, (v_pfn_v)FIR_f32_calc);

    myFIR0_handle->init(myFIR0_handle);
}

