/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flashapi_ex3_boot.c
*                         TITLE: Boot Source Code
*******************************************************************************************/

//
// Includes
//
#include "cpu1bootrom.h"
#include "flash_kernel_ex3_commands.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"


#include "assert.h"

//
// Globals
//

// getWordData is a pointer to the function that interfaces to the peripheral.
// Each loader assigns this pointer to it's particular getWordData function.
//
uint16fptr getWordData;

typedef struct
{
    uint16_t status;
    uint32_t address;
    uint16_t flashAPIError;
    uint32_t flashAPIFsmStatus;
} StatusCode;
extern StatusCode statusCode;

//
// Because of ECC, must be multiple of 64 bits, or 4 words, BUFFER_SIZE % 4 == 0
//
#define BUFFER_SIZE                 0x80 

//
// Function Prototypes
//
extern void sciSendChecksum(void);
uint32_t getLongData(void);
void copyData(void);
void readReservedFn(void);
uint32_t findSector(uint32_t address);
uint16_t findSize(uint32_t address);
void exampleError(Fapi_StatusType status);
void setFlashAPIError(Fapi_StatusType status);

//
// 48(16x3) sectors.
//
unsigned char erasedAlready[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//
// void CopyData(void) - This routine copies multiple blocks of data from the host
//                       to the specified RAM locations.  There is no error
//                       checking on any of the destination addresses.
//                       That is it is assumed all addresses and block size
//                       values are correct.
//
//                       Multiple blocks of data are copied until a block
//                       size of 00 00 is encountered.
//
void copyData()
{
    struct HEADER
    {
        uint16_t BlockSize;
        uint32_t DestAddr;
    } BlockHeader;

    uint16_t Buffer[BUFFER_SIZE];
    
    //
    // useful for 8-word access to flash with
    //
    uint16_t miniBuffer[8]; 
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t k = 0;
    uint32_t sectorAddress;
    uint16_t sectorSize;
    uint16_t wordData;
    uint16_t wordsWritten = 0;
    int fail = 0;

    assert(BUFFER_SIZE % 8 == 0);

    if (statusCode.status != NO_ERROR)
    {
        fail++;
    }

    //
    // Reset erase status of all flash sectors
    // 36 sectors.
    //
    for (i = 0; i < 32; i++)
    {
        erasedAlready[i] = 0;
    }

    //
    // Send checksum to satisfy before we begin
    //
#if checksum_enable
    sciSendChecksum();
#endif

    //
    // Get the size in words of the first block
    //
    BlockHeader.BlockSize = (*getWordData)();

    //
    // While the block size is > 0 copy the data
    // to the DestAddr.  There is no error checking
    // as it is assumed the DestAddr is a valid
    // memory location
    //

    EALLOW;
    while (BlockHeader.BlockSize != (uint16_t) 0x0000)
    {
        Fapi_StatusType oReturnCheck;
        Fapi_FlashStatusWordType oFlashStatusWord;
        Fapi_FlashStatusType oFlashStatus;
       
        BlockHeader.DestAddr = getLongData();
        for (i = 0; i < BlockHeader.BlockSize; i += 0)
        {
            if (BlockHeader.BlockSize < BUFFER_SIZE)
            {
                for (j = 0; j < BlockHeader.BlockSize; j++)
                {
                    wordData = (*getWordData)();
                    Buffer[j] = wordData;
                    i++;
                }
                for (j = BlockHeader.BlockSize; j < BUFFER_SIZE; j++)
                {
                    Buffer[j] = 0xFFFF;
                }
            }
            
            //
            // BlockHeader.BlockSize >= BUFFER_SIZE
            //
            else 
            {
                //
                // less than one BUFFER_SIZE left
                //
                if ((BlockHeader.BlockSize - i) < BUFFER_SIZE) 
                {
                    //
                    // fill Buffer with rest of data
                    //
                    for (j = 0; j < BlockHeader.BlockSize - i; j++) 
                    {
                        wordData = (*getWordData)();
                        Buffer[j] = wordData;
                    }
                    
                    //
                    // increment i outside here so it doesn't affect loop above
                    //
                    i += j; 
                    
                    //
                    // fill the rest with 0xFFFF
                    //
                    for (; j < BUFFER_SIZE; j++) 
                    {
                        Buffer[j] = 0xFFFF;
                    }
                }
                else
                {
                    //
                    // fill up like normal, up to BUFFER_SIZE
                    //
                    for (j = 0; j < BUFFER_SIZE; j++) 
                    {
                        wordData = (*getWordData)();
                        Buffer[j] = wordData;
                        i++;
                    }
                }
            }
            for (k = 0; k < (BUFFER_SIZE / 8); k++)
            {
                miniBuffer[0] = Buffer[k * 8 + 0];
                miniBuffer[1] = Buffer[k * 8 + 1];
                miniBuffer[2] = Buffer[k * 8 + 2];
                miniBuffer[3] = Buffer[k * 8 + 3];
                miniBuffer[4] = Buffer[k * 8 + 4];
                miniBuffer[5] = Buffer[k * 8 + 5];
                miniBuffer[6] = Buffer[k * 8 + 6];
                miniBuffer[7] = Buffer[k * 8 + 7];
                
                //
                // check that all the words have not already been written
                //
                if (wordsWritten < BlockHeader.BlockSize)
                {
                    if(fail == 0)
                    {
                        //
                        // clean out flash banks if needed
                        //
                        sectorAddress = findSector(BlockHeader.DestAddr);
                        if (sectorAddress != 0xdeadbeef)
                        {
                            //
                            // FindSize returns size of sector in 32 bit words. 
                            //
                            sectorSize = findSize(sectorAddress);
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
                                statusCode.status = CLEAR_MORE_ERROR;
                                //
                                // Address not needed for this error
                                //
                                statusCode.address = 0;
                                setFlashAPIError(oReturnCheck);
                                statusCode.flashAPIFsmStatus = 0;
                                fail++;
                            }
                            if(fail == 0)
                            {
                            oReturnCheck = Fapi_issueAsyncCommandWithAddress(
                                    Fapi_EraseSector, (uint32_t *) sectorAddress);

                            while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}
                            
                            oFlashStatus = Fapi_getFsmStatus();

                            oReturnCheck = Fapi_doBlankCheck(
                                    (uint32_t *) sectorAddress, sectorSize,
                                    &oFlashStatusWord);
                            if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
                            {
                                //
                                //first fail
                                //
                                if (fail == 0) 
                                {
                                    statusCode.status = BLANK_ERROR;
                                    statusCode.address =
                                            oFlashStatusWord.au32StatusWord[0];
                                    setFlashAPIError(oReturnCheck);
                                    statusCode.flashAPIFsmStatus = oFlashStatus;
                                }
                                fail++;
                            }
                          }
                        }
                    }
                    if(fail == 0)
                    {
                        //
                        //program 8 words at once, 128-bits
                        //
                        oReturnCheck = Fapi_issueProgrammingCommand(
                                (uint32_t *) BlockHeader.DestAddr, miniBuffer,
                                sizeof(miniBuffer), 0, 0, Fapi_AutoEccGeneration);
                        
                        while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
                        
                        oFlashStatus = Fapi_getFsmStatus();
                        if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
                        {
                            //
                            // first fail
                            //
                            if (fail == 0) 
                            {
                                statusCode.status = PROGRAM_ERROR;
                                statusCode.address =
                                        BlockHeader.DestAddr;
                                setFlashAPIError(oReturnCheck);
                                statusCode.flashAPIFsmStatus = oFlashStatus;
                            }
                            fail++;
                        }
                    }
                    for (j = 0; j < 8; j += 2)
                    {
                        uint32_t toVerify = miniBuffer[j + 1];
                        toVerify = toVerify << 16;
                        toVerify |= miniBuffer[j];
                        if(fail == 0)
                        {
                            oReturnCheck = Fapi_doVerify(
                                    (uint32_t *) (BlockHeader.DestAddr + j), 1,
                                    (uint32_t *) (&toVerify), &oFlashStatusWord);
                            if (oReturnCheck != Fapi_Status_Success)
                            {
                                //
                                // first fail
                                //
                                if (fail == 0) 
                                {
                                    statusCode.status = VERIFY_ERROR;
                                    statusCode.address =
                                            oFlashStatusWord.au32StatusWord[0];
                                    setFlashAPIError(oReturnCheck);
                                    //
                                    // FMSTAT not checked for Verify 
                                    //
                                    statusCode.flashAPIFsmStatus = 0; 
                                }
                                fail++;
                            }
                        }
                    } //for j; for Fapi_doVerify
                } //check if all the words are not already written
                BlockHeader.DestAddr += 0x8;
                wordsWritten += 0x8;
            } //for(int k); loads miniBuffer with Buffer elements
#if checksum_enable
            sciSendChecksum();
#endif
        }
        //
        // get the size of the next block
        //
        BlockHeader.BlockSize = (*getWordData)();
        wordsWritten = 0;
    }
    EDIS;
    return;
}


