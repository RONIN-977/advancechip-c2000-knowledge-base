/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :dsp_program.c
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "dsp_program.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"
#include "crc_verify.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef void (* application)(void);

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/
const uint32_t sectAddress[32] = {
    Bzero_Sector0_start, Bzero_Sector1_start,
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
    Bone_Sector14_start, Bone_Sector15_start
};

DCSM_CSMPasswordKey psCMDKey;

uint32_t* Write_Address = (uint32_t*)APP_WRITE_ADDRESS;//FLASH写入地址指针
uint16_t Write_Total_Len = 0;//累计接收到BIN固件的大小，单位是word

uint16_t jump_flag;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
#pragma CODE_SECTION(Flash_API_Init, ".TI.ramfunc");
#pragma CODE_SECTION(Flash_App_Section_Erase, ".TI.ramfunc");
#pragma CODE_SECTION(Flash_App_Section_Write, ".TI.ramfunc");
#pragma CODE_SECTION(Flash_App_Section_Write_UpdataFlag, ".TI.ramfunc");

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:flash api初始化
*Input:
*Output:
-----------------------------------------------------------------------------*/
void Flash_API_Init(void)
{
    Fapi_StatusType oReturnCheck = Fapi_Status_Success;

    psCMDKey.csmKey0 = (uint32_t)0xFFFF | ((uint32_t)0xFFFF << 16);
    psCMDKey.csmKey1 = (uint32_t)0xFFFF | ((uint32_t)0x4D7F << 16);
    psCMDKey.csmKey2 = (uint32_t)0xFFFF | ((uint32_t)0xFFFF << 16);
    psCMDKey.csmKey3 = (uint32_t)0xFFFF | ((uint32_t)0xFFFF << 16);
    DCSM_unlockZone1CSM(&psCMDKey);
    if(DCSM_getZone1CSMSecurityStatus() == DCSM_STATUS_LOCKED)
    {
        return;//解锁异常
    }

//    EALLOW;
//    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 100);
//    if(oReturnCheck != Fapi_Status_Success)
//    {
//        return;
//    }
//    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
//    if(oReturnCheck != Fapi_Status_Success)
//    {
//        return;
//    }
//    EDIS;
}

/******************************************************************************
*Description:初始化APP写入FLASH的地址指针
-----------------------------------------------------------------------------*/
void Flash_App_Section_Write_Init(void)
{
    Write_Address = (uint32_t*)APP_WRITE_ADDRESS;
    Write_Total_Len = 0;
}

/******************************************************************************
*Description:app跳回boot时，往一个固定的RAM地址写入一个升级请求标志，在boot内检测到该标志，使程序停在boot待升级，并回复响应会话
-----------------------------------------------------------------------------*/
void Flash_App_Section_Set_UpdataFlag(void)
{
    *(uint16_t*)(APP_UPGRADE_FLAG_RAM_ADDRESS) = APP_UPGRADE_FLAG_APP_JUMP_BOOT;
}

/******************************************************************************
*Description:boot跳回app时，往一个固定的RAM地址写入一个升级请求标志，在app发起复位响应
-----------------------------------------------------------------------------*/
void Flash_Boot_Section_Set_UpdataFlag(void)
{
    *(uint16_t*)(APP_UPGRADE_FLAG_RAM_ADDRESS) = APP_UPGRADE_FLAG_BOOT_JUMP_APP;
}

/******************************************************************************
*Description:清除升级检测标志
-----------------------------------------------------------------------------*/
void Flash_AB_Section_Clear_UpdataFlag(void)
{
    *(uint16_t*)(APP_UPGRADE_FLAG_RAM_ADDRESS) = APP_UPGRADE_FLAG_CLEAR;
}

/******************************************************************************
*Description:获取固定RAM地址的升级请求标志，判断是否需要升级
-----------------------------------------------------------------------------*/
uint16_t Flash_App_Section_Get_UpdataFlag(void)
{
    uint16_t flag_data = *(uint16_t*)(APP_UPGRADE_FLAG_RAM_ADDRESS);
    return flag_data;
}

