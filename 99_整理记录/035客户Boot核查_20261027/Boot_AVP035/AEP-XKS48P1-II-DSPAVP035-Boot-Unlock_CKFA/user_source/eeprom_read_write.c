#include "F2806x_Device.h"     // DSP2803x Headerfile Include File
#include "F2806x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

int f_i2cw_stop = 1;
int timer_i2c = 0;


#pragma CODE_SECTION(E2promDelay, "ramfuncs");
void E2promDelay(void)
{
    ServiceDog();
    DELAY_US(2000L);
    ServiceDog();
    DELAY_US(2000L);
    ServiceDog();
    DELAY_US(2000L);
    ServiceDog();
    DELAY_US(2000L);
    ServiceDog();
    DELAY_US(2000L);
    ServiceDog();
}

#pragma CODE_SECTION(Start_I2c, "ramfuncs");
static void Start_I2c()
{
    SDA_SET();
    DELAY_US(3L);

    SCL_SET();
    DELAY_US(15L);

    SDA_CLR();
    DELAY_US(3L);

    SCL_CLR();
    DELAY_US(3L);
}

#pragma CODE_SECTION(Stop_I2c, "ramfuncs");
static void Stop_I2c()
{
    SDA_CLR();
    DELAY_US(3L);

    SCL_SET();
    DELAY_US(3L);

    SDA_SET();
    DELAY_US(3L);
}

#pragma CODE_SECTION(SendByte, "ramfuncs");
static unsigned int SendByte(unsigned int send_byte)
{
    unsigned int bit_cnt  = 0;
    unsigned int send_bit_buf[8] = {0,0,0,0,0,0,0,0};
    unsigned int send_ack = 0;

    SDA_OUT();

    for(bit_cnt=0;bit_cnt<8;bit_cnt++)
    {
        if((send_byte<<bit_cnt) & 0x80)
        {
            send_bit_buf[bit_cnt] = 1;
        }
        else
        {
            send_bit_buf[bit_cnt] = 0;
        }
    }

    for(bit_cnt=0; bit_cnt<8; bit_cnt++)
    {
        SCL_CLR();
        if(send_bit_buf[bit_cnt])
        {
            SDA_SET();
        }
        else
        {
            SDA_CLR();
        }
        DELAY_US(3L);
        SCL_SET();
        DELAY_US(3L);
    }

    SCL_CLR();
    SDA_IN();
    DELAY_US(3L);
    SCL_SET();

    if(SDA_DATA)
    {
        send_ack = 1;
    }
    else
    {
        send_ack = 0;
    }

    DELAY_US(3L);
    SCL_CLR();
    DELAY_US(3L);
    SDA_OUT();

    ServiceDog();

    return send_ack;
}

#pragma CODE_SECTION(RecvByte, "ramfuncs");
static unsigned int RecvByte()
{
    unsigned int recv_byte = 0;
    unsigned int bit_cnt = 0;

    SCL_CLR();
    SDA_IN();

    for(bit_cnt=0; bit_cnt<8; bit_cnt++)
    {
        SCL_CLR();
        DELAY_US(3L);
        SCL_SET();

        recv_byte = recv_byte<<1;

        if(SDA_DATA)
        {
            recv_byte = recv_byte+1;
        }
        DELAY_US(3L);
    }

    SCL_CLR();
    DELAY_US(3L);
    SDA_OUT();

    ServiceDog();

    return(recv_byte);
}


#pragma CODE_SECTION(PageWrite, "ramfuncs");
void PageWrite(unsigned int addr,unsigned int *data,unsigned int length)
{
    unsigned int i;

    //启动总线
    Start_I2c();

    //发送写命令
    SendByte(0xa0);

    //发送地址高8位
    SendByte((addr>>8)&0xFF);

    //发送地址低8位
    SendByte(addr&0xFF);

    //连续写数据
    for(i=0;i<length;i++)
    {
        SendByte(data[i]);
    }

    //结束总线
    Stop_I2c();
    f_i2cw_stop = 1;
}

#pragma CODE_SECTION(PageRead, "ramfuncs");
void PageRead(unsigned int addr,unsigned int *recv,unsigned int length)
{
    unsigned int i;

    //启动总线
    Start_I2c();

    //发送写命令
    SendByte(0xA0);

    //发送地址
    SendByte(addr>>8);

    //发送地址
    SendByte(addr & 0x00FF);

    //启动总线
    Start_I2c();

    //发送读命令
    SendByte(0xA1);


    for(i=0;i<length-1;i++)
    {
        recv[i]=RecvByte();

        //主机响应ACK
        SCL_CLR();
        DELAY_US(3L);

        SDA_CLR();
        DELAY_US(3L);

        SCL_SET();
        DELAY_US(3L);
    }

    //最后一个数据无需应答
    recv[length-1] = RecvByte();

    //不应答ACK
    SCL_CLR();
    DELAY_US(3L);

    SDA_SET();
    DELAY_US(3L);

    SCL_SET();
    DELAY_US(3L);

    //结束总线
    Stop_I2c();
}





