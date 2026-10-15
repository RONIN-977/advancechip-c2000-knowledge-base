/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flash_kernel_ex3_erase.c
*                         TITLE: Erase Source Code
*******************************************************************************************/

//
// Includes
//
#include "flash_kernel_ex3_erase.h"


//
// Globals
//
typedef struct
{
    uint16_t status;
    uint32_t address;
    uint16_t flashAPIError;
    uint32_t flashAPIFsmStatus;
} StatusCode;
extern StatusCode statusCode;
const uint32_t sectAddress[48] = {Bzero_Sector0_start,
                                  Bzero_Sector1_start,
                                  Bzero_Sector2_start,
                                  Bzero_Sector3_start,
                                  Bzero_Sector4_start,
                                  Bzero_Sector5_start,
                                  Bzero_Sector6_start,   
                                  Bzero_Sector7_start,
                                  Bzero_Sector8_start,
                                  Bzero_Sector9_start,
                                  Bzero_Sector10_start,
                                  Bzero_Sector11_start,
                                  Bzero_Sector12_start,
                                  Bzero_Sector13_start,
                                  Bzero_Sector14_start,
                                  Bzero_Sector15_start,
                                  Bone_Sector0_start,
                                  Bone_Sector1_start,
                                  Bone_Sector2_start,
                                  Bone_Sector3_start,
                                  Bone_Sector4_start,
                                  Bone_Sector5_start,
                                  Bone_Sector6_start,   
                                  Bone_Sector7_start,
                                  Bone_Sector8_start,
                                  Bone_Sector9_start,
                                  Bone_Sector10_start,
                                  Bone_Sector11_start,
                                  Bone_Sector12_start,
                                  Bone_Sector13_start,
                                  Bone_Sector14_start,
                                  Bone_Sector15_start,
                                  Btwo_Sector0_start,
                                  Btwo_Sector1_start,
                                  Btwo_Sector2_start,
                                  Btwo_Sector3_start,
                                  Btwo_Sector4_start,
                                  Btwo_Sector5_start,
                                  Btwo_Sector6_start,  
                                  Btwo_Sector7_start,
                                  Btwo_Sector8_start,
                                  Btwo_Sector9_start,
                                  Btwo_Sector10_start,
                                  Btwo_Sector11_start,
                                  Btwo_Sector12_start,
                                  Btwo_Sector13_start,
                                  Btwo_Sector14_start,
                                  Btwo_Sector15_start};  

const uint16_t sectSize = Sector8KB_u32length;


//
// Function Prototypes
//
void sharedErase(uint32_t sectors, uint32_t bank);


//
// void sharedErase(uint32_t sectors,
//                  uint32_t bank) -    This routine takes the 32-bit sectors
//                                      variable and bank variable as parameters.  
//                                      Each bit corresponds to a sector, starting 
//                                      with bit 0 and sector 0.  This routine
//                                      attempts to erase the sectors specified 
//                                      within the bank specified.
//
void sharedErase(uint32_t sectors, uint32_t bank)
{
    statusCode.status = NO_ERROR;
    statusCode.address = 0x12345678;
    int i = 0;
    Fapi_StatusType oReturnCheck;
    Fapi_FlashStatusWordType oFlashStatusWord;
    Fapi_FlashStatusType oFlashStatus;
    int fail = 0;

    EALLOW;
    //
    // Set Bank Erase mask by taking the bitwise NOT of the sectors passed in
    // The sectors masked will not be erased
    //
    uint16_t mask = ~sectors;

    //
    // Set bankAddress based on bank value passed in
    //
    uint32_t bankAddress = Bzero_Sector0_start;
    if(bank == 1)
    {
        bankAddress = Bone_Sector0_start;
    }
    else if(bank == 2)
    {
        bankAddress = Btwo_Sector0_start;
    }

    oReturnCheck = Fapi_issueBankEraseCommand((uint32 *)bankAddress, mask);

    // Wait until FSM is done with erase bank operation
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

    oFlashStatus = Fapi_getFsmStatus();

    if(oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
    {
        //
        // first fail
        //
            statusCode.status = BANK_ERASE_ERROR;
            //
            // Address is not needed for Bank Erase Error
            //
            statusCode.address = 0;
            setFlashAPIError(oReturnCheck);
            statusCode.flashAPIFsmStatus = oFlashStatus;
            fail = 1;
    }

    for (i = 0; i < 16; i++)
    {
        if ((sectors & 0x00000001) == 0x00000001 && fail == 0)
        {

            //
            // All 8K sector size
            //
            oReturnCheck = Fapi_doBlankCheck(((uint32_t *) (sectAddress[i + (16 * bank)])),
                                             sectSize,
                                             &oFlashStatusWord);

            if (oReturnCheck != Fapi_Status_Success)
            {

                statusCode.status = BLANK_ERROR;
                statusCode.address = oFlashStatusWord.au32StatusWord[0];
                setFlashAPIError(oReturnCheck);
                statusCode.flashAPIFsmStatus = 0;

                fail++;
            }
        }
        sectors = sectors >> 1;
    }
    EDIS;
    return;
}




//
// End of file
//
