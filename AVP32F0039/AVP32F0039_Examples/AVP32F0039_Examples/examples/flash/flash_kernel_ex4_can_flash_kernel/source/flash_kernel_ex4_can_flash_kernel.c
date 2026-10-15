/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flash_kernel_ex4_can_flash_kernel.c
*                         TITLE: MCAN Flash Kernel Example for F28003x
*******************************************************************************************/

//
// Included Files
//
#include "cpu1bootrom.h"
#include "device.h"
#include "gpio.h"
#include "driverlib.h"

//
// Globals
//
extern uint32_t MCAN_Boot();

//
// Function Prototypes
//
void (*ApplicationPtr) (void);

//
// main - This is an example code demonstrating F021 Flash API usage.
//        This code is in RAM
//
uint32_t main(void)
{

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO
    //
    Device_initGPIO();

    //
    // Initialize MCAN and wait to recieve messages
    //
    return MCAN_Boot(MCAN_BOOT_SENDTEST, 0, 0, 120, 0, 1);
}

//
// End of File
//

