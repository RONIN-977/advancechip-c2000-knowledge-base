#include "CANcomm.h"
#include "Main.h"
#include "Param_lr.h"
#include "MotorInclude.h"
#include "FrqSrc.h"
#include "Parameter.h"

char Can_ID_ChangeSign =1;
char Can_ID_TestSign =0;
char Can_ID_Change =0;
CanRecv_MsgData_Struct CanRecv_MsgData_Buffer;
CanMailBox_MsgData_Struct     CanMailBox_MsgData;
Mailbox_ReadyBits          CanMailBoxReadyFlag;
RUN_MODE_CHANGE RunModeChange = RUN_MODE_PWM;
union CAN_T_MDL CanTxL;
union CAN_T_MDH CanTxH;
union CAN_T_MDL CanTxL_TEST;
union CAN_T_MDH CanTxH_TEST;
union CAN_R_MDL CanRxL;
union CAN_R_MDH CanRxH;

RCAN_Msg CANRxMsg[]={
        {0, {0},0,0}
};

RCAN_Msg *PtrCanRx = &CANRxMsg[0];

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
        //mCanCtrlStatusShadow.CANGIF0.bit.BOIF0 = 1;
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
interrupt  void CanMailBox_IsrSrv(void) {

    struct ECAN_REGS    mCanCtrlStatusShadow;
    struct  MBOX *pBoxEntry;
    Uint16  IntMailNo = 0x00;
    //CanMsgData_Entry mCanMsgDataEntry;
    Uint16   mMailBoxRecvFlag = 0x00;

    DINT;
    mCanCtrlStatusShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;

    IntMailNo = mCanCtrlStatusShadow.CANGIF1.bit.MIV1;
    pBoxEntry = &ECanaMboxes.MBOX0 + IntMailNo;

    CanMailBoxReadyFlag.all  |= ECanaRegs.CANTA.all;

    mCanCtrlStatusShadow.CANTA.all = ECanaRegs.CANTA.all;
    ECanaRegs.CANTA.all = mCanCtrlStatusShadow.CANTA.all;

    if(mCanCtrlStatusShadow.CANGIF1.bit.GMIF1 == 0x1)
    {
        mCanCtrlStatusShadow.CANRMP.all = ECanaRegs.CANRMP.all;
		mCanCtrlStatusShadow.CANTA.all   = ECanaRegs.CANTA.all;
        if(((mCanCtrlStatusShadow.CANRMP.all >> IntMailNo) & 1) == 0x1)
        {
            mCanCtrlStatusShadow.CANRMP.all |= (1 << IntMailNo);
            ECanaRegs.CANRMP.all = mCanCtrlStatusShadow.CANRMP.all;
            mMailBoxRecvFlag = 1;
        } else if (((mCanCtrlStatusShadow.CANTA.all >> IntMailNo) & 1) == 0x1) {
            mCanCtrlStatusShadow.CANTA.all = ECanaRegs.CANTA.all;
            mCanCtrlStatusShadow.CANTA.all |= (1 << IntMailNo);
            ECanaRegs.CANTA.all = mCanCtrlStatusShadow.CANTA.all;
        } else {

        }
    } else if (mCanCtrlStatusShadow.CANGIF1.bit.MTOF1 == 0x1) {
        mCanCtrlStatusShadow.CANTOS.all = ECanaRegs.CANTOS.all;
        mCanCtrlStatusShadow.CANTOS.all |= (1 << IntMailNo);
        ECanaRegs.CANTOS.all = mCanCtrlStatusShadow.CANTOS.all;
    } else {

    }
    ECanaRegs.CANGIF1.all = mCanCtrlStatusShadow.CANGIF1.all;
    EINT;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    if (mMailBoxRecvFlag == 1) {
        CanMsgData_Push(pBoxEntry);
        mMailBoxRecvFlag = 0;
    }
}

