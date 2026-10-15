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
#define    PACK_DATA_SEGMENT_LEN_MAX    160//单位:字节

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
typedef enum{
    SF = 0, //single frame
    FF = 1, //first frame
    CF = 2, //consecutive frame
    FC = 3  //flow control
}N_PCIType;

typedef struct
{
    uint16_t         send_message_flag;
    CAN_BUF_QUEUE    send_queue;
    CAN_BUF_QUEUE    receive_queue;
}CAN_DRIVER;

typedef struct
{
    FRAME_DATA    readframe;
    FRAME_DATA    send_frame;
    uint16_t      frametype;
    uint16_t      frame_length;
    uint16_t      frame_length_cnt;
    uint16_t      frame_seq;
    uint16_t      frame_last_seq;
    uint16_t      frame_receive_complete;

    uint16_t      frametype_send;
    uint16_t      frame_data_send[64];
    uint16_t      frame_length_send;
    uint16_t      frame_seq_send;
    uint16_t      frame_cnt_send;

    uint16_t      pack_head;
    uint16_t      pack_length;
    uint16_t      pack_queue_num;
    uint16_t      pack_master_id;
    uint16_t      pack_sub_id;
    uint16_t      pack_check_sum;
    uint32_t      pack_check_sum_calc;
    uint16_t      pack_data_segment_len;
    uint16_t      pack_data[PACK_DATA_SEGMENT_LEN_MAX];

    uint16_t      pack_data_send_buff[PACK_DATA_SEGMENT_LEN_MAX];
    uint16_t      pack_data_send[PACK_DATA_SEGMENT_LEN_MAX];
    uint16_t      pack_check_sum_send;
    uint16_t      pack_master_id_send;
    uint16_t      pack_sub_id_send;
    uint16_t      pack_length_send;
    uint16_t      pack_data_segment_len_send;
    uint16_t      pack_send_request;
}PACK_MESSAGE;

typedef struct
{
    PACK_MESSAGE    unpack;
    uint32_t        address;
    uint32_t        address_value;
    uint32_t        api_rec;
    uint32_t        api_rec1;
    uint32_t        api_rec2;

    uint32_t        app_flash_zoom;
    uint32_t        app_firmware_len_byte;
    uint32_t        app_firmware_len_word;
    uint16_t        app_firmware_crc16;
    uint32_t        app_flash_write_address;
    uint16_t        app_flash_write_data_buf[64];
    uint16_t        app_flash_buf_index;
    uint32_t        app_flash_write_data_len;
    uint16_t        app_flash_crc16;

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
void Can_send_write(uint32_t frame_id, uint16_t len, uint16_t *buf);
void Can_receive_write(uint32_t frame_id, uint16_t len, uint16_t *buf);
void Receive_Data_Frame(void);
void Receive_Data_Unpack(void);
void Can_Task_run(void);
void Send_Data_Pack(void);
void Check_Run_Condition(void);








#endif
