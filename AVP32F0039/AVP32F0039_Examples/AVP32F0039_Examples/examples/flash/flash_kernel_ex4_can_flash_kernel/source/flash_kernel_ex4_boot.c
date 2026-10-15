/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flash_kernel_ex4_boot.c
*                         TITLE: Boot loader shared functions
*******************************************************************************************/


//
// Included Files
//
#include "cpu1bootrom.h"
#include "cpu1brom_utils.h"

#include "flash_programming_f28003x.h"
#include "F021_F28003x_C28x.h"


#include "driverlib.h"

#define BUFFER_SIZE               0x200

typedef enum
{
    MCAN_DATA_SIZE_16BITS = 2U,
    MCAN_DATA_SIZE_32BITS = 4U
}MCAN_dataTypeSize;

#define LOWER_FIRST_BLOCK_SIZE      22U
#define UPPER_FIRST_BLOCK_SIZE      23U
//
// GetWordData is a pointer to the function that interfaces to the peripheral.
// Each loader assigns this pointer to it's particular GetWordData function.
//
uint16fptr GetWordData;

//
// Function prototypes
//
uint32_t GetLongData();
void CopyData(void);
void ReadReservedFn(void);
extern void sharedErase(uint32_t sectors);
uint32_t findSector(uint32_t address);
uint16_t findSize(uint32_t address);
extern uint32_t MCAN_getDataFromBuffer(MCAN_dataTypeSize dataTypeSize);
extern uint16_t msgBufferIndex;

