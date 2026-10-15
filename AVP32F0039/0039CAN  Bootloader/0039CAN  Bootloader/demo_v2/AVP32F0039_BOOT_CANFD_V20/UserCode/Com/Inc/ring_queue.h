/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :ring_queue.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
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
#define    CAN_FRAME_BUF_LEN              (64)//CAN帧数据长度，考虑到高8位低8位转换效率，直接按字节处理
#define    CAN_QUEUE_FRAME_MAX            (5)//队列长度，按照协议，一次最大存储300字节

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef struct
{
    uint32_t  id;
    uint16_t  len;
    uint16_t  data[CAN_FRAME_BUF_LEN];
}FRAME_DATA;

typedef struct
{
    uint16_t frame_num;
    uint16_t queue_head;
    uint16_t queue_tail;
    FRAME_DATA queue_data[CAN_QUEUE_FRAME_MAX];
}CAN_BUF_QUEUE;

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void Can_queue_write(CAN_BUF_QUEUE *queue, uint32_t frame_id, uint16_t len, uint16_t *buf);
FRAME_DATA Can_queue_read(CAN_BUF_QUEUE *queue);
uint16_t Can_queue_frame_num(CAN_BUF_QUEUE *queue);
void Can_sendframe_clear(CAN_BUF_QUEUE *queue);














#endif
