/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   flashapi_ex2_sciKernel.c
*                         TITLE:  SCI Flash Kernel Example for F0049
*******************************************************************************************/


//
// Includes
//
#include <string.h>
#include "flashapi_ex2_erase.h"
#include "flashapi_ex2_flash_programming_c28.h"
#include "F021_F28004x_C28x.h"
#include "flashapi_ex2_bootrom.h"
#include "device.h"
#include "flash.h"
#include "gpio.h"
#include "driverlib.h"

//
// Function Prototypes
//
void exampleError(Fapi_StatusType status);
void initFlashSectors(void);
extern uint32_t sciGetFunction(uint32_t  BootMode);
extern void sciaFlush(void);

//
// main - This is an example code demonstrating F021 Flash API usage.
//        This code is in Flash
//

uint32_t main(void)
{
    //
    // flush SCIA TX port by waiting while it is busy, driverlib.
    //
    sciaFlush();

    //
    // initialize device and GPIO, driverlib.
    //
    Device_init();
    Device_initGPIO();

    //
    // init interrupt and vectorTable, drivelib.
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // initialize flash_sectors, fapi + driverlib
    //
    initFlashSectors();

    uint32_t EntryAddr;

    //
    // parameter SCI_BOOT for GPIO28 (RX),29 (TX) is default.
    //
    EntryAddr = sciGetFunction(SCI_BOOT);
    return(EntryAddr);
}


//
// initFlashSectors - Initializes the flash API
//
void initFlashSectors(void)
{
    EALLOW;

    Fapi_StatusType oReturnCheck;
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 100);
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
// Functions are not placed in RAM for the Live DFU configurations
//
#ifndef LIVE_UPDATE

#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(exampleError,".TI.ramfunc");
#endif

#endif

//
// Example_Error - Error function that will halt debugger
//
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
