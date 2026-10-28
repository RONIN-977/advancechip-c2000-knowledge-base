/*

 */
#include "flash_hal.h"




#pragma  DATA_SECTION(Buffer,"DataBufferSection");
//uint16   Buffer[WORDS_IN_FLASH_BUFFER];
uint32   *Buffer32 ;//= (uint32 *)Buffer;

uint8 flash_hal_init(void)
{
	Fapi_StatusType  oReturnCheck;
	Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, 4);
	oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS,
                                      DEVICE_SYSCLK_FREQ/1000000U);

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for possible errors
        //
       // Example_Error(oReturnCheck);
	   return 1;
		
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
        //Example_Error(oReturnCheck);
		return 2;
    }
	return 0;
}

#pragma CODE_SECTION(flash_hal_erase, ".TI.ramfunc");
uint8 flash_hal_erase(uint32 startAddr,uint32 len)
{
	Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    //
    // Erase the sector that is programmed in the above example
    // Erase Sector6
    //
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,
                   (uint32 *)startAddr);
    //
    // Wait until FSM is done with erase sector operation
    //
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    if(oReturnCheck != Fapi_Status_Success)
    {
		return 1;
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
        return 2;
    }

    //
    // Verify that Sector6 is erased
    //
    oReturnCheck = Fapi_doBlankCheck((uint32 *)Bzero_Sector15_start,
                   Sector8KB_u32length,
                   &oFlashStatusWord);

    if(oReturnCheck != Fapi_Status_Success)
    {
        //
        // Check Flash API documentation for error info
        //
       return 3;
    }
	return 0;
	
}

void flash_hal_read(uint32 startAddr,uint32 Len,uint8 *Data)
{
	uint32 i;
	for(i=0;i<Len;i++)
	{
	    *(Data+i) = *((uint8 *)(startAddr+i));
	}
}
uint32 startAddr_temp;
//uint16 data_temp[512];
#pragma CODE_SECTION(flash_hal_write, ".TI.ramfunc");
//uint32 startAddr_temp;
//uint8 flash_hal_write(uint8 type,uint32 startAddr,uint32 Len,uint8 *Data)
uint8 flash_hal_write(uint32 startAddr,uint32 Len,uint8 *Data)
{
	uint32 u32Index = 0;
    uint16 i = 0;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;
//    uint16 data_temp[512];
    uint16 j;
    uint16 dataLen;

    if(Len > 1024)
    {
        return 4;
    }

//    if(type == 1)
//    {
//        if((dataLen % 2)>0)
//        {
//           dataLen = Len/2;
//           for(j=0;j<dataLen;j++)
//           {
//               data_temp[j] = Data[2*j]|(Data[2*j+1]<<8);
////               data_temp[j] = Data[2*j+1]|(Data[2*j]<<8);
//           }
//           data_temp[dataLen+1] = Data[2*(dataLen+1)]|(0xff00);
//           dataLen++;
//        }
//        else
//        {
//            dataLen = Len/2;
//            for(j=0;j<dataLen;j++)
//           {
//                data_temp[j] = Data[2*j]|(Data[2*j+1]<<8);
////                data_temp[j] = Data[2*j+1]|(Data[2*j]<<8);
//           }
//        }
//        if(startAddr > 0x86000)
//        {
//            startAddr_temp = 0x86000+(startAddr-0x86000)/2;
//        }
//        else
//        {
//            startAddr_temp = startAddr;
//        }
//        for(i=0, u32Index = startAddr_temp;
//           (u32Index < (startAddr_temp + dataLen));
//           i+= 8, u32Index+= 8)
//        {
//            oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index,(uint16 *)data_temp+i,
//                                                   8, 0, 0, Fapi_AutoEccGeneration);
//
//            //
//            // Wait until the Flash program operation is over
//            //
//            while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
//
//            if(oReturnCheck != Fapi_Status_Success)
//            {
//                //
//                // Check Flash API documentation for possible errors
//                //
//                return 1;
//            }
//
//            //
//            // Read FMSTAT register contents to know the status of FSM after
//            // program command to see if there are any program operation related
//            // errors
//            //
//            oFlashStatus = Fapi_getFsmStatus();
//            if(oFlashStatus != 0)
//            {
//                //
//                //Check FMSTAT and debug accordingly
//                //
//               // FMSTAT_Fail();
//                return 2;
//            }
//
//            Buffer32 = (uint32 *)Data;
//
//            //
//            // Verify the programmed values.  Check for any ECC errors.
//            //
//    //        oReturnCheck = Fapi_doVerify((uint32 *)u32Index,
//    //                                     4, Buffer32+(i/2),
//    //                                     &oFlashStatusWord);
//    //
//    //        if(oReturnCheck != Fapi_Status_Success)
//    //        {
//    //            //
//    //            // Check Flash API documentation for possible errors
//    //            //
//    ////            Example_Error(oReturnCheck);
//    //			return 3;
//    //        }
//        }
//    }
//    else
    {
        for(i=0, u32Index = startAddr;
           (u32Index < (startAddr + Len));
           i+= 8, u32Index+= 8)
        {
            oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)u32Index,(uint16 *)Data+i,
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
                return 1;
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
               // FMSTAT_Fail();
                return 2;
            }

    }
    }
	return 0;
}


