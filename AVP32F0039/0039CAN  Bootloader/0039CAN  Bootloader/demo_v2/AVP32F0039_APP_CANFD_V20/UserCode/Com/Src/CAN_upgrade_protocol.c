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
    uint16_t i;

    proto_data.unpack.frametype = 0x0000;
    proto_data.unpack.frame_length = 0x0000;
    proto_data.unpack.frame_length_cnt = 0x0000;
    proto_data.unpack.frame_seq = 0x0000;
    proto_data.unpack.frame_last_seq = 0x0000;
    proto_data.unpack.frame_receive_complete = 0x0000;

    proto_data.unpack.frametype_send = 0x0000;
    proto_data.unpack.frame_length_send = 0x0000;
    proto_data.unpack.frame_seq_send = 0x0000;
    proto_data.unpack.frame_cnt_send = 0x0000;

    proto_data.unpack.pack_head = 0x0000;
    proto_data.unpack.pack_length = 0x0000;
    proto_data.unpack.pack_queue_num = 0x0000;
    proto_data.unpack.pack_master_id = 0x0000;
    proto_data.unpack.pack_sub_id = 0x0000;
    proto_data.unpack.pack_check_sum = 0x0000;
    proto_data.unpack.pack_check_sum_calc = 0x0000;
    proto_data.unpack.pack_data_segment_len = 0x0000;

    proto_data.unpack.pack_check_sum_send = 0x0000;
    proto_data.unpack.pack_master_id_send = 0x0000;
    proto_data.unpack.pack_sub_id_send = 0x0000;
    proto_data.unpack.pack_length_send = 0x0000;
    proto_data.unpack.pack_data_segment_len_send = 0x0000;
    proto_data.unpack.pack_send_request = 0x0000;

    for(i=0; i<PACK_DATA_SEGMENT_LEN_MAX; i++)
    {
        proto_data.unpack.pack_data[i] = 0x0000;
        proto_data.unpack.pack_data_send_buff[i] = 0x0000;
        proto_data.unpack.pack_data_send[i] = 0x0000;
    }

    for(i=0; i<64; i++)
    {
        proto_data.unpack.frame_data_send[i] = 0x0000;
    }

    Can_sendframe_clear(&CanDrv.send_queue);
    Can_sendframe_clear(&CanDrv.receive_queue);
    CanDrv.send_message_flag = 0;

    Check_Run_Condition();//回复报文检测
}

/******************************************************************************
*Description:发送队列写接口
-----------------------------------------------------------------------------*/
void Can_send_write(uint32_t frame_id, uint16_t len, uint16_t *buf)
{
    Can_queue_write(&CanDrv.send_queue, frame_id, len, buf);
}

/******************************************************************************
*Description:接收队列写接口
-----------------------------------------------------------------------------*/
void Can_receive_write(uint32_t frame_id, uint16_t len, uint16_t *buf)
{
    Can_queue_write(&CanDrv.receive_queue, frame_id, len, buf);
}

