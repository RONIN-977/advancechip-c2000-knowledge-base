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
#ifndef RING_QUEUE_H_
#define RING_QUEUE_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "device.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    CAN_FRAME_BUF_LEN              (8)//CAN帧数据长度
#define    CAN_QUEUE_FRAME_MAX            (40)//队列长度

#define    UART_QUEUE_LEN                 (32)

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef struct
{
    uint16_t  Frametype;
    uint32_t  Id;
    uint16_t  databuf[CAN_FRAME_BUF_LEN];
}FRAME_DATA;

typedef struct
{
    uint16_t frame_num;
    uint16_t queue_head;
    uint16_t queue_tail;
    FRAME_DATA queue_buf[CAN_QUEUE_FRAME_MAX];
}CAN_BUF_QUEUE;

typedef struct
{
    uint16_t Head;
    uint16_t Tail;
    uint16_t Lenght;
    uint16_t RingBuff[UART_QUEUE_LEN];
}CIRCULAR_QUEUE_T;

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void Can_queue_write(CAN_BUF_QUEUE *queue, uint32_t frame_id, uint16_t frame_type, uint16_t *buf);
FRAME_DATA Can_queue_read(CAN_BUF_QUEUE *queue);
uint16_t Can_queue_frame_num(CAN_BUF_QUEUE *queue);
void Can_sendframe_clear(FRAME_DATA *frame);

uint16_t uart_get_data_length(CIRCULAR_QUEUE_T* queue);
uint16_t uart_write_data(CIRCULAR_QUEUE_T* queue, uint16_t* pdata, uint16_t length);
uint16_t uart_read_data(CIRCULAR_QUEUE_T* queue, uint16_t* pdata);












#endif
