/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :can_config.c
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
MCAN_TxBufElement canfdtxMsg;//将发送块数据放到全局，避免占用栈空间

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern __interrupt void CanFD_ISR(void);
void CanFD_Mailbox_Init(void);
void CanFD_Interrupt_Init(void);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:CAN时钟源为SYSCLKOUT/3，即40MHz
*Input:
*Output:
-----------------------------------------------------------------------------*/
void CanFD_Init(void)
{
    MCAN_InitParams initParams;
    MCAN_MsgRAMConfigParams    msgRAMConfigParams;
    MCAN_StdMsgIDFilterElement stdFiltelem;
    MCAN_BitTimingParams       bitTimes;
    MCAN_ExtMsgIDFilterElement extFiltelem;


    GPIO_setPinConfig(GPIO_4_MCAN_TX);
    GPIO_setPinConfig(GPIO_5_MCAN_RX);

    SysCtl_setMCANClk(SYSCTL_MCANCLK_DIV_3);//120/3 = 40MHz

    initParams.fdMode            = 0x1U; // FD operation enabled.
    initParams.brsEnable         = 0x1U; // Bit rate switching for transmissions enabled.

    // Transmitter Delay Compensation parameters.
    initParams.tdcConfig.tdcf    = 0xAU;
    initParams.tdcConfig.tdco    = 0x6U;
    initParams.emulationEnable   = 0x01;

    msgRAMConfigParams.txStartAddr = MCAN_TX_BUFF_START_ADDR;// Tx Buffers Start Address.
    msgRAMConfigParams.txBufNum    = MCAN_TX_BUFF_SIZE;// Number of Dedicated Transmit Buffers.
    msgRAMConfigParams.txBufMode   = 0U;
    msgRAMConfigParams.txFIFOSize  = MCAN_TX_FQ_SIZE;// Number of Tx FIFO or Tx Queue Elements
    msgRAMConfigParams.txBufElemSize = MCAN_TX_BUFF_ELEM_SIZE;// Tx Buffer Element Size.

    msgRAMConfigParams.flssa          = MCAN_STD_ID_FILT_START_ADDR;
    msgRAMConfigParams.lss            = MCAN_STD_ID_FILTER_NUM;
    msgRAMConfigParams.rxBufStartAddr = MCAN_RX_BUFF_START_ADDR;
    msgRAMConfigParams.rxBufElemSize  = MCAN_RX_BUFF_ELEM_SIZE;

    stdFiltelem.sfid2              = 0x0U; // Standard ID Filter Mask. A 0 in any bit-position indicates "don't care"
    stdFiltelem.sfid1              = 0x7A0U; // Standard ID Filter
    stdFiltelem.sfec               = 0x7U; // Store into Rx Buffer
//    stdFiltelem.sft                = 0x2U; // SFID1 = filter; SFID2 = mask

    // (40MHz/(3+1)) / (15+1 + 2+1 + 1) = 0.5MHz = 500KHz
    bitTimes.nomRatePrescalar   = 3U; // Nominal Baud Rate Pre-scaler
    bitTimes.nomTimeSeg1        = 15U; // Nominal Time segment before SP
    bitTimes.nomTimeSeg2        = 2U; // Nominal Time segment after SP
    bitTimes.nomSynchJumpWidth  = 1U; // Nominal SJW

    // (40MHz/(1+1)) / (6+1 + 1+1 + 1) = 2MHz
    bitTimes.dataRatePrescalar  = 1U; // Data Baud Rate Pre-scaler
    bitTimes.dataTimeSeg1       = 6U; // Data Time segment before SP
    bitTimes.dataTimeSeg2       = 1U; // Data Time segment after SP
    bitTimes.dataSynchJumpWidth = 1U; // Data SJW

    // Wait for memory initialization to happen.
    while(FALSE == MCAN_isMemInitDone(MCANA_DRIVER_BASE))
    {
    }

    MCAN_setOpMode(MCANA_DRIVER_BASE, MCAN_OPERATION_MODE_SW_INIT);

    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(MCANA_DRIVER_BASE))
    {}

    MCAN_init(MCANA_DRIVER_BASE, &initParams);
    MCAN_setBitTime(MCANA_DRIVER_BASE, &bitTimes);
    MCAN_msgRAMConfig(MCANA_DRIVER_BASE, &msgRAMConfigParams);
    MCAN_addStdMsgIDFilter(MCANA_DRIVER_BASE, 0U, &stdFiltelem);

    MCAN_setOpMode(MCANA_DRIVER_BASE, MCAN_OPERATION_MODE_NORMAL);
    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(MCANA_DRIVER_BASE))
    {
    }

    //配置接收中断
    MCAN_enableIntr(MCANA_DRIVER_BASE, MCAN_INTR_MASK_ALL, TRUE);//使能所有的中断
    MCAN_selectIntrLine(MCANA_DRIVER_BASE, MCAN_INTR_MASK_ALL, MCAN_INTR_LINE_NUM_1);
    MCAN_enableIntrLine(MCANA_DRIVER_BASE, MCAN_INTR_LINE_NUM_1, TRUE);

    Interrupt_register(INT_MCANA_1, &CanFD_ISR);
    Interrupt_enable(INT_MCANA_1);
}

