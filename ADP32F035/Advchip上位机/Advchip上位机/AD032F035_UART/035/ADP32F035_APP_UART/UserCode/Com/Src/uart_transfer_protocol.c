/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :uart_transfer_protocol.c
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
#include "uart_transfer_protocol.h"
#include "sci_config.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
UART_LIST_T uart_list[1]=
{
    {{0,0,0,{0}},{0,0,0,{0}}},
};

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/
UART_RX_FRAME_T    RxDataFrame;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern void Can_up_receive_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void Uart_Transfer_Init(void)
{
    Uint16 i;

    uart_list[Uart1].UartRX.Head = 0;
    uart_list[Uart1].UartRX.Lenght = 0;
    uart_list[Uart1].UartRX.Tail = 0;

    uart_list[Uart1].UartTX.Head = 0;
    uart_list[Uart1].UartTX.Lenght = 0;
    uart_list[Uart1].UartTX.Tail = 0;

    RxDataFrame.step = 0;
    RxDataFrame.cnt = 0;
    RxDataFrame.len = 0;
    RxDataFrame.id = 0;
    RxDataFrame.checksum1 = 0;
    RxDataFrame.checksum2 = 0;
    RxDataFrame.datalen = 0;
    RxDataFrame.NewMsgCnt = 0;

    for(i=0; i<UART_QUEUE_LEN; i++)
    {
        RxDataFrame.buf[i] = 0;
    }
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void uart1_tx_write_data(Uint16 *pdata, Uint16 length)
{
    uart_write_data(&uart_list[Uart1].UartTX, pdata, length);
}

void uart1_tx_read_data(Uint16 *pdata)
{
    uart_read_data(&uart_list[Uart1].UartTX, pdata);
}

Uint16 uart1_tx_get_data_len(void)
{
    return uart_get_data_length(&uart_list[Uart1].UartTX);
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void uart1_rx_write_data(Uint16 *pdata, Uint16 length)
{
    uart_write_data(&uart_list[Uart1].UartRX, pdata, length);
}

void uart1_rx_read_data(Uint16 *pdata)
{
    uart_read_data(&uart_list[Uart1].UartRX, pdata);
}

Uint16 uart1_rx_get_data_len(void)
{
    return uart_get_data_length(&uart_list[Uart1].UartRX);
}

/******************************************************************************
*Description:发送数据打包
-----------------------------------------------------------------------------*/
void Uart_Send_Pack(Uint16 id, Uint16 *pbuff, Uint16 len)
{
    Uint16 sbuf[16];
    Uint16 i;
    Uint32 cks = 0;

    sbuf[0] = 0xFF;
    sbuf[1] = 0xFF;
    sbuf[2] = 0xB1;//ID为DSP回复上位机的升级帧
    for(i=0; i<8; i++)
    {
        sbuf[3+i] = pbuff[i];
    }
    for(i=0; i<11; i++)
    {
        cks += sbuf[i];
    }
    sbuf[11] = (cks & 0x000000FF);
    sbuf[12] = 0x0A;

    uart_write_data(&uart_list[Uart1].UartTX, (Uint16*)&sbuf, 13);
}

/******************************************************************************
*Description:放在1ms任务运行
-----------------------------------------------------------------------------*/
void Uart_run(void)
{
    Uint16 data;
    Uint16 datalen = uart_get_data_length(&uart_list[Uart1].UartTX);

    if(datalen > 0)
    {
        uart_read_data(&uart_list[Uart1].UartTX, &data);
        SCI_A_Send_Byte(data);
    }
}

/******************************************************************************
*Description:放在1ms任务运行
-----------------------------------------------------------------------------*/
void Uart_Receive_Data(void)
{
    UART_RX_FRAME_T*  v = &RxDataFrame;
    Uint16 databuf;
    Uint16 data[8];
    Uint16 i;

    if(uart1_rx_get_data_len() > 0)
    {
        uart1_rx_read_data(&databuf);
        switch(v->step)
        {
        case 0://接收帧头1状态
            if (databuf == 0xFF)
            {
                v->step++;
                v->cnt = 0;
                v->datalen = 0;
                v->id = 0;
                v->buf[v->cnt++] = databuf;
            }
            break;

        case 1://接收帧头2状态
            if (databuf == 0xFF)
            {
                v->step++;
                v->buf[v->cnt++] = databuf;
            }
            else
            {
                v->step = 0;
            }
            break;

        case 2://接收数据ID
            v->step++;
            v->buf[v->cnt++] = databuf;
            v->id = databuf;
            if (v->id == 0xA1)
            {
                ;
            }
            else if (v->id == 0xB3)
            {
                ;
            }
            else if (v->id == 0xC5)
            {
                ;
            }
            else
            {
                ;//没有定义
            }
            break;

        case 3://接收len字节数据状态
            v->buf[v->cnt++] = databuf;
            if(v->cnt >= 11)
            {
                v->step++;
            }
            break;

        case 4://接收校验和
            v->checksum1 = databuf;
            v->checksum2 = 0;
            for (i = 0; i < 11; i++)
            {
                v->checksum2 += v->buf[i];
            }
            v->checksum2 = (v->checksum2 & 0x000000FF);
            if(v->checksum1 == v->checksum2)
            {
                v->step++;
            }
            else
            {
                v->step = 0;
                //校验和错误，合理的应该要反馈给上位机需要重发
            }
            break;

        case 5:
            if(databuf == 0x0A)
            {
                //把串口帧的数据存入CAN协议的处理入口
                data[0] = v->buf[3];
                data[1] = v->buf[4];
                data[2] = v->buf[5];
                data[3] = v->buf[6];

                data[4] = v->buf[7];
                data[5] = v->buf[8];
                data[6] = v->buf[9];
                data[7] = v->buf[10];
                Can_up_receive_write(0x7A0, 0,  (Uint16*)&data);
                v->step = 0;
            }
            else
            {
                v->step = 0;
            }
            break;

        default:// 多余状态，正常情况下不可能出现
            v->step = 0;
            break;
        }
    }
}













/* End of file */
