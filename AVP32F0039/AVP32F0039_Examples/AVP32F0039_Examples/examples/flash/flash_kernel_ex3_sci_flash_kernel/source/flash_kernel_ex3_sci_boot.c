/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flashapi_ex3_sci_boot.c
*                         TITLE: SCI Boot Mode Routines
*******************************************************************************************/

//
// Includes
//
#include "cpu1bootrom.h"
#include "flash_kernel_ex3_commands.h"
#include "driverlib.h"
#include "Types.h"

//
// Globals
//
typedef struct
{
   uint16_t status;
   uint32_t address;
   uint16_t flashAPIError;
   uint32_t flashAPIFsmStatus;
}  StatusCode;
extern StatusCode statusCode;

//
// getWordData is a pointer to the function that interfaces to the peripheral.
// Each loader assigns this pointer to it's particular getWordData function.
//
extern uint16fptr getWordData;

//
// Function Prototypes 
//
extern uint16_t sciaGetWordData(void);
extern void copyData(void);
uint32_t getLongData(void);
extern void readReservedFn(void);
extern void exampleError(Fapi_StatusType status);
uint32_t sciBootApplication(uint32_t BootMode);

//
// uint32_t sciBootApplication - This module is the main SCI boot routine.
//                    It will load code via the SCI-A port.
//                    It will return a entry point address back
//                    to the InitBoot routine which in turn calls
//                    the ExitBoot routine.
//
uint32_t sciBootApplication(uint32_t BootMode)
{
    statusCode.status = NO_ERROR;
    statusCode.address = 0x12346578;
    statusCode.flashAPIError = NO_ERROR;
    statusCode.flashAPIFsmStatus = 0;

    uint32_t EntryAddr;

    //
    // Assign getWordData to the SCI-A version of the
    // function; getWordData is a pointer to a function.
    //
    getWordData = sciaGetWordData;

    //
    // If the KeyValue was invalid, abort the load
    // and return the flash entry point.
    //
    if (sciaGetWordData() != 0x08AA)
    {
        statusCode.status = VERIFY_ERROR;
        statusCode.address = FLASH_ENTRY_POINT;
        statusCode.flashAPIError = NO_ERROR;
        statusCode.flashAPIFsmStatus = 0;
    }

    //
    // reads and discards 8 reserved words
    //
    readReservedFn(); 

    EntryAddr = getLongData();

    copyData();

    uint16_t x = 0;
    for(x = 0; x < 32676; x++){}
    for(x = 0; x < 32676; x++){}

    return EntryAddr;
}

//
// End of file
//