// CAN模块IO复用
void InitECanaGpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;     // Enable pull-up for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;     // Enable pull-up for GPIO31 (CANTXA)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)

    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 for CANTXA operation

    EDIS;
}

// CAN模块初始化
void InitECana_1(void)
{
    struct ECAN_REGS ECanaShadow;
    struct MBOX *TmpMailbox = (struct MBOX *)0x00;
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

    TmpMailbox = &ECanaMboxes.MBOX0;

    for (iLoop = 0x00; iLoop < MAILBOX_LENGTH; iLoop++) {
        TmpMailbox = TmpMailbox + iLoop;
        TmpMailbox->MSGCTRL.all = 0x00;
    }

    ECanaRegs.CANTA.all = 0xFFFFFFFF;
    ECanaRegs.CANRMP.all = 0xFFFFFFFF;
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 1 ;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    do {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1);

    ECanaShadow.CANBTC.all = 0;

#ifdef CAN_BAUD_250K
    //250kps, sample point  80%
    ECanaShadow.CANBTC.bit.BRPREG = 9;//5;//3;
    ECanaShadow.CANBTC.bit.SJWREG = 3;//2;
    ECanaShadow.CANBTC.bit.TSEG2REG = 3;//2;
    ECanaShadow.CANBTC.bit.TSEG1REG = 14;//10;
#endif

#ifdef CAN_BAUD_500K
    //500kps, 100MHZ CPU, sample point  80%
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

    do {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );
    ECanaRegs.CANME.all = 0;
    EDIS;
}

void CAN_Init_Interrupt(void)
{
    struct ECAN_REGS ECanaShadow;

    EALLOW;
    //PieVectTable.ECAN0INTA= &canA_isr0;
    //PieVectTable.ECAN1INTA= &canA_isr1;

    //PieVectTable.TINT0 = &CanTimer_IsrFunc;
    PieVectTable.ECAN0INTA= &CanSystem_IsrSrv;
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

    #if 1//fixme
    //PieCtrlRegs.PIEIER9.bit.INTx5 = 1;      // Enable INT 9.5 in the PIE
    PieCtrlRegs.PIEIER9.bit.INTx6 = 1;      // Enable INT 9.6 in the PIE
    #endif

    EDIS;
}
//
void CANInit()
{

    InitECanaGpio(); //官方库函数
    InitECana_1();     //配置CAN 波特率等

    CanTransmit_MailBoxInit(1, 0x580, 0, 8);
    CanTransmit_MailBoxInit(2, 0x7FF, 0, 8);
    CanTransmit_MailBoxInit(3, 0x491, 0, 8); //for text
    //CanTransmit_MailBoxInit(4, 0x001, 0, 8); //for BOOT test---
    //CanReceive_MailBoxInit(0, 0x001, 0, 800); //for BOOT---
    //CanReceive_MailBoxInit(12, 0x490, 0, 800);
    CanReceive_MailBoxInit(14, 0x67F, 0, 800);
    CanReceive_MailBoxInit(15, 0x1F1F1F1F, 1, 800);

    struct  ECAN_REGS  ECanaShadow;
    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= (0x0000FFFF);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all &= (0x0000FFFF);
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    CAN_Init_Interrupt();  //配置CAN 接收中断
 // CanBusStatus_Setup(CANBUS_BOOT_STATUS);

    //Gpio_SetL();//fixme:delete

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

    Init_CanMsgData();

}



void  Init_CanMsgData(void) {
    CanRecv_MsgData_Buffer.mCount = 0x00;
    CanRecv_MsgData_Buffer.mHeader = 0x00;
    CanRecv_MsgData_Buffer.mTailer = 0x00;
}

