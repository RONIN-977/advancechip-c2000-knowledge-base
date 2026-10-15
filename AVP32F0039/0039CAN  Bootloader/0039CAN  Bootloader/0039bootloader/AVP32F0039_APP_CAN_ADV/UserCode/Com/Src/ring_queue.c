/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :ring_queue.c
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
#include "ring_queue.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:队列数据写
-----------------------------------------------------------------------------*/
void Can_queue_write(CAN_BUF_QUEUE *queue, uint32_t frame_id, uint16_t frame_type, uint16_t *buf)
{
    uint16_t i;
    if(queue->frame_num >= CAN_QUEUE_FRAME_MAX)
    {
        return;
    }
    queue->queue_buf[queue->queue_tail].Frametype = frame_type;
    queue->queue_buf[queue->queue_tail].Id = frame_id;
    for(i=0; i<CAN_FRAME_BUF_LEN; i++)
    {
        queue->queue_buf[queue->queue_tail].databuf[i] = buf[i];
    }
    queue->frame_num++;
    queue->queue_tail++;
    if(queue->queue_tail >= CAN_QUEUE_FRAME_MAX)
    {
        queue->queue_tail = 0;
    }
}

/******************************************************************************
*Description:队列数据读
-----------------------------------------------------------------------------*/
FRAME_DATA Can_queue_read(CAN_BUF_QUEUE *queue)
{
    FRAME_DATA rec_frame;
    uint16_t i;
    if(queue->frame_num == 0)
    {
        return rec_frame;
    }
    rec_frame.Frametype = queue->queue_buf[queue->queue_head].Frametype;
    rec_frame.Id = queue->queue_buf[queue->queue_head].Id;
    for(i=0; i<CAN_FRAME_BUF_LEN; i++)
    {
        rec_frame.databuf[i] = queue->queue_buf[queue->queue_head].databuf[i];
    }
    queue->frame_num--;
    queue->queue_head++;
    if(queue->queue_head >= CAN_QUEUE_FRAME_MAX)
    {
        queue->queue_head = 0;
    }
    return rec_frame;
}

/******************************************************************************
*Description:队列数据个数
-----------------------------------------------------------------------------*/
uint16_t Can_queue_frame_num(CAN_BUF_QUEUE *queue)
{
    return queue->frame_num;
}

/******************************************************************************
*Description:队列缓冲区清除
-----------------------------------------------------------------------------*/
void Can_sendframe_clear(FRAME_DATA *frame)
{
    uint16_t i;
    frame->Id = 0;
    frame->Frametype = 0;
    for(i=0; i<CAN_FRAME_BUF_LEN; i++)
    {
        frame->databuf[i] = 0;
    }
}















/* End of file */
