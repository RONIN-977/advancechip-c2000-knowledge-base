/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :CAN_upgrade_protocol.c
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
#include "CAN_upgrade_protocol.h"
#include "can_config.h"
#include "crc_verify.h"
#include "dsp_program.h"
#include "timer_config.h"
#include "main.h"

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
Proto_t       proto_data;
CAN_DRIVER    CanDrv;
FRAME_DATA    Send_frame_buf;
FRAME_DATA    Read_frame;
Uint16        BufArray[8];
Uint16        TempDataBuff[128] = {0};
Uint16*       pBuf;

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:升级协议初始化
-----------------------------------------------------------------------------*/
void Protocol_Init(void)
{
    proto_data.CurrSession = Default_Session;
    proto_data.FrameControl = 0;
    proto_data.Rxindex = 0;
    proto_data.Index = 1;//上电默认选定本设备，能返回报文
    proto_data.Index_id = 0;//上位机发送的指定设备ID
    proto_data.local_id = 0;//设备ID 高8位在前 低8位在后，怎么获取?  :todo 新增获取ID的功能

    //测试用ID

#if 1
    proto_data.local_id = 4;
#elif 0
    proto_data.local_id = 2;
#elif 0
    proto_data.local_id = 3;
#endif

    CanDrv.send_message_flag = 0;
    CanDrv.receive_queue.frame_num = 0;
    CanDrv.receive_queue.queue_head = 0;
    CanDrv.receive_queue.queue_tail = 0;
    CanDrv.send_queue.frame_num = 0;
    CanDrv.send_queue.queue_head = 0;
    CanDrv.send_queue.queue_tail = 0;

    pBuf = (Uint16*)&TempDataBuff;
}

/******************************************************************************
*Description:发送队列写接口
-----------------------------------------------------------------------------*/
void Can_up_send_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf)
{
    Can_queue_write(&CanDrv.send_queue, frame_id, frame_type, buf);
}

/******************************************************************************
*Description:接收队列写接口
-----------------------------------------------------------------------------*/
void Can_up_receive_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf)
{
    Can_queue_write(&CanDrv.receive_queue, frame_id, frame_type, buf);
}

/******************************************************************************
*Description:CAN帧数据解析
-----------------------------------------------------------------------------*/
void Receive_Data_Frame(void)
{
    pProto v = &proto_data;
    Uint16 i = 0;

    switch(v->CurrSession)
    {
    case Default_Session:
        break;

    case Program_Session:
        break;

    case Extend_Session:
        break;

    default:
        break;
    }

    if(Can_queue_frame_num(&CanDrv.receive_queue) > 0)
    {
        Read_frame = Can_queue_read(&CanDrv.receive_queue);
        v->FrameControl = (Read_frame.databuf[0]>>4)&0x000F;

        // 设置会话超时计数清零
        v->SessionTimeout = 0;

        switch(v->FrameControl)
        {
        case SF://单帧
            v->Rxindex = 0;
            v->DL = Read_frame.databuf[0] & 0x000F;
            v->ID = Read_frame.databuf[1];
            v->SID = Read_frame.databuf[2];
            v->RxDataLen = v->DL - 2;//去掉ID和SID
            if(v->RxDataLen > 0)
            {
                for(i=0; i<v->RxDataLen; i++)
                {
                    v->RxDataBuf[v->Rxindex] = Read_frame.databuf[3+i];
                    v->Rxindex++;
                }
            }

            //执行ID服务
            Receive_Data_Unpack(v);
            break;

        case FF://首帧
            v->Rxindex = 0;
            v->DL = ((Read_frame.databuf[0]&0x000F)<<8)|(Read_frame.databuf[1]);
            v->ID = Read_frame.databuf[2];
            v->SID = Read_frame.databuf[3];
            if(v->ID == 0x36)
            {
                v->RxDataLen = v->DL - 2;//36 ID 一包130字节，128字节是数据，最后2字节是校验，不需要校验，这里减掉
            }
            else
            {
                v->RxDataLen = v->DL;//其他ID的首帧要注意数据的起始字节不一定从0开始
            }
            if(v->RxDataLen > 0)
            {
                for(i=0; i<4; i++)
                {
                    v->RxDataBuf[v->Rxindex] = Read_frame.databuf[4+i];
                    v->Rxindex++;
                }
            }

            //发送流控帧
            BufArray[0] = 0x30;
            BufArray[1] = 0x00;
            BufArray[2] = 0x00;
            BufArray[3] = 0x00;
            BufArray[4] = 0x00;
            BufArray[5] = 0x00;
            BufArray[6] = 0x00;
            BufArray[7] = 0x00;
            if(proto_data.Index == 1)
            {
                Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
            }
            break;

        case CF://续帧
            v->SN = Read_frame.databuf[0] & 0x000F;
            for(i=0; i<7; i++)
            {
                v->RxDataBuf[v->Rxindex] = Read_frame.databuf[1+i];
                v->Rxindex++;
                if(v->Rxindex >= v->RxDataLen)
                {
                    //执行ID服务
                    Receive_Data_Unpack(v);
                    break;
                }
            }
            break;

        case FC://流控帧
            break;

        default:
            break;
        }
    }
    else
    {
        // 检测诊断超时  屏蔽诊断超时，非UDS
//        v->SessionTimeout++;
//        if(v->SessionTimeout >= 5000)
//        {
//            v->SessionTimeout = 0;
//            v->CurrSession = Default_Session;//退回默认会话
//            v->unlockflag = 0;//上锁
//        }
    }
}

