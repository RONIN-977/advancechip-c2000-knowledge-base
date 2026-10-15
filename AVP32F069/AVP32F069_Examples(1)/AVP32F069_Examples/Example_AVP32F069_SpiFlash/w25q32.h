#ifndef W25Q32_H
#define W25Q32_H
#include "OnChip_Driver_Inc.h"

#define SPIFLASH_ID 0x64  //通信中，flash的ID
#define FLASH_ID_I 0
#define FLASH_CMD_I 1
#define FLASH_ADDRH_I 2
#define FLASH_ADDRM_I 3
#define FLASH_ADDRL_I 4
#define FLASH_DATALENH_I 5
#define FLASH_DATALENM_I 6
#define FLASH_DATALENL_I 7
#define FLASH_CY_DATA_I 8

typedef struct W25Q32Flash_Type{
    Uint16         Jedec_ID;
    Uint16         SReg;
    Uint32         Addr;
    unsigned char  CMD;
    Uint32         Len;
}
W25Q32Flash;

extern  W25Q32Flash SPIFlash;
extern unsigned char buffer_128[128];

#define W25Q32  0XEF15

extern Uint16 SPI_FLASH_TYPE;//定义我们使用的flash芯片型号

////////////////////////////////////////////////////////////////////////////

//指令表
#define W25X_WriteEnable        0x06    //写使能
#define W25X_WriteDisable       0x04    //写禁止
#define W25X_ReadStatusReg      0x05    //读状态寄存器
#define W25X_WriteStatusReg     0x01    //写状态寄存器
#define W25X_ReadData           0x03    //读数据
#define W25X_FastReadData       0x0B    //快速读数据
#define W25X_FastReadDual       0x3B    //快速异步读数据
#define W25X_PageProgram        0x02    //页编程
#define W25X_BlockErase         0xD8    //块擦除
#define W25X_SectorErase        0x20    //扇区擦除
#define W25X_ChipErase          0xC7    //片擦除
#define W25X_PowerDown          0xB9    //关机
#define W25X_ReleasePowerDown           0xAB    //释放关机
#define W25X_DeviceID           0xAB    //读取设备ID
#define W25X_ManufactDeviceID           0x90    //
#define W25X_JedecDeviceID      0x9F    //

void SpiFlashInit(void);

Uint16  SpiFlashReadID(void);          //读取FLASH ID

Uint8   SpiFlashReadSR(void);        //读取状态寄存器

void SpiFlashWriteSR(Uint8 sr);    //写状态寄存器

void SpiFlashWriteEnable(void);  //写使能

void SpiFlashWriteDisable(void);    //写保护

void SpiFlashWriteNoCheck(Uint8* pBuffer,Uint32 WriteAddr,Uint16 NumByteToWrite);

void SpiFlashRead(Uint8* pBuffer,Uint32 ReadAddr,Uint16 NumByteToRead);   //读取flash

void SpiFlashWrite(Uint8* pBuffer,Uint32 WriteAddr,Uint16 NumByteToWrite);//写入flash

void SpiFlashEraseChip(void);         //整片擦除

void SpiFlashEraseSector(Uint32 Dst_Addr);//扇区擦除

void SpiFlashWaitBusy(void);           //等待空闲

void SpiFlashPowerDown(void);           //进入掉电模式

void SpiFlashWakeUp(void);            //唤醒

Uint8 SpiFlashCheck(void);              //flash检测

void W25Q32_HardwareInit(void);

unsigned char handleCommFlash(void);

#endif 
