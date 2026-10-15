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
#include "AVP33X_API_Library.h"
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
FLASH_ST EraseStatus;
FLASH_ST ProgStatus;
FLASH_ST VerifyStatus;

Uint16 *Write_Address = (Uint16*)APP_WRITE_ADDRESS;//FLASH写入地址指针
Uint32 Write_Total_Len = 0;//累计接收到BIN固件的大小，单位是word

Uint16 jump_flag;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/

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
    Uint16 rec = 0;

    jump_flag = 0;

    DINT;
    rec = CsmUnlock();

    MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);
    EALLOW;
    Flash_CPUScaleFactor = SCALE_FACTOR;
    Flash_CallbackPtr = 0;
    EDIS;
}

/******************************************************************************
*Description:初始化APP写入FLASH的地址指针
-----------------------------------------------------------------------------*/
void Flash_App_Section_Write_Init(void)
{
    Write_Address = (Uint16*)APP_WRITE_ADDRESS;
    Write_Total_Len = 0;
}

/******************************************************************************
*Description:app跳回boot时，往一个固定的RAM地址写入一个升级请求标志，在boot内检测到该标志，使程序停在boot待升级，并回复响应会话
-----------------------------------------------------------------------------*/
void Flash_App_Section_Set_UpdataFlag(void)
{
    *(Uint16*)(APP_UPGRADE_FLAG_RAM_ADDRESS) = APP_UPGRADE_FLAG_APP_JUMP_BOOT;
}

/******************************************************************************
*Description:boot跳回app时，往一个固定的RAM地址写入一个升级请求标志，在app发起复位响应
-----------------------------------------------------------------------------*/
void Flash_Boot_Section_Set_UpdataFlag(void)
{
    *(Uint16*)(APP_UPGRADE_FLAG_RAM_ADDRESS) = APP_UPGRADE_FLAG_BOOT_JUMP_APP;
}

/******************************************************************************
*Description:清除升级检测标志
-----------------------------------------------------------------------------*/
void Flash_AB_Section_Clear_UpdataFlag(void)
{
    *(Uint16*)(APP_UPGRADE_FLAG_RAM_ADDRESS) = APP_UPGRADE_FLAG_CLEAR;
}

/******************************************************************************
*Description:获取固定RAM地址的升级请求标志，判断是否需要升级
-----------------------------------------------------------------------------*/
Uint16 Flash_App_Section_Get_UpdataFlag(void)
{
    Uint16 flag_data = *(Uint16*)(APP_UPGRADE_FLAG_RAM_ADDRESS);
    return flag_data;
}

/******************************************************************************
*Description:擦除APP段的FLASH空间
-----------------------------------------------------------------------------*/
Uint16 Flash_App_Section_Erase(void)
{
    Uint16 Status = 0;

    DINT;
    Status = Flash_Erase(SECTORH|SECTORG|SECTORF|SECTORE|SECTORD|SECTORC|SECTORB, &EraseStatus);
    EINT;
    return Status;
}

/******************************************************************************
*Description:固件写入FLASH，一次写64word，写成功后检验写入的数据，校验成功才偏移下一次写入的地址和递增接收的固件长度
-----------------------------------------------------------------------------*/
Uint16 Flash_App_Section_Write(Uint16* pAddress, Uint16 length)
{
    Uint16 Status = 0;
    Uint16 Status2 = 0;
    DINT;
    Status = Flash_Program(Write_Address, pAddress, length, &ProgStatus);
    if(Status == 0)
    {
        Status2 = Flash_Verify( Write_Address, pAddress, length, &VerifyStatus);
        if(Status2 == 0)
        {
            Write_Total_Len += length;//接收的bin长度累加，作为写地址指针的偏移量
            Write_Address = (Uint16*)(APP_WRITE_ADDRESS + Write_Total_Len);
        }
        else
        {
            Status |= (Status2<<8);
        }
    }
    EINT;
    return Status;
}

