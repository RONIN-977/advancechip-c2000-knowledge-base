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

#pragma RETAIN(dsp_info)
const DSP_INFORMATION_T dsp_info =
{
 /* index[2] */0x3412, 0x7856,//固定值
 /* dsp_type */0x08,
 /* firmware_type */0x00,//0:boot 1:app1 2:app2
 /* boot_version[8] */'v', '2', '.', '0', '0', '.', '0', '0',
 /* app_version[8] */ 'v', '2', '.', '0', '0', '.', '0', '0',
 /* boot_download_date[10] */'2', '0', '2', '5', '.', '1', '2', '.', '0', '8',
 /* app_download_date[10] */ '2', '0', '2', '5', '.', '1', '2', '.', '0', '8',
 /* app_head_address[2] */APP_BEGIN_ADDRESS >> 16, APP_BEGIN_ADDRESS & 0xFFFF,
 /* app_tail_address[2] */0x3F1FFD >> 16, 0x3F1FFD & 0xFFFF,
 /* app_total_length[2] */APP_FLASH_LENGHT >> 16, APP_FLASH_LENGHT & 0xFFFF,
 /* app_length_address[2] */APP_LENGTH_ADDRESS >> 16, APP_LENGTH_ADDRESS & 0xFFFF,
 /* app_verify_address[2] */APP_CRC_ADDRESS >> 16, APP_CRC_ADDRESS & 0xFFFF,
 /* app_verify_type */0x01,//0:没有校验  1:在烧录时，由上位机写校验  2:在编译时，产生校验到固定位置  3:在程序中固定地址使用固定校验标志
};

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
#pragma CODE_SECTION(Flash_API_Init, ".TI.ramfunc");
#pragma CODE_SECTION(Flash_App_Section_Erase, ".TI.ramfunc");
#pragma CODE_SECTION(Flash_App_Section_Write, ".TI.ramfunc");

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

    EALLOW;
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 100);
    if(oReturnCheck != Fapi_Status_Success)
    {
        return;
    }
    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    if(oReturnCheck != Fapi_Status_Success)
    {
        return;
    }
    EDIS;
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
*Description:写入报文的包序列号
-----------------------------------------------------------------------------*/
void RAM_Write_Package_Seq(uint16_t n)
{
    *(uint16_t*)(APP_UPGRADE_FLAG_RAM_ADDRESS + 1) = n;
}

/******************************************************************************
*Description:读出报文的包序列号
-----------------------------------------------------------------------------*/
uint16_t RAM_Read_Package_Seq(void)
{
    return *(uint16_t*)(APP_UPGRADE_FLAG_RAM_ADDRESS + 1);
}

/******************************************************************************
*Description:擦除APP段的FLASH空间
-----------------------------------------------------------------------------*/
uint32_t Flash_App_Section_Erase(void)
{
    uint16_t i = 0;
    Fapi_FlashStatusType        oFlashStatus;
    Fapi_StatusType             oReturnCheck;
    Fapi_StatusType             oReturnCheck2;
    Fapi_FlashStatusWordType    oFlashStatusWord;

    EALLOW;
    for(i=0; i < APP_FLASH_SECTOR_CNT; i++)
    {
        oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t*)(sectAddress[i+4]));
        while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady);
        oFlashStatus = Fapi_getFsmStatus();
        if((oReturnCheck == Fapi_Status_Success))
        {
            oReturnCheck2 = Fapi_doBlankCheck((uint32_t*)(sectAddress[i+4]), Sector8KB_u32length, &oFlashStatusWord);
            if(oReturnCheck2 != Fapi_Status_Success)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    EDIS;
    return (uint32_t)(((uint32_t)oReturnCheck << 16) | ((uint32_t)oReturnCheck2 & 0xFFFF));
}

