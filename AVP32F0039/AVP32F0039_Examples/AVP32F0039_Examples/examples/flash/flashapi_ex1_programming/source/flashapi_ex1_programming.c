/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flashapi_ex1_programming.c
*                         TITLE: Flash programming example
*******************************************************************************************/


//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Include Flash API include file
//
#include "F021_F28003x_C28x.h"

// Include Flash API example header file
//
#include "flash_programming_f28003x.h"

//
// Defines
//

//
// Length (in 16-bit words) of data buffer used for program
//
#define  WORDS_IN_FLASH_BUFFER    0x100

//
// Globals
//

//
// Data Buffers used for program operation using the flash API program function
//
#pragma  DATA_SECTION(Buffer,"DataBufferSection");
uint16   Buffer[WORDS_IN_FLASH_BUFFER];
uint32   *Buffer32 = (uint32 *)Buffer;


//
// Prototype of the functions used in this example
//
void Example_Error(Fapi_StatusType status);
void Example_Done(void);
void Example_CallFlashAPI(void);
void FMSTAT_Fail(void);
void ECC_Fail(void);
void Example_EraseSector(void);

void Example_EraseBanks(void);
void Example_ProgramUsingAutoECC(void);
void Example_ProgramBankUsingAutoECC(void);
void Example_ProgramUsingDataOnlyECCOnly(void);
void Example_ProgramUsingDataAndECC(void);

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    // Copy the Flash initialization code from Flash to RAM
    // Copy the Flash API from Flash to RAM
    // Configure Flash wait-states, fall back power mode, performance features
    // and ECC
    //
    Device_init();

    //
    // Initialize GPIO
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // At 120MHz, execution wait-states for external oscillator is 5. Modify the
    // wait-states when the system clock frequency is changed.
    //
    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, 5);


    //
    // Flash API functions should not be executed from the same bank on which
    // erase/program operations are in progress.
    // Also, note that there should not be any access to the Flash bank on
    // which erase/program operations are in progress.  Hence below function
    // is mapped to RAM for execution.
    //
    Example_CallFlashAPI();


    //
    // Example is done here
    //
    Example_Done();
}


//*****************************************************************************
//  Example_CallFlashAPI
//
//  This function will interface to the flash API.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_CallFlashAPI, ".TI.ramfunc");
#endif
void Example_CallFlashAPI(void)
{
    uint16 i = 0;
    Fapi_StatusType  oReturnCheck;

    //
    // Initialize the Flash API by providing the Flash register base address
    // and operating frequency(in MHz).
    // This function is required to initialize the Flash API based on System
    // frequency before any other Flash API operation can be performed.
    // This function must also be called whenever System frequency or RWAIT is
    // changed.
    //
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 
                                      DEVICE_SYSCLK_FREQ/1000000U);

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for possible errors
        //
        Example_Error(oReturnCheck);
    }

    //
    // Initialize the Flash banks and FMC for erase and program operations.
    // Fapi_setActiveFlashBank() function sets the Flash banks and FMC for
    // further Flash operations to be performed on the banks.
    //
    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for possible errors
        //
        Example_Error(oReturnCheck);
    }

    //
    // Erase the sector before programming
    //
    Example_EraseSector();
    //
    // Fill a buffer with data to program into the flash.
    //
    for(i=0; i < WORDS_IN_FLASH_BUFFER; i++)
    {
        Buffer[i] = i;
    }

    //
    // Program the sector using AutoECC option
    //
    Example_ProgramUsingAutoECC();

    //
    // Erase the sector before programming
    //
    Example_EraseSector();

    //
    // Program the sector using DataOnly and ECCOnly options
    //
    Example_ProgramUsingDataOnlyECCOnly();

    //
    // Erase the sector before programming
    //
    Example_EraseSector();

    //
    // Program the sector using DataAndECC option
    //
    Example_ProgramUsingDataAndECC();

    //
    // Erase the sector for cleaner exit from the example.
    //
    Example_EraseSector();

    //
    // Program Bank 1 using AutoECC option
    //
    Example_ProgramBankUsingAutoECC();

    //
    // Erase bank 1 before programming
    //
    Example_EraseBanks();

}