void  CanMsgData_Push(struct  MBOX *msgBoxData) {
    //CanMsgData_Entry mCanMsgDataEntry;
    if (CanRecv_MsgData_Buffer.mCount >= CANRECV_BOXDATA_LENGTH) {
        return;
    }
    CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].mMsgId = msgBoxData->MSGID.all;
    CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].mMsgCtrl = msgBoxData->MSGCTRL.all;
    CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].CanDataLow = msgBoxData->MDL.all;
    CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mTailer].CanDataHigh = msgBoxData->MDH.all;
    CanRecv_MsgData_Buffer.mTailer = (CanRecv_MsgData_Buffer.mTailer + 1) % CANRECV_BOXDATA_LENGTH;
    CanRecv_MsgData_Buffer.mCount = CanRecv_MsgData_Buffer.mCount + 1;
}

CanMsgData_Entry *CanMsgData_Pop(void) {
    CanMsgData_Entry *pMsgEntry;
    if (CanRecv_MsgData_Buffer.mCount <= 0x00) {
        return (CanMsgData_Entry *)0x00;
    }
    pMsgEntry = (CanMsgData_Entry *)&(CanRecv_MsgData_Buffer.mCanMsg_Entry[CanRecv_MsgData_Buffer.mHeader]);
    CanRecv_MsgData_Buffer.mHeader = (CanRecv_MsgData_Buffer.mHeader + 1) % CANRECV_BOXDATA_LENGTH;
    CanRecv_MsgData_Buffer.mCount = (CanRecv_MsgData_Buffer.mCount - 1);
    return pMsgEntry;
}

void  CanDevice_Reset(void) {
    struct ECAN_REGS  ECanaShadow;
    EALLOW;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SRES = 1;
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;
    #if 1
    InitECanaGpio();
    InitECana_1();
    //canMailboxInit();
    CanTransmit_MailBoxInit(1, 0x111, 0, 8);
    CanTransmit_MailBoxInit(2, 0x011, 0, 8);
    //CanReceive_MailBoxInit(16, 0x4E0, 0, 500);
    CAN_Init_Interrupt();
    #endif
}


