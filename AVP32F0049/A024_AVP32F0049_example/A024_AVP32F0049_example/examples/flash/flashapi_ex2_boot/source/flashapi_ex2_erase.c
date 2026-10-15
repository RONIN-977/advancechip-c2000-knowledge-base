/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  flashapi_ex2_erase.c
*                         TITLE: Erase Source Code
*******************************************************************************************/

//
// Includes
//
#include "flashapi_ex2_erase.h"


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

//
//************ 32(16x2) sectors Bzero/one_Sector0~15_start **********
//
const uint32_t sectAddress[32] = { Bzero_Sector0_start, Bzero_Sector1_start,
                                 Bzero_Sector2_start, Bzero_Sector3_start,
                                 Bzero_Sector4_start, Bzero_Sector5_start,
                                 Bzero_Sector6_start, Bzero_Sector7_start,
                                 Bzero_Sector8_start, Bzero_Sector9_start,
                                 Bzero_Sector10_start, Bzero_Sector11_start,
                                 Bzero_Sector12_start, Bzero_Sector13_start,
                                 Bzero_Sector14_start, Bzero_Sector15_start,
                                 Bone_Sector0_start, Bone_Sector1_start,
                                 Bone_Sector2_start, Bone_Sector3_start,
                                 Bone_Sector4_start, Bone_Sector5_start,
                                 Bone_Sector6_start, Bone_Sector7_start,
                                 Bone_Sector8_start, Bone_Sector9_start,
                                 Bone_Sector10_start, Bone_Sector11_start,
                                 Bone_Sector12_start, Bone_Sector13_start,
                                 Bone_Sector14_start, Bone_Sector15_start };

//
// Function Prototypes
//
void sharedErase(uint32_t sectors);
extern void exampleError(Fapi_StatusType status);
extern void sendACK(void);
extern void sendNAK(void);
extern void setFlashAPIError(Fapi_StatusType status);

//
// void sharedErase(uint32_t sectors) - This routine takes the 32-bit sectors
//                                      variable as a parameter.  Each bit
//                                      corresponds to a sector, starting with
//                                      bit 0 and sector A.  This routine
//                                      attempts to erase the sectors specified.
//
void sharedErase(uint32_t sectors)
{
    statusCode.status = NO_ERROR;
    statusCode.address = 0x12345678;
    statusCode.flashAPIError = NO_ERROR;
    statusCode.flashAPIFsmStatus = 0;

    int i = 0;
    Fapi_StatusType oReturnCheck;
    Fapi_FlashStatusType oFlashStatus;
    Fapi_FlashStatusWordType oFlashStatusWord;
    int fail = 0;

    EALLOW;
    for (i = 0; i < 32; i++)
    {
        if ((sectors & 0x00000001) == 0x00000001 && fail == 0)
        {
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(
                    Fapi_EraseSector, (uint32_t *) (sectAddress[i]));

            //
            // wait until AsyncCommand is done.
            //
            while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady)
            {
            }

            oFlashStatus = Fapi_getFsmStatus();

            //
            // All 8K sector size
            //
            oReturnCheck = Fapi_doBlankCheck((uint32_t *) (sectAddress[i]),
            B_8KSector_u32length,
                                             &oFlashStatusWord);
            if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
            {
                //
                // first fail
                //
                if (fail == 0) 
                {
                    statusCode.status = BLANK_ERROR;
                    statusCode.address = oFlashStatusWord.au32StatusWord[0];
                    setFlashAPIError(oReturnCheck);
                    statusCode.flashAPIFsmStatus = oFlashStatus;
                }
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
