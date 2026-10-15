#include "SPI.h"
#include "w25q32.h"
#include "BspInc.h"

#define USE_SPI1			1
#define USE_SPI2			2

//模块内部宏定义区
#define W25Q32_SPI		USE_SPI1

#if W25Q32_SPI==1
#define W25Q32_ReadWriteByte(n)    SPIA_WriteRead_8bit(n)
//CS
#define CS_DELAT_CNT		3//2   //延时可以根据时序需要增加或去掉
#define W25Q32_CS_H()			{GpioDataRegs.GPASET.bit.GPIO19 = 1; Delay_nUS(CS_DELAT_CNT);}
#define W25Q32_CS_L()			{GpioDataRegs.GPACLEAR.bit.GPIO19 = 1; Delay_nUS(CS_DELAT_CNT);}
//输出并上拉
#define W25Q32_CS_OUT()		{\
                                GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;\
								GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;\
								GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;\
							}
#else
#define W25Q32_ReadWriteByte(n)    SPIB_WriteRead_8bit(n)
//CS
#define CS_DELAT_CNT        2   //延时可以根据时序需要增加或去掉
#define W25Q32_CS_H()           {GpioDataRegs.GPASET.bit.GPIO15 = 1; Delay_nUS(CS_DELAT_CNT);}
#define W25Q32_CS_L()           {GpioDataRegs.GPACLEAR.bit.GPIO15 = 1; Delay_nUS(CS_DELAT_CNT);}
//输出并上拉
#define W25Q32_CS_OUT()     {\
                                GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;\
                                GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;\
                                GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;\
                            }
#endif
//引脚初始化宏定义
#define W25Q32_CTRL_PORT_INIT()	{\
									EALLOW;\
									W25Q32_CS_OUT();\
									EDIS;\
								}


Uint16 SPI_FLASH_TYPE=W25Q32;//默认就是W25Q32

W25Q32Flash SPIFlash;
unsigned char buffer_128[128]={0};

void W25Q32_HardwareInit(void)
{
    W25Q32_CTRL_PORT_INIT();       //控制硬件初始化
#if W25Q32_SPI==1
    SPIA_MsterConfig(0, 8);     //模式0,8位数据模式
    SPIA_SpeedSet(3);           //配置速度
#else
    SPIB_MsterConfig(0, 8);     //模式0,8位数据模式
    SPIB_SpeedSet(3);           //配置速度
#endif
}

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q32
//容量为4M字节,共有64个Block,1024个Sector
//4194304个byte
//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
Uint8 SpiFlashReadSR(void)
{
    Uint8 byte=0;
    W25Q32_CS_L();                            //使能器件
    W25Q32_ReadWriteByte(W25X_ReadStatusReg);     //发送读取状态寄存器命令
    byte=W25Q32_ReadWriteByte(0Xff);              //读取一个字节
    W25Q32_CS_H();                           //取消片选
    return byte;
}

//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SpiFlashWriteSR(Uint8 sr)
{
    W25Q32_CS_L();                           //使能器件
    W25Q32_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令
    W25Q32_ReadWriteByte(sr);               //写入一个字节
    W25Q32_CS_H();                            //取消片选
}

//SPI_FLASH写使能
//将WEL置位
void SpiFlashWriteEnable(void)
{
    W25Q32_CS_L();                            //使能器件
    W25Q32_ReadWriteByte(W25X_WriteEnable);      //发送写使能
    W25Q32_CS_H();                            //取消片选
}

//SPI_FLASH写禁止
//将WEL清零
void SpiFlashWriteDisable(void)
{
    W25Q32_CS_L();                            //使能器件
    W25Q32_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令
    W25Q32_CS_H();                           //取消片选
}

//读取芯片ID W25X16的ID:0XEF14
Uint16 SpiFlashReadID(void)
{
    Uint16 Temp = 0;
    W25Q32_CS_L();          //使能器件
    W25Q32_ReadWriteByte(0x90);//发送读取ID命令
    W25Q32_ReadWriteByte(0x00);
    W25Q32_ReadWriteByte(0x00);
    W25Q32_ReadWriteByte(0x00);
    Temp|=W25Q32_ReadWriteByte(0xFF)<<8;
    Temp|=W25Q32_ReadWriteByte(0xFF);
    W25Q32_CS_H();
    return Temp;
}