/******************************************************************************
*Description:CAN帧数据解析
-----------------------------------------------------------------------------*/
void Receive_Data_Frame(void)
{
    uint16_t i;
    pProto v = &proto_data;

    if(Can_queue_frame_num(&CanDrv.receive_queue) > 0)
    {
        v->unpack.readframe = Can_queue_read(&CanDrv.receive_queue);
        v->unpack.frametype = (v->unpack.readframe.data[0] >> 4) & 0x000F;

        switch(v->unpack.frametype)
        {
        case SF:
            if(v->unpack.readframe.len <= 8)
            {
                v->unpack.frame_length = v->unpack.readframe.data[0] & 0x0F;
                for(i=0; i<v->unpack.frame_length; i++)
                {
                    v->unpack.pack_data[i] = v->unpack.readframe.data[i + 1];
                }
                v->unpack.frame_length_cnt = v->unpack.frame_length;
                v->unpack.frame_last_seq = 0;
                v->unpack.frame_receive_complete = 1;
            }
            else if((v->unpack.readframe.len > 8) && (v->unpack.readframe.len <= 62))
            {
                v->unpack.frame_length = ((v->unpack.readframe.data[0] & 0x0F)<<8) | (v->unpack.readframe.data[1] & 0x00FF);
                for(i=0; i<v->unpack.frame_length; i++)
                {
                    v->unpack.pack_data[i] = v->unpack.readframe.data[i + 2];
                }
                v->unpack.frame_length_cnt = v->unpack.frame_length;
                v->unpack.frame_last_seq = 0;
                v->unpack.frame_receive_complete = 1;
            }
            break;

        case FF:
            v->unpack.frame_length = ((v->unpack.readframe.data[0] & 0x0F)<<8) | (v->unpack.readframe.data[1] & 0x00FF);
            for(i=0; i<62; i++)
            {
                v->unpack.pack_data[i] = v->unpack.readframe.data[i + 2];
            }

            v->unpack.frame_length_cnt = 62;
            v->unpack.frame_last_seq = 0;
            v->unpack.frame_receive_complete = 0;
            break;

        case CF:
            v->unpack.frame_seq = v->unpack.readframe.data[0] & 0x000F;

            v->unpack.frame_last_seq += 1;
            if(v->unpack.frame_last_seq > 0x000F)
                v->unpack.frame_last_seq = 0;
            if(v->unpack.frame_last_seq == v->unpack.frame_seq)
            {
                ;//续帧队列号匹配
            }
            else
            {
                ;//续帧队列号不匹配
            }

            for(i=0; i<63; i++)
            {
                v->unpack.pack_data[v->unpack.frame_length_cnt] = v->unpack.readframe.data[i + 1];
                v->unpack.frame_length_cnt++;
                if(v->unpack.frame_length_cnt == v->unpack.frame_length)
                {
                    v->unpack.frame_receive_complete = 1;
                    break;
                }
            }
            break;

        case FC://流控帧不需要处理
            break;

        default:
            break;
        }
    }
}

/******************************************************************************
*Description:把发送的数据包，按CAN帧格式拆分，存储到发送队列
-----------------------------------------------------------------------------*/
void Send_Data_Frame(void)
{
    uint16_t i,j,k;
    pProto v = &proto_data;

    if(v->unpack.pack_send_request == 1)
    {
        v->unpack.pack_send_request = 0;
        if(v->unpack.pack_length_send < 8)
        {
            v->unpack.frametype_send = SF;
            v->unpack.frame_length_send = v->unpack.pack_length_send;
            v->unpack.frame_seq_send = 0;

            v->unpack.frame_data_send[0] = (v->unpack.frametype_send << 4) | v->unpack.frame_length_send;
            for(i=0; i<v->unpack.frame_length_send; i++)
            {
                v->unpack.frame_data_send[i + 1] = v->unpack.pack_data_send[i];
            }
            Can_send_write(SLAVE_STATION_INDEX_RESPONSE_ID, v->unpack.frame_length_send + 1, (uint16_t*)&v->unpack.frame_data_send);
        }
        else if((v->unpack.pack_length_send >= 8) && (v->unpack.pack_length_send < 63))
        {
            v->unpack.frametype_send = SF;
            v->unpack.frame_length_send = v->unpack.pack_length_send;
            v->unpack.frame_seq_send = 0;

            v->unpack.frame_data_send[0] = (v->unpack.frametype_send << 4) | ((v->unpack.frame_length_send >> 8) & 0x00FF);
            v->unpack.frame_data_send[1] = v->unpack.frame_length_send & 0x00FF;
            for(i=0; i<v->unpack.frame_length_send; i++)
            {
                v->unpack.frame_data_send[i + 2] = v->unpack.pack_data_send[i];
            }
            Can_send_write(SLAVE_STATION_INDEX_RESPONSE_ID, v->unpack.frame_length_send + 2, (uint16_t*)&v->unpack.frame_data_send);
        }
        else if(v->unpack.pack_length_send >= 63)
        {
            //单次数据超过7字节，需要用首帧+续帧方式分段发送
            //处理首帧
            v->unpack.frametype_send = FF;
            v->unpack.frame_length_send = v->unpack.pack_length_send;
            v->unpack.frame_seq_send = 0;

            v->unpack.frame_data_send[0] = (v->unpack.frametype_send << 4) | ((v->unpack.frame_length_send >> 8) & 0x00FF);
            v->unpack.frame_data_send[1] = v->unpack.frame_length_send & 0x00FF;
            for(i=0; i<62; i++)
            {
                v->unpack.frame_data_send[i + 2] = v->unpack.pack_data_send[i];
            }
            Can_send_write(SLAVE_STATION_INDEX_RESPONSE_ID, 64, (uint16_t*)&v->unpack.frame_data_send);

            //处理续帧
            v->unpack.frametype_send = CF;
            k = 62;
            if(((v->unpack.frame_length_send - 62) % 63) == 0)//剩余帧数
            {
                v->unpack.frame_cnt_send = (v->unpack.frame_length_send - 62) / 63;
            }
            else
            {
                v->unpack.frame_cnt_send = ((v->unpack.frame_length_send - 62) / 63) + 1;
            }

            for(i=0; i<v->unpack.frame_cnt_send; i++)
            {
                v->unpack.frame_seq_send++;
                if(v->unpack.frame_seq_send > 0x000F)
                    v->unpack.frame_seq_send = 0;

                v->unpack.frame_data_send[0] = (v->unpack.frametype_send << 4) | (v->unpack.frame_seq_send & 0x000F);
                for(j=0; j<63; j++)
                {
                    if(k < v->unpack.frame_length_send)
                    {
                        v->unpack.frame_data_send[j + 1] = v->unpack.pack_data_send[i*63 + j + 62];
                    }
                    else
                    {
                        v->unpack.frame_data_send[j + 1] = 0x0000;
                    }
                    k++;
                }
                Can_send_write(SLAVE_STATION_INDEX_RESPONSE_ID, 64, (uint16_t*)&v->unpack.frame_data_send);
            }
        }
    }
}

