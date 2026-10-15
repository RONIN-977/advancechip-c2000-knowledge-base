/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  dsp_examples_setup.c
*                         TITLE: DSP Examples Common Code
*******************************************************************************************/


//*****************************************************************************
// the includes
//*****************************************************************************
#include "dsp_examples_setup.h"
#include "sysctl.h"
#include "fpu.h"


//*****************************************************************************
// the defines
//*****************************************************************************
#if defined(ADC_INPUT)
#define TEST_SIZE       (256U) //must be same as dsp_rfft_adc.c
                               //fpu32 examples, set to 512U
                               //fpu64 examples, set to 512U
#endif //defined(ADC_INPUT)

//*****************************************************************************
// the globals
//*****************************************************************************
#ifdef __cplusplus
#pragma DATA_SECTION("FpuRegsFile")
#else
#pragma DATA_SECTION(FpuRegs,"FpuRegsFile");
#endif
//! \brief FPU Register object
//
volatile struct FPU_REG FpuRegs;


// End of File