//flash检测 检测到返回0 失败返回1
Uint8 SpiFlashCheck(void)
{
    Uint16 flashId = 0;
    flashId = SpiFlashReadID();
    if(flashId == W25Q32)
        return 0;
    else
        return 1;
}

//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SpiFlashRead(Uint8* pBuffer,Uint32 ReadAddr,Uint16 NumByteToRead)
{
    Uint16 i;
    W25Q32_CS_L();                           //使能器件
    W25Q32_ReadWriteByte(W25X_ReadData);         //发送读取命令
    W25Q32_ReadWriteByte((Uint8)((ReadAddr)>>16));  //发送24bit地址
    W25Q32_ReadWriteByte((Uint8)((ReadAddr)>>8));
    W25Q32_ReadWriteByte((Uint8)ReadAddr);
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=W25Q32_ReadWriteByte(0XFF);   //循环读数
    }
    W25Q32_CS_H();                           //取消片选
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void SPIFlashWritePage(Uint8* pBuffer,Uint32 WriteAddr,Uint16 NumByteToWrite)
{
    Uint16 i;
    SpiFlashWriteEnable();                  //SET WEL
    W25Q32_CS_L();                            //使能器件
    W25Q32_ReadWriteByte(W25X_PageProgram);      //发送写页命令
    W25Q32_ReadWriteByte((Uint8)((WriteAddr)>>16)); //发送24bit地址
    W25Q32_ReadWriteByte((Uint8)((WriteAddr)>>8));
    W25Q32_ReadWriteByte((Uint8)WriteAddr);
    for(i=0;i<NumByteToWrite;i++)
        W25Q32_ReadWriteByte(pBuffer[i]);//循环写数
    W25Q32_CS_H();                           //取消片选
    SpiFlashWaitBusy();                    //等待写入结束
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SpiFlashWriteNoCheck(Uint8* pBuffer,Uint32 WriteAddr,Uint16 NumByteToWrite)
{
    Uint16 pageremain;
    pageremain=256-WriteAddr%256; //单页剩余的字节数

    if(NumByteToWrite<=pageremain)
        pageremain=NumByteToWrite;//不大于256个字节
    while(1)
    {
        SPIFlashWritePage(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)
        {
            break;//写入结束了
        }
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain;           //减去已经写入了的字节数
            if(NumByteToWrite>256)
            {
                pageremain=256; //一次可以写入256个字节
            }
            else
            {
                pageremain=NumByteToWrite;       //不够256个字节了
            }
        }
    };
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
Uint8 SPI_FLASH_BUF[4096];

void SpiFlashWrite(Uint8* pBuffer,Uint32 WriteAddr,Uint16 NumByteToWrite)
{
    Uint32 secpos;
    Uint16 secoff;
    Uint16 secremain;
    Uint16 i;

    secpos=WriteAddr/4096;//扇区地址 0~511 for w25x16
    secoff=WriteAddr%4096;//在扇区内的偏移
    secremain=4096-secoff;//扇区剩余空间大小

    if(NumByteToWrite<=secremain)
    {
        secremain=NumByteToWrite;//不大于4096个字节
    }

    while(1)
    {
        SpiFlashRead(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(SPI_FLASH_BUF[secoff+i]!=0XFF)
                break;//需要擦除
        }
        if(i<secremain)//需要擦除
        {
            SpiFlashEraseSector(secpos);//擦除这个扇区
            for(i=0;i<secremain;i++)       //复制
            {
                SPI_FLASH_BUF[i+secoff]=pBuffer[i];
            }
            SpiFlashWriteNoCheck(SPI_FLASH_BUF,secpos*4096,4096);//写入整个扇区

        }
        else
        {
            SpiFlashWriteNoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
        }

        if(NumByteToWrite==secremain)
        {
            break;//写入结束了
        }
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff=0;//偏移位置为0

            pBuffer+=secremain;  //指针偏移
            WriteAddr+=secremain;//写地址偏移
            NumByteToWrite-=secremain;              //字节数递减

            if(NumByteToWrite>4096)
            {
                secremain=4096;  //下一个扇区还是写不完
            }
            else
            {
                secremain=NumByteToWrite;          //下一个扇区可以写完了
             }
        }
    };
}

