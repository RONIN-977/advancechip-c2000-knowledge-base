/******************************************************************************
*Copyright(C),2025,Hunan Advancechip Technology CO.,LTD.
*FileName   :dsp_base.c
*Author     :
*Version    :v1.0
*Date       :2025.7.2
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "can_config.h"

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
extern __interrupt void Can_ISR(void);
void Can_Mailbox_Init(void);
void Can_Interrupt_Init(void);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:CAN时钟源为SYSCLKOUT/2，即30MHz
*Input:
*Output:
-----------------------------------------------------------------------------*/
void CAN_Init(void)
{
    GPIO_setPinConfig(GPIO_4_CANA_TX);
    GPIO_setPinConfig(GPIO_5_CANA_RX);

    CAN_initModule(CANA_BASE);
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 500000, 20);

    Can_Interrupt_Init();

    Can_Mailbox_Init();
}

/******************************************************************************
*Description:配置收发邮箱，MSG 0~15为发送邮箱，MSG 16~31为接收邮箱
*            邮箱0：发送诊断报文0x7B0
*            邮箱16：接收诊断报文0x7A0
-----------------------------------------------------------------------------*/
void Can_Mailbox_Init(void)
{
    CAN_setupMessageObject(CANA_BASE, 1, 0x7B0, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0, CAN_MSG_OBJ_NO_FLAGS, 8);
    CAN_setupMessageObject(CANA_BASE, 16, 0x7A0, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_RX, 0, CAN_MSG_OBJ_RX_INT_ENABLE, 8);

    CAN_startModule(CANA_BASE);
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void Can_Interrupt_Init(void)
{
    CAN_enableInterrupt(CANA_BASE, CAN_INT_IE0 | CAN_INT_ERROR | CAN_INT_STATUS);

    Interrupt_register(INT_CANA0, &Can_ISR);
    Interrupt_enable(INT_CANA0);
    CAN_enableGlobalInterrupt(CANA_BASE, CAN_GLOBAL_INT_CANINT0);
}


/******************************************************************************
*Description:CANH、CANL短路，CAN节点自动关闭，需要手动查询清除
-----------------------------------------------------------------------------*/
void Can_Busoff_recover(void)
{

}

/******************************************************************************
*Description:CAN报文发送函数
-----------------------------------------------------------------------------*/
void CAN_Send_MSG(uint16_t mbid, uint32_t msgid, uint16_t *pdata)
{
    CAN_sendMessage(CANA_BASE, mbid, 8, pdata);
}

/******************************************************************************
*Description:给UDS接口使用
-----------------------------------------------------------------------------*/
uint16_t Can_UDS_Write(uint32_t ID,uint16_t *Buf, uint16_t size)
{
    CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, size, Buf);
}

/******************************************************************************
*Description:测试CAN发送功能
-----------------------------------------------------------------------------*/
void CAN_Send_Test(void)
{
    static uint16_t cnt = 0;
    uint16_t buf[4];

    cnt++;
    buf[0] = cnt;
    buf[1] = 0x2301;
    buf[2] = 0x6745;
    buf[3] = 0xAB89;

    CAN_Send_MSG(TX_MSG_OBJ_ID, 0x7B0, (uint16_t*)&buf);
}








/* End of file */