/******************************************************************************
*Description:协议层处理
-----------------------------------------------------------------------------*/
void Receive_Data_Unpack(void)
{
    uint16_t i;
    pProto v = &proto_data;

    if(v->unpack.frame_receive_complete == 1)
    {
        v->unpack.frame_receive_complete = 0;

        v->unpack.pack_check_sum_calc = 0;

        //包头识别码
        v->unpack.pack_head = (v->unpack.pack_data[0] << 8) | (v->unpack.pack_data[1]);
        if(v->unpack.pack_head == 0x55AA)
        {
            ;
        }
        else
        {
            ;
        }

        //数据包长度
        v->unpack.pack_length = (v->unpack.pack_data[2] << 8) | (v->unpack.pack_data[3]);
        v->unpack.pack_data_segment_len = v->unpack.pack_length - 9;

        //包队列号
        v->unpack.pack_queue_num = v->unpack.pack_data[4];

        //功能ID
        v->unpack.pack_master_id = v->unpack.pack_data[5];

        //子功能ID
        v->unpack.pack_sub_id = v->unpack.pack_data[6];

        //16位校验和
        v->unpack.pack_check_sum = (v->unpack.pack_data[v->unpack.pack_length - 2] << 8) | (v->unpack.pack_data[v->unpack.pack_length - 1]);

        //校验和
        for(i=0; i<(v->unpack.pack_length - 2); i++)
        {
            v->unpack.pack_check_sum_calc += v->unpack.pack_data[i];
        }
        v->unpack.pack_check_sum_calc = v->unpack.pack_check_sum_calc & 0xFFFF;
        if(v->unpack.pack_check_sum_calc == v->unpack.pack_check_sum)
        {
            //执行请求
            switch(v->unpack.pack_master_id)
            {
                case 0x01:
                {
                    switch(v->unpack.pack_sub_id)
                    {
                        case 0x01://查询程序运行在boot?app?
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            v->unpack.pack_data_send_buff[0] = 0x01;//在app
                            v->unpack.pack_data_segment_len_send = 1;//返回数据段的长度
                            break;

                        case 0x02://查询boot版本号
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            for(i=0; i<8; i++)
                            {
                                v->unpack.pack_data_send_buff[i] = dsp_info.info.boot_version[i];
                            }
                            v->unpack.pack_data_segment_len_send = 8;
                            break;

                        case 0x03://查询app版本号
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            for(i=0; i<8; i++)
                            {
                                v->unpack.pack_data_send_buff[i] = dsp_info.info.app_version[i];
                            }
                            v->unpack.pack_data_segment_len_send = 8;
                            break;

                        case 0x04://查询UID
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
//                            v->unpack.pack_data_send_buff[0] = (*(uint16_t*)0x3D7C00) & 0x00FF;
//                            v->unpack.pack_data_send_buff[1] = (*(uint16_t*)0x3D7C00) >> 8;
//                            v->unpack.pack_data_send_buff[2] = (*(uint16_t*)0x3D7C01) & 0x00FF;
//                            v->unpack.pack_data_send_buff[3] = (*(uint16_t*)0x3D7C01) >> 8;
//                            v->unpack.pack_data_send_buff[4] = (*(uint16_t*)0x3D7C02) & 0x00FF;
//                            v->unpack.pack_data_send_buff[5] = (*(uint16_t*)0x3D7C02) >> 8;
//                            v->unpack.pack_data_send_buff[6] = (*(uint16_t*)0x3D7C03) & 0x00FF;
//                            v->unpack.pack_data_send_buff[7] = (*(uint16_t*)0x3D7C03) >> 8;
                            v->unpack.pack_data_segment_len_send = 8;
                            break;

                        case 0x05://查询PID
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
//                            v->unpack.pack_data_send_buff[0] = (*(uint16_t*)0x3D7C04) & 0x00FF;
//                            v->unpack.pack_data_send_buff[1] = (*(uint16_t*)0x3D7C04) >> 8;
                            v->unpack.pack_data_segment_len_send = 2;
                            break;

                        case 0x06://查询boot烧录日期
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            for(i=0; i<10; i++)
                            {
                                v->unpack.pack_data_send_buff[i] = dsp_info.info.boot_download_date[i];
                            }
                            v->unpack.pack_data_segment_len_send = 10;
                            break;

                        case 0x07://查询app烧录日期
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            for(i=0; i<10; i++)
                            {
                                v->unpack.pack_data_send_buff[i] = dsp_info.info.app_download_date[i];
                            }
                            v->unpack.pack_data_segment_len_send = 10;
                            break;

                        case 0x08://查询传入地址指向的值
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            // 7 8 9 10(下标) 这4个字节是地址
                            v->address = ((uint32_t)v->unpack.pack_data[7]<<24) | ((uint32_t)v->unpack.pack_data[8]<<16) |
                                    ((uint32_t)v->unpack.pack_data[9]<<8) | ((uint32_t)v->unpack.pack_data[10]);

                            // 11(下标)字节是数据类型
                            if(v->unpack.pack_data[11] == 0)
                            {
                                v->address_value = *(uint16_t*)v->address;
                            }
                            else if(v->unpack.pack_data[11] == 1)
                            {
                                v->address_value = *(uint32_t*)v->address;
                            }

                            v->unpack.pack_data_send_buff[0] = v->address_value >> 24;
                            v->unpack.pack_data_send_buff[1] = (v->address_value >> 16) & 0x00FF;
                            v->unpack.pack_data_send_buff[2] = (v->address_value >> 8) & 0x00FF;
                            v->unpack.pack_data_send_buff[3] = v->address_value & 0x000000FF;

                            v->unpack.pack_data_send_buff[4] = v->unpack.pack_data[11];

                            v->unpack.pack_data_segment_len_send = 5;
                            break;

                        case 0x09://查询固件校验数据
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            v->unpack.pack_data_send_buff[0] = (*(uint16_t*)APP_CRC_ADDRESS) & 0x00FF;
                            v->unpack.pack_data_send_buff[1] = (*(uint16_t*)APP_CRC_ADDRESS) >> 8;
                            v->unpack.pack_data_segment_len_send = 2;
                            break;

                        case 0x0A://查询固件标志
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            v->unpack.pack_data_send_buff[0] = (*(uint16_t*)APP_CRC_ADDRESS) & 0x00FF;
                            v->unpack.pack_data_send_buff[1] = (*(uint16_t*)APP_CRC_ADDRESS) >> 8;
                            v->unpack.pack_data_segment_len_send = 2;
                            break;

                        case 0x0B://查询FLASH APP区的起始地址、末地址、总长度、crc16地址
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            v->address = ((uint32_t)dsp_info.info.app_head_address[0]<<16) | ((uint32_t)dsp_info.info.app_head_address[1]);
                            v->unpack.pack_data_send_buff[0] = v->address >> 24;
                            v->unpack.pack_data_send_buff[1] = (v->address >> 16) & 0x00FF;
                            v->unpack.pack_data_send_buff[2] = (v->address >> 8) & 0x00FF;
                            v->unpack.pack_data_send_buff[3] = v->address & 0x000000FF;

                            v->address = ((uint32_t)dsp_info.info.app_tail_address[0]<<16) | ((uint32_t)dsp_info.info.app_tail_address[1]);
                            v->unpack.pack_data_send_buff[4] = v->address >> 24;
                            v->unpack.pack_data_send_buff[5] = (v->address >> 16) & 0x00FF;
                            v->unpack.pack_data_send_buff[6] = (v->address >> 8) & 0x00FF;
                            v->unpack.pack_data_send_buff[7] = v->address & 0x000000FF;

                            v->address = ((uint32_t)dsp_info.info.app_total_length[0]<<16) | ((uint32_t)dsp_info.info.app_total_length[1]);
                            v->unpack.pack_data_send_buff[8] = v->address >> 24;
                            v->unpack.pack_data_send_buff[9] = (v->address >> 16) & 0x00FF;
                            v->unpack.pack_data_send_buff[10] = (v->address >> 8) & 0x00FF;
                            v->unpack.pack_data_send_buff[11] = v->address & 0x000000FF;

                            v->address = ((uint32_t)dsp_info.info.app_verify_address[0]<<16) | ((uint32_t)dsp_info.info.app_verify_address[1]);
                            v->unpack.pack_data_send_buff[12] = v->address >> 24;
                            v->unpack.pack_data_send_buff[13] = (v->address >> 16) & 0x00FF;
                            v->unpack.pack_data_send_buff[14] = (v->address >> 8) & 0x00FF;
                            v->unpack.pack_data_send_buff[15] = v->address & 0x000000FF;

                            v->unpack.pack_data_segment_len_send = 16;
                            break;

                        case 0x0C://查询DSP型号
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            v->unpack.pack_data_send_buff[0] = 0x08;//0039
                            v->unpack.pack_data_segment_len_send = 1;
                            break;

                        case 0x0D://查询DSP烧录的APP长度
                            v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                            v->address = ((uint32_t)dsp_info.info.app_length_address[0]<<16) | ((uint32_t)dsp_info.info.app_length_address[1]);
                            v->address_value = *(uint32_t*)v->address;
                            v->unpack.pack_data_send_buff[0] = v->address_value >> 24;
                            v->unpack.pack_data_send_buff[1] = (v->address_value >> 16) & 0x00FF;
                            v->unpack.pack_data_send_buff[2] = (v->address_value >> 8) & 0x00FF;
                            v->unpack.pack_data_send_buff[3] = v->address_value & 0x000000FF;

                            v->unpack.pack_data_segment_len_send = 4;
                            break;

                        default://case 0xFF //接收DSP的数据包校验和错误，需要重发

                            break;
                    }
                }
                break;

                case 0x02:
                {
                    switch(v->unpack.pack_sub_id)
                    {
                    case 0x01://直接刷写到单一固件区
                        v->unpack.pack_data_send_buff[0] = 1;
                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 1;
                        break;

                    case 0x02://刷写到临时代码区
                        v->unpack.pack_data_send_buff[0] = 1;
                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 1;
                        break;

                    case 0x03://刷写到备份区
                        v->unpack.pack_data_send_buff[0] = 1;
                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 1;
                        break;

                    case 0x04://擦除固件标志
                        v->unpack.pack_data_send_buff[0] = 0;
                        v->unpack.pack_data_send_buff[1] = 0;
                        v->unpack.pack_data_send_buff[2] = 0;
                        v->unpack.pack_data_send_buff[3] = 0;

                        v->unpack.pack_data_send_buff[4] = 0;

                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 5;
                        break;

                    case 0x05://擦除固件校验
                        v->unpack.pack_data_send_buff[0] = 0;
                        v->unpack.pack_data_send_buff[1] = 0;
                        v->unpack.pack_data_send_buff[2] = 0;
                        v->unpack.pack_data_send_buff[3] = 0;

                        v->unpack.pack_data_send_buff[4] = 0;

                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 5;
                        break;

                    case 0x06://擦除FLASH扇区，存放固件区域
                        v->api_rec = Flash_App_Section_Erase();

                        v->api_rec1 = v->api_rec >> 16;
                        v->api_rec2 = v->api_rec & 0xFFFF;
                        //擦除函数返回值
                        v->unpack.pack_data_send_buff[0] = (v->api_rec1) & 0x00FF;
                        v->unpack.pack_data_send_buff[1] = (v->api_rec1) >> 8;

                        //校验函数返回值
                        v->unpack.pack_data_send_buff[2] = (v->api_rec2) & 0x00FF;
                        v->unpack.pack_data_send_buff[3] = (v->api_rec2) >> 8;

                        v->app_flash_write_address = (uint32_t)APP_WRITE_ADDRESS;
                        v->app_firmware_len_word = 0;

                        //擦除结果
                        if(v->api_rec1 == 0)
                        {
                            v->unpack.pack_data_send_buff[4] = 0;
                        }
                        else
                        {
                            v->unpack.pack_data_send_buff[4] = 1;
                        }
                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 5;
                        break;

                    case 0x07://分包传输固件
                        v->app_flash_buf_index = 0;
                        for(i=0; i < 128; i+=2)
                        {
                            v->app_flash_write_data_buf[v->app_flash_buf_index] = (v->unpack.pack_data[i + 7]) |
                                                                                    (v->unpack.pack_data[i + 8] << 8);
                            v->app_flash_buf_index++;
                        }
                        v->api_rec = Flash_App_Section_Write(v->app_flash_write_address, (uint16_t*)&v->app_flash_write_data_buf, 64);
                        v->api_rec1 = v->api_rec >> 16;
                        v->api_rec2 = v->api_rec & 0x0000FFFF;

                        v->unpack.pack_data_send_buff[0] = v->api_rec1 >> 8;
                        v->unpack.pack_data_send_buff[1] = v->api_rec1 & 0x00FF;

                        v->unpack.pack_data_send_buff[2] = v->api_rec2 >> 8;
                        v->unpack.pack_data_send_buff[3] = v->api_rec2 & 0x00FF;

                        v->unpack.pack_data_send_buff[4] = v->app_flash_write_address >> 24;
                        v->unpack.pack_data_send_buff[5] = (v->app_flash_write_address >> 16) & 0x00FF;
                        v->unpack.pack_data_send_buff[6] = (v->app_flash_write_address >> 8) & 0x00FF;
                        v->unpack.pack_data_send_buff[7] = v->app_flash_write_address & 0x000000FF;

                        if(v->api_rec1 == 0)
                        {
                            v->unpack.pack_data_send_buff[8] = 0;
                            if(v->api_rec2 != 0)
                            {
                                v->unpack.pack_data_send_buff[8] = 2;
                            }
                            else
                            {
                                v->app_firmware_len_word += 64;
                                v->app_flash_write_address += 64;
                            }
                        }
                        else
                        {
                            v->unpack.pack_data_send_buff[8] = 1;
                        }

                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 9;
                        break;

                    case 0x08://校验写入的固件
                        v->app_firmware_crc16 = (v->unpack.pack_data[7]<<8) | (v->unpack.pack_data[8]);
                        v->app_flash_crc16 = CRC16Bit_APP(APP_WRITE_ADDRESS, v->app_firmware_len_word);
                        if(v->app_firmware_crc16 == v->app_flash_crc16)
                        {
                            v->unpack.pack_data_send_buff[2] = 0;
                        }
                        else
                        {
                            v->unpack.pack_data_send_buff[2] = 1;
                        }
                        v->unpack.pack_data_send_buff[0] = v->app_flash_crc16 >> 8;
                        v->unpack.pack_data_send_buff[1] = v->app_flash_crc16 & 0x00FF;

                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 3;
                        break;

                    case 0x09://写入固件校验
                        //固件长度
                        v->app_flash_write_data_buf[0] = v->app_firmware_len_word >> 16;
                        v->app_flash_write_data_buf[1] = v->app_firmware_len_word & 0x0000FFFF;

                        //固件校验值
                        v->app_flash_write_data_buf[2] = v->app_flash_crc16;

                        v->app_flash_write_address = APP_LENGTH_ADDRESS;

                        v->api_rec = Flash_App_Section_Write(v->app_flash_write_address, (uint16_t*)&v->app_flash_write_data_buf, 3);
                        v->api_rec1 = v->api_rec >> 16;
                        v->api_rec2 = v->api_rec & 0x0000FFFF;

                        v->unpack.pack_data_send_buff[0] = v->api_rec1 >> 8;
                        v->unpack.pack_data_send_buff[1] = v->api_rec1 & 0x00FF;

                        v->unpack.pack_data_send_buff[2] = v->api_rec2 >> 8;
                        v->unpack.pack_data_send_buff[3] = v->api_rec2 & 0x00FF;

                        v->unpack.pack_data_send_buff[4] = v->app_flash_write_address >> 24;
                        v->unpack.pack_data_send_buff[5] = (v->app_flash_write_address >> 16) & 0x00FF;
                        v->unpack.pack_data_send_buff[6] = (v->app_flash_write_address >> 8) & 0x00FF;
                        v->unpack.pack_data_send_buff[7] = v->app_flash_write_address & 0x000000FF;

                        if(v->api_rec1 == 0)
                        {
                            v->unpack.pack_data_send_buff[8] = 0;
                            if(v->api_rec2 != 0)
                            {
                                v->unpack.pack_data_send_buff[8] = 2;
                            }
                        }
                        else
                        {
                            v->unpack.pack_data_send_buff[8] = 1;
                        }

                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 9;
                        break;

                    case 0x0A://检验固件大小与FLASH固件区大小范围是否匹配
                        v->app_firmware_len_byte = ((uint32_t)v->unpack.pack_data[7]<<24) | ((uint32_t)v->unpack.pack_data[8]<<16) |
                                                    ((uint32_t)v->unpack.pack_data[9]<<8) | ((uint32_t)v->unpack.pack_data[10]);

                        v->app_flash_zoom = (uint32_t)APP_FLASH_LENGHT * 2;
                        if(v->app_firmware_len_byte <= v->app_flash_zoom)
                        {
                            v->unpack.pack_data_send_buff[4] = 0x0;
                        }
                        else
                        {
                            v->unpack.pack_data_send_buff[4] = 0x1;
                        }

                        v->unpack.pack_data_send_buff[0] = v->app_flash_zoom >> 24;
                        v->unpack.pack_data_send_buff[1] = (v->app_flash_zoom >> 16) & 0x00FF;
                        v->unpack.pack_data_send_buff[2] = (v->app_flash_zoom >> 8) & 0x00FF;
                        v->unpack.pack_data_send_buff[3] = v->app_flash_zoom & 0x000000FF;

                        v->unpack.pack_sub_id_send = v->unpack.pack_sub_id;
                        v->unpack.pack_data_segment_len_send = 5;
                        break;

                    default://case 0xFF //接收DSP的数据包校验和错误，需要重发

                        break;
                    }
                }
                break;

                case 0x03:
                {
                    switch(v->unpack.pack_sub_id)
                    {
                    case 0x01://DSP软件复位

                        break;

                    case 0x02://跳转boot请求
                        RAM_Write_Package_Seq(v->unpack.pack_queue_num);
                        DSP_JumpToBoot();//立即跳转，在boot回复
                        break;

                    case 0x03://跳转app请求

                        break;

                    case 0x04://控制LED正常闪烁

                        break;

                    case 0x05://控制LED常灭

                        break;

                    case 0x06://控制蜂鸣器响

                        break;

                    case 0x07://控制蜂鸣器灭

                        break;

                    case 0x08://控制测试报文外发

                        break;

                    case 0x09://控制测试报文停止外发

                        break;

                    default://case 0xFF //接收DSP的数据包校验和错误，需要重发

                        break;
                    }
                }
                break;

                case 0x04:
                {
                    switch(v->unpack.pack_sub_id)
                    {
                    case 0x00://向DSP发送字符串或指令

                        break;
                    }
                }
                break;
            }
        }
        else
        {
            //校验和错误
            v->unpack.pack_sub_id_send = 0xFF;
            v->unpack.pack_data_send_buff[0] = 0x00;//返回的数据段
            v->unpack.pack_data_segment_len_send = 1;//返回数据段的长度
        }

        Send_Data_Pack();//把发送的数据，按照协议打包
    }
}