//擦除整个芯片
//整片擦除时间:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//等待时间超长...
void SpiFlashEraseChip(void)
{
    SpiFlashWriteEnable();                  //SET WEL
    SpiFlashWaitBusy();
    W25Q32_CS_L();                            //使能器件
    W25Q32_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令
    W25Q32_CS_H();                             //取消片选
    SpiFlashWaitBusy();                    //等待芯片擦除结束
}

//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SpiFlashEraseSector(Uint32 Dst_Addr)
{
    Dst_Addr*=4096;
    SpiFlashWriteEnable();                  //SET WEL
    SpiFlashWaitBusy();
    W25Q32_CS_L();                             //使能器件
    W25Q32_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令
    W25Q32_ReadWriteByte((Uint8)((Dst_Addr)>>16));  //发送24bit地址
    W25Q32_ReadWriteByte((Uint8)((Dst_Addr)>>8));
    W25Q32_ReadWriteByte((Uint8)Dst_Addr);
    W25Q32_CS_H();                           //取消片选
    SpiFlashWaitBusy();                    //等待擦除完成
}

//等待空闲
void SpiFlashWaitBusy(void)
{
    while ((SpiFlashReadSR()&0x01)==0x01);   // 等待BUSY位清空
}

//进入掉电模式
void SpiFlashPowerDown(void)
{
    W25Q32_CS_L();                         //使能器件
    W25Q32_ReadWriteByte(W25X_PowerDown);        //发送掉电命令
    W25Q32_CS_H();                            //取消片选
    Delay_nMS(3);                               //等待TPD
}

//唤醒
void SpiFlashWakeUp(void)
{
    W25Q32_CS_L();                           //使能器件
    W25Q32_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    W25Q32_CS_H();                            //取消片选
    Delay_nMS(3);                              //等待TRES1
}