//
// 48(16x3) sectors.
//
unsigned char erasedAlready[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//
// CopyApplication - This routine copies multiple blocks of data from the host
//                   to the specified Flash locations. It is assumed that the
//                   application is linked to Flash correctly and that the image is
//                   128 bit aligned. Errors from the Flash API are not currently
//                   being relayed to the host.
//
//                   Multiple blocks of data are copied until a block
//                   size of 00 00 is encountered.
//

void CopyApplication(MCAN_RxBufElement rxMsg)
{
    struct HEADER {
      uint16_t BlockSize;
      uint32_t DestAddr;
    } BlockHeader;

    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t k = 0;
    uint16_t fail = 0;
    uint32_t sectorAddress;
    uint16_t sectorSize;
    uint16_t wordsWritten = 0;

    //
    // wordData: Stores a word of data
    //
    uint16_t wordData;

    //
    // miniBuffer: Useful for 4-word access to flash
    //
    uint16_t miniBuffer[8];

    //
    // Buffer: Used to program data to flash
    //
    uint16_t Buffer[BUFFER_SIZE];

    //
    // Error return variable
    //
    Fapi_StatusType oReturnCheck;

    //
    // FAPI initialization
    //
    Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 120);
    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);

    if(oReturnCheck != Fapi_Status_Success)
    {
        return;
    }


    //
    // Get the size in words of the first block
    //
    BlockHeader.BlockSize = BUILD_WORD(rxMsg.data[LOWER_FIRST_BLOCK_SIZE],
                                       rxMsg.data[UPPER_FIRST_BLOCK_SIZE]);

    //
    // Set the message buffer index for reading next stream data
    //
    msgBufferIndex = UPPER_FIRST_BLOCK_SIZE + 1U;

    //
    // While the block size is > 0 flash the data
    // to the DestAddr.  There is no error checking
    // as it is assumed the DestAddr is a valid
    // memory location
    //
    while(BlockHeader.BlockSize != (uint16_t)0x0000)
    {
            Fapi_StatusType oReturnCheck;
            Fapi_FlashStatusWordType oFlashStatusWord;
            Fapi_FlashStatusType oFlashStatus;

            BlockHeader.DestAddr = MCAN_getDataFromBuffer(MCAN_DATA_SIZE_32BITS);

            //
            // Iterate through the block of data in order to program the data
            // in flas
            //
            for (i = 0; i < BlockHeader.BlockSize; i += 0)
            {
                //
                // If the size of the block of data is less than the size of the buffer,
                // then fill the buffer with the block of data and pad the remaining
                // elements
                //
                if (BlockHeader.BlockSize < BUFFER_SIZE)
                {
                    //
                    // Receive the block of data one word at a time and place it in
                    // the buffer
                    //
                    for (j = 0; j < BlockHeader.BlockSize; j++)
                    {
                        //
                        // Receive one word of data
                        //
                        wordData = (uint16_t)(MCAN_getDataFromBuffer(MCAN_DATA_SIZE_16BITS));

                        //
                        // Put the word of data in the buffer
                        //
                        Buffer[j] = wordData;

                        //
                        // Increment i to keep track of how many words have been received
                        //
                        i++;
                    }

                    //
                    // Pad the remaining elements of the buffer
                    //
                    for (j = BlockHeader.BlockSize; j < BUFFER_SIZE; j++)
                    {
                        //
                        // Put 0xFFFF into the current element of the buffer. OxFFFF is equal to erased
                        // data and has no effect
                        //
                        Buffer[j] = 0xFFFF;
                    }
                }

                //
                // Block is to big to fit into our buffer so we must program it in
                // chunks
                //
                else
                {
                    //
                    // Less than one BUFFER_SIZE left
                    //
                    if ((BlockHeader.BlockSize - i) < BUFFER_SIZE)
                    {
                        //
                        // Fill Buffer with rest of data
                        //
                        for (j = 0; j < BlockHeader.BlockSize - i; j++)
                        {
                           //
                           // Receive one word of data
                           //
                           wordData = (uint16_t)(MCAN_getDataFromBuffer(MCAN_DATA_SIZE_16BITS));

                           //
                           // Put the word of data into the current element of Buffer
                           //
                           Buffer[j] = wordData;
                        }

                        //
                        // Increment i outside here so it doesn't affect loop above
                        //
                        i += j;

                        //
                        // Fill the rest with 0xFFFF
                        //
                        for (; j < BUFFER_SIZE; j++)
                        {
                           Buffer[j] = 0xFFFF;
                        }
                    }
                    else
                    {
                        //
                        // Fill up like normal, up to BUFFER_SIZE
                        //
                        for (j = 0; j < BUFFER_SIZE; j++)
                        {
                           wordData = (uint16_t)(MCAN_getDataFromBuffer(MCAN_DATA_SIZE_16BITS));
                           Buffer[j] = wordData;
                           i++;
                        }
                    }
                }

                //
                // Fill miniBuffer with the data in Buffer in order to program the data
                // to flash; miniBuffer takes data from Buffer, 88 words at a time.
                //
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
                                // FindSize just returns 0x4000.
                                //
                                sectorSize = findSize(sectorAddress);
                                oReturnCheck = Fapi_issueAsyncCommandWithAddress(
                                        Fapi_EraseSector, (uint32_t *) sectorAddress);

                                while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

                                oFlashStatus = Fapi_getFsmStatus();

                                oReturnCheck = Fapi_doBlankCheck(
                                        (uint32_t *) sectorAddress, sectorSize,
                                        &oFlashStatusWord);
                                if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
                                {
                                    fail++;
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
                                    fail++;
                                }
                            }
                        } //for j; for Fapi_doVerify
                    } //check if all the words are not already written
                    BlockHeader.DestAddr += 0x8;
                    wordsWritten += 0x8;
                } //for(int k); loads miniBuffer with Buffer elements
        }
        //
        // Get the size of the next block
        //
        BlockHeader.BlockSize = (uint16_t)(MCAN_getDataFromBuffer(MCAN_DATA_SIZE_16BITS));
        wordsWritten = 0;
    }
    return;
}

//
// getLongData -    Fetches a 32-bit value from the peripheral
//                  input stream.
//

uint32_t GetLongData(void)
{
    uint32_t longData;

    //
    // Fetch the upper 1/2 of the 32-bit value
    //
    longData = ( (uint32_t)(*GetWordData)() << 16);

    //
    // Fetch the lower 1/2 of the 32-bit value
    //
    longData |= (uint32_t)(*GetWordData)();

    return longData;
}

//
// Read_ReservedFn -    Reads 8 reserved words in the header.
//                      None of these reserved words are used by the
//                      this boot loader at this time, they may be used in
//                      future devices for enhancments.  Loaders that use
//                      these words use their own read function.
//

void ReadReservedFn(void)
{
    uint16_t i;

    //
    // Read and discard the 8 reserved words.
    //
    for(i = 1; i <= 8; i++)
    {
       GetWordData();
    }
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