/******************************************************************************
*Description:固件写入FLASH，一次写64word，写成功后检验写入的数据
-----------------------------------------------------------------------------*/
uint32_t Flash_App_Section_Write(uint32_t address, uint16_t* pbuf, uint16_t length)
{
    uint16_t i = 0;
    Fapi_FlashStatusType        oFlashStatus;
    Fapi_StatusType             oReturnCheck;
    Fapi_StatusType             oReturnCheck2;
    Fapi_FlashStatusWordType    oFlashStatusWord;
    uint16_t                    write_cnt = 0;

    if((length % 8) == 0)
    {
        write_cnt = length / 8;//写入循环次数
    }
    else
    {
        write_cnt = (length / 8) + 1;
    }

    EALLOW;
    for(i=0; i<write_cnt; i++)//length的长度要满64word，分8次写入
    {
        //一次写8 word  即128bit对齐
        oReturnCheck = Fapi_issueProgrammingCommand((uint32_t*)(address + i * 8), (uint16_t*)(pbuf+i*8), 8, 0, 0, Fapi_AutoEccGeneration);
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
        oFlashStatus = Fapi_getFsmStatus();
        if (oReturnCheck != Fapi_Status_Success || oFlashStatus != 0)
        {
            break;
        }
        else
        {
            oReturnCheck2 = Fapi_doVerifyBy16bits((uint16_t*)(address + i * 8), 8, (uint16_t*)(pbuf+i*8), &oFlashStatusWord);
            if(oReturnCheck2 != Fapi_Status_Success)
            {
                break;
            }
        }
    }
    EDIS;
    return (uint32_t)(((uint32_t)oReturnCheck << 16) | ((uint32_t)oReturnCheck2 & 0xFFFF));
}

/******************************************************************************
*Description:从boot跳到app的指针函数，立即跳转
-----------------------------------------------------------------------------*/
void DSP_JumpToApp(void)
{
    Flash_Boot_Section_Set_UpdataFlag();

    DINT;
    application application_pstr;
    application_pstr = (application )APP_BEGIN_ADDRESS;
    (*application_pstr)();
}

void DSP_JumpToApp_NoSet(void)
{
    DINT;
    application application_pstr;
    application_pstr = (application )APP_BEGIN_ADDRESS;
    (*application_pstr)();
}

/******************************************************************************
*Description:检查是否从app跳回boot，如果是需要回复报文    0:不回复   1：回复
-----------------------------------------------------------------------------*/
uint16_t Is_App_Jump_Boot_Check(void)
{
    uint16_t flag = Flash_App_Section_Get_UpdataFlag();
    uint16_t rec = 0;

    Flash_AB_Section_Clear_UpdataFlag();
    if(flag == APP_UPGRADE_FLAG_APP_JUMP_BOOT)
    {
        rec = 1;
    }
    else
    {
        rec = 0;
    }
    return rec;
}

/******************************************************************************
*Description:程序跳转检测
-----------------------------------------------------------------------------*/
void Jump_Check(void)
{
    uint16_t flag = Flash_App_Section_Get_UpdataFlag();
    uint16_t crc;
    uint16_t checkCal;
    uint32_t length;

    //上电跳转到app的条件检测
    if(dsp_info.info.app_verify_type == 0)
    {
        //没有校验
        if(flag != APP_UPGRADE_FLAG_APP_JUMP_BOOT)
        {
            Flash_AB_Section_Clear_UpdataFlag();//上电跳转，就不需要app返回报文
            DSP_JumpToApp_NoSet();
        }
    }
    else if(dsp_info.info.app_verify_type == 1)
    {
        //上位机写入的固件校验
        crc = *(uint16_t*)APP_CRC_ADDRESS;
        length = ((uint32_t)*(uint16_t*)APP_LENGTH_ADDRESS)<<16;
        length |= *(uint16_t*)(APP_LENGTH_ADDRESS+1);

        if(flag != APP_UPGRADE_FLAG_APP_JUMP_BOOT)
        {
            if((crc != 0xFFFF) && (crc != 0x0))
            {
                //上电跳转
                checkCal = CRC16Bit_APP(APP_BEGIN_ADDRESS, length);
                if(checkCal == crc)
                {
                    Flash_AB_Section_Clear_UpdataFlag();//上电跳转，就不需要app返回报文
                    DSP_JumpToApp_NoSet();
                }
            }
        }
        else
        {
            //app请求的升级，停留在boot
        }
    }
    else if(dsp_info.info.app_verify_type == 2)
    {
        //编译时，产生校验到固定位置
    }
    else if(dsp_info.info.app_verify_type == 3)
    {
        //使用固定校验值
    }
}







/* End of file */