//FLASH写入操作，只能从逻辑1写为逻辑1或者逻辑0； 擦除操作，以块为单位进行擦除，擦除后的块内容全为0xFF
//串口操作SPI Flash W25Q32
//写入 通信格式: 0x64 + 0x06 + 3byte FLASH地址 + 3byte 数据长度 + 1byte 数据（）
//读出 通信格式: 0x64 + 0x03 + 3byte FLASH地址 + 3byte 数据长度
//擦除整个芯片 : 0X64 + FF
/************************************************************************/
unsigned char handleCommFlash(void)
{
    Uint16 i;
    Uint32 readBufIndex=0;

    if(SCI_Msg.rxData[FLASH_ID_I] != SPIFLASH_ID)
    {
        SCI_Msg.rxWriteIndex = 0;
        SCI_Msg.rxReadIndex = 0;
        return 1;
    }

    SPIFlash.CMD = SCI_Msg.rxData[FLASH_CMD_I];
    //这里处理接收到的数据
    //wrIndex = (SCI_Msg.rxWriteIndex!=0)?(SCI_Msg.rxWriteIndex-1):(UartRxLEN-1); //获取写入FIFO指针，上传到PC
    //printf("\r\nrxData[%d] = %c ; wr:%d",SCI_Msg.rxReadIndex,SCI_Msg.rxData[SCI_Msg.rxReadIndex],wrIndex);
    //SCI_Msg.rxReadIndex=(++SCI_Msg.rxReadIndex)%(UartRxLEN);

    //获取地址
    SPIFlash.Addr =  SCI_Msg.rxData[FLASH_ADDRH_I]; SPIFlash.Addr<<=8;
    SPIFlash.Addr += SCI_Msg.rxData[FLASH_ADDRM_I]; SPIFlash.Addr<<=8;
    SPIFlash.Addr += SCI_Msg.rxData[FLASH_ADDRL_I];
    //这里可增加对地址范围的判断


    //获取写入长度
    SPIFlash.Len  =   SCI_Msg.rxData[FLASH_DATALENH_I]; SPIFlash.Len<<=8;
    SPIFlash.Len += SCI_Msg.rxData[FLASH_DATALENM_I];SPIFlash.Len<<=8;
    SPIFlash.Len += SCI_Msg.rxData[FLASH_DATALENL_I];
    //这里可增加对数据长度做出范围判断

    //操作判断
      switch(SPIFlash.CMD)
      {
          case 3: //读出
                printf("\r\n\r\n读出起始地址：0x%0.8lx  长度0x%0.8lx ",SPIFlash.Addr,SPIFlash.Len);
                if(SPIFlash.Len>128)
                {
                    SPIFlash.Len=128;
                    printf("\r\n本例程一次读出数据长度限制为128字节");
                }
                else if(SPIFlash.Len<=16)
                {
                    printf("\r\n以0x%0.8lx为始地址的%ld个数据：",SPIFlash.Addr,SPIFlash.Len);
                    SpiFlashRead(buffer_128,SPIFlash.Addr,SPIFlash.Len);
                    for(i=0;i<SPIFlash.Len;i++)
                    {
                       printf("\t%x",buffer_128[i]);
                    }
                }
                else
                {
                    for(;SPIFlash.Len>16;)//这个upper_128数组只有128个长度，注意不要溢出了
                    {
                        printf("\r\n以0x%0.8lx为始地址的16个数据：",SPIFlash.Addr);
                        SpiFlashRead(buffer_128,SPIFlash.Addr,16);
                        for(i=0;i<16;i++)
                        {
                           printf("\t%x",buffer_128[i]);
                        }
                        SPIFlash.Len-=16;
                        SPIFlash.Addr+=16;
                        readBufIndex+=16;
                    }
                    if(SPIFlash.Len>0)
                    {
                      printf("\r\n以0x%0.8lx为始地址的%ld个数据：",SPIFlash.Addr,SPIFlash.Len);
                      SpiFlashRead(&buffer_128[readBufIndex],SPIFlash.Addr,SPIFlash.Len);
                      for(i=0;i<SPIFlash.Len;i++)
                      {
                         printf("\t%x",buffer_128[readBufIndex+i]);
                      }
                    }

                }

            break;

          case 6: //写入

                printf("\r\n\r\n写入起始地址：0x%0.8lx ；长度 0x%0.8lx ；重复写入的数据 0x%x；对FLASH操作，只能由1写为1或0，不能由0写为1，擦除则全为1",SPIFlash.Addr,SPIFlash.Len,SCI_Msg.rxData[FLASH_CY_DATA_I]);

                if(SPIFlash.Len>128)
                {
                    SPIFlash.Len = 128;
                    printf("\r\n本例程一次写入数据长度限制为128字节");
                }

                printf("\r\n写入前数据为 ");
                SpiFlashRead(buffer_128,SPIFlash.Addr,SPIFlash.Len);
                for(i=0;i<SPIFlash.Len;i++)
                {
                   printf("\t%x",buffer_128[i]);
                }

                for(i=0;i<SPIFlash.Len;i++)
                {
                    buffer_128[i] = SCI_Msg.rxData[FLASH_CY_DATA_I+i];
                }
                SpiFlashWriteEnable();
                SpiFlashWrite(buffer_128,SPIFlash.Addr,SPIFlash.Len);
                SpiFlashWaitBusy();

                printf("\r\n写入后数据为 ");
                SpiFlashRead(buffer_128,SPIFlash.Addr,SPIFlash.Len);
                for(i=0;i<SPIFlash.Len;i++)
                {
                   printf("\t%x",buffer_128[i]);
                }


            break;

          case 0xff: //擦除整个芯片
                printf("\r\n\r\n请不要发频繁擦除芯片；正开始擦除整个芯片.....稍等片刻");
                SpiFlashWriteEnable();
                SpiFlashEraseChip();
                SpiFlashWaitBusy();
                printf("\r\n芯片擦除完毕");

            break;
          default:break;
      }

    //操作完毕，复位变量
    SPIFlash.CMD = 0;
    SPIFlash.Addr = 0;
    SPIFlash.Len = 0;

    SCI_Msg.rxWriteIndex = 0;
    SCI_Msg.rxReadIndex = 0;

    return 0;

}
