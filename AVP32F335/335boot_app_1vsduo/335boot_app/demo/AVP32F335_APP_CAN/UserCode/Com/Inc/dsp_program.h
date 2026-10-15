/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :dsp_program.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef DSP_PROGRAM_H_
#define DSP_PROGRAM_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "AVP33x_Project.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    BOOT_BEGIN_ADDRESS                0x33FFF6 //boot区跳转入口
#define    BOOT_FLASH_ADDRESS                0x338000 //boot区程序写入首地址
#define    APP_BEGIN_ADDRESS                 0x300000 //app区条转入口
#define    APP_WRITE_ADDRESS                 0x300000 //app区程序写入首地址
#define    APP_UPGRADE_FLAG_RAM_ADDRESS      0x00FFFE //查询升级标志的RAM地址

#define    EEPROM_ADDRESS                    0x000000 //EEPROM地址
#define    APP_CRC_ADDRESS                   0x330002 //app固件crc16存放地址，占用1 word
#define    APP_LENGTH_ADDRESS                0x330000 //app固件长度存放地址，占用2 word

#define    APP_FLASH_LENGHT                  0x02FFFE //app的flash区最大空间范围，160k word， 320KB

#define    APP_UPGRADE_FLAG_BOOT_JUMP_APP    0xAA01   //升级查询标志，用于标记boot跳到app时使用
#define    APP_UPGRADE_FLAG_APP_JUMP_BOOT    0xAA02   //升级查询标志，用于标记app跳到boot时使用
#define    APP_UPGRADE_FLAG_CLEAR            0x0000   //清空升级查询标志，表示正常上电跳转

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void Flash_API_Init(void);
void Flash_App_Section_Write_Init(void);
void Flash_App_Section_Set_UpdataFlag(void);
void Flash_Boot_Section_Set_UpdataFlag(void);
void Flash_AB_Section_Clear_UpdataFlag(void);
Uint16 Flash_App_Section_Get_UpdataFlag(void);
Uint16 Flash_App_Section_Erase(void);
Uint16 Flash_App_Section_Write(Uint16* pAddress, Uint16 length);
Uint16 Flash_App_Section_Write_UpdataFlag(Uint32 length, Uint16 crc);
Uint16 Flash_EEPROM_Section_Erase(void);
Uint16 Flash_EEPROM_Section_Write(Uint32 address, Uint16* pdata, Uint16 length);
Uint16 Flash_EEPROM_Section_Read(Uint32 address, Uint16* pdata, Uint16 length);
Uint16 Flash_EEPROM_AT_RAM_Section_Write(Uint32 address, Uint16* pdata, Uint16 length);
Uint16 Flash_EEPROM_AT_RAM_Section_Read(Uint32 address, Uint16* pdata, Uint16 length);
Uint16 Flash_App_Section_CRC(void);
Uint32 Flash_App_Section_Length(void);
void Set_JumpToBoot(void);
void DSP_JumpToBoot(void);
void DSP_JumpToBoot_delay(void);
void Jump_Check(void);







#endif
