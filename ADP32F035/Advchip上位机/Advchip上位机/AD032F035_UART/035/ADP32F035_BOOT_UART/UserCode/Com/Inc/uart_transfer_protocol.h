/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :uart_transfer_protocol.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef UART_TRANSFER_PROTOCOL_H_
#define UART_TRANSFER_PROTOCOL_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "ADP32F03x_Project.h"
#include "ring_queue.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef enum
{
    Uart_Start=0x00,
    Uart1=Uart_Start,
    UartEnd
}UART_NAME_T;

typedef struct
{
    CIRCULAR_QUEUE_T UartRX;
    CIRCULAR_QUEUE_T UartTX;
}UART_LIST_T,*pUart_para;

typedef struct
{
    Uint16 step; // 状态机步骤
    Uint16 cnt;  // 缓冲区下标
    Uint16 len;  // 帧数据长度
    Uint16 id;   // 帧ID
    Uint16 datalen; // 数据段的长度
    Uint16 buf[32];
    Uint16 checksum1;
    Uint32 checksum2;
    Uint16 NewMsgCnt;
}UART_RX_FRAME_T,*pRxFrame;

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/
extern UART_LIST_T uart_list[];

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void Uart_Transfer_Init(void);
void uart1_tx_write_data(Uint16 *pdata, Uint16 length);
void uart1_tx_read_data(Uint16 *pdata);
Uint16 uart1_tx_get_data_len(void);
void uart1_rx_write_data(Uint16 *pdata, Uint16 length);
void uart1_rx_read_data(Uint16 *pdata);
Uint16 uart1_rx_get_data_len(void);
void Uart_run(void);
void Uart_Receive_Data(void);

void Uart_Send_Pack(Uint16 id, Uint16 *pbuff, Uint16 len);









#endif