//
// uint32_t findSector(uint32_t address) - This routine finds what sector the mentioned address
//                                         is a part of.
//
uint32_t findSector(uint32_t address)
{
    //
    //******************** Bzero_sector0~15_start~End *********************
    //
     if((address >= Bzero_Sector0_start) && (address < Bzero_Sector1_start) &&
       (erasedAlready[0] == 0))
    {
        erasedAlready[0] = 1;
        return (uint32_t)Bzero_Sector0_start;
    }
    else if((address >= Bzero_Sector1_start) &&
            (address < Bzero_Sector2_start) && (erasedAlready[1] == 0))
    {
        erasedAlready[1] = 1;
        return (uint32_t)Bzero_Sector1_start;
    }
    else if((address >= Bzero_Sector2_start) &&
            (address < Bzero_Sector3_start) && (erasedAlready[2] == 0))
    {
        erasedAlready[2] = 1;
        return (uint32_t)Bzero_Sector2_start;
    }
    else if((address >= Bzero_Sector3_start) &&
            (address < Bzero_Sector4_start) && (erasedAlready[3] == 0))
    {
        erasedAlready[3] = 1;
        return (uint32_t)Bzero_Sector3_start;
    }
    else if((address >= Bzero_Sector4_start) &&
            (address < Bzero_Sector5_start) && (erasedAlready[4] == 0))
    {
        erasedAlready[4] = 1;
        return (uint32_t)Bzero_Sector4_start;
    }
    else if((address >= Bzero_Sector5_start) &&
            (address < Bzero_Sector6_start) && (erasedAlready[5] == 0))
    {
        erasedAlready[5] = 1;
        return (uint32_t)Bzero_Sector5_start;
    }
    else if((address >= Bzero_Sector6_start) &&
            (address < Bzero_Sector7_start) && (erasedAlready[6] == 0))
    {
        erasedAlready[6] = 1;
        return (uint32_t)Bzero_Sector6_start;
    }
    else if((address >= Bzero_Sector7_start) &&
            (address < Bzero_Sector8_start) && (erasedAlready[7] == 0))
    {
        erasedAlready[7] = 1;
        return (uint32_t)Bzero_Sector7_start;
    }
    else if((address >= Bzero_Sector8_start) &&
            (address < Bzero_Sector9_start) && (erasedAlready[8] == 0))
    {
        erasedAlready[8] = 1;
        return (uint32_t)Bzero_Sector8_start;
    }
    else if((address >= Bzero_Sector9_start) &&
            (address < Bzero_Sector10_start) && (erasedAlready[9] == 0))
    {
        erasedAlready[9] = 1;
        return (uint32_t)Bzero_Sector9_start;
    }
    else if((address >= Bzero_Sector10_start) &&
            (address < Bzero_Sector11_start) && (erasedAlready[10] == 0))
    {
        erasedAlready[10] = 1;
        return (uint32_t)Bzero_Sector10_start;
    }
    else if((address >= Bzero_Sector11_start) &&
            (address < Bzero_Sector12_start) && (erasedAlready[11] == 0))
    {
        erasedAlready[11] = 1;
        return (uint32_t)Bzero_Sector11_start;
    }
    else if((address >= Bzero_Sector12_start) &&
            (address < Bzero_Sector13_start) && (erasedAlready[12] == 0))
    {
        erasedAlready[12] = 1;
        return (uint32_t)Bzero_Sector12_start;
    }
    else if((address >= Bzero_Sector13_start) &&
            (address < Bzero_Sector14_start) && (erasedAlready[13] == 0))
    {
        erasedAlready[13] = 1;
        return (uint32_t)Bzero_Sector13_start;
    }
    else if((address >= Bzero_Sector14_start) &&
               (address < Bzero_Sector15_start) && (erasedAlready[14] == 0))
   {
       erasedAlready[14] = 1;
       return (uint32_t)Bzero_Sector14_start;
   }
    else if((address >= Bzero_Sector15_start) &&
               (address <= FlashBank0EndAddress) && (erasedAlready[15] == 0))
   {
       erasedAlready[15] = 1;
       return (uint32_t)Bzero_Sector15_start;
   }
    //
    //***************** Bone_Sector0~15_start~End *************************
    //
    else if ((address >= Bone_Sector0_start) && (address < Bone_Sector1_start)
            && (erasedAlready[16] == 0))
    {
        erasedAlready[16] = 1;
        return (uint32_t) Bone_Sector0_start;
    }
    else if ((address >= Bone_Sector1_start) && (address < Bone_Sector2_start)
            && (erasedAlready[17] == 0))
    {
        erasedAlready[17] = 1;
        return (uint32_t) Bone_Sector1_start;
    }
    else if ((address >= Bone_Sector2_start) && (address < Bone_Sector3_start)
            && (erasedAlready[18] == 0))
    {
        erasedAlready[18] = 1;
        return (uint32_t) Bone_Sector2_start;
    }
    else if ((address >= Bone_Sector3_start) && (address < Bone_Sector4_start)
            && (erasedAlready[19] == 0))
    {
        erasedAlready[19] = 1;
        return (uint32_t) Bone_Sector3_start;
    }
    else if ((address >= Bone_Sector4_start) && (address < Bone_Sector5_start)
            && (erasedAlready[20] == 0))
    {
        erasedAlready[20] = 1;
        return (uint32_t) Bone_Sector4_start;
    }
    else if ((address >= Bone_Sector5_start) && (address < Bone_Sector6_start)
            && (erasedAlready[21] == 0))
    {
        erasedAlready[21] = 1;
        return (uint32_t) Bone_Sector5_start;
    }
    else if ((address >= Bone_Sector6_start) && (address < Bone_Sector7_start)
            && (erasedAlready[22] == 0))
    {
        erasedAlready[22] = 1;
        return (uint32_t) Bone_Sector6_start;
    }
    else if ((address >= Bone_Sector7_start) && (address < Bone_Sector8_start)
            && (erasedAlready[23] == 0))
    {
        erasedAlready[23] = 1;
        return (uint32_t) Bone_Sector7_start;
    }
    else if ((address >= Bone_Sector8_start) && (address < Bone_Sector9_start)
            && (erasedAlready[24] == 0))
    {
        erasedAlready[24] = 1;
        return (uint32_t) Bone_Sector8_start;
    }
    else if ((address >= Bone_Sector9_start) && (address < Bone_Sector10_start)
            && (erasedAlready[25] == 0))
    {
        erasedAlready[25] = 1;
        return (uint32_t) Bone_Sector9_start;
    }
    else if ((address >= Bone_Sector10_start) && (address < Bone_Sector11_start)
            && (erasedAlready[26] == 0))
    {
        erasedAlready[26] = 1;
        return (uint32_t) Bone_Sector10_start;
    }
    else if ((address >= Bone_Sector11_start) && (address < Bone_Sector12_start)
            && (erasedAlready[27] == 0))
    {
        erasedAlready[27] = 1;
        return (uint32_t) Bone_Sector11_start;
    }
    else if ((address >= Bone_Sector12_start) && (address < Bone_Sector13_start)
            && (erasedAlready[28] == 0))
    {
        erasedAlready[28] = 1;
        return (uint32_t) Bone_Sector12_start;
    }
    else if ((address >= Bone_Sector13_start) && (address < Bone_Sector14_start)
            && (erasedAlready[29] == 0))
    {
        erasedAlready[29] = 1;
        return (uint32_t) Bone_Sector13_start;
    }
    else if ((address >= Bone_Sector14_start) && (address < Bone_Sector15_start)
            && (erasedAlready[30] == 0))
    {
        erasedAlready[30] = 1;
        return (uint32_t) Bone_Sector14_start;
    }
    else if ((address >= Bone_Sector15_start) && (address <= Btwo_Sector1_start)
            && (erasedAlready[31] == 0))
    {
        erasedAlready[31] = 1;
        return (uint32_t) Bone_Sector15_start;
    }
    //
    //***************** Btwo_Sector0~15_start~End *************************
    //
     else if ((address >= Btwo_Sector0_start) && (address < Btwo_Sector1_start)
            && (erasedAlready[32] == 0))
    {
        erasedAlready[32] = 1;
        return (uint32_t) Btwo_Sector0_start;
    }
    else if ((address >= Btwo_Sector1_start) && (address < Btwo_Sector2_start)
            && (erasedAlready[33] == 0))
    {
        erasedAlready[33] = 1;
        return (uint32_t) Btwo_Sector1_start;
    }
    else if ((address >= Btwo_Sector2_start) && (address < Btwo_Sector3_start)
            && (erasedAlready[34] == 0))
    {
        erasedAlready[34] = 1;
        return (uint32_t) Btwo_Sector2_start;
    }
    else if ((address >= Btwo_Sector3_start) && (address < Btwo_Sector4_start)
            && (erasedAlready[35] == 0))
    {
        erasedAlready[35] = 1;
        return (uint32_t) Btwo_Sector3_start;
    }
    else if ((address >= Btwo_Sector4_start) && (address < Btwo_Sector5_start)
            && (erasedAlready[36] == 0))
    {
        erasedAlready[36] = 1;
        return (uint32_t) Btwo_Sector4_start;
    }
    else if ((address >= Btwo_Sector5_start) && (address < Btwo_Sector6_start)
            && (erasedAlready[37] == 0))
    {
        erasedAlready[37] = 1;
        return (uint32_t) Btwo_Sector5_start;
    }
    else if ((address >= Btwo_Sector6_start) && (address < Btwo_Sector7_start)
            && (erasedAlready[38] == 0))
    {
        erasedAlready[38] = 1;
        return (uint32_t) Btwo_Sector6_start;
    }
    else if ((address >= Btwo_Sector7_start) && (address < Btwo_Sector8_start)
            && (erasedAlready[39] == 0))
    {
        erasedAlready[39] = 1;
        return (uint32_t) Btwo_Sector7_start;
    }
    else if ((address >= Btwo_Sector8_start) && (address < Btwo_Sector9_start)
            && (erasedAlready[40] == 0))
    {
        erasedAlready[40] = 1;
        return (uint32_t) Btwo_Sector8_start;
    }
    else if ((address >= Btwo_Sector9_start) && (address < Btwo_Sector10_start)
            && (erasedAlready[41] == 0))
    {
        erasedAlready[41] = 1;
        return (uint32_t) Btwo_Sector9_start;
    }
    else if ((address >= Btwo_Sector10_start) && (address < Btwo_Sector11_start)
            && (erasedAlready[42] == 0))
    {
        erasedAlready[42] = 1;
        return (uint32_t) Btwo_Sector10_start;
    }
    else if ((address >= Btwo_Sector11_start) && (address < Btwo_Sector12_start)
            && (erasedAlready[43] == 0))
    {
        erasedAlready[43] = 1;
        return (uint32_t) Btwo_Sector11_start;
    }
    else if ((address >= Btwo_Sector12_start) && (address < Btwo_Sector13_start)
            && (erasedAlready[44] == 0))
    {
        erasedAlready[44] = 1;
        return (uint32_t) Btwo_Sector12_start;
    }
    else if ((address >= Btwo_Sector13_start) && (address < Btwo_Sector14_start)
            && (erasedAlready[45] == 0))
    {
        erasedAlready[45] = 1;
        return (uint32_t) Btwo_Sector13_start;
    }
    else if ((address >= Btwo_Sector14_start) && (address < Btwo_Sector15_start)
            && (erasedAlready[46] == 0))
    {
        erasedAlready[46] = 1;
        return (uint32_t) Btwo_Sector14_start;
    }
    else if ((address >= Btwo_Sector15_start) && (address <= FlashBank2EndAddress)
            && (erasedAlready[47] == 0))
    {
        erasedAlready[47] = 1;
        return (uint32_t) Btwo_Sector15_start;
    }
    else
    {
        return 0xdeadbeef; // a proxy address to signify that it is not
                           // a flash sector.
    }
}

