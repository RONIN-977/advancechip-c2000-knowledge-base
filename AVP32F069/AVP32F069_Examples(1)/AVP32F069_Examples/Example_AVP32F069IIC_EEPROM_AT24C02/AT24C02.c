#include "AVP32F069_Project.h"
#include "BspInc.h"

interrupt void i2c_int1a_isr(void);
extern void   I2CA_Init(void);                            //AIP650.c定义
extern Uint16 I2CA_Rxdata_STOP(struct I2CSlaveMSG *msg);  //AIP650.c定义
extern void    softResetIIC_BUS(void);                    //AIP650.c定义


//不带停止位的发送，
//IIC Maste Mode，发送模式， 发送从器件地址，无STOP
Uint16 I2CA_Txdata_NoSTOP(struct I2CSlaveMSG *msg)
{

    if(I2caRegs.I2CMDR.bit.STP ==1)//等待STOP信号消失
    {
      return I2C_STP_NOT_READY_ERROR;
    }
    I2caRegs.I2CSAR = msg->SlavePHYAddress;

    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
       return I2C_BUS_BUSY_ERROR;
    }
    I2caRegs.I2CCNT = 1;
    I2caRegs.I2CDXR = msg->LogicAddr;
    I2caRegs.I2CMDR.all = 0x6620; // Send data to setup EEPROM address
    //TRX = 1 发送模式,STP = 0，不发送STOP
    return I2C_SUCCESS;
}




//发送函数
//IIC Maste Mode，发送模式
//I2caRegs.I2CCNT 为发送的字节长度，不包括从器件的物理地址
//发送格式：START ADDRESS CONTROL_BYTE DA_BYTE STOP
//当写入I2caRegs.I2CMDR.all = 0x2E20，硬件IIC进行操作，STOP信号结束
Uint16 I2CA_Txdata_STOP(struct I2CSlaveMSG *msg)
{
    Uint16 i;

    if(I2caRegs.I2CMDR.bit.STP==1)//等待STOP信号
    {
      return I2C_STP_NOT_READY_ERROR;
    }
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
       return I2C_BUS_BUSY_ERROR;
    }

    I2caRegs.I2CSAR = msg->SlavePHYAddress;//设置访问从器件地址，物理地址

    I2caRegs.I2CDXR = msg->LogicAddr;   //软件上的的逻辑地址，占用1个字节的长度

    //FIFO为4个字节，所以最大放入 3个 数据  和 1个逻辑地址
    for(i=0;i<msg->Len-1;i++)     // 小于等于3个byte的数据
    {
        I2caRegs.I2CDXR = msg->MsgOutBuffer[i];
    }

    I2caRegs.I2CCNT = msg->Len; //设置发送长度，最大为4个byte

    I2caRegs.I2CMDR.all = 0x2E20;       // Send restart as master receiver
    //TRX  = 1 ,发送模式； STP = 1 发送停止位
    return I2C_SUCCESS;
}