/******************************************************************************
*Description:协议层处理
-----------------------------------------------------------------------------*/
void Receive_Data_Unpack(pProto v)
{
    Uint16 rec = 0;
    Uint16 data_len = 0;
    Uint16 i = 0;
    Uint16 j = 0;

    BufArray[0] = 0x00;
    BufArray[1] = 0x00;
    BufArray[2] = 0x00;
    BufArray[3] = 0x00;
    BufArray[4] = 0x00;
    BufArray[5] = 0x00;
    BufArray[6] = 0x00;
    BufArray[7] = 0x00;

    switch(v->ID)
    {
    case 0x10://进入扩展会话(进入升级请求)

        if(v->SID == 0x01)
        {
            v->CurrSession = Default_Session;
            v->unlockflag = 0; // 0:未解锁      1：已解锁

            BufArray[0] = 0x04;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
            BufArray[3] = (v->local_id >> 8);
            BufArray[4] = v->local_id & 0x00FF;
        }
        else if(v->SID == 0x02)
        {
            v->CurrSession = Program_Session;

            BufArray[0] = 0x04;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
            BufArray[3] = (v->local_id >> 8);
            BufArray[4] = v->local_id & 0x00FF;

            // 在app时设置延时跳回boot；在boot不做处理
        }
        else if(v->SID == 0x03)
        {
            v->CurrSession = Extend_Session;

            BufArray[0] = 0x04;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
            BufArray[3] = (v->local_id >> 8);
            BufArray[4] = v->local_id & 0x00FF;
        }
        else if(v->SID == 0x04)
        {
            //搜索设备ID

            BufArray[0] = 0x04;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
            BufArray[3] = (v->local_id >> 8);
            BufArray[4] = v->local_id & 0x00FF;

            proto_data.Index = 1;//搜索强制打开发送
        }
        else if(v->SID == 0x05)
        {
            //上位机指定的设备ID
            v->Index_id =  (v->RxDataBuf[0]<<8) | v->RxDataBuf[1];
            if(v->Index_id == v->local_id)
            {
                proto_data.Index = 1;//选定通信
            }
            else
            {
                proto_data.Index = 0;//没有选定的，不回复报文
            }
            BufArray[0] = 0x04;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
            BufArray[3] = (v->local_id >> 8);
            BufArray[4] = v->local_id & 0x00FF;
        }

        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    case 0x27://安全访问(升级授权)
        if(v->CurrSession == Extend_Session)
        {
            if(v->SID == 0x01)
            {
                // 获取种子
                Generate_Seed((Uint16*)&(v->seed), GET_SYSTICKCNT());

                BufArray[0] = 0x06;
                BufArray[1] = (v->ID|0x40);
                BufArray[2] = v->SID;
                BufArray[3] = v->seed[0];
                BufArray[4] = v->seed[1];
                BufArray[5] = v->seed[2];
                BufArray[6] = v->seed[3];
            }
            else if(v->SID == 0x02)
            {
                Adv_Security_Algorithm((Uint16*)&(v->seed), (Uint16*)&(v->key));

                // 使能解锁
                if((v->key[0] == v->RxDataBuf[0])&&
                    (v->key[1] == v->RxDataBuf[1])&&
                    (v->key[2] == v->RxDataBuf[2])&&
                    (v->key[3] == v->RxDataBuf[3]))
                {
                    v->unlockflag = 1;
                    BufArray[0] = 0x03;
                    BufArray[1] = (v->ID|0x40);
                    BufArray[2] = v->SID;
                    BufArray[3] = 0x00;// 0:秘钥通过
                }
                else
                {
                    v->unlockflag = 0;
                    BufArray[0] = 0x03;
                    BufArray[1] = 0x7F;
                    BufArray[2] = v->SID;
                    BufArray[3] = 0x01;// 1:秘钥错误
                }
            }
        }
        else
        {
            // 当前会话不支持访问
            BufArray[0] = 0x03;
            BufArray[1] = 0x7F;
            BufArray[2] = v->SID;
            BufArray[3] = 0x00;
        }
        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    case 0x31://例程控制(查询状态)
        if((v->CurrSession == Extend_Session) && (v->unlockflag == 1))
        {
            BufArray[0] = 0x03;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
            BufArray[3] = 0x00;//0在boot，1在app
        }
        else
        {
            // 当前会话不支持访问
            BufArray[0] = 0x03;
            BufArray[1] = 0x7F;
            BufArray[2] = v->SID;
            BufArray[3] = 0x00;
        }
        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    case 0x34://下载请求(擦除FLASH)
        if((v->CurrSession == Extend_Session) && (v->unlockflag == 1))
        {
            //判断固件包大小
            v->app_pack_len = ((Uint32)v->RxDataBuf[1]<<24)|((Uint32)v->RxDataBuf[2]<<16)|((Uint32)v->RxDataBuf[3]<<8)|((Uint32)v->RxDataBuf[4]);
            v->app_pack_len = v->app_pack_len/2;//传输过来的是字节数，这里判断的是 字 (2个字节)
            if(v->app_pack_len <= APP_FLASH_LENGHT)
            {
                //擦除
                rec = Flash_App_Section_Erase();
                if(rec != 0)
                {
                    BufArray[0] = 0x04;
                    BufArray[1] = 0x7F;
                    BufArray[2] = v->SID;
                    BufArray[3] = 0x02;//2：擦除失败
                    BufArray[4] = rec;
                }
                else
                {
                    Flash_App_Section_Write_Init();

                    BufArray[0] = 0x03;
                    BufArray[1] = (v->ID|0x40);
                    BufArray[2] = v->SID;
                    BufArray[3] = 0x00;//0：擦除ok
                }
            }
            else
            {
                BufArray[0] = 0x03;
                BufArray[1] = 0x7F;
                BufArray[2] = v->SID;
                BufArray[3] = 0x01;//1：固件过大
            }
        }
        else
        {
            // 当前会话不支持访问
            BufArray[0] = 0x03;
            BufArray[1] = 0x7F;
            BufArray[2] = v->SID;
            BufArray[3] = 0x00;
        }
        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    case 0x36://传输固件包(写固件)
        if((v->CurrSession == Extend_Session) && (v->unlockflag == 1))
        {
            // 把字节数据组成word数据
            data_len = v->RxDataLen/2;
            for(i=0; i<v->RxDataLen; i+=2)
            {
                *(pBuf + j) = v->RxDataBuf[i]|(v->RxDataBuf[i+1]<<8);
                j++;
            }
            // 写flash
            rec = Flash_App_Section_Write((Uint16*)&TempDataBuff, data_len);

            if(rec != 0)
            {
                BufArray[0] = 0x05;
                BufArray[1] = 0x7F;
                BufArray[2] = v->SID;
                BufArray[3] = 0x01;//1：写失败
                BufArray[4] = rec&0x00FF;//写FLASH结果
                BufArray[5] = rec>>8;//校验数据结果
            }
            else
            {
                BufArray[0] = 0x03;
                BufArray[1] = (v->ID|0x40);
                BufArray[2] = v->SID;
                BufArray[3] = 0x00;//0：写OK
            }
        }
        else
        {
            // 当前会话不支持访问
            BufArray[0] = 0x03;
            BufArray[1] = 0x7F;
            BufArray[2] = v->SID;
            BufArray[3] = 0x00;
        }
        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    case 0x37://退出传输(校验固件包)
        if((v->CurrSession == Extend_Session) && (v->unlockflag == 1))
        {
            //校验写入的CRC和上位机下发的CRC是否一致
            v->BinCRC = ((Uint16)v->RxDataBuf[0]<<8)|((v->SID&0x00FF));
            Uint16 APP_CRC = CRC16Bit_APP(APP_WRITE_ADDRESS, v->app_pack_len);

            if(APP_CRC == v->BinCRC)
            {
                // 比较校验码 或查询app最后的地址标志
                rec = Flash_App_Section_Write_UpdataFlag(v->app_pack_len, APP_CRC);
                if(rec != 0)
                {
                    BufArray[0] = 0x02;
                    BufArray[1] = 0x7F;
                    BufArray[2] = 0x01;
                }
                else
                {
                    BufArray[0] = 0x02;
                    BufArray[1] = (v->ID|0x40);
                    BufArray[2] = 0x00;//0：校验ok，1：校验码错误
                }
            }
            else
            {
                BufArray[0] = 0x04;
                BufArray[1] = 0x7F;
                BufArray[2] = 0x02;//2:CRC错误
                BufArray[3] = APP_CRC & 0x00FF;
                BufArray[4] = APP_CRC >> 8;
            }
        }
        else
        {
            // 当前会话不支持访问
            BufArray[0] = 0x02;
            BufArray[1] = 0x7F;
            BufArray[2] = 0x00;
        }
        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    case 0x11://复位请求
        if((v->CurrSession == Extend_Session) && (v->unlockflag == 1))
        {
            // 设置软件延时复位，等待回复报文发完，50ms
            Set_JumpToApp();

            BufArray[0] = 0x02;
            BufArray[1] = (v->ID|0x40);
            BufArray[2] = v->SID;
        }
        else
        {
            // 当前会话不支持访问
            BufArray[0] = 0x02;
            BufArray[1] = 0x7F;
            BufArray[2] = v->SID;
        }
        if(proto_data.Index == 1)
        {
            Can_up_send_write(0x7B0, frame_std, (Uint16 *)&BufArray);
        }
        break;

    }
}

/******************************************************************************
*Description:协议报文外发响应，放在1ms任务
-----------------------------------------------------------------------------*/
void Can_Task_run(void)
{
    if(Can_queue_frame_num(&CanDrv.send_queue) > 0)
    {
        //先判断发送邮箱是否为空
        if(CAN_Check_Txbuf_Empty() == 0)
        {
            Send_frame_buf = Can_queue_read(&CanDrv.send_queue);
            CanDrv.send_message_flag = 1;
        }
    }
    else
    {
        CanDrv.send_message_flag = 0;
    }

    if(CanDrv.send_message_flag == 1)
    {
        CanDrv.send_message_flag = 0;
        if(Send_frame_buf.Frametype == frame_std)
        {
            if(Send_frame_buf.Id == 0x7B0)
            {
                Can_UDS_Write(Send_frame_buf.Id, (Uint16*)&Send_frame_buf.databuf, CAN_FRAME_BUF_LEN);
            }
        }
        Can_sendframe_clear(&Send_frame_buf);
    }
}





/* End of file */