/******************************************************************************
*Description:擦除APP段的FLASH空间
-----------------------------------------------------------------------------*/
uint16_t Flash_App_Section_Erase(void)
{
    uint16_t i = 0;
    uint16_t fail = 0;
    uint16_t Status = 0;
    Fapi_FlashStatusType        oFlashStatus;
    Fapi_StatusType             oReturnCheck;
    Fapi_FlashStatusWordType    oFlashStatusWord;

    DINT;
    EALLOW;
    for(i=0; i < APP_FLASH_SECTOR_CNT; i++)
    {
        oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t*)(sectAddress[i+3]));
        while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady);
        oFlashStatus = Fapi_getFsmStatus();
        oReturnCheck = Fapi_doBlankCheck((uint32_t*)(sectAddress[i+3]), Sector8KB_u32length, &oFlashStatusWord);
        if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
        {
            fail++;
        }
    }
    EDIS;
    EINT;
    Status = fail;//oReturnCheck;
    return Status;
}

/******************************************************************************
*Description:固件写入FLASH，一次写64word，写成功后检验写入的数据，校验成功才偏移下一次写入的地址和递增接收的固件长度
-----------------------------------------------------------------------------*/
uint16_t Flash_App_Section_Write(uint16_t* pAddress, uint16_t length)
{
    uint16_t Status = 0;
    uint16_t i = 0;
    Fapi_FlashStatusType        oFlashStatus;
    Fapi_StatusType             oReturnCheck;
    Fapi_FlashStatusWordType    oFlashStatusWord;

    DINT;
    EALLOW;
    for(i=0; i<8; i++)//length的长度要满64word，分8次写入
    {
        //一次写8 word  即128bit对齐
        oReturnCheck = Fapi_issueProgrammingCommand(Write_Address, (uint16_t*)(pAddress+i*8), 8, 0, 0, Fapi_AutoEccGeneration);
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
        oFlashStatus = Fapi_getFsmStatus();
        if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
        {
            Status = oReturnCheck;
            break;
        }
        else
        {
            oReturnCheck = Fapi_doVerifyBy16bits((uint16_t*)Write_Address, 8, (uint16_t*)(pAddress+i*8), &oFlashStatusWord);
            if(oReturnCheck == Fapi_Status_Success)
            {
                Write_Total_Len += 8;
                Write_Address = (uint32_t*)(APP_WRITE_ADDRESS + Write_Total_Len);
            }
            else
            {
                Status = oReturnCheck;
            }
        }
    }
    EDIS;
    EINT;
    return Status;
}