//*****************************************************************************
//  Example_ProgramUsingAutoECC
//
//  Example function to Program data in Flash using "AutoEccGeneration" option.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_ProgramUsingAutoECC, ".TI.ramfunc");
#endif
void Example_ProgramUsingAutoECC(void)
{
    uint32 u32Index = 0;
    uint16 i = 0;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    //
    // A data buffer of max 8 16-bit words can be supplied to the program
    // function.
    // Each word is programmed until the whole buffer is programmed or a
    // problem is found. However to program a buffer that has more than 8
    // words, program function can be called in a loop to program 8 words for
    // each loop iteration until the whole buffer is programmed.
    //
    // Remember that the main array flash programming must be aligned to
    // 64-bit address boundaries and each 64 bit word may only be programmed
    // once per write/erase cycle.  Meaning the length of the data buffer
    // (3rd parameter for Fapi_issueProgrammingCommand() function) passed
    // to the program function can only be either 4 or 8.
    //
    // Program data in Flash using "AutoEccGeneration" option.
    // When AutoEccGeneration option is used, Flash API calculates ECC for the
    // given 64-bit data and programs it along with the 64-bit main array data.
    // Note that any unprovided data with in a 64-bit data slice
    // will be assumed as 1s for calculating ECC and will be programmed.
    //
    // Note that data buffer (Buffer) is aligned on 64-bit boundary for verify
    // reasons.
    //
    // Monitor ECC address for the sector below while programming with 
    // AutoEcc mode.
    //
    // In this example, the number of bytes specified in the flash buffer
    // are programmed in the flash sector below along with auto-generated
    // ECC.
    //

    for(i=0, u32Index = Bone_Sector1_start;
       (u32Index < (Bone_Sector1_start + WORDS_IN_FLASH_BUFFER));
       i+= 8, u32Index+= 8)
    {

        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index,Buffer+i,
                                               8, 0, 0, Fapi_AutoEccGeneration);

        //
        // Wait until the Flash program operation is over
        //
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }

        //
        // Read FMSTAT register contents to know the status of FSM after
        // program command to see if there are any program operation related
        // errors
        //
        oFlashStatus = Fapi_getFsmStatus();
        if(oFlashStatus != 0)
        {
            //
            //Check FMSTAT and debug accordingly
            //
            FMSTAT_Fail();
        }

        //
        // Verify the programmed values.  Check for any ECC errors.
        //
        oReturnCheck = Fapi_doVerify((uint32 *)u32Index,
                                     4, Buffer32+(i/2),
                                     &oFlashStatusWord);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }
    }
}

