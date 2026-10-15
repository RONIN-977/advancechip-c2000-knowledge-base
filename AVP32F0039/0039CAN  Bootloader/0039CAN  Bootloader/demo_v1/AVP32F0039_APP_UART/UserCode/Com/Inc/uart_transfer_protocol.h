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
#ifndef UART_TRANSFER_PROTOCOL_H_
#define UART_TRANSFER_PROTOCOL_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "device.h"
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
    uint16_t step; // 状态机步骤
    uint16_t cnt;  // 缓冲区下标
    uint16_t len;  // 帧数据长度
    uint16_t id;   // 帧ID
    uint16_t datalen; // 数据段的长度
    uint16_t buf[32];
    uint16_t checksum1;
    uint16_t checksum2;
    uint16_t NewMsgCnt;
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
void uart1_tx_write_data(uint16_t *pdata, uint16_t length);
void uart1_tx_read_data(uint16_t *pdata);
uint16_t uart1_tx_get_data_len(void);
void uart1_rx_write_data(uint16_t *pdata, uint16_t length);
void uart1_rx_read_data(uint16_t *pdata);
uint16_t uart1_rx_get_data_len(void);
void Uart_run(void);
void Uart_Receive_Data(void);

void Uart_Send_Pack(uint16_t id, uint16_t *pbuff, uint16_t len);









#endif