/******************************************************************************
*Description:当固件传输完成且APP数据校验完成后，写入APP的CRC值,APP的长度值
-----------------------------------------------------------------------------*/
Uint16 Flash_App_Section_Write_UpdataFlag(Uint32 length, Uint16 crc)
{
    Uint16 buf[3] = {0x0, 0x0, 0xFFFF};
    Uint16 Status = 0;

    buf[0] = length >> 16;
    buf[1] = length & 0xFFFF;
    buf[2] = crc;

    DINT;
    Status = Flash_Program((Uint16*)APP_LENGTH_ADDRESS, (Uint16*)&buf, 3, &ProgStatus);
    EINT;
    return Status;
}

/******************************************************************************
*Description:擦除E²所在的flash区域
-----------------------------------------------------------------------------*/
Uint16 Flash_EEPROM_Section_Erase(void)
{
    Uint16 Status = 0;

    DINT;
    Status = Flash_Erase(SECTORC, &EraseStatus);
    EINT;
    return Status;
}

/******************************************************************************
*Description:写E²数据
-----------------------------------------------------------------------------*/
Uint16 Flash_EEPROM_Section_Write(Uint32 address, Uint16* pdata, Uint16 length)
{
    Uint16 Status = 0;
    Uint16 Status2 = 0;
    DINT;
    Status = Flash_Program((Uint16*)&address, pdata, length, &ProgStatus);
    if(Status == 0)
    {
        Status2 = Flash_Verify((Uint16*)&address, pdata, length, &VerifyStatus);
        if(Status2 != 0)
        {
            Status |= (Status2<<8);
        }
    }
    EINT;
    return Status;
}

/******************************************************************************
*Description:读E²数据
-----------------------------------------------------------------------------*/
Uint16 Flash_EEPROM_Section_Read(Uint32 address, Uint16* pdata, Uint16 length)
{
    Uint16 Status = 0;
    Uint16 index = 0;
    DINT;
    do
    {
        *(pdata + index) = *((Uint16*)(address + index));
        index++;
    }while(index != length);
    EINT;
    return Status;
}

/******************************************************************************
*Description:写E²的RAM区数据
-----------------------------------------------------------------------------*/
Uint16 Flash_EEPROM_AT_RAM_Section_Write(Uint32 address, Uint16* pdata, Uint16 length)
{
    Uint16 Status = 0;
    Uint16 index = 0;
    DINT;
    do
    {
        *((Uint16*)(address + index)) = *(pdata + index);
        index++;
    }while(index != length);
    EINT;
    return Status;
}

/******************************************************************************
*Description:读E²的RAM区数据
-----------------------------------------------------------------------------*/
Uint16 Flash_EEPROM_AT_RAM_Section_Read(Uint32 address, Uint16* pdata, Uint16 length)
{
    Uint16 Status = 0;
    Uint16 index = 0;
    DINT;
    do
    {
        *(pdata + index) = *((Uint16*)(address + index));
        index++;
    }while(index != length);
    EINT;
    return Status;
}

/******************************************************************************
*Description:获取app crc值
-----------------------------------------------------------------------------*/
Uint16 Flash_App_Section_CRC(void)
{
    Uint16 temp = *((Uint16*)APP_CRC_ADDRESS);
    return temp;
}

/******************************************************************************
*Description:获取app 长度值
-----------------------------------------------------------------------------*/
Uint32 Flash_App_Section_Length(void)
{
    Uint32 temp = *((Uint32*)APP_LENGTH_ADDRESS);
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
    static Uint16 delaytime = 0;

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
    Uint16 flag = Flash_App_Section_Get_UpdataFlag();
    Uint16 crc = *(Uint16*)APP_CRC_ADDRESS;
    Uint16 checkCal;
    Uint32 length = ((Uint32)*(Uint16*)APP_LENGTH_ADDRESS)<<16;
    length |= *(Uint16*)(APP_LENGTH_ADDRESS+1);

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
