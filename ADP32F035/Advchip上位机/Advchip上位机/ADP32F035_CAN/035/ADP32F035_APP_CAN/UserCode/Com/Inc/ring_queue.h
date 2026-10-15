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
#include "ADP32F03x_Project.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    CAN_FRAME_BUF_LEN              (8)//CAN帧数据长度
#define    CAN_QUEUE_FRAME_MAX            (40)//队列长度

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef struct
{
    Uint16  Frametype;
    Uint32  Id;
    Uint16  databuf[CAN_FRAME_BUF_LEN];
}FRAME_DATA;

typedef struct
{
    Uint16 frame_num;
    Uint16 queue_head;
    Uint16 queue_tail;
    FRAME_DATA queue_buf[CAN_QUEUE_FRAME_MAX];
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
void Can_queue_write(CAN_BUF_QUEUE *queue, Uint32 frame_id, Uint16 frame_type, Uint16 *buf);
FRAME_DATA Can_queue_read(CAN_BUF_QUEUE *queue);
Uint16 Can_queue_frame_num(CAN_BUF_QUEUE *queue);
void Can_sendframe_clear(FRAME_DATA *frame);














#endif