/************************************************************************/
//串口获取操作EEprom命令
//写入 通信格式:  0xA2 + 0x06 + 1byte 地址 + 1byte 数据长度 + nbyte 数据
//读出通信格式：       0xA2 + 0X03 + 1byte 地址 + 1byte 数据长度
/************************************************************************/
unsigned char handleCommEEprom(struct I2CSlaveMSG *msg)
{
    Uint16 i,len,cnt;
    Uint16 iicdata;

    if(SCI_Msg.rxData[EE_ID_I] != EE_COMM_ID)
    {
        SCI_Msg.rxWriteIndex = 0;
        SCI_Msg.rxReadIndex = 0;
        printf("\r\n获取AT24C02 ID错误，第一个字节ID应该为 十六进制的  %x",EE_COMM_ID);
        return 1;
    }

      //解析操作
      switch(SCI_Msg.rxData[EE_CMD_I])
      {
          case 3://读出操作
                //获取地址
                msg->LogicAddr =  SCI_Msg.rxData[EE_ADDR_I]; //逻辑地址，即为EEPROM内部的地址
                //获取数据长度
                len = SCI_Msg.rxData[EE_LEN_I];
                cnt = 0;
                printf("\r\n\r\n读出起始地址：0x%0.2x ；长度 0x%0.2x ",msg->LogicAddr,len);

                for(;len>4;) //一次读出4个字节，采用FIFO
                {
                    //发送逻辑地址

                    I2CA_Txdata_NoSTOP(msg);

                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_READ_STEP1;
                    while((I2caRegs.I2CSTR.all & I2C_ARDY_BIT) == 0)
                    {

                        if(msg->IIC_TimerOUT > AT24CO2_TIMER_OUT)//超时判断
                        {
                            I2caRegs.I2CMDR.all &=~IIC_RST_BIT;  //复位IIC， 复位 I2CSTR 寄存器
                            I2caRegs.I2CMDR.all |=IIC_RST_BIT;   //重新使能IIC
                            softResetIIC_BUS();                  //软件复位IIC从设备

                            printf("\r\n Timer OUT in %d , Soft Reset",msg->MasterStatus);
                            msg->MasterStatus = IIC_IDLE;
                        }

                    }

                    I2caRegs.I2CSTR.all |= I2C_ARDY_BIT;//清除标志位


                    msg->Len  = 4;
                    I2CA_Rxdata_STOP(msg);//发送数据，带STOP

                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_READ_STEP2;
                    while((I2caRegs.I2CSTR.all & I2C_SCD_BIT)==0)
                    {
                        /*
                        if(msg->IIC_TimerOUT > AT24CO2_TIMER_OUT)//超时判断
                        {
                            I2caRegs.I2CMDR.all &=~IIC_RST_BIT;  //复位IIC， 复位 I2CSTR 寄存器
                            I2caRegs.I2CMDR.all |=IIC_RST_BIT;   //重新使能IIC
                            softResetIIC_BUS();                  //软件复位IIC从设备

                            printf("\r\n Timer OUT in %d , Soft Reset",msg->MasterStatus);
                            msg->MasterStatus = IIC_IDLE;
                        }
                        */

                    }
                    I2caRegs.I2CSTR.all |= I2C_SCD_BIT;//清除标志位


                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_READ_STEP3;
                    printf("\r\n 数据 ：");
                    while(I2caRegs.I2CFFRX.bit.RXFFST)//读取完毕FIFO里的数据
                    {
                        iicdata = I2caRegs.I2CDRR;
                        printf(" 0x%x",iicdata);
                    }
                    len-=4;
                    cnt+=4;
                    msg->LogicAddr+=4;
                    printf("read Data cnt %d ok",cnt);
                }

                if(len>0)//读出剩余的字节
                {
                    //发送逻辑地址
                    I2CA_Txdata_NoSTOP(msg);
                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_READ_STEP1;
                    while((I2caRegs.I2CSTR.all & I2C_ARDY_BIT) == 0)
                    {

                        if(msg->IIC_TimerOUT > AT24CO2_TIMER_OUT)//超时判断
                        {
                            I2caRegs.I2CMDR.all &=~IIC_RST_BIT;  //复位IIC， 复位 I2CSTR 寄存器
                            I2caRegs.I2CMDR.all |=IIC_RST_BIT;   //重新使能IIC
                            softResetIIC_BUS();                  //软件复位IIC从设备

                            printf("\r\n Timer OUT in %d , Soft Reset",msg->MasterStatus);
                            msg->MasterStatus = IIC_IDLE;

                        }

                    }
                    I2caRegs.I2CSTR.all |= I2C_ARDY_BIT;//清除标志位

                    msg->Len  = len;
                    I2CA_Rxdata_STOP(msg);//发送数据，带STOP
                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_READ_STEP2;
                    while((I2caRegs.I2CSTR.all & I2C_SCD_BIT)==0)
                    {

                        if(msg->IIC_TimerOUT > AT24CO2_TIMER_OUT)//超时判断
                        {
                            I2caRegs.I2CMDR.all &=~IIC_RST_BIT;  //复位IIC， 复位 I2CSTR 寄存器
                            I2caRegs.I2CMDR.all |=IIC_RST_BIT;   //重新使能IIC
                            softResetIIC_BUS();                  //软件复位IIC从设备

                            printf("\r\n Timer OUT in %d , Soft Reset",msg->MasterStatus);
                            msg->MasterStatus = IIC_IDLE;
                        }

                    }

                    I2caRegs.I2CSTR.all |= I2C_SCD_BIT;//清除标志位

                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_READ_STEP3;
                    printf("\r\n 数据 ：");
                    while(I2caRegs.I2CFFRX.bit.RXFFST)//读取完毕FIFO里的数据
                    {
                        printf(" 0x%x",I2caRegs.I2CDRR);
                    }
                    printf("     read Data cnt %d ok",cnt+len);
                }


                msg->MasterStatus = IIC_IDLE;


          break;

          case 6: //写入操作

                //获取地址
                msg->LogicAddr =  SCI_Msg.rxData[EE_ADDR_I]; //逻辑地址，即为EEPROM内部的地址

                //获取数据长度
                len =  SCI_Msg.rxData[EE_LEN_I];
                cnt = 0;

                printf("\r\n\r\n欲写入起始地址：0x%0.2x ；长度 0x%0.2x",msg->LogicAddr,len);

                printf("\r\n欲写入的数据是 ");
                for(i=0;i<len;i++)
                {
                    printf(" 0x%0.2x",SCI_Msg.rxData[EE_DATA_START + i]);
                }

                for(;len>0;)//依次按照地址写入数据
                {
                    msg->Len  = 2; //1 个数据 加 1个 逻辑地址
                    msg->MsgOutBuffer[0] = SCI_Msg.rxData[EE_DATA_START+0+cnt];
                    I2CA_Txdata_STOP(msg);//发送数据，带STOP

                    msg->IIC_TimerOUT = 0;
                    msg->MasterStatus = IIC_WRITE;
                    while((I2caRegs.I2CSTR.all & I2C_SCD_BIT)==0)
                    {
                        if(msg->IIC_TimerOUT > AT24CO2_TIMER_OUT)//超时判断
                        {
                            I2caRegs.I2CMDR.all &=~IIC_RST_BIT;  //复位IIC， 复位 I2CSTR 寄存器
                            I2caRegs.I2CMDR.all |=IIC_RST_BIT;   //重新使能IIC
                            softResetIIC_BUS();                  //软件复位IIC从设备
                            msg->MasterStatus = IIC_IDLE;
                            printf("\r\n Timer OUT in %d , Soft Reset",msg->MasterStatus);
                        }
                    }
                    I2caRegs.I2CSTR.all |= I2C_SCD_BIT;//清除标志位
                    len-=1;
                    cnt+=1;
                    msg->LogicAddr+=1;
                    printf("\r\nwrite Data cnt %d ok",cnt);
                    DELAY_US(5000);
                }

                printf("\r\nWrite AT24C02 OK");

                msg->MasterStatus = IIC_IDLE;

            break;

          default:break;
      }

    SCI_Msg.rxWriteIndex = 0;
    SCI_Msg.rxReadIndex = 0;

    return 0;

}

//===========================================================================
// No more.
//===========================================================================