void Send_Data_Pack(void)
{
    uint16_t i;
    pProto v = &proto_data;

    v->unpack.pack_check_sum_send = 0;
    v->unpack.pack_send_request = 0;

    v->unpack.pack_data_send[0] = 0x0055;
    v->unpack.pack_data_send[1] = 0x00AA;

    v->unpack.pack_length_send = 9 + v->unpack.pack_data_segment_len_send;
    v->unpack.pack_data_send[2] = v->unpack.pack_length_send >> 8;
    v->unpack.pack_data_send[3] = v->unpack.pack_length_send & 0x00FF;

    v->unpack.pack_data_send[4] = v->unpack.pack_queue_num;

    v->unpack.pack_master_id_send = v->unpack.pack_master_id | 0x0080;
    v->unpack.pack_data_send[5] = v->unpack.pack_master_id_send;

    v->unpack.pack_data_send[6] = v->unpack.pack_sub_id_send;

    for(i=0; i<v->unpack.pack_data_segment_len_send; i++)
    {
        v->unpack.pack_data_send[i + 7] = v->unpack.pack_data_send_buff[i];
    }

    for(i=0; i<(v->unpack.pack_length_send - 2); i++)
    {
        v->unpack.pack_check_sum_send += v->unpack.pack_data_send[i];
    }
    v->unpack.pack_check_sum_send = v->unpack.pack_check_sum_send  & 0xFFFF;

    v->unpack.pack_data_send[v->unpack.pack_length_send - 2] = v->unpack.pack_check_sum_send >> 8;
    v->unpack.pack_data_send[v->unpack.pack_length_send - 1] = v->unpack.pack_check_sum_send & 0x00FF;

    v->unpack.pack_send_request = 1;

    Send_Data_Frame();
}

