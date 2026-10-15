#include "CANcomm.h"
#include "Main.h"
#include "Parameter.h"
#include "MotorInclude.h"

CanRecv_MsgData_Struct     CanRecv_MsgData_Buffer;
CanMailBox_MsgData_Struct  CanMailBox_MsgData;
Mailbox_ReadyBits          CanMailBoxReadyFlag;

typedef struct _TEST_CANMSG_MDL {
    Uint16 BYTE0:8;
    Uint16 BYTE1:8;
    Uint16 BYTE2:8;
    Uint16 BYTE3:8;
}TEST_CANMSG_MDL;

typedef struct _TEST_CANMAS_MDH {
    Uint16 BYTE4:8;
    Uint16 BYTE5:8;
    Uint16 BYTE6:8;
    Uint16 BYTE7:8;
}TEST_CANMSG_MDH;

TEST_CANMSG_MDL TEST_MDL;
TEST_CANMSG_MDH TEST_MDH;

unsigned int reserve;
unsigned int RealTimeDataSendFlag = 0;
extern unsigned int RunSignal;
const unsigned int  *VarAddr[28] =
{
    &RunSignal,                          //0  运行命令
    (unsigned int *)&RefSet,             //1  使能系数状态数据上传
    &reserve,                            //2  保留
    &pm_control_mode,                    //3  控制方式(VF/矢量)
    (unsigned int *)&LOW_SPEED_KP,       //4  低速段速度环Kp
    (unsigned int *)&LOW_SPEED_KI,       //5  低速段速度环Ki
    (unsigned int *)&HIGH_SPEED_KP_GAIN, //6  高速段速度环Kp
    (unsigned int *)&DCStart_flag,       //7  启动方式
    (unsigned int *)&HIGH_SPEED_KI_GAIN, //8  高速段速度环Ki
    (unsigned int *)&D_CUR_KP,           //9  d轴电流环Kp
    (unsigned int *)&D_CUR_KI,           //10 d轴电流环Ki
    (unsigned int *)&gFluxWeak.Mode,     //11 弱磁模式
    (unsigned int *)&Q_CUR_KP,           //12 q轴电流环Kp
    (unsigned int *)&Q_CUR_KI,           //13 q轴电流环Ki
    &FIELD_WEAK_REF,                     //14 弱磁点
    &reserve,                            //15 控制模式(速度模式/转矩模式)
    &F_CHANGING_POINT_1_PI,              //16 频率切换点1
    &F_CHANGING_POINT_2_PI,              //17 频率切换点2
    &SOFT_OC_POINT,                      //18 软件过流点
    &reserve,                            //19 目标转矩
    &OVER_DC_VOL_POINT,                  //20 过压点
    &UNDER_DC_VOL_POINT,                 //21 欠压点
    &MOTOR_RATED_VOL,                    //22 额定电压
    (unsigned int *)&TORQUE_CUR_LIMIT,   //23 限流点
    &reserve,                            //24 电流采样系数
    &reserve,                            //25 电压采样系数
    &MOTOR_RATED_CUR,                    //26 额定电流
    &RealTimeDataSendFlag                //27 实时数据监控使能
};

