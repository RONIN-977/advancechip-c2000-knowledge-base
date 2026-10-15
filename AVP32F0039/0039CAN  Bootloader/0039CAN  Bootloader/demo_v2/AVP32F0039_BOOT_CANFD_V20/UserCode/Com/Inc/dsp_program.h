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
#include "device.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    BOOT_BEGIN_ADDRESS                0x080000 //boot区跳转入口
#define    BOOT_FLASH_ADDRESS                0x080000 //boot区程序写入首地址
#define    APP_BEGIN_ADDRESS                 0x084000 //app区跳条转入口
#define    APP_WRITE_ADDRESS                 0x084000 //app区程序写入首地址
#define    APP_UPGRADE_FLAG_RAM_ADDRESS      0x009FFE //查询升级标志的RAM地址

#define    EEPROM_ADDRESS                    0x08F000 //EEPROM地址
#define    APP_CRC_ADDRESS                   0x08EFFA //app固件crc16存放地址，占用1 word
#define    APP_LENGTH_ADDRESS                0x08EFF8 //app固件长度存放地址，占用2 word

#define    APP_FLASH_LENGHT                  0x00AFFB //app的flash区最大空间范围

#define    APP_UPGRADE_FLAG_BOOT_JUMP_APP    0xAA01   //升级查询标志，用于标记boot跳到app时使用
#define    APP_UPGRADE_FLAG_APP_JUMP_BOOT    0xAA02   //升级查询标志，用于标记app跳到boot时使用
#define    APP_UPGRADE_FLAG_CLEAR            0x0000   //清空升级查询标志，表示正常上电跳转

#define    APP_FLASH_SECTOR_CNT              11 // SEC4 ~ SEC14

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef struct
{
    union
    {
        struct
        {
            uint16_t index[2];
            uint16_t dsp_type;
            uint16_t firmware_type;
            uint16_t boot_version[8];
            uint16_t app_version[8];
            uint16_t boot_download_date[10];
            uint16_t app_download_date[10];
            uint16_t app_head_address[2];
            uint16_t app_tail_address[2];
            uint16_t app_total_length[2];
            uint16_t app_length_address[2];
            uint16_t app_verify_address[2];
            uint16_t app_verify_type;
        }info;
        uint16_t data_array[51];
    };
}DSP_INFORMATION_T;

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/
extern const DSP_INFORMATION_T dsp_info;

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

uint32_t Flash_App_Section_Erase(void);
uint32_t Flash_App_Section_Write(uint32_t address, uint16_t* pbuf, uint16_t length);

void DSP_JumpToApp(void);
uint16_t Is_App_Jump_Boot_Check(void);
void Jump_Check(void);
void DSP_JumpToApp_NoSet(void);
void RAM_Write_Package_Seq(uint16_t n);
uint16_t RAM_Read_Package_Seq(void);







#endif