/******************************************************************************
*Description:协议报文外发响应，放在1ms任务
-----------------------------------------------------------------------------*/
void Can_Task_run(void)
{
    pProto v = &proto_data;
    static uint16_t tick = 0;

    if(tick == 0)//3ms发一次，更稳定
    {
        if(Can_queue_frame_num(&CanDrv.send_queue) > 0)
        {
            v->unpack.send_frame = Can_queue_read(&CanDrv.send_queue);
            CanFD_Send_Msg(v->unpack.send_frame.id, 0, v->unpack.send_frame.len, (uint16_t*)&v->unpack.send_frame.data);
            //Can_sendframe_clear((CAN_BUF_QUEUE *)&v->unpack.send_frame);
        }
    }

    tick++;
    if(tick >= 3)
    {
        tick = 0;
    }
}

/******************************************************************************
*Description:检查上电运行，还是跳转运行
-----------------------------------------------------------------------------*/
void Check_Run_Condition(void)
{
    pProto v = &proto_data;

    if(Is_Boot_Jump_App_Check() == 1)
    {
        v->unpack.pack_data_send_buff[0] = 0;//跳转成功
        v->unpack.pack_data_segment_len_send = 1;

        v->unpack.pack_master_id = 0x03;
        v->unpack.pack_sub_id_send = 0x02;//跳转boot应答
        v->unpack.pack_queue_num = RAM_Read_Package_Seq();//数据包序列号从app保存，在boot读取

        Send_Data_Pack();//准备发送报文
    }
}



/* End of file */