//*****************************************************************************
//  Example_ProgramBankUsingAutoECC
//
//  Example function to Program data in Flash using "AutoEccGeneration" option.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_ProgramBankUsingAutoECC, ".TI.ramfunc");
#endif
void Example_ProgramBankUsingAutoECC(void)
{
    uint32 u32Index = 0;
    uint32 sector = 0;
    uint16 i = 0;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    //
    // A data buffer of max 8 16-bit words can be supplied to the program
    // function.
    // Each word is programmed until the whole buffer is programmed or a
    // problem is found. However to program a buffer that has more than 8
    // words, program function can be called in a loop to program 8 words for
    // each loop iteration until the whole buffer is programmed.
    //
    // Remember that the main array flash programming must be aligned to
    // 64-bit address boundaries and each 64 bit word may only be programmed
    // once per write/erase cycle.  Meaning the length of the data buffer
    // (3rd parameter for Fapi_issueProgrammingCommand() function) passed
    // to the program function can only be either 4 or 8.
    //
    // Program data in Flash using "AutoEccGeneration" option.
    // When AutoEccGeneration option is used, Flash API calculates ECC for the
    // given 64-bit data and programs it along with the 64-bit main array data.
    // Note that any unprovided data with in a 64-bit data slice
    // will be assumed as 1s for calculating ECC and will be programmed.
    //
    // Note that data buffer (Buffer) is aligned on 64-bit boundary for verify
    // reasons.
    //
    // Monitor ECC address for Bank 1 while programming with AutoEcc mode.
    //
    // In this example, 0x100 bytes are programmed in Flash Bank 1
    // along with auto-generated ECC.
    //

    for(sector = FlashBank1StartAddress; sector < FlashBank1EndAddress;
        sector += Sector8KB_u32length)
    {
        for(i=0, u32Index = sector;
           (u32Index < (sector + WORDS_IN_FLASH_BUFFER));
            i+= 8, u32Index+= 8)
        {
            oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index,
                                                        Buffer+i, 8, 0, 0,
                                                       Fapi_AutoEccGeneration);

            //
            // Wait until the Flash program operation is over
            //
            while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

            if(oReturnCheck != Fapi_Status_Success)
            {
                //
                // Check Flash API documentation for possible errors
                //
                Example_Error(oReturnCheck);
            }

            //
            // Read FMSTAT register contents to know the status of FSM after
            // program command to see if there are any program operation
            // related errors
            //
            oFlashStatus = Fapi_getFsmStatus();
            if(oFlashStatus != 0)
            {
                //
                //Check FMSTAT and debug accordingly
                //
                FMSTAT_Fail();
            }

            //
            // Verify the programmed values.  Check for any ECC errors.
            //
            oReturnCheck = Fapi_doVerify((uint32 *)u32Index,
                                         4, Buffer32+(i/2),
                                         &oFlashStatusWord);

            if(oReturnCheck != Fapi_Status_Success)
            {
                //
                // Check Flash API documentation for possible errors
                //
                Example_Error(oReturnCheck);
            }
        }
    }
}

//*****************************************************************************
//  Example_ProgramUsingDataOnlyECCOnly
//
//  Example function to Program data in Flash using "DataOnly" option and ECC
//  using "EccOnly" option.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_ProgramUsingDataOnlyECCOnly, ".TI.ramfunc");
#endif
void Example_ProgramUsingDataOnlyECCOnly(void)
{
    uint32 u32Index = 0;
    uint16 i = 0, ECC_B = 0, ECC_LB = 0, ECC_HB = 0;
    uint64 *LData, *HData, dataLow, dataHigh;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    //
    // Program data using "DataOnly" option and ECC using "EccOnly" option.
    //
    // When DataOnly option is used, Flash API will program only the data
    // portion in Flash at the address specified.
    //
    // When EccOnly option is used, Flash API will program only the ECC portion
    // in Flash ECC memory space (Flash main array address should be provided
    // for this function and not the corresponding ECC address).
    // Fapi_calculateEcc is used to calculate the corresponding ECC of the data.
    //
    // Note that data buffer (Buffer) is aligned on 64-bit boundary for verify
    // reasons.
    //
    // In this example, 0x100 bytes are programmed in Flash Sector6
    // along with the specified ECC.
    //

    for(i=0, u32Index = Bone_Sector1_start;
       (u32Index < (Bone_Sector1_start + WORDS_IN_FLASH_BUFFER));
       i+= 8, u32Index+= 8)
    {
        //
        // Point LData to the lower 64 bit data
        // and   HData to the higher 64 bit data
        //
        LData = (uint64 *)(Buffer32 + i/2);
        HData = (uint64 *)(Buffer32 + i/2 + 2);

        //
        // Calculate ECC for lower 64 bit and higher 64 bit data
        //
        ECC_LB = Fapi_calculateEcc(u32Index,*LData);
        ECC_HB = Fapi_calculateEcc(u32Index+4,*HData);
        ECC_B = ((ECC_HB<<8) | ECC_LB);

        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index,Buffer+i,
                                                        8, 0, 0, Fapi_DataOnly);

        //
        // Wait until the Flash program operation is over
        //
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }

        //
        // Read FMSTAT register contents to know the status of FSM after
        // program command to see if there are any program operation related
        // errors
        //
        oFlashStatus = Fapi_getFsmStatus();
        if(oFlashStatus != 0)
        {
            //Check FMSTAT and debug accordingly
            FMSTAT_Fail();
        }
        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index, 0, 0,
                                                    &ECC_B, 2, Fapi_EccOnly);

        //
        // Wait until the Flash program operation is over
        //
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }

        //
        // Read FMSTAT register contents to know the status of FSM after
        // program command to see if there are any program operation related
        // errors
        //
        oFlashStatus = Fapi_getFsmStatus();
        if(oFlashStatus != 0)
        {
            //
            // Check FMSTAT and debug accordingly
            //
            FMSTAT_Fail();
        }

        Flash_enableECC(FLASH0ECC_BASE);
        //
        // Read back the programmed data to check if there are any ECC failures
        //
        dataLow = *(uint64 *)(u32Index);

        Flash_ErrorStatus errorStatusLow = Flash_getLowErrorStatus(FLASH0ECC_BASE);
        if((errorStatusLow != FLASH_NO_ERR) || (dataLow != *LData))
        {
            ECC_Fail();
        }

        dataHigh = *(uint64 *)(u32Index + 4);

        Flash_ErrorStatus errorStatusHigh = Flash_getHighErrorStatus(FLASH0ECC_BASE);
        if((errorStatusHigh != FLASH_NO_ERR) || (dataHigh != *HData))
        {
            ECC_Fail();
        }


        //
        // Verify the programmed values.  Check for any ECC errors.
        //
        oReturnCheck = Fapi_doVerify((uint32 *)u32Index,
                                     4, Buffer32+(i/2),
                                     &oFlashStatusWord);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }
    }
}