void  CanReceive_MailBoxInit(Uint16  mailNo, Uint32 filterNo, Uint16 filterType, Uint32 mTimout) {
    struct  MBOX       *pMailbox;
    struct  ECAN_REGS  ECanaShadow;
    struct  MBOX       mTmpMailBox;
    Uint32             *pMailMoTo;

    pMailMoTo = &ECanaMOTORegs.MOTO0 + mailNo;
    pMailbox = &ECanaMboxes.MBOX0 + mailNo;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= ~(1 << mailNo);//?
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    ECanaShadow.CANTOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.all |= (1 << mailNo);
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTOC.all;
    *pMailMoTo = mTimout;

    mTmpMailBox.MSGID.bit.AAM = 0;
    mTmpMailBox.MSGID.bit.AME = 0;
    if (filterType == 0) {
        mTmpMailBox.MSGID.bit.IDE = 0;
        mTmpMailBox.MSGID.bit.STDMSGID = filterNo;
    } else {
        mTmpMailBox.MSGID.bit.IDE = 1;
        mTmpMailBox.MSGID.bit.STDMSGID = (filterNo >> 18) & 0x7FF;
        mTmpMailBox.MSGID.bit.EXTMSGID_H = (filterNo >> 16) & 0x3;
        mTmpMailBox.MSGID.bit.EXTMSGID_L = (filterNo & 0xFFFF);
    }
    pMailbox->MSGID.all = mTmpMailBox.MSGID.all;

    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all |= (1 << mailNo);
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    ECanaShadow.CANOPC.all = ECanaRegs.CANOPC.all;
    ECanaShadow.CANOPC.all |= (1 << mailNo);
    ECanaRegs.CANOPC.all = ECanaShadow.CANOPC.all;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= (1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}

void  CanTransmit_MailBoxInit(Uint16  mailNo, Uint32 filterNo, Uint16 filterType, Uint16 msgNum) {
    struct  MBOX       *pMailbox;
    struct  ECAN_REGS  ECanaShadow;
    struct  MBOX       mTmpMailBox;

    pMailbox = &ECanaMboxes.MBOX0 + mailNo;

    ECanaShadow.CANTRR.all = ECanaRegs.CANTRR.all;
    ECanaShadow.CANTRR.all |= (1 << mailNo);
    ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all;

    do {
        ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    } while(((ECanaShadow.CANTRS.all >> mailNo) & 1) == 1);


    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all &= ~(1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    mTmpMailBox.MSGID.bit.AAM = 0;
    mTmpMailBox.MSGID.bit.AME = 0;
    if (filterType == 0) {
        mTmpMailBox.MSGID.bit.IDE = 0;
        mTmpMailBox.MSGID.bit.STDMSGID = filterNo;
    } else {
        mTmpMailBox.MSGID.bit.IDE = 1;
        mTmpMailBox.MSGID.bit.STDMSGID = (filterNo >> 18) & 0x7FF;
        mTmpMailBox.MSGID.bit.EXTMSGID_H = (filterNo >> 16) & 0x3;
        mTmpMailBox.MSGID.bit.EXTMSGID_L = (filterNo & 0xFFFF);
    }
    mTmpMailBox.MSGCTRL.bit.DLC = msgNum;
    mTmpMailBox.MSGCTRL.bit.RTR = 0;
    mTmpMailBox.MSGCTRL.bit.TPL = 0;
    pMailbox->MSGID.all = mTmpMailBox.MSGID.all;
    pMailbox->MSGCTRL.all = mTmpMailBox.MSGCTRL.all;

    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all &= ~(1 << mailNo);
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= (1 << mailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}

void  CanTransmit_MsgData(Uint16 mChanNo, Uint16 msgNum, Uint16 flag, CanMailBox_MsgData_Struct *msgData) {
    struct  MBOX       *Mailbox;
    struct  ECAN_REGS  ECanaShadow;
    struct  MBOX       mTmpMailBox;
    //Uint32  mCanMsgData;
    Uint16  mMailNo = mChanNo;
    Mailbox = &ECanaMboxes.MBOX0 + mMailNo;


    mTmpMailBox.MSGCTRL.bit.DLC = msgNum;
    mTmpMailBox.MSGCTRL.bit.RTR = 0;
    mTmpMailBox.MSGCTRL.bit.TPL = 0;

    Mailbox->MSGCTRL.all = mTmpMailBox.MSGCTRL.all;

    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.all &= ~(1 << mMailNo);
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.all |= (1 << mMailNo);
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;
    Can_tx_data();
    //MSG_ID==580+ID
    if (mChanNo == 1) {
        Mailbox->MDH.all = CanTxH.all;
        Mailbox->MDL.all = CanTxL.all;
    } else if (mChanNo == 3){
        Mailbox->MDH.all = CanTxH_TEST.all;
        Mailbox->MDL.all = CanTxL_TEST.all;
    } else {
        Mailbox->MDH.all = CanTxH.all;
        Mailbox->MDL.all = CanTxL.all;
    }


    ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
    ECanaShadow.CANTRS.all |= (1 << mMailNo);
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

    ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
    ECanaShadow.CANTA.all |= (1 << mMailNo);
    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
}

//extern int RunSignal;
void  HandleCanRecveMsg(void) {
    Uint16  mRecvMsgNum = 0x00;
    CanMsgData_Entry *mCanMsgEntry;
    Uint32 MsgId;

    mRecvMsgNum = CanRecv_MsgData_Buffer.mCount;
    if (mRecvMsgNum <= 0 ) {
        return;
    }
    mCanMsgEntry = CanMsgData_Pop();
    MsgId = mCanMsgEntry->mMsgId&0X1FFC0000;//28~18
    MsgId >>= 18;

    CanRxL.bit.ECC_R_BYTE0    =  (mCanMsgEntry->CanDataLow&0XFF000000) >> 24;
    CanRxL.bit.ECC_R_BYTE1    =  (mCanMsgEntry->CanDataLow&0X00FF0000) >> 16;
    CanRxL.bit.ECC_R_BYTE2    =  (mCanMsgEntry->CanDataLow&0X0000FF00) >>8;
    CanRxL.bit.ECC_R_BYTE3    =  mCanMsgEntry->CanDataLow&0X000000FF;
    CanRxH.bit.ECC_R_BYTE4    =  (mCanMsgEntry->CanDataHigh&0XFF000000)>> 24;
    CanRxH.bit.ECC_R_BYTE5    =  (mCanMsgEntry->CanDataHigh&0X00FF0000)>>16;
    CanRxH.bit.ECC_R_BYTE6    =  (mCanMsgEntry->CanDataHigh&0X0000FF00)>> 8;
    CanRxH.bit.ECC_R_BYTE7    =  mCanMsgEntry->CanDataHigh&0X000000FF;
    if (MsgId == 0x67F) {
        //Bit1:0 表示无操作/1 表示设置风机 ID
        if((CanRxL.bit.ECC_R_BYTE1&0x02) == 0x02) {
            Can_ID_ChangeSign = 1;
            Can_ID_Change = CanRxL.bit.ECC_R_BYTE0;
            CanTransmit_MailBoxInit(1, (0x580+Can_ID_Change), 0, 8);
            CanTransmit_MailBoxInit(3, 0x491, 0, 8);

            CanReceive_MailBoxInit(12, 0x490, 0, 800);
            CanReceive_MailBoxInit(13, (0x600+Can_ID_Change), 0, 800);
            CanReceive_MailBoxInit(14, 0x67F, 0, 800);
            CanReceive_MailBoxInit(15, 0x1F1F1F1F, 1, 800);

            struct  ECAN_REGS  ECanaShadow;
            ECanaShadow.CANME.all = ECanaRegs.CANME.all;
            ECanaShadow.CANME.all &= (0x0000FFFF);
            ECanaRegs.CANME.all = ECanaShadow.CANME.all;

            ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
            ECanaShadow.CANMD.all &= (0x0000FFFF);
            ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
        }
    }
    //CAN接受数据
    if((MsgId == 0x67F)||(MsgId == 0x600+Can_ID_Change)){
        RunSignal = CanRxL.bit.ECC_R_BYTE0;
        RefSet = CanRxL.bit.ECC_R_BYTE1 * 10;
    }
    if (MsgId == 0x490) {
        Can_ID_TestSign = 1;
    }
}
void CanTransmit2ms(void)
{
    CanMsgData_Entry *mCanMsgEntry;
     struct ECAN_REGS  ECanaShadow;
     struct MBOX RxCanMassage;
     struct MBOX TxCanMassage;
     Uint16 Addr, Data;

    static unsigned short CanT100ms;
    if( ECanaRegs.CANGIF0.bit.BOIF0 == 1) {
        ECanaRegs.CANTRR.bit.TRR1 = 1;
        ECanaRegs.CANTRR.bit.TRR2 = 1;
        ECanaRegs.CANGIF0.bit.BOIF0 = 1;
        struct  ECAN_REGS  ECanaShadow;
        EALLOW;
        ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
        ECanaShadow.CANMC.bit.CCR = 0;
        //ECanaShadow.CANMC.bit.SRES = 1;
        ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
        EDIS;
    }
    CanT100ms++;
    if(CanT100ms == 500){
        CanT100ms = 0;
        if(Can_ID_ChangeSign == 1){
            CanTransmit_MsgData(1, 8, 0, &CanMailBox_MsgData);
            if(Can_ID_TestSign) {
                CanTransmit_MsgData(3, 8, 0, &CanMailBox_MsgData);
            }
        }
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

    if((ECanaMboxes.MBOX15.MDL.all == 0x00AA00AA)&&(ECanaMboxes.MBOX15.MDH.all == 0x00AA00AA)) {
//        asm("    MOVL XAR7, #0x3F7FF6"); //0x3F7FF6是BootLoader程序的入口地址
//        asm("    LB *XAR7");
        asm("    MOVL XAR7, #0x3F7F7E"); //0x3F7FF6是BootLoader程序的入口地址
        asm("    LB *XAR7");    }
    HandleCanRecveMsg();
}

long TestData = 0;
long pm_est_omg_show = 0;
int  TestData1,TestData2,TestData3,TestData4;
long TestData5,TestData6;
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

    if(RealTimeDataSendFlag == 1)
    {
        if((CanMailBoxReadyFlag.bit.Mailbox31 == 1)&&(CanMailBoxReadyFlag.bit.Mailbox30 == 1))
        {
            TestData1++;
            TestData2++;
            TestData3++;
            TestData4++;
            TestData5++;
            TestData6++;
            CHData1 = TestData1;//pm_ref_iq;
            CHData2 = TestData2;//gIMT.T;
            CHData3 = TestData3;//pm_ref_id;
            CHData4 = TestData4;//gIMT.M;

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

            lCHData1 = TestData5;//pm_ref_speed;
            lCHData2 = TestData6;//pm_est_omg;

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


void Can_tx_data(void)
{
    //BYTE0 运行命令
    //byte1 故障码
    //Byte2~3 转速
    //byte 4~5 相电流
    //byte 6~7 母线电压
    if(gMainCmd.Command.bit.Start == 1){
        CanTxL.bit.CAN_T_BYTE0 = 0x01;
    }else{
        CanTxL.bit.CAN_T_BYTE0 = 0x00;
    }
    CanTxL.bit.CAN_T_BYTE0 = gTemperature.Temp;
    static unsigned char err_buff;
    if(errorCodeFromCore == 0){
        err_buff = 0;
    }else if(errorCodeFromCore == 12){
        err_buff = 0x01;
    }else if((errorCodeFromCore == 13)&&(gMainCmd.Command.bit.Start == 1)){
        err_buff = 0x02;
    }else if((errorCodeFromCore == 14)&&(gMainCmd.Command.bit.Start == 1)){
        err_buff = 0x04;
    }else if((errorCodeFromCore == 13)&&(gMainCmd.Command.bit.Start == 0)){
        err_buff = 0x08;
    }else if((errorCodeFromCore == 14)&&(gMainCmd.Command.bit.Start == 0)){
        err_buff = 0x10;
    }else if(errorCodeFromCore == 33){
        err_buff = 0x40;
    }else if(errorCodeFromCore == 16){
        err_buff = 0x80;
    }else{
        err_buff = errorCodeFromCore;
    }
    CanTxL.bit.CAN_T_BYTE1 = err_buff;

    static unsigned int omg_buff;
    unsigned int omg_buff1;
    omg_buff1 = pm_est_omg/1607;
    omg_buff1 = omg_buff1*12;
    omg_buff = omg_buff - (omg_buff>>4) + (omg_buff1>>4);

    CanTxL.bit.CAN_T_BYTE2 = omg_buff&0x00ff;
    CanTxL.bit.CAN_T_BYTE3 = (omg_buff&0xff00)>>8;

    unsigned long Curbuff;
    Curbuff = gLineCur.CurPer;//*100;
   // Curbuff = Curbuff*MOTOR_RATED_CUR;
   // Curbuff >>= 12;
    CanTxH.bit.CAN_T_BYTE4 =  Curbuff&0x000000ff;
    CanTxH.bit.CAN_T_BYTE5 = (Curbuff&0x0000ff00)>>8;
    if(gMainCmd.Command.bit.Start == 0){
        CanTxH.bit.CAN_T_BYTE4 = 0;
        CanTxH.bit.CAN_T_BYTE5 = 0;
    }
    unsigned int udcbuff;
    udcbuff = gUDC.uDCFilter/10;
    CanTxH.bit.CAN_T_BYTE6 = udcbuff&0x00ff;
    CanTxH.bit.CAN_T_BYTE7 = (udcbuff&0xff00)>>8;

    //0x491监测
    //BYTE 0~1 目标频率
    //BYTE 2 监测到的PWM占空比
    //BYTE 3   IPM温度
    //BYTE 4   TempADC/16(5(缩小)+4(偏移))
    //BYTE 5   pm_ref_id/128
    //BYTE 6   pm_ref_iq/128

    CanTxL_TEST.bit.CAN_T_BYTE0 = frqAimTmp0&0x00ff;
    CanTxL_TEST.bit.CAN_T_BYTE1 = (frqAimTmp0&0xff00)>>8;
    CanTxL_TEST.bit.CAN_T_BYTE2 = Ecap_PwmValve.pwm_duty/10;
    CanTxL_TEST.bit.CAN_T_BYTE3 = gTemperature.Temp;
    CanTxH_TEST.bit.CAN_T_BYTE4 = 0;
    CanTxH_TEST.bit.CAN_T_BYTE5 = pm_ref_id>>7;
    CanTxH_TEST.bit.CAN_T_BYTE6 = pm_ref_iq>>7;
    CanTxH_TEST.bit.CAN_T_BYTE7 = 0;
}

//捕获 光耦反接，外面输入高电平时，IO口为低电平
ECAP_VALUE Ecap_PwmValve={0,0};
#define DISABLE 1
#define ENABLE 0
long Highcnt = 0,Lowcnt = 0;
char Gpio_Input;
void Ecap_Handle(void)
{
    unsigned long buff_speed;
    if(ECap1Regs.ECFLG.bit.CEVT3 != 0)
    {
        Ecap_PwmValve.pwm_t1 = ECap1Regs.CAP1;
        Ecap_PwmValve.pwm_t2 = ECap1Regs.CAP2;
        Ecap_PwmValve.pwm_t3 = ECap1Regs.CAP3;
        Ecap_PwmValve.pwm_period = Ecap_PwmValve.pwm_t3 - Ecap_PwmValve.pwm_t1;
        Ecap_PwmValve.pwm_high = Ecap_PwmValve.pwm_t2 - Ecap_PwmValve.pwm_t1;
        //百分比放大1000倍
        Ecap_PwmValve.pwm_duty_buff = Ecap_PwmValve.pwm_high*1000/Ecap_PwmValve.pwm_period;
        Ecap_PwmValve.pwm_duty = Ecap_PwmValve.pwm_duty - (Ecap_PwmValve.pwm_duty>>3) + (Ecap_PwmValve.pwm_duty_buff>>3);
        //<5% 停机
        //5%~10% 1000转
        //10%~90% 1000~5000转
        //>90%5000转
        if(Ecap_PwmValve.pwm_duty < 50) {
            buff_speed = 0;
        } else if(Ecap_PwmValve.pwm_duty <= 100) {
            buff_speed = 1000;
        }else if(Ecap_PwmValve.pwm_duty > 900) {
            buff_speed = 5000;
        } else {
            //y = 4.7x + 765
            buff_speed = Ecap_PwmValve.pwm_duty * 4812>>10;
            buff_speed = buff_speed +765;
        }
        Ecap_PwmValve.ElecFreq = buff_speed*10/12; //5对极压缩机

        ECap1Regs.ECCLR.bit.CEVT3 = 1;
    }

    Gpio_Input = GpioDataRegs.GPADAT.bit.GPIO24;
    //高电平为无效
    if(Gpio_Input == DISABLE) {
        Highcnt++;
        //3s
        if(Highcnt >= 30000){
            Ecap_PwmValve.ElecFreq = 0;
        }
    } else {
        Highcnt = 0;
    }
    //低电平有效
    if(Gpio_Input == ENABLE) {
        Lowcnt++;
        //3s
        if(Lowcnt >= 30000){
            Ecap_PwmValve.ElecFreq = MAX_LIMIT_FREQ;
        }
    } else {
        Lowcnt = 0;
    }

}