// CAN模块系统中断
interrupt void  CanSystem_IsrSrv(void) {
    struct ECAN_REGS    mCanCtrlStatusShadow;
    Uint16  IntMailNo = 0x00;

    DINT;
    mCanCtrlStatusShadow.CANGIF0.all = ECanaRegs.CANGIF0.all;
    IntMailNo = mCanCtrlStatusShadow.CANGIF0.bit.MIV0;

    if (mCanCtrlStatusShadow.CANGIF0.bit.AAIF0 == 0x1) {
        mCanCtrlStatusShadow.CANAA.all = ECanaRegs.CANAA.all;
        mCanCtrlStatusShadow.CANAA.all |= (1 << IntMailNo);
        ECanaRegs.CANAA.all = mCanCtrlStatusShadow.CANAA.all;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.RMLIF0 == 0x01) {
        mCanCtrlStatusShadow.CANRMP.all = ECanaRegs.CANRMP.all;
        mCanCtrlStatusShadow.CANRMP.all |= (1 << IntMailNo);
        ECanaRegs.CANRMP.all = mCanCtrlStatusShadow.CANRMP.all;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.WDIF0 == 0x1) {
        mCanCtrlStatusShadow.CANGIF0.bit.WDIF0 = 1;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.WUIF0 == 0x1) {
        mCanCtrlStatusShadow.CANGIF0.bit.WUIF0 = 1;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.BOIF0 == 0x1) {
//        mCanCtrlStatusShadow.CANGIF0.bit.BOIF0 = 1;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.EPIF0 == 0x1) {
        mCanCtrlStatusShadow.CANGIF0.bit.EPIF0 = 1;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.WLIF0 == 0x1) {
        mCanCtrlStatusShadow.CANGIF0.bit.WLIF0 = 1;
    } else if (mCanCtrlStatusShadow.CANGIF0.bit.TCOF0 == 0x1) {
        mCanCtrlStatusShadow.CANGIF0.bit.TCOF0 = 1;
    } else {

    }
    ECanaRegs.CANGIF0.all = mCanCtrlStatusShadow.CANGIF0.all;
    EINT;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN模块邮箱中断
interrupt  void CanMailBox_IsrSrv(void)
{
    struct ECAN_REGS mCanCtrlStatusShadow;
    struct MBOX *pBoxEntry;
    Uint16 IntMailNo;

    mCanCtrlStatusShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;

    IntMailNo = mCanCtrlStatusShadow.CANGIF1.bit.MIV1;
    pBoxEntry = (struct MBOX *)&ECanaMboxes.MBOX0 + IntMailNo;

    CanMailBoxReadyFlag.all  |= ECanaRegs.CANTA.all;

    mCanCtrlStatusShadow.CANTA.all = ECanaRegs.CANTA.all;
    ECanaRegs.CANTA.all = mCanCtrlStatusShadow.CANTA.all;

    if(mCanCtrlStatusShadow.CANGIF1.bit.GMIF1 == 0x1)
    {
        mCanCtrlStatusShadow.CANRMP.all = ECanaRegs.CANRMP.all;
		mCanCtrlStatusShadow.CANTA.all   = ECanaRegs.CANTA.all;
        if(((mCanCtrlStatusShadow.CANRMP.all >> IntMailNo) & 1) == 0x1)
        {
            CanMsgData_Push(pBoxEntry);
            mCanCtrlStatusShadow.CANRMP.all |= (1 << IntMailNo);
            ECanaRegs.CANRMP.all = mCanCtrlStatusShadow.CANRMP.all;
        }
        else if(((mCanCtrlStatusShadow.CANTA.all >> IntMailNo) & 1) == 0x1)
        {
            //mCanCtrlStatusShadow.CANTA.all = ECanaRegs.CANTA.all;
            //mCanCtrlStatusShadow.CANTA.all |= (1 << IntMailNo);
            //ECanaRegs.CANTA.all = mCanCtrlStatusShadow.CANTA.all;
        }
    }
    else if (mCanCtrlStatusShadow.CANGIF1.bit.MTOF1 == 0x1)
    {
        mCanCtrlStatusShadow.CANTOS.all = ECanaRegs.CANTOS.all;
        mCanCtrlStatusShadow.CANTOS.all |= (1 << IntMailNo);
        ECanaRegs.CANTOS.all = mCanCtrlStatusShadow.CANTOS.all;
    }
    ECanaRegs.CANGIF1.all = mCanCtrlStatusShadow.CANGIF1.all;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// CAN模块IO复用
void InitECanaGpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO0  = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;

    GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 3;

    GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 1;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;
    EDIS;
}

void CANTransceiverEnable(void)
{
    EALLOW;
    // CAN_EN
    GpioCtrlRegs.GPAPUD.bit.GPIO1  = 0;  // Enable pullup on GPIO18
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;  // GPIO18 = GPIO18
    GpioCtrlRegs.GPADIR.bit.GPIO1  = 1;  // GPIO18 = output

    // CAN_STB
    GpioCtrlRegs.GPBPUD.bit.GPIO34  = 0;  // Enable pullup on GPIO19
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;  // GPIO19 = GPIO19
    GpioCtrlRegs.GPBDIR.bit.GPIO34  = 1;  // GPIO19 = output
    EDIS;

    // Enable
    GpioDataRegs.GPASET.bit.GPIO1  = 1;
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;
}

// CAN模块初始化
void InitECana_1(void)
{
    struct ECAN_REGS ECanaShadow;
    struct MBOX *TmpMailbox;
    int   iLoop = 0x00;

    EALLOW;
    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SCB = 1;
    ECanaShadow.CANMC.bit.ABO = 0;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaRegs.CANME.all = 0x00;

    TmpMailbox = (struct MBOX *)&ECanaMboxes.MBOX0;
    for (iLoop = 0x00; iLoop < MAILBOX_LENGTH; iLoop++)
    {
        TmpMailbox = TmpMailbox + iLoop;
        TmpMailbox->MSGCTRL.all = 0x00;
    }

    ECanaRegs.CANTA.all   = 0xFFFFFFFF;
    ECanaRegs.CANRMP.all  = 0xFFFFFFFF;
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 1 ;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1);

    ECanaShadow.CANBTC.all = 0;

#ifdef CAN_BAUD_250K
    //250kps, sample point  80%
    ECanaShadow.CANBTC.bit.BRPREG = 9;
    ECanaShadow.CANBTC.bit.SJWREG = 3;
    ECanaShadow.CANBTC.bit.TSEG2REG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 14;
#endif

#ifdef CAN_BAUD_500K
    //500kps, sample point  80%
    ECanaShadow.CANBTC.bit.BRPREG = 4;
    ECanaShadow.CANBTC.bit.SJWREG = 3;
    ECanaShadow.CANBTC.bit.TSEG2REG = 3;
    ECanaShadow.CANBTC.bit.TSEG1REG = 14;
#endif

    ECanaShadow.CANBTC.bit.SAM = 0;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );
    ECanaRegs.CANME.all = 0;
    EDIS;
}

