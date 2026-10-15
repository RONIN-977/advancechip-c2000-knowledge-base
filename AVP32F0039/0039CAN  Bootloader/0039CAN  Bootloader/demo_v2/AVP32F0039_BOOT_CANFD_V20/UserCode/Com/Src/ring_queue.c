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
void Can_queue_write(CAN_BUF_QUEUE *queue, uint32_t frame_id, uint16_t len, uint16_t *buf)
{
    uint16_t i;
    if(queue->frame_num >= CAN_QUEUE_FRAME_MAX)
    {
        return;
    }
    queue->queue_data[queue->queue_tail].id = frame_id;
    queue->queue_data[queue->queue_tail].len = len;
    for(i=0; i<CAN_FRAME_BUF_LEN; i++)
    {
        queue->queue_data[queue->queue_tail].data[i] = buf[i];
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
    rec_frame.id = queue->queue_data[queue->queue_head].id;
    rec_frame.len = queue->queue_data[queue->queue_head].len;
    for(i=0; i<CAN_FRAME_BUF_LEN; i++)
    {
        rec_frame.data[i] = queue->queue_data[queue->queue_head].data[i];
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
void Can_sendframe_clear(CAN_BUF_QUEUE *queue)
{
    uint16_t i,j;
    queue->frame_num = 0;
    queue->queue_head = 0;
    queue->queue_tail = 0;
    for(j=0; j<CAN_QUEUE_FRAME_MAX; j++)
    {
        queue->queue_data[j].id = 0x0000;
        queue->queue_data[j].len = 0x0000;
        for(i=0; i<CAN_FRAME_BUF_LEN; i++)
        {
            queue->queue_data[j].data[i] = 0x0000;
        }
    }
}















/* End of file */
