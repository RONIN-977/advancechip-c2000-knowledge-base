/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :CAN_upgrade_protocol.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef CAN_UPGRADE_PROTOCOL_H_
#define CAN_UPGRADE_PROTOCOL_H_
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
    frame_std = 0,
    frame_ext = 1,
}FRAME_TYPE;

typedef enum{
    SF = 0, //single frame
    FF = 1, //first frame
    CF = 2, //consecutive frame
    FC = 3  //flow control
}N_PCIType;

typedef enum{
    Default_Session = 1, //
    Program_Session = 2, //
    Extend_Session = 3, //
}SessionType;

typedef struct
{
    uint16_t ID;//诊断服务号
    uint16_t SID;//子服务号
    uint16_t DL;//帧长度
    uint16_t SN;//续帧的序列号
    uint16_t TxDataBuf[256];//数据缓冲区
}SendData;

typedef struct
{
    uint16_t         send_message_flag;
    CAN_BUF_QUEUE    send_queue;
    CAN_BUF_QUEUE    receive_queue;
}CAN_DRIVER;

typedef struct
{
    uint16_t CurrSession;//当前会话
    uint16_t SessionTimeout;//会话超时计时
    uint16_t FrameControl;//帧接收状态控制

    uint16_t ID;//诊断服务号
    uint16_t SID;//子服务号
    uint16_t DL;//帧长度
    uint16_t SN;//续帧的序列号
    uint16_t RxDataBuf[256];//数据缓冲区
    uint16_t RxDataLen;//续帧实时接收的长度
    uint16_t Rxindex;//续帧实时接收的下标

    uint16_t seed[4];//种子
    uint16_t key[4];//秘钥
    uint16_t unlockflag;//解锁标志

    uint32_t app_pack_len;
    uint16_t BinCRC;
}Proto_t,*pProto;

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void Protocol_Init(void);
void Can_up_send_write(uint32_t frame_id, uint16_t frame_type, uint16_t *buf);
void Can_up_receive_write(uint32_t frame_id, uint16_t frame_type, uint16_t *buf);
void Receive_Data_Frame(void);
void Receive_Data_Unpack(pProto v);
void Can_Task_run(void);









#endif