void CAN_Init_Interrupt(void)
{
    struct ECAN_REGS ECanaShadow;

    EALLOW;

    //PieVectTable.ECAN0INTA= &CanSystem_IsrSrv;
    PieVectTable.ECAN1INTA= &CanMailBox_IsrSrv;
    EDIS;

    EALLOW;
    ECanaShadow.CANMIL.all = ECanaRegs.CANMIL.all;
    ECanaShadow.CANMIL.all = 0xFFFFFFFF;
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;


    ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all;
    ECanaShadow.CANMIM.all = 0xFFFFFFFF;
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

    ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all;
    ECanaShadow.CANGIM.all =0XFFFFFFFF;
    ECanaShadow.CANGIM.bit.GIL = 0;
    ECanaShadow.CANGIM.bit.BOIM = 1;
    ECanaShadow.CANGIM.bit.EPIM = 1;
    ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;

    //PieCtrlRegs.PIEIER9.bit.INTx5 = 1;      // Enable INT 9.5 in the PIE
    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;      // Enable INT 9.6 in the PIE

    EDIS;
}
//
void CANInit()
{
    struct  ECAN_REGS  ECanaShadow;

    InitECanaGpio();
    InitECana_1();

    //发送邮箱设置
    ECanaMboxes.MBOX3.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX3.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX3.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX3.MSGID.bit.STDMSGID = 0x492;

    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX3.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX3.MSGCTRL.bit.TPL = 2;

    ECanaMboxes.MBOX31.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX31.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX31.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX31.MSGID.bit.STDMSGID = 0x481;

    ECanaMboxes.MBOX31.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX31.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX31.MSGCTRL.bit.TPL = 3;

    ECanaMboxes.MBOX30.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX30.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX30.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX30.MSGID.bit.STDMSGID = 0x482;

    ECanaMboxes.MBOX30.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX30.MSGCTRL.bit.RTR = 0;
    ECanaMboxes.MBOX30.MSGCTRL.bit.TPL = 3;

    //接收邮箱设置
    ECanaMboxes.MBOX12.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX12.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX12.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX12.MSGID.bit.STDMSGID = 0x491;

    ECanaMboxes.MBOX15.MSGID.bit.AAM = 0;
    ECanaMboxes.MBOX15.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX15.MSGID.bit.IDE  = 1;
    ECanaMboxes.MBOX15.MSGID.bit.STDMSGID   = (0x1F1F1F1F >> 18) & 0x7FF;
    ECanaMboxes.MBOX15.MSGID.bit.EXTMSGID_H = (0x1F1F1F1F >> 16) & 0x3;
    ECanaMboxes.MBOX15.MSGID.bit.EXTMSGID_L = (0x1F1F1F1F & 0xFFFF);


    ECanaMboxes.MBOX16.MSGID.bit.AAM = 1;
    ECanaMboxes.MBOX16.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX16.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX16.MSGID.bit.STDMSGID = 0x181;

    ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 1;
    ECanaMboxes.MBOX16.MSGCTRL.bit.TPL = 0;

    ECanaMboxes.MBOX17.MSGID.bit.AAM = 1;
    ECanaMboxes.MBOX17.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX17.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX17.MSGID.bit.STDMSGID = 0x281;

    ECanaMboxes.MBOX17.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX17.MSGCTRL.bit.RTR = 1;
    ECanaMboxes.MBOX17.MSGCTRL.bit.TPL = 0;

    ECanaMboxes.MBOX18.MSGID.bit.AAM = 1;
    ECanaMboxes.MBOX18.MSGID.bit.AME = 0;
    ECanaMboxes.MBOX18.MSGID.bit.IDE  = 0;
    ECanaMboxes.MBOX18.MSGID.bit.STDMSGID = 0x381;

    ECanaMboxes.MBOX18.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX18.MSGCTRL.bit.RTR = 1;
    ECanaMboxes.MBOX18.MSGCTRL.bit.TPL = 0;
 
    ECanaShadow.CANME.all = 0xC0079008;
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    ECanaShadow.CANMD.all = 0x00009000;
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    CanMailBoxReadyFlag.all = 0xFFFFFFFF;

    CAN_Init_Interrupt();  //配置CAN 接收中断
    Init_CanMsgData();
}