/******************************************************************************
*Description:当固件传输完成且APP数据校验完成后，写入APP的CRC值,APP的长度值
-----------------------------------------------------------------------------*/
uint16_t Flash_App_Section_Write_UpdataFlag(uint32_t length, uint16_t crc)
{
    uint16_t buf[8] = {0x0, 0x0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    Fapi_FlashStatusType        oFlashStatus;
    Fapi_StatusType             oReturnCheck;
    Fapi_FlashStatusWordType    oFlashStatusWord;
    uint16_t Status = 0;

    buf[0] = length >> 16;
    buf[1] = length & 0xFFFF;
    buf[2] = crc;

    DINT;
    oReturnCheck = Fapi_issueProgrammingCommand((uint32_t*)APP_LENGTH_ADDRESS, (uint16_t*)&buf, 8, 0, 0, Fapi_AutoEccGeneration);
    while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
    oFlashStatus = Fapi_getFsmStatus();
    if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
    {
        Status = oReturnCheck;
    }
    else
    {
        oReturnCheck = Fapi_doVerifyBy16bits((uint16_t*)APP_LENGTH_ADDRESS, 8, (uint16_t*)&buf, &oFlashStatusWord);
        if(oReturnCheck != Fapi_Status_Success)
        {
            Status = oReturnCheck;
        }
    }
    EINT;
    return Status;
}

/******************************************************************************
*Description:擦除E²所在的flash区域
-----------------------------------------------------------------------------*/
uint16_t Flash_EEPROM_Section_Erase(void)
{
    uint16_t fail = 0;

    return fail;
}

/******************************************************************************
*Description:写E²数据
-----------------------------------------------------------------------------*/
uint16_t Flash_EEPROM_Section_Write(uint32_t address, uint16_t* pdata, uint16_t length)
{
    uint16_t Status = 0;

    return Status;
}

/******************************************************************************
*Description:读E²数据
-----------------------------------------------------------------------------*/
uint16_t Flash_EEPROM_Section_Read(uint32_t address, uint16_t* pdata, uint16_t length)
{
    uint16_t Status = 0;
    uint16_t index = 0;
    DINT;
    do
    {
        *(pdata + index) = *((uint16_t*)(address + index));
        index++;
    }while(index != length);
    EINT;
    return Status;
}

/******************************************************************************
*Description:写E²的RAM区数据
-----------------------------------------------------------------------------*/
uint16_t Flash_EEPROM_AT_RAM_Section_Write(uint32_t address, uint16_t* pdata, uint16_t length)
{
    uint16_t Status = 0;
    uint16_t index = 0;
    DINT;
    do
    {
        *((uint16_t*)(address + index)) = *(pdata + index);
        index++;
    }while(index != length);
    EINT;
    return Status;
}

/******************************************************************************
*Description:读E²的RAM区数据
-----------------------------------------------------------------------------*/
uint16_t Flash_EEPROM_AT_RAM_Section_Read(uint32_t address, uint16_t* pdata, uint16_t length)
{
    uint16_t Status = 0;
    uint16_t index = 0;
    DINT;
    do
    {
        *(pdata + index) = *((uint16_t*)(address + index));
        index++;
    }while(index != length);
    EINT;
    return Status;
}

/******************************************************************************
*Description:获取app crc值
-----------------------------------------------------------------------------*/
uint16_t Flash_App_Section_CRC(void)
{
    uint16_t temp = *((uint16_t*)APP_CRC_ADDRESS);
    return temp;
}

/******************************************************************************
*Description:获取app 长度值
-----------------------------------------------------------------------------*/
uint32_t Flash_App_Section_Length(void)
{
    uint32_t temp = *((uint32_t*)APP_LENGTH_ADDRESS);
    return temp;
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void Set_JumpToBoot(void)
{
    jump_flag = 1;
}

/******************************************************************************
*Description:从boot跳到app的指针函数，立即跳转
-----------------------------------------------------------------------------*/
void DSP_JumpToBoot(void)
{
    Flash_App_Section_Set_UpdataFlag();

    DINT;
    application application_pstr;
    application_pstr = (application )BOOT_BEGIN_ADDRESS;
    (*application_pstr)();
}

/******************************************************************************
*Description:从boot跳到app的指针函数,放在10ms运行
-----------------------------------------------------------------------------*/
void DSP_JumpToBoot_delay(void)
{
    static uint16_t delaytime = 0;

    if(jump_flag == 1)
    {
        delaytime++;
        if(delaytime > 5)
        {
            delaytime = 0;
            jump_flag = 0;
            Flash_App_Section_Set_UpdataFlag();

            DINT;
            application application_pstr;
            application_pstr = (application )BOOT_BEGIN_ADDRESS;
            (*application_pstr)();
        }
    }
}

/******************************************************************************
*Description:程序跳转检测
-----------------------------------------------------------------------------*/
#if(0)
void Jump_Check(void)
{
    uint16_t flag = Flash_App_Section_Get_UpdataFlag();
    uint16_t crc = *(uint16_t*)APP_CRC_ADDRESS;
    uint16_t checkCal;
    uint32_t length = ((uint32_t)*(uint16_t*)APP_LENGTH_ADDRESS)<<16;
    length |= *(uint16_t*)(APP_LENGTH_ADDRESS+1);

    if(flag == APP_UPGRADE_FLAG_APP_JUMP_BOOT)
    {
        Flash_AB_Section_Clear_UpdataFlag();
    }

    if((crc != 0xFFFF) && (crc != 0x0))
    {
        //跳到app
        checkCal = CRC16Bit_APP(APP_BEGIN_ADDRESS, length);
        if((checkCal == crc) && (flag != APP_UPGRADE_FLAG_APP_JUMP_BOOT))
        {
            DSP_JumpToApp();
        }
    }
}
#endif






/* End of file */
