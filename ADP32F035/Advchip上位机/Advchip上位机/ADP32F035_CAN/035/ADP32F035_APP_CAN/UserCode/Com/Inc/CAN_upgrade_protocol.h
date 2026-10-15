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
    Uint16 ID;//诊断服务号
    Uint16 SID;//子服务号
    Uint16 DL;//帧长度
    Uint16 SN;//续帧的序列号
    Uint16 TxDataBuf[256];//数据缓冲区
}SendData;

typedef struct
{
    Uint16           send_message_flag;
    CAN_BUF_QUEUE    send_queue;
    CAN_BUF_QUEUE    receive_queue;
}CAN_DRIVER;

typedef struct
{
    Uint16 CurrSession;//当前会话
    Uint16 SessionTimeout;//会话超时计时
    Uint16 FrameControl;//帧接收状态控制

    Uint16 ID;//诊断服务号
    Uint16 SID;//子服务号
    Uint16 DL;//帧长度
    Uint16 SN;//续帧的序列号
    Uint16 RxDataBuf[256];//数据缓冲区
    Uint16 RxDataLen;//续帧实时接收的长度
    Uint16 Rxindex;//续帧实时接收的下标

    Uint16 seed[4];//种子
    Uint16 key[4];//秘钥
    Uint16 unlockflag;//解锁标志

    Uint32 app_pack_len;
    Uint16 BinCRC;
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
void Can_up_send_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf);
void Can_up_receive_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf);
void Receive_Data_Frame(void);
void Receive_Data_Unpack(pProto v);
void Can_Task_run(void);









#endif