void Init_CanMsgData(void)
{
    CanRecv_MsgData_Buffer.mCount = 0x00;
    CanRecv_MsgData_Buffer.mHeader = 0x00;
    CanRecv_MsgData_Buffer.mTailer = 0x00;
}

void CanMsgData_Push(struct  MBOX *msgBoxData)
{
    if(CanRecv_MsgData_Buffer.mCount < CANRECV_BOXDATA_LENGTH)
    {
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].mMsgId = msgBoxData->MSGID.all;
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].mMsgCtrl = msgBoxData->MSGCTRL.all;
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].CanDataLow = msgBoxData->MDL.all;
        CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].CanDataHigh = msgBoxData->MDH.all;

        CanRecv_MsgData_Buffer.mTailer = CanRecv_MsgData_Buffer.mTailer + 1;
        if(CanRecv_MsgData_Buffer.mTailer >= CANRECV_BOXDATA_LENGTH)
        {
            CanRecv_MsgData_Buffer.mTailer = 0;
        }
        CanRecv_MsgData_Buffer.mCount = CanRecv_MsgData_Buffer.mCount + 1;
    }
}

CanMsgData_Entry* CanMsgData_Pop(void)
{
    CanMsgData_Entry *pMsgEntry;
    if (CanRecv_MsgData_Buffer.mCount <= 0x00)
    {
        return (CanMsgData_Entry *)0x00;
    }
    pMsgEntry = (CanMsgData_Entry *)&(CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mHeader]);
    CanRecv_MsgData_Buffer.mHeader = CanRecv_MsgData_Buffer.mHeader + 1;
    if(CanRecv_MsgData_Buffer.mHeader >= CANRECV_BOXDATA_LENGTH)
    {
        CanRecv_MsgData_Buffer.mHeader = 0;
    }
    CanRecv_MsgData_Buffer.mCount = (CanRecv_MsgData_Buffer.mCount - 1);
    return pMsgEntry;
}
CanMsgData_Entry *mCanMsgEntry;
void CanTransmit2ms(void)
{
//    CanMsgData_Entry *mCanMsgEntry;
    struct ECAN_REGS  ECanaShadow;
    struct MBOX RxCanMassage;
    struct MBOX TxCanMassage;
    Uint16 Addr, Data;

    if( ECanaRegs.CANGIF0.bit.BOIF0 == 1)
    {
        ECanaRegs.CANTRR.bit.TRR1 = 1;
        ECanaRegs.CANTRR.bit.TRR2 = 1;
        ECanaRegs.CANGIF0.bit.BOIF0 = 1;

        EALLOW;
        ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
        ECanaShadow.CANMC.bit.CCR = 0;
        //ECanaShadow.CANMC.bit.SRES = 1;
        EDIS;
    }

    if (CanRecv_MsgData_Buffer.mCount <= 0 )
    {
        return;
    }
    mCanMsgEntry = CanMsgData_Pop();

    RxCanMassage.MSGID.all   = mCanMsgEntry->mMsgId;
    RxCanMassage.MDL.all = mCanMsgEntry->CanDataLow;
    RxCanMassage.MDH.all = mCanMsgEntry->CanDataHigh;

    Addr = ((Uint16)RxCanMassage.MDL.byte.BYTE1<<8) + (RxCanMassage.MDL.byte.BYTE2);
    Data = ((Uint16)RxCanMassage.MDL.byte.BYTE3<<8) + (RxCanMassage.MDH.byte.BYTE4);

    TxCanMassage.MDL.all = 0x00000000;
    TxCanMassage.MDH.all = 0x00000000;
    if(RxCanMassage.MSGID.bit.STDMSGID == 0x491)
    {
        if(RxCanMassage.MDL.byte.BYTE0 == 0x5A)    //读
        {
            TxCanMassage.MDL.byte.BYTE0 = 0x5A;
            TxCanMassage.MDL.byte.BYTE1  = RxCanMassage.MDL.byte.BYTE1;
            TxCanMassage.MDL.byte.BYTE2  = RxCanMassage.MDL.byte.BYTE2;
            if(Addr < 28)
            {
                TxCanMassage.MDL.byte.BYTE3 = *VarAddr[Addr]>>8;
                TxCanMassage.MDH.byte.BYTE4 = *VarAddr[Addr]&0xFF;
            }
            else
            {
                TxCanMassage.MDH.word.LOW_WORD = 0;
            }
        }
        else if(RxCanMassage.MDL.byte.BYTE0 == 0xA5)//写
        {
            TxCanMassage.MDL.byte.BYTE0 = 0xA5;
            TxCanMassage.MDL.byte.BYTE1  = RxCanMassage.MDL.byte.BYTE1;
            TxCanMassage.MDL.byte.BYTE2  = RxCanMassage.MDL.byte.BYTE2;
            if(Addr < 28)
            {
                *(Uint16 *)VarAddr[Addr] = Data;
                TxCanMassage.MDH.byte.BYTE4 = 0;
            }
            else
            {
                TxCanMassage.MDH.byte.BYTE4 = 1;
            }
        }
        ECanaMboxes.MBOX3.MDL.all = TxCanMassage.MDL.all;
        ECanaMboxes.MBOX3.MDH.all = TxCanMassage.MDH.all;

        if(CanMailBoxReadyFlag.bit.Mailbox3 == 1)
        {
            ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
            ECanaShadow.CANTRS.bit.TRS3 = 1;
            ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

            CanMailBoxReadyFlag.bit.Mailbox3 = 0;
        }
    }
    else //if(RxCanMassage.MSGID.bit.STDMSGID == )
    {
        if((ECanaMboxes.MBOX15.MDL.all == 0x00AA00AA) && (ECanaMboxes.MBOX15.MDH.all == 0x00AA00AA))
        {
            DisableDrive();
            EPwm1Regs.TZFRC.bit.OST = 1;
            EPwm2Regs.TZFRC.bit.OST = 1;
            EPwm3Regs.TZFRC.bit.OST = 1;
            DINT;
            asm(" MOVL XAR7, #0x3F7FF6 ");
            asm(" LB *XAR7 ");
        }
    }
}

