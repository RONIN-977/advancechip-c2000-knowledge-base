/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flash_kernel_ex3_sci_flash_kernel.c
*                         TITLE: Flash Programming Solution using SCI.
*******************************************************************************************/

//
// Included Files
//

#include <string.h>
#include "flash_programming_f28003x.h" // Flash API example header file
#include "F021_F28003x_C28x.h"
#include "cpu1bootrom.h"
#include "device.h"
#include "flash.h"
#include "gpio.h"
#include "interrupt.h"
#include "driverlib.h"


//
// Function Prototypes
//
void exampleError(Fapi_StatusType status);
void initFlashSectors(void);
extern uint32_t sciGetFunction(uint32_t BootMode);
extern void sciaFlush(void);

//
// Main
//
uint32_t main(void)
{
//
// flush SCIA TX port by waiting while it is busy, driverlib.
//
    sciaFlush();
//
// Step 1. Initialize System Control:
// Enable Peripheral Clocks
//
    Device_init();
//
// Step 2. Initialize GPIO:
//
    Device_initGPIO();

//
// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
//
    DINT;

//
// init interrupt and vectorTable, driverlib.
//
    Interrupt_initModule();
    Interrupt_initVectorTable();


//
// Disable CPU interrupts and clear all CPU interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

   Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, 5);
    initFlashSectors();

    uint32_t EntryAddr;

//
// parameter SCI_BOOT for GPIO28,29
// refer to cpu1bootrom.h for alternate SCI Boot Pin configurations
//
    EntryAddr = sciGetFunction(SCI_BOOT);
    return(EntryAddr);
}

//
// initFlashSectors - Initialize flash API and active flash bank sectors
//
void initFlashSectors(void)
{
    EALLOW;
    Fapi_StatusType oReturnCheck;
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 120); 
    if(oReturnCheck != Fapi_Status_Success)
    {
        exampleError(oReturnCheck);
    }

    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    if(oReturnCheck != Fapi_Status_Success)
    {
        exampleError(oReturnCheck);
    }
    EDIS;
}

//
// exampleError - For this example, if an error is found just stop here
//
#ifdef __TI_COMPILER_VERSION__
    #if __TI_COMPILER_VERSION__ >= 15009000
        #pragma CODE_SECTION(exampleError,".TI.ramfunc");
    #else
        #pragma CODE_SECTION(exampleError,"ramfuncs");
    #endif
#endif
void exampleError(Fapi_StatusType status)
{
    //
    // Error code will be in the status parameter
    //
    __asm("    ESTOP0");
}

//
// End of file
//
