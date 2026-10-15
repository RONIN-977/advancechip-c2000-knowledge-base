/******************************************************************************
*Copyright(C),2025,Hunan Advancechip Technology CO.,LTD.
*FileName   :can_config.h
*Author     :
*Version    :v1.0
*Date       :2025.7.2
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef DSP_PROGRAM_H_
#define DSP_PROGRAM_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "device.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    BOOT_BEGIN_ADDRESS                0x080000 //boot区跳转入口
#define    BOOT_FLASH_ADDRESS                0x080000 //boot区程序写入首地址
#define    APP_BEGIN_ADDRESS                 0x083000 //app区跳条转入口
#define    APP_WRITE_ADDRESS                 0x083000 //app区程序写入首地址
#define    APP_UPGRADE_FLAG_RAM_ADDRESS      0x009FFE //查询升级标志的RAM地址

#define    EEPROM_ADDRESS                    0x08F000 //EEPROM地址
#define    APP_CRC_ADDRESS                   0x08EFFA //app固件crc16存放地址，占用1 word
#define    APP_LENGTH_ADDRESS                0x08EFF8 //app固件长度存放地址，占用2 word

#define    APP_FLASH_LENGHT                  0x00BFFB //app的flash区最大空间范围

#define    APP_UPGRADE_FLAG_BOOT_JUMP_APP    0xAA01   //升级查询标志，用于标记boot跳到app时使用
#define    APP_UPGRADE_FLAG_APP_JUMP_BOOT    0xAA02   //升级查询标志，用于标记app跳到boot时使用
#define    APP_UPGRADE_FLAG_CLEAR            0x0000   //清空升级查询标志，表示正常上电跳转

#define    APP_FLASH_SECTOR_CNT              12 // SEC3 ~ SEC14

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
uint16_t Flash_App_Section_Get_UpdataFlag(void);
uint16_t Flash_App_Section_Erase(void);
uint16_t Flash_App_Section_Write(uint16_t* pAddress, uint16_t length);
uint16_t Flash_App_Section_Write_UpdataFlag(uint32_t length, uint16_t crc);
uint16_t Flash_EEPROM_Section_Erase(void);
uint16_t Flash_EEPROM_Section_Write(uint32_t address, uint16_t* pdata, uint16_t length);
uint16_t Flash_EEPROM_Section_Read(uint32_t address, uint16_t* pdata, uint16_t length);
uint16_t Flash_EEPROM_AT_RAM_Section_Write(uint32_t address, uint16_t* pdata, uint16_t length);
uint16_t Flash_EEPROM_AT_RAM_Section_Read(uint32_t address, uint16_t* pdata, uint16_t length);
uint16_t Flash_App_Section_CRC(void);
uint32_t Flash_App_Section_Length(void);
void Set_JumpToApp(void);
void DSP_JumpToApp(void);
void DSP_JumpToApp_delay(void);
void Jump_Check(void);







#endif
