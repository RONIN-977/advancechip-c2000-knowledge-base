/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :can_config.h
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
#ifndef CAN_CONFIG_H_
#define CAN_CONFIG_H_
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "device.h"

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/
#define    RX_MSG_OBJ_ID    16
#define    TX_MSG_OBJ_ID    1

#define NUM_OF_MSG                      (1U)
#define MCAN_STD_ID_FILTER_NUM          (1U)
#define MCAN_EXT_ID_FILTER_NUM          (0U)
#define MCAN_FIFO_0_NUM                 (0U)
#define MCAN_FIFO_0_ELEM_SIZE           (MCAN_ELEM_SIZE_64BYTES)
#define MCAN_FIFO_1_NUM                 (0U)
#define MCAN_FIFO_1_ELEM_SIZE           (MCAN_ELEM_SIZE_64BYTES)
#define MCAN_RX_BUFF_NUM                (10U)
#define MCAN_RX_BUFF_ELEM_SIZE          (MCAN_ELEM_SIZE_64BYTES)
#define MCAN_TX_BUFF_SIZE               (NUM_OF_MSG)
#define MCAN_TX_FQ_SIZE                 (0U)
#define MCAN_TX_BUFF_ELEM_SIZE          (MCAN_ELEM_SIZE_64BYTES)
#define MCAN_TX_EVENT_SIZE              (0U)

#define MCAN_STD_ID_FILT_START_ADDR     (0x0U)
#define MCAN_EXT_ID_FILT_START_ADDR     (MCAN_STD_ID_FILT_START_ADDR + ((MCAN_STD_ID_FILTER_NUM * MCANSS_STD_ID_FILTER_SIZE_WORDS * 4U)))
#define MCAN_FIFO_0_START_ADDR          (MCAN_EXT_ID_FILT_START_ADDR + ((MCAN_EXT_ID_FILTER_NUM * MCANSS_EXT_ID_FILTER_SIZE_WORDS * 4U)))
#define MCAN_FIFO_1_START_ADDR          (MCAN_FIFO_0_START_ADDR + (MCAN_getMsgObjSize(MCAN_FIFO_0_ELEM_SIZE) * 4U * MCAN_FIFO_0_NUM))
#define MCAN_RX_BUFF_START_ADDR         (MCAN_FIFO_1_START_ADDR + (MCAN_getMsgObjSize(MCAN_FIFO_1_ELEM_SIZE) * 4U * MCAN_FIFO_1_NUM))
#define MCAN_TX_BUFF_START_ADDR         (MCAN_RX_BUFF_START_ADDR + (MCAN_getMsgObjSize(MCAN_RX_BUFF_ELEM_SIZE) * 4U * MCAN_RX_BUFF_NUM))
#define MCAN_TX_EVENT_START_ADDR        (MCAN_TX_BUFF_START_ADDR + (MCAN_getMsgObjSize(MCAN_TX_BUFF_ELEM_SIZE) * 4U * (MCAN_TX_BUFF_SIZE + MCAN_TX_FQ_SIZE)))


#define    MASTER_STATION_INDEX_RESQUEST_ID        0x7A0 //指令请求帧ID
#define    MASTER_STATION_CHARACTER_RESQUEST_ID    0x650 //字符帧，未使用

#define    SLAVE_STATION_INDEX_RESPONSE_ID         0x7B0 //DSP返回帧ID
#define    SLAVE_STATION_CHARACTER_RESPONSE_ID     0x651 //DSP向上位机发送字符ID，未使用





/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported Variables
-----------------------------------------------------------------------------*/

/******************************************************************************
Inline functions
-----------------------------------------------------------------------------*/

/******************************************************************************
Exported function prototypes
-----------------------------------------------------------------------------*/
void CanFD_Init(void);
void CanFD_Busoff_recover(void);
void CanFD_Send_Msg(uint32_t id, uint16_t msg_type, uint16_t data_len, uint16_t* pbuf);
uint16_t CanFD_UDS_Write(uint32_t ID,uint16_t *Buf, uint16_t size);
void CanFD_Send_Test(void);
















#endif
