/******************************************************************************
*Copyright(C),2025,Hunan Advancechip Technology CO.,LTD.
*FileName   :dsp_base.c
*Author     :
*Version    :v1.0
*Date       :2025.7.2
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
extern void Can_up_receive_write(uint32_t frame_id, uint16_t frame_type, uint16_t *buf);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void Uart_Transfer_Init(void)
{
    uint16_t i;

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
void uart1_tx_write_data(uint16_t *pdata, uint16_t length)
{
    uart_write_data(&uart_list[Uart1].UartTX, pdata, length);
}

void uart1_tx_read_data(uint16_t *pdata)
{
    uart_read_data(&uart_list[Uart1].UartTX, pdata);
}

uint16_t uart1_tx_get_data_len(void)
{
    return uart_get_data_length(&uart_list[Uart1].UartTX);
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void uart1_rx_write_data(uint16_t *pdata, uint16_t length)
{
    uart_write_data(&uart_list[Uart1].UartRX, pdata, length);
}

void uart1_rx_read_data(uint16_t *pdata)
{
    uart_read_data(&uart_list[Uart1].UartRX, pdata);
}

uint16_t uart1_rx_get_data_len(void)
{
    return uart_get_data_length(&uart_list[Uart1].UartRX);
}

/******************************************************************************
*Description:发送数据打包
-----------------------------------------------------------------------------*/
void Uart_Send_Pack(uint16_t id, uint16_t *pbuff, uint16_t len)
{
    uint16_t sbuf[16];
    uint16_t i;
    uint32_t cks = 0;

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

    uart_write_data(&uart_list[Uart1].UartTX, (uint16_t*)&sbuf, 13);
}

/******************************************************************************
*Description:放在1ms任务运行
-----------------------------------------------------------------------------*/
void Uart_run(void)
{
    uint16_t data;
    uint16_t datalen = uart_get_data_length(&uart_list[Uart1].UartTX);
    static uint16_t period = 0;

    if(period == 0)
    {
        if(datalen > 0)
        {
            uart_read_data(&uart_list[Uart1].UartTX, &data);
            SCI_A_Send_Byte(data);
        }
    }
    period++;
    if(period >= 2)
    {
        period = 0;
    }
}

/******************************************************************************
*Description:放在1ms任务运行
-----------------------------------------------------------------------------*/
void Uart_Receive_Data(void)
{
    UART_RX_FRAME_T*  v = &RxDataFrame;
    uint16_t databuf;
    uint16_t data[8];
    uint16_t i;

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
                Can_up_receive_write(0x7A0, 0,  (uint16_t*)&data);
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
