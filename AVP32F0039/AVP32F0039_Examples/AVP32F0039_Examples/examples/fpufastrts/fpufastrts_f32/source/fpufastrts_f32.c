/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  FPUfastRTS_Example.c
*                         TITLE: FPUfastRTS FPU32 Example
*******************************************************************************************/


//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

void main(void)
{
    //
    // CPU Initialization
    //
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();
    Board_init();
    C2000Ware_libraries_init();

    //
    // Parameter initialization
    //
    float32_t param0 = 0.5;
    float32_t param1 = 2.0;

    //
    // Arc Cosine
    //
    float32_t acosResult = acosf(param0);

    //
    // Arc Sine
    //
    param0 = 0.5;
    float32_t asinResult = asinf(param0);

    //
    // Arc Tangent
    //
    param0 = 0.5;
    float32_t atanResult = atanf(param0);

    //
    // Arc Tangent 2 (4-quadrant inverse tangent)
    //
    param0 = 1.0;
    float32_t atan2Result = atan2f(param0, param1);

    //
    // Cosine
    //
    param0 = 0.5;
    float32_t cosResult = cosf(param0);

    //
    // Division (uses '/' operator)
    //
    param0 = 1.0;
    float32_t divResult = param0/param1;

    //
    // Exponential
    //
    param0 = 1.0;
    float32_t expResult = expf(param0);

    //
    // Inverse Square Root
    //
    param0 = 2.0;
    float32_t isqrtResult = isqrtf(param0);

    //
    // Logarithm
    //
    param0 = 1233.2323;
    float32_t logResult = logf(param0);

    //
    // Power
    //
    param0 = 0.5;
    float32_t powResult = powf(param0, param1);

    //
    // Sine
    //
    param0 = 0.5;
    float32_t sinResult = sinf(param0);

    //
    // Square Root
    //
    param0 = 9.0;
    float32_t sqrtResult = sqrtf(param0);

    while(1);
}
