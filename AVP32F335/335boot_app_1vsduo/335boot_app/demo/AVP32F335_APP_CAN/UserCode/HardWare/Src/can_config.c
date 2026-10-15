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
extern void Can_up_receive_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf);

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
extern __interrupt void Can_ISR0(void);
extern __interrupt void Can_ISR1(void);
void Can_Mainbox_Init(void);
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
    InitECanGpio();
	InitECan(); //eCAN模式，波特率500K，采样点80%
	Can_Mainbox_Init();
    Can_Interrupt_Init();
}

/******************************************************************************
*Description:配置收发邮箱，MSG 0~15为发送邮箱，MSG 16~31为接收邮箱
*            邮箱0：发送诊断报文0x7B0
*            邮箱16：接收诊断报文0x7A0
-----------------------------------------------------------------------------*/
void Can_Mainbox_Init(void)
{
    struct ECAN_REGS ECanaShadow;

    EALLOW;
    ECanaShadow.CANGAM.all = ECanaRegs.CANGAM.all;
	ECanaShadow.CANGAM.bit.AMI = 1; // 配置标准帧和扩展帧都允许接收
    ECanaRegs.CANGAM.all = ECanaShadow.CANGAM.all;
    EDIS;

    // Tx0
    ECanaMboxes.MBOX0.MSGID.bit.AME = 1; // 接收掩码使能位，接收邮箱才有效，即邮箱16~31
	//AAM：自动应答模式，只对发送邮箱有效
	//1：如果匹配到远程请求，通过发送邮箱的内容响应请求
	//0：不回复远程请求帧
	ECanaMboxes.MBOX0.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX0.MSGID.bit.IDE = 0; // 是否使用扩展帧标志位，0不使用，1使用
	ECanaMboxes.MBOX0.MSGID.bit.STDMSGID = 0x7B0; // ID标识位
	ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;//报文长度

	// Rx16
    ECanaMboxes.MBOX16.MSGID.bit.AME = 1;
    ECanaMboxes.MBOX16.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX16.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX16.MSGID.bit.STDMSGID = 0x7A0;
    ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
	//可以接收标准帧和扩展帧
	ECanaLAMRegs.LAM16.bit.LAMI = 1;
	//对于一个标准帧，只有前11位(位28到位18)使用标识符和本地接收掩码
	//因此该数据位于32:16，需要左移2位，ID最低位与18位对齐；相应位写0.要求该位匹配，相应位写1，则不匹配该位ID
	ECanaLAMRegs.LAM16.bit.LAM_H = (0x000)<<2;
    ECanaLAMRegs.LAM16.bit.LAM_L = 0;
	
	EALLOW;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.DBO = 1; // 字节顺序，0：MSB，1：LSB
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
	//配置邮箱功能：0发送，1接收，增强模式16~31为接收邮箱
	ECanaRegs.CANMD.bit.MD16 = 1;
	
	//使能需要用到的邮箱号，不使能的不能打开
	ECanaRegs.CANME.bit.ME0 = 1; //0关闭邮箱，1打开邮箱
	ECanaRegs.CANME.bit.ME16 = 1;
	
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.STM = 0;    // 0：正常模式，1：自检模式
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
	EDIS;
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
void Can_Interrupt_Init(void)
{
    struct ECAN_REGS ECanaShadow;

    EALLOW;
    PieVectTable.ECAN0INTA = &Can_ISR0;    // eCAN有两个中断向量，line 0，处理CAN总线错误中断
    PieVectTable.ECAN1INTA = &Can_ISR1;    // line 1，处理ID滤波后接收的CAN报文
    EDIS;

    EALLOW;
    ECanaShadow.CANMIL.all = ECanaRegs.CANMIL.all;
    ECanaShadow.CANMIL.all = 0xFFFFFFFF;            // 邮箱中断级别       邮箱号的中断全部绑定到line 1
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;

    // 邮箱中断使能
    ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all;
    ECanaShadow.CANMIM.bit.MIM16 = 1;
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

    // GIL = 0  即把所有的非邮箱类故障中断绑定到line0    邮箱中断级别
    ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all;
    ECanaShadow.CANGIM.bit.GIL =0;            //P47
//    ECanaShadow.CANGIM.bit.AAIM = 1;// 发送消息终止确认中断
//    ECanaShadow.CANGIM.bit.WDIM = 1; // 写拒绝中断掩码
    ECanaShadow.CANGIM.bit.WUIM = 1; // 唤醒中断掩码
    ECanaShadow.CANGIM.bit.BOIM = 1; // 总线关闭中断掩码    busoff出现时，这个故障会持续进中断
//    ECanaShadow.CANGIM.bit.EPIM = 1; // 错误被动中断掩码
//    ECanaShadow.CANGIM.bit.WLIM = 1; // 警告级中断掩码

    ECanaShadow.CANGIM.bit.I1EN = 1 ;  // ECAN1INT line 使能
    ECanaShadow.CANGIM.bit.I0EN = 1 ;  // ECAN0INT line 使能
    ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;

    ECanaRegs.CANTA.all = 0xFFFFFFFF;   /* Clear all TAn bits   发送成功标志位   写1清除标志   */
    ECanaRegs.CANRMP.all = 0xFFFFFFFF;  /* Clear all RMPn bits   接收挂起标志位   写1清除标志   */
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /* Clear all interrupt flag bits  全局中断标志位   写1清除标志   */
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;
    IER |= M_INT9;

    EDIS;
}


/******************************************************************************
*Description:CANH、CANL短路，CAN节点自动关闭，需要手动查询清除
-----------------------------------------------------------------------------*/
void Can_Busoff_recover(void)
{
    Uint16 data[8];
    static Uint32 timeout = 0xFFFF;
    // BOF中断标志在中断服务被清除，   通过检测CCE被置位的情况恢复CAN总线
    if(ECanaRegs.CANES.bit.CCE == 0x01)
    {
        struct ECAN_REGS ECanaShadow;
        timeout = 0xFFFFF;
        EALLOW;

        ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
        ECanaShadow.CANMC.bit.CCR = 0 ;            // 在配置CANBTC寄存器后，请求eCAN正常工作
        ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

        do
        {
            timeout--;
            ECanaShadow.CANES.all = ECanaRegs.CANES.all;
        } while((ECanaShadow.CANES.bit.CCE != 0) || (timeout == 0));       // Wait for CCE bit to be  cleared..
        EDIS;
    }

    //解决跳到APP没有进入中断的情况
    if((ECanaRegs.CANGIF1.bit.GMIF1 == 1) && (ECanaRegs.CANRMP.bit.RMP16 == 1))
    {
        data[0] = ECanaMboxes.MBOX16.MDL.byte.BYTE3;
        data[1] = ECanaMboxes.MBOX16.MDL.byte.BYTE2;
        data[2] = ECanaMboxes.MBOX16.MDL.byte.BYTE1;
        data[3] = ECanaMboxes.MBOX16.MDL.byte.BYTE0;

        data[4] = ECanaMboxes.MBOX16.MDH.byte.BYTE7;
        data[5] = ECanaMboxes.MBOX16.MDH.byte.BYTE6;
        data[6] = ECanaMboxes.MBOX16.MDH.byte.BYTE5;
        data[7] = ECanaMboxes.MBOX16.MDH.byte.BYTE4;

        //把数据存储到消息队列
        Can_up_receive_write(0x7A0, 0, (Uint16*)&data);

        ECanaRegs.CANRMP.bit.RMP16 = 1; //写1清除邮箱接收中断标志位

        PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
    }
}

/******************************************************************************
*Description:CAN报文发送函数
*Input:mbid:发送邮箱号
*      msgid:CAN报文ID号
*      *pdata:数据指针，DA: data[1]<<8 | data[0]
*                    DB: data[3]<<8 | data[2]
*                    DC: data[5]<<8 | data[4]
*                    DD: data[7]<<8 | data[6]
*                    对于数组排列，DA的低八位数据在第0个下标，DA的高八位数据在第1个下标
-----------------------------------------------------------------------------*/
void CAN_Send_MSG(Uint16 mbid, Uint32 msgid, Uint16 *pdata)
{
    static Uint32 lastSendMBox = 0;
    volatile struct MBOX *Mailbox;
    Uint32 CANData,NowMailBox;
    Uint32 CANTaFlag = 1;

    //直接清空上一次的发送标志位，忽略是否发送成功
    NowMailBox = CANTaFlag << mbid;
    ECanaRegs.CANTA.all |= lastSendMBox;//发送完成标志位不会自己清除吗?

    //选定发送邮箱，只用低16个邮箱发送，不需要判断
    //if(CANTx->MBXnbr < 16)
    //{
        Mailbox = &ECanaMboxes.MBOX0 + mbid;
    //}
    //else
    //{
    //    Mailbox = &ECanaMboxes.MBOX16 + (CANTx->MBXnbr-16);
    //}

    //装入数据，高16位在DB，低16位在DA
    CANData = pdata[1];
    CANData<<=16;
    CANData += pdata[0];
    Mailbox->MDL.all = CANData;

    CANData = pdata[3];
    CANData<<=16;
    CANData += pdata[2];
    Mailbox->MDH.all = CANData;

    //使能发送
    ECanaRegs.CANTRS.all |= NowMailBox;
    //记录上次发送的邮箱号
    lastSendMBox = NowMailBox;
}

/******************************************************************************
*Description:给UDS接口使用
-----------------------------------------------------------------------------*/
Uint16 Can_UDS_Write(Uint32 ID,Uint16 *Buf, Uint16 size)
{
    Uint16 temp[4];

    temp[0] = Buf[1]<<8 | Buf[0];
    temp[1] = Buf[3]<<8 | Buf[2];
    temp[2] = Buf[5]<<8 | Buf[4];
    temp[3] = Buf[7]<<8 | Buf[6];

    CAN_Send_MSG(0, ID, &temp);//UDS发送固定使用0号邮箱
}

/******************************************************************************
*Description:测试CAN发送功能
-----------------------------------------------------------------------------*/
void CAN_Send_Test(void)
{
    static Uint16 cnt = 0;
    Uint16 buf[4];

    cnt++;
    buf[0] = cnt;
    buf[1] = 0x2301;
    buf[2] = 0x6745;
    buf[3] = 0xAB89;

    CAN_Send_MSG(0, 0x7B0, (Uint16*)&buf);
}








/* End of file */