long pm_est_omg_show = 0;
void CanRemoteAckDataFill(void)
{
    struct MBOX TxCanMassage;
    struct ECAN_REGS  ECanaShadow;
    Uint16 TempData;
    Uint16 CHData1, CHData2, CHData3, CHData4;
    Uint32 lCHData1,lCHData2;

    TxCanMassage.MDL.byte.BYTE0 = gMainCmd.Command.bit.Start;
    TxCanMassage.MDL.byte.BYTE1 = RunSignal;
    TxCanMassage.MDL.byte.BYTE2 = (gUDC.uDCFilter >> 8) & 0xFF;
    TxCanMassage.MDL.byte.BYTE3 = gUDC.uDCFilter & 0xFF;

    pm_est_omg_show = pm_est_omg_show - (pm_est_omg_show>>4) + (pm_est_omg>>4);
    TempData = pm_est_omg_show/1600;
    TxCanMassage.MDH.byte.BYTE4 = (RefSet >> 8) & 0xFF;
    TxCanMassage.MDH.byte.BYTE5 = RefSet & 0xFF;
    TxCanMassage.MDH.byte.BYTE6 = (TempData >> 8) & 0xFF;
    TxCanMassage.MDH.byte.BYTE7 = TempData & 0xFF;

    ECanaMboxes.MBOX16.MDL.all = TxCanMassage.MDL.all;
    ECanaMboxes.MBOX16.MDH.all = TxCanMassage.MDH.all;

    TempData = ((long)gLineCur.CurPer * MOTOR_RATED_CUR) >> 12;
    TxCanMassage.MDL.byte.BYTE0 = (errorCodeFromCore>>8) & 0xFF;
    TxCanMassage.MDL.byte.BYTE1 = errorCodeFromCore & 0xFF;
    TxCanMassage.MDL.byte.BYTE2 = (TempData >> 8) & 0xFF;
    TxCanMassage.MDL.byte.BYTE3 = TempData & 0xFF;

    TxCanMassage.MDH.byte.BYTE4 = (pm_ref_id >> 8) & 0xFF;
    TxCanMassage.MDH.byte.BYTE5 = pm_ref_id & 0xFF;
    TxCanMassage.MDH.byte.BYTE6 = (pm_ref_iq >> 8) & 0xFF;
    TxCanMassage.MDH.byte.BYTE7 = pm_ref_iq & 0xFF;

    ECanaMboxes.MBOX17.MDL.all = TxCanMassage.MDL.all;
    ECanaMboxes.MBOX17.MDH.all = TxCanMassage.MDH.all;

    TxCanMassage.MDL.byte.BYTE0 = (gTemperature.Temp >> 8) & 0xFF;
    TxCanMassage.MDL.byte.BYTE1 = gTemperature.Temp & 0xFF;
    TxCanMassage.MDL.byte.BYTE2 = (gRatio >> 8) & 0xff;
    TxCanMassage.MDL.byte.BYTE3 = gRatio & 0xFF;

    TxCanMassage.MDH.byte.BYTE4 = 0;
    TxCanMassage.MDH.byte.BYTE5 = 0;
    TxCanMassage.MDH.byte.BYTE6 = 0;
    TxCanMassage.MDH.byte.BYTE7 = 0;

    ECanaMboxes.MBOX18.MDL.all = TxCanMassage.MDL.all;
    ECanaMboxes.MBOX18.MDH.all = TxCanMassage.MDH.all;

//    if(RealTimeDataSendFlag == 1)
    if (1)
    {
        if((CanMailBoxReadyFlag.bit.Mailbox31 == 1)&&(CanMailBoxReadyFlag.bit.Mailbox30 == 1))
        {
            CHData1 = pm_ref_iq;
            CHData2 = gIMT.T;
            CHData3 = pm_ref_id;
            CHData4 = gIMT.M;

            TxCanMassage.MDL.byte.BYTE3 = (CHData2 >> 8) & 0xFF;
            TxCanMassage.MDL.byte.BYTE2 = CHData2 & 0xFF;
            TxCanMassage.MDL.byte.BYTE1 = (CHData1 >> 8) & 0xFF;
            TxCanMassage.MDL.byte.BYTE0 = CHData1 & 0xFF;

            TxCanMassage.MDH.byte.BYTE7 = (CHData4 >> 8) & 0xFF;
            TxCanMassage.MDH.byte.BYTE6 = CHData4 & 0xFF;
            TxCanMassage.MDH.byte.BYTE5 = (CHData3 >> 8) & 0xFF;
            TxCanMassage.MDH.byte.BYTE4 = CHData3 & 0xFF;

            ECanaMboxes.MBOX31.MDL.all = TxCanMassage.MDL.all;
            ECanaMboxes.MBOX31.MDH.all = TxCanMassage.MDH.all;

            lCHData1 = pm_ref_speed;
            lCHData2 = pm_est_omg;

            TxCanMassage.MDL.byte.BYTE3 = (lCHData1 >> 24) & 0xFF;
            TxCanMassage.MDL.byte.BYTE2 = (lCHData1 >> 16) & 0xFF;
            TxCanMassage.MDL.byte.BYTE1 = (lCHData1 >> 8) & 0xFF;
            TxCanMassage.MDL.byte.BYTE0 = lCHData1 & 0xFF;

            TxCanMassage.MDH.byte.BYTE7 = (lCHData2 >> 24) & 0xFF;
            TxCanMassage.MDH.byte.BYTE6 = (lCHData2 >> 16) & 0xFF;
            TxCanMassage.MDH.byte.BYTE5 = (lCHData2 >> 8) & 0xFF;
            TxCanMassage.MDH.byte.BYTE4 = lCHData2 & 0xFF;

            ECanaMboxes.MBOX30.MDL.all = TxCanMassage.MDL.all;
            ECanaMboxes.MBOX30.MDH.all = TxCanMassage.MDH.all;

            ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
            ECanaShadow.CANTRS.bit.TRS31 = 1;
            ECanaShadow.CANTRS.bit.TRS30 = 1;
            ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
            CanMailBoxReadyFlag.bit.Mailbox31 = 0;
            CanMailBoxReadyFlag.bit.Mailbox30 = 0;
        }
    }
}