//*****************************************************************************
//  Example_ProgramUsingDataAndECC
//
//  Example function to Program data in Flash using "DataAndEcc" option.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_ProgramUsingDataAndECC, ".TI.ramfunc");
#endif
void Example_ProgramUsingDataAndECC(void)
{
    uint32 u32Index = 0;
    uint16 i = 0, ECC_B = 0, ECC_LB = 0, ECC_HB = 0;
    uint64 *LData, *HData, dataLow, dataHigh;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    //
    // Program data and ECC in Flash using "DataAndEcc" option.
    // When DataAndECC option is used, Flash API will program both the supplied
    // data and ECC in Flash at the address specified.
    // Fapi_calculateEcc is used to calculate the corresponding ECC of the data.
    //
    // Note that data buffer (Buffer) is aligned on 64-bit boundary for verify
    // reasons.
    //
    // In this example, 0x100 bytes are programmed in Flash Sector6
    // along with the specified ECC.
    //

    for(i=0, u32Index = Bone_Sector1_start;
       (u32Index < (Bone_Sector1_start + WORDS_IN_FLASH_BUFFER));
       i+= 8, u32Index+= 8)
    {
        //
        // Point LData to the lower 64 bit data
        // and   HData to the higher 64 bit data
        //
        LData = (uint64 *)(Buffer32 + i/2);
        HData = (uint64 *)(Buffer32 + i/2 + 2);

        //
        // Calculate ECC for lower 64 bit and higher 64 bit data
        //
        ECC_LB = Fapi_calculateEcc(u32Index,*LData);
        ECC_HB = Fapi_calculateEcc(u32Index+4,*HData);
        ECC_B = ((ECC_HB<<8) | ECC_LB);

        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index,Buffer+i,
                                                 8, &ECC_B, 2, Fapi_DataAndEcc);

        //
        // Wait until the Flash program operation is over
        //
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }

        //
        // Read FMSTAT register contents to know the status of FSM after
        // program command to see if there are any program operation related
        // errors
        //
        oFlashStatus = Fapi_getFsmStatus();
        if(oFlashStatus != 0)
        {
            //
            // Check FMSTAT and debug accordingly
            //
            FMSTAT_Fail();
        }

        Flash_enableECC(FLASH0ECC_BASE);

        //
        // Read back the programmed data to check if there are any ECC failures
        //
        dataLow = *(uint64 *)(u32Index);

        Flash_ErrorStatus errorStatusLow = Flash_getLowErrorStatus(FLASH0ECC_BASE);
        if((errorStatusLow != FLASH_NO_ERR) || (dataLow != *LData))
        {
            ECC_Fail();
        }

        dataHigh = *(uint64 *)(u32Index + 4);

        Flash_ErrorStatus errorStatusHigh = Flash_getHighErrorStatus(FLASH0ECC_BASE);
        if((errorStatusHigh != FLASH_NO_ERR) || (dataHigh != *HData))
        {
            ECC_Fail();
        }

        //
        // Verify the programmed values.  Check for any ECC errors.
        //
        oReturnCheck = Fapi_doVerify((uint32 *)u32Index,
                                     4, Buffer32+(i/2),
                                     &oFlashStatusWord);

        if(oReturnCheck != Fapi_Status_Success)
        {
            //
            // Check Flash API documentation for possible errors
            //
            Example_Error(oReturnCheck);
        }
    }

}