/******************************************************************************
*Description:CANH、CANL短路，CAN节点自动关闭，需要手动查询清除
-----------------------------------------------------------------------------*/
void CanFD_Busoff_recover(void)
{

}

/******************************************************************************
*Description:CANFD报文发送函数  msg_type : 0-标准帧ID  1-扩展帧ID
-----------------------------------------------------------------------------*/
void CanFD_Send_Msg(uint32_t id, uint16_t msg_type, uint16_t data_len, uint16_t* pbuf)
{
    uint16_t msg_dlc = 0;
    uint16_t i;

    if(data_len <= 8)
    {
        msg_dlc = 8;
    }
    else if((data_len <= 12) && (data_len > 8))
    {
        msg_dlc = 9;
    }
    else if((data_len <= 16) && (data_len > 12))
    {
        msg_dlc = 10;
    }
    else if((data_len <= 20) && (data_len > 16))
    {
        msg_dlc = 11;
    }
    else if((data_len <= 24) && (data_len > 20))
    {
        msg_dlc = 12;
    }
    else if((data_len <= 32) && (data_len > 24))
    {
        msg_dlc = 13;
    }
    else if((data_len <= 48) && (data_len > 32))
    {
        msg_dlc = 14;
    }
    else
    {
        msg_dlc = 15;
    }

    if(msg_type == 0)
    {
        canfdtxMsg.id = id << 18U; //标准帧ID
    }
    else
    {
        canfdtxMsg.id = id; //扩展帧ID
    }
    canfdtxMsg.rtr   = 0U;// RTR = 0 (Data frame)
    canfdtxMsg.xtd   = msg_type;// XTD = 0 (11-bit standard identifier)
    canfdtxMsg.esi   = 0U;
    canfdtxMsg.dlc   = msg_dlc;
    canfdtxMsg.brs   = 1U;// Bit-rate switching enabled
    canfdtxMsg.fdf   = 1U;// Frame transmitted in CAN FD format
    canfdtxMsg.efc   = 1U;// Store TX events
    canfdtxMsg.mm    = 0xAAU;

    for(i=0; i<64; i++)
    {
        if(i<data_len)
        {
            canfdtxMsg.data[i] = pbuf[i];
        }
        else
        {
            canfdtxMsg.data[i] = 0x0000;//没有发送的长度要清空
        }
    }

    MCAN_writeMsgRam(MCANA_DRIVER_BASE, MCAN_MEM_TYPE_BUF, 0, &canfdtxMsg);
    MCAN_txBufAddReq(MCANA_DRIVER_BASE, 0U);
}

/******************************************************************************
*Description:给UDS接口使用
-----------------------------------------------------------------------------*/
uint16_t CanFD_UDS_Write(uint32_t ID,uint16_t *Buf, uint16_t size)
{
    CanFD_Send_Msg(0x7B0, 0, size, Buf);
}

/******************************************************************************
*Description:测试CAN发送功能
-----------------------------------------------------------------------------*/
void CanFD_Send_Test(void)
{
    uint16_t msg_data[64];//在栈空间申请内存，是否考虑溢出?
    uint16_t i;
    static uint16_t tick = 0;

    msg_data[0] = tick >> 8;
    msg_data[1] = tick & 0x00FF;

    for(i=2; i<64; i++)
    {
        if(i < 32)
            msg_data[i] = 0xAA;
        else
            msg_data[i] = 0xCC;
    }

    CanFD_Send_Msg(0x300, 0, 64, (uint16_t*)&msg_data);

    tick++;
    if(tick >= 0xFFFF)
    {
        tick = 0;
    }
}








/* End of file */