//
// uint32_t findSize(uint32_t address) - This routine finds the size of the sector under use.
//
uint16_t findSize(uint32_t address)
{
    //
    // set erasedAlready, all sectors are 8K in 32bits in size.
    //
    return Sector8KB_u32length;

}

//
// uint32_t getLongData(void) - This routine fetches a 32-bit value from the peripheral
//                              input stream.
//
uint32_t getLongData()
{
    uint32_t longData;

    //
    // Fetch the upper 1/2 of the 32-bit value
    //
    longData = ((uint32_t) (*getWordData)() << 16);

    //
    // Fetch the lower 1/2 of the 32-bit value
    //
    longData |= (uint32_t) (*getWordData)();

    return longData;
}

//
//
// void readReservedFn(void) - This function reads 8 reserved words in the header.
//                             None of these reserved words are used by the
//                             this boot loader at this time, they may be used in
//                             future devices for enhancements.  Loaders that use
//                             these words use their own read function.
//
void readReservedFn()
{
    uint16_t i;

    //
    // Read and discard the 8 reserved words.
    //
    for (i = 1; i <= 8; i++)
    {
        getWordData();
    }
    return;
}

//
// void setFlashAPIError(Fapi_StatusType status) - sets Flash API error from 
//                                                 operation into struct to 
//                                                 display on console
//
void setFlashAPIError(Fapi_StatusType status)
{
    if(status == Fapi_Error_AsyncIncorrectDataBufferLength)
    {
        statusCode.flashAPIError = INCORRECT_DATA_BUFFER_LENGTH;
    }
    else if(status == Fapi_Error_AsyncIncorrectEccBufferLength)
    {
        statusCode.flashAPIError = INCORRECT_ECC_BUFFER_LENGTH;
    }
    else if(status == Fapi_Error_AsyncDataEccBufferLengthMismatch)
    {
        statusCode.flashAPIError = DATA_ECC_BUFFER_LENGTH_MISMATCH;
    }
    else if(status == Fapi_Error_FlashRegsNotWritable)
    {
        statusCode.flashAPIError = FLASH_REGS_NOT_WRITABLE;
    }
    else if(status == Fapi_Error_FeatureNotAvailable)
    {
        statusCode.flashAPIError = FEATURE_NOT_AVAILABLE;
    }
    else if(status == Fapi_Error_InvalidAddress)
    {
        statusCode.flashAPIError = INVALID_ADDRESS;
    }
    else if(status == Fapi_Error_Fail)
    {
        statusCode.flashAPIError = FAILURE;
    }
    else
    {
        statusCode.status = NOT_RECOGNIZED;
    }
}