//*****************************************************************************
//  Example_EraseSector
//
//  Example function to Erase data of a sector in Flash.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_EraseSector, ".TI.ramfunc");
#endif
void Example_EraseSector(void)
{
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    //
    // Issue ClearMore command
    //
    oReturnCheck = Fapi_issueAsyncCommand(Fapi_ClearMore);

    //
    // Wait until FSM is done with erase sector operation
    //
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for possible errors
        //
        Example_Error(oReturnCheck);
    }

    //
    // Erase the sector that is programmed in the above example
    // Erase Sector1
    //
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,
                   (uint32 *)Bone_Sector1_start);
    //
    // Wait until FSM is done with erase sector operation
    //
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for possible errors
        //
        Example_Error(oReturnCheck);
    }

    //
    // Read FMSTAT register contents to know the status of FSM after
    // erase command to see if there are any erase operation related errors
    //
    oFlashStatus = Fapi_getFsmStatus();
        if(oFlashStatus != 0)
    {
        //
        // Check Flash API documentation for FMSTAT and debug accordingly
        // Fapi_getFsmStatus() function gives the FMSTAT register contents.
        // Check to see if any of the EV bit, ESUSP bit, CSTAT bit or
        // VOLTSTAT bit is set (Refer to API documentation for more details).
        //
        FMSTAT_Fail();
    }

    //
    // Verify that Sector1 is erased
    //
    oReturnCheck = Fapi_doBlankCheck((uint32 *)Bone_Sector1_start,
                   Sector8KB_u32length,
                   &oFlashStatusWord);

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for error info
        //
        Example_Error(oReturnCheck);
    }
}

//*****************************************************************************
//  Example_EraseBanks
//
//  Example function to Erase data of a sector in Flash.
//  Flash API functions used in this function are executed from RAM in this
//  example.
//*****************************************************************************
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#else
#pragma CODE_SECTION(Example_EraseBanks, ".TI.ramfunc");
#endif
void Example_EraseBanks(void)
{
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;
    uint32 u32CurrentAddress;

    u32CurrentAddress = Bzero_Sector8_start;
    oReturnCheck = Fapi_issueBankEraseCommand((uint32 *)u32CurrentAddress, 0x001F);

    // Wait until FSM is done with bank erase operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady)
    {
         //
         // Initialize the Erase Pulses to zero after issuing max pulses
         //
         if(HWREG(FLASH0CTRL_BASE + FLASH_O_ACC_EP) > MAX_ERASE_PULSE)
         {
             EALLOW;

             //
             // Enable Flash Optimization
             //
             HWREG(FLASH0CTRL_BASE + FLASH_O_OPT) = OPT_ENABLE;

             HWREG(FLASH0CTRL_BASE + FLASH_O_ERA_PUL) =
                     HWREG(FLASH0CTRL_BASE + FLASH_O_ERA_PUL) &
                     ~(uint32_t)FLASH_ERA_PUL_MAX_ERA_PUL_M;

             //
             // Disable Flash Optimization
             //
             HWREG(FLASH0CTRL_BASE + FLASH_O_OPT) = OPT_DISABLE;

             EDIS;
         }
    }

    if(oReturnCheck != Fapi_Status_Success)
    {
        // Check Flash API documentation for possible errors
        Example_Error(oReturnCheck);
    }

    // Read FMSTAT register contents to know the status of FSM after
    // erase command to see if there are any erase operation related errors
    oFlashStatus = Fapi_getFsmStatus();
    if(oFlashStatus != 0)
    {
        // Check Flash API documentation for FMSTAT and debug accordingly
        // Fapi_getFsmStatus() function gives the FMSTAT register contents.
        // Check to see if any of the EV bit, ESUSP bit, CSTAT bit or
        // VOLTSTAT bit is set (Refer to API documentation for more details).
        FMSTAT_Fail();
    }

    u32CurrentAddress = Bone_Sector0_start;
    oReturnCheck = Fapi_issueBankEraseCommand((uint32 *)u32CurrentAddress, 0x0000);

    // Wait until FSM is done with bank erase operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady)
    {
        //
        // Initialize the Erase Pulses to zero after issuing max pulses
        //
        if(HWREG(FLASH0CTRL_BASE + FLASH_O_ACC_EP) > MAX_ERASE_PULSE)
        {
            EALLOW;

            //
            // Enable Flash Optimization
            //
            HWREG(FLASH0CTRL_BASE + FLASH_O_OPT) = OPT_ENABLE;

            HWREG(FLASH0CTRL_BASE + FLASH_O_ERA_PUL) =
                    HWREG(FLASH0CTRL_BASE + FLASH_O_ERA_PUL) &
                    ~(uint32_t)FLASH_ERA_PUL_MAX_ERA_PUL_M;

            //
            // Disable Flash Optimization
            //
            HWREG(FLASH0CTRL_BASE + FLASH_O_OPT) = OPT_DISABLE;

            EDIS;
         }
    }

    if(oReturnCheck != Fapi_Status_Success)
    {
        // Check Flash API documentation for possible errors
        Example_Error(oReturnCheck);
    }

    // Read FMSTAT register contents to know the status of FSM after
    // erase command to see if there are any erase operation related errors
    oFlashStatus = Fapi_getFsmStatus();
    if(oFlashStatus != 0)
    {
        // Check Flash API documentation for FMSTAT and debug accordingly
        // Fapi_getFsmStatus() function gives the FMSTAT register contents.
        // Check to see if any of the EV bit, ESUSP bit, CSTAT bit or
        // VOLTSTAT bit is set (Refer to API documentation for more details).
        FMSTAT_Fail();
    }

    // Do blank check
    // Verify that Bank0 sectors 5-15 and Bank 1 sectors 0-15 are erased.
    // The Erase command itself does a verify as it goes.
    // Hence erase verify by CPU reads (Fapi_doBlankCheck()) is optional.
    u32CurrentAddress = Bzero_Sector5_start;
    oReturnCheck = Fapi_doBlankCheck((uint32 *)u32CurrentAddress,
                   (27*Sector8KB_u32length),
                   &oFlashStatusWord);

    if(oReturnCheck != Fapi_Status_Success)
    {
        // Check Flash API documentation for error info
        Example_Error(oReturnCheck);
    }
}

//******************************************************************************
// For this example, just stop here if an API error is found
//******************************************************************************
void Example_Error(Fapi_StatusType status)
{
    //
    //  Error code will be in the status parameter
    //
    __asm("    ESTOP0");
}

//******************************************************************************
//  For this example, once we are done just stop here
//******************************************************************************
void Example_Done(void)
{
    __asm("    ESTOP0");
}

//******************************************************************************
// For this example, just stop here if FMSTAT fail occurs
//******************************************************************************
void FMSTAT_Fail(void)
{
    __asm("    ESTOP0");
}

//******************************************************************************
// For this example, just stop here if ECC fail occurs
//******************************************************************************
void ECC_Fail(void)
{
    __asm("    ESTOP0");
}

//
// End of File
//
