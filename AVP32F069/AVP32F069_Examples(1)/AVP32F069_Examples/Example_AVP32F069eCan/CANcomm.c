#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

interrupt void canA_isr0(void);
interrupt void canA_isr1(void);

RCAN_Msg CANRxMsg[]={
		{0, {0},0,0}
};

RCAN_Msg *PtrCanRx = &CANRxMsg[0];


Uint32 lastSendMBox =0; //上一次发送的邮箱号码

TCAN_Msg  CANTxMsg[]={
		{0,0,0,0, 1,0xA0}
};

TCAN_Msg *PtrCanTx = &CANTxMsg[0];


//CAN的GPIO初始化
//这里采用GPIO30 GPIO31
void Init_CANA_Gpio(void)
{
   EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;     // Enable pull-up for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;     // Enable pull-up for GPIO31 (CANTXA)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)

    //IO30 IO21配置为CAN外设引脚
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 for CANTXA operation

    EDIS;
}

//CAN初始化函数
void CANInit()
{
	Init_CANA_Gpio(); //官方库函数
	InitECana_1();	   //配置CAN 波特率等
	canMailboxInit();  //配置发送、接收邮箱（MBOX16 ID= 0xA0）、ID
	CAN_Init_Interrupt();//配置CAN 接收中断
}

//复位邮箱、配置波特率等操作
void InitECana_1(void)        // Initialize eCAN-A module
{

/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return
 false data. */

struct ECAN_REGS ECanaShadow;

    EALLOW;     // EALLOW enables access to protected bits

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/

    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */
                                    // HECC mode also enables time-stamping feature

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SCB = 1;               //P35  Select eCAN mode.
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

/* Initialize all bits of 'Message Control Register' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//  as a matter of precaution.
    ECanaRegs.CANTA.all = 0xFFFFFFFF;   /* Clear all TAn bits */
    ECanaRegs.CANRMP.all = 0xFFFFFFFF;  /* Clear all RMPn bits */
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /* Clear all interrupt flag bits */
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

/* Configure bit timing parameters for eCANA*/
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    // Wait until the CPU has been granted permission to change the configuration registers
    do
    {
      ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 1 );       // Wait for CCE bit to be set..

    ECanaShadow.CANBTC.all = 0;
    /* The following block is only for 60 MHz SYSCLKOUT. (30 MHz CAN module clock Bit rate = 1 Mbps
       See Note at end of file. */
/*
    ECanaShadow.CANBTC.bit.BRPREG = 2;
    ECanaShadow.CANBTC.bit.TSEG2REG = 1;
    ECanaShadow.CANBTC.bit.TSEG1REG = 6;

    ECanaShadow.CANBTC.bit.SAM = 1;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;
*/
    //-------------------------自定义的配置------------------------------------------
    //rate Bit =(SYSCLKOUT/2)/(BRP * Bit-time )
    //BRP = BRPreg+1
    //Bit-time = (TSEG1reg + 1) + (TSEG2reg+ 1) + 1
    //配置TQ 和BRP ，
    //方案一 TQ为10， 则TSEG1REG = 6， BRP = 5 ，为500K
    //方案二，TQ为12，则TSEG1REG = 8， BRP = 4 ，为500K
    //500KHz
	/*ECanaShadow.CANBTC.bit.BRPREG = 8;//5;
	ECanaShadow.CANBTC.bit.TSEG2REG = 1;
	ECanaShadow.CANBTC.bit.TSEG1REG = 6;*/

    ECanaShadow.CANBTC.bit.BRPREG = 1;
    ECanaShadow.CANBTC.bit.TSEG2REG = 4;
    ECanaShadow.CANBTC.bit.TSEG1REG = 13;
	ECanaShadow.CANBTC.bit.SAM = 1;
	ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    // Wait until the CPU no longer has permission to change the configuration registers
    do
    {
      ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 );       // Wait for CCE bit to be  cleared..

/* Disable all Mailboxes  */
    ECanaRegs.CANME.all = 0;        // Required before writing the MSGIDs

    EDIS;
}

//配置发送、接收邮箱等操作
//数据区8个长度字节
//发送邮箱   1到6
//接收邮箱   16， ID为0x0A
void canMailboxInit(void)
{
   struct ECAN_REGS ECanaShadow;

   EALLOW;
    ECanaShadow.CANGAM.all = ECanaRegs.CANGAM.all;
	ECanaShadow.CANGAM.bit.AMI=1; // Standard and extended frames can be received.
	ECanaRegs.CANGAM.all = ECanaShadow.CANGAM.all;
   EDIS;

//In standard identifier mode,if the IDE bit(MSGID.31) =0,the message identifier is storedin bits ID.28:18.
//IDE =0:The RECEIVED message had a standard identifier
//Auto answer mode bit. AAM =0 ;  Normal transmit mode
//AME = 1 ;The corresponding acceptance mask is used

//MSGID.31=IDE; MSGID.30=AME; MSGID.29=AAM
	// Mailboxs can be written to 16-bits or 32-bits at a time

 //TX 1
    ECanaMboxes.MBOX1.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX1.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX1.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX1.MSGID.bit.STDMSGID = 0x010;

 //TX 2
    ECanaMboxes.MBOX2.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX2.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX2.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX2.MSGID.bit.STDMSGID = 0x012;

 //TX 3
    ECanaMboxes.MBOX3.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX3.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX3.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX3.MSGID.bit.STDMSGID = 0x013;
 //TX 4
    ECanaMboxes.MBOX4.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX4.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX4.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX4.MSGID.bit.STDMSGID = 0x014;
 //TX 5
    ECanaMboxes.MBOX5.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX5.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX5.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX5.MSGID.bit.STDMSGID = 0x015;
 //TX 6
    ECanaMboxes.MBOX6.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX6.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX6.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX6.MSGID.bit.STDMSGID = 0x020;

//RX
    ECanaMboxes.MBOX16.MSGID.bit.AME = 1;
	ECanaMboxes.MBOX16.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX16.MSGID.bit.IDE = 0;
    ECanaMboxes.MBOX16.MSGID.bit.STDMSGID = 0xA0;
	//P71  P64
	ECanaLAMRegs.LAM16.bit.LAMI = 1;
	ECanaLAMRegs.LAM16.bit.LAM_H= (0x000)<<2;;  //   LAM_H:13;     // 16:28  ,只接收符合ID的报文
	//ECanaLAMRegs.LAM16.bit.LAM_H= (0x7ff)<<2;;//   LAM_H:13;     // 16:28  ,标准的要28:18
	//ECanaLAMRegs.LAM16.bit.LAM_L=0;	        //   LAM_L:16;     // 0:15

    // Specify that 8 bits will be sent/received //8字节数据
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX2.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX3.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX4.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX5.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX6.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
////////////////////////////////////////////////////////////////////////////////////
    EALLOW;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.DBO = 1;
    ECanaShadow.CANMC.bit.SCB = 1;//Select eCAN mode
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
 // Configure  Mailbox Direction: MBOX1--6 as Tx, MBOX16 as Rx
    ECanaRegs.CANMD.all = 0x00010000;
    // Enable all Mailboxes */
    // Since this write is to the entire register (instead of a bit
    // field) a shadow register is not required.
    ECanaRegs.CANME.all = 0x0001007E;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.STM = 0;    // The module is in normal mode
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;
}

//配置CAN接收中断
void CAN_Init_Interrupt(void)
{
/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return
 false data. */
	struct ECAN_REGS ECanaShadow;

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.ECAN0INTA = &canA_isr0;    //------------中断配置步骤-----2
	PieVectTable.ECAN1INTA = &canA_isr1;
	EDIS;    // This is needed to disable write to EALLOW protected registers


    EALLOW;     // EALLOW enables access to protected bits
	ECanaShadow.CANMIL.all = ECanaRegs.CANMIL.all;
    ECanaShadow.CANMIL.all = 0xFFFFFFFF ;            //P76, mailbox interrupts to level 1
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;


	ECanaShadow.CANMIM.all = ECanaRegs.CANMIM.all;
    ECanaShadow.CANMIM.all =0x00010000 ;            //P48,相应的邮箱中断使能位 Mailbox interrupt is enabled.
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

	ECanaShadow.CANGIM.all = ECanaRegs.CANGIM.all;
	ECanaShadow.CANGIM.bit.GIL =0;            //P47, Set CANGIM.GIL = 0;All global interrupts are mapped to the ECAN0INT interrupt line
											  //Globalinterrupt level for the interrupts TCOF,WDIF,WUIF,BOIF,EPIF,RMLIF,AAIFandWLIF.
	ECanaShadow.CANGIM.bit.AAIM=1;
	ECanaShadow.CANGIM.bit.WDIM=1;
	ECanaShadow.CANGIM.bit.WUIM=1;
	ECanaShadow.CANGIM.bit.BOIM=1;
	ECanaShadow.CANGIM.bit.EPIM=1;
	ECanaShadow.CANGIM.bit.WLIM=1;

//------------中断配置步骤-----1
	ECanaShadow.CANGIM.bit.I1EN = 1 ;            //P47, This bit globally enables all interrupts for the ECAN1INT line if the corresponding masks are set.
	ECanaShadow.CANGIM.bit.I0EN = 1 ;
	ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//  as a matter of precaution.

    ECanaRegs.CANTA.all = 0xFFFFFFFF;   /* Clear all TAn bits */
    ECanaRegs.CANRMP.all = 0xFFFFFFFF;  /* Clear all RMPn bits */
    ECanaRegs.CANGIF0.all = 0xFFFFFFFF; /* Clear all interrupt flag bits */
    ECanaRegs.CANGIF1.all = 0xFFFFFFFF;

//------------中断配置步骤-----3
// Enable CAN in PIE
   PieCtrlRegs.PIEIER9.bit.INTx5 = 1;      // Enable INT 9.5 in the PIE
   PieCtrlRegs.PIEIER9.bit.INTx6 = 1;      // Enable INT 9.6 in the PIE

//------------中断配置步骤-----4
   IER |= M_INT9;                          // Enable CPU Interrupt 9

   EDIS;

}

//配置CAN发送函数
void CAN_Send(TCAN_Msg *CANTx)
{
   	volatile struct MBOX *Mailbox;
   	struct ECAN_REGS ECanaShadow;
   	Uint32 CANData,NowMailBox;
   	Uint32 CANTaFlag = 1;

   	NowMailBox = CANTaFlag<<CANTx->MBXnbr;
#ifdef CANLASTWAIT
	//等待上一个邮箱发送完毕
	while(ECanaRegs.CANTA.all != lastSendMBox);
	//清空邮箱
	ECanaRegs.CANTA.all|= lastSendMBox;
#elif defined CANSENDNOWAIT
	//直接清空上一次的发送标志，忽略是否发送成功，测试用
	//清空邮箱
	ECanaRegs.CANTA.all|= lastSendMBox;

#endif

   	//选定发送邮箱
	if(CANTx->MBXnbr < 16)Mailbox = &ECanaMboxes.MBOX0 + CANTx->MBXnbr;
	else                  Mailbox = &ECanaMboxes.MBOX16 + (CANTx->MBXnbr-16);

	//指定接收邮箱ID
	//先关闭ME，再配置ID
	//----------------------------配置接收ID开始--------------------------------------
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.all &=~ NowMailBox;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all ;

	ECanaShadow.CANTRR.all = ECanaRegs.CANTRR.all;
	ECanaShadow.CANTRR.all |= NowMailBox;
	ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all ;

	Mailbox->MSGID.bit.STDMSGID = CANTx->RevMBXnID;

	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.all |= NowMailBox;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all ;
	//----------------------------配置接收ID完毕--------------------------------------


	//装入数据
	CANData = CANTx->DB;
	CANData<<=16;
	CANData +=  CANTx->DA;
	Mailbox->MDL.all = CANData;

	CANData = CANTx->DD;
	CANData<<=16;
	CANData +=  CANTx->DC;
	Mailbox->MDH.all = CANData;

	//使能发送
	ECanaRegs.CANTRS.all |= (CANTaFlag<<CANTx->MBXnbr);
//	printf("\r\n%0.8lx %0.8lx %0.8lx",(CANTaFlag<<CANTx->MBXnbr),NowMailBox,ECanaRegs.CANTRS.all);//打印测试

	//记录上次发送的邮箱号
	lastSendMBox = NowMailBox;

#ifdef CANNOWWAIT
	//等待上一个邮箱发送完毕
	while(ECanaRegs.CANTA.all != lastSendMBox);
	//清空邮箱
	ECanaRegs.CANTA.all|= lastSendMBox;
#endif


}

//CAN错误处理函数
interrupt void canA_isr0(void) //用于系统中断，故消息部分的函数可去
{
	struct ECAN_REGS ECanaShadow;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

	//EALLOW;
	ECanaShadow.CANGIF0.all = ECanaRegs.CANGIF0.all;//CANGIF0
	//EDIS;

	if((ECanaShadow.CANGIF0.all & 0x00037f00) != 0) //有错误产生中断
	{
		//添加 处理错误代码
		//添加 清除错误产生的标记 P76 Table3-4.eCANInterrupt Assertion/Clearing
		ECanaRegs.CANGIF0.all =	ECanaShadow.CANGIF0.all  & 0x00013700;
		ECanaRegs.CANTOS.all = 0x0000FFFF; //bits17,MTOF 因为低十六位用于发送，所以一次性全清除超时中断
		ECanaRegs.CANAA.all =  0x0000FFFF; //P29 Bit14 AAIF ,If the transmission of the message in mailbox n was aborted, the bit AA[n] is set and the AAIF (GIF.14) bit
		ECanaRegs.CANRMP.all=  0xFFFF0000; // Bit11,RMLIF   ,

	}
	//GMIF0/1 :: Global mailbox interrupt flag
	else if(ECanaShadow.CANGIF0.bit.GMIF0 == 1) //One of the mailboxes transmitted or received a message successfully
	{ 											//查看 MIV0/MIV1(GIF0.4-0/GIF1.4-0) ， 读取相应的mailbox 数据

		if(ECanaShadow.CANRMP.all != 0)//注意是 ECanaShadow，而ECanaRegs.CANRMP已被清除 == 0xFFFF0000
												//本例程的中断只做了mailbox16的接收中断，在16--32的mailbox中，优先级最低，
											  //则认为mailbox16接收到数据了，其他的mailbox也接收到数据了
		{
		/*	CAN_REV(16,&REV_Data[0][1],&REV_Data[0][0]);
		    CAN_Reci_Data.DA=(int)(REV_Data[0][0]);
			CAN_Reci_Data.DB=(int)(REV_Data[0][0]>>16);
			CAN_Reci_Data.DC=(int)(REV_Data[0][1]);
			CAN_Reci_Data.DD=(int)(REV_Data[0][1]>>16);	*/
		}
		else
		{
		//EALLOW;
			ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
			ECanaShadow.CANRMP.all &= 0xFFFF0000;
			ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;//清除中断标志
		//	EDIS;
		}

	}
}

//CAN中断接收函数
interrupt void canA_isr1(void) //用于 Message 中断 ，故系统部分的函数可取消　 经常用的中断
{
	struct ECAN_REGS ECanaShadow;
	Uint32 RxDH,RxDL;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

	EALLOW;
	ECanaShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;
	EDIS;

	if((ECanaShadow.CANGIF1.all & 0x00037f00) != 0) //有错误产生的中断
	{
		//添加处理错误代码
		//添加清除错误产生的标记 P76 Table3-4.eCANInterrupt Assertion/Clearing
		ECanaRegs.CANGIF1.all =	ECanaShadow.CANGIF1.all  & 0x00013700;
		ECanaRegs.CANTOS.all = 0x0000FFFF; //bits17,MTOF 因为低十六位用于发送，所以一次性全清除超时中断
		ECanaRegs.CANAA.all =  0x0000FFFF; //P29 Bit14 AAIF ,If the transmission of the message in mailbox n was aborted, the bit AA[n] is set and the AAIF (GIF.14) bit
		ECanaRegs.CANRMP.all=  0xFFFF0000; //

	}

	else if(ECanaShadow.CANGIF1.bit.GMIF1 == 1) //接收或者发送中断			//查看 MIV0/MIV1(GIF0.4-0/GIF1.4-0) ， 读取相应的mailbox 数据
  	{
		ECanaRegs.CANRMP.all = 0Xffff0000;

		PtrCanRx->Mark_Para.Status_Bits.rFifoDataflag = 1;
		if((PtrCanRx->rxWriteIndex + 1 ) != PtrCanRx->rxReadIndex )
		{
			//接收数据
			RxDL = ECanaMboxes.MBOX16.MDL.all;
			RxDH = ECanaMboxes.MBOX16.MDH.all;
			PtrCanRx->rxData[PtrCanRx->rxWriteIndex][0] = RxDL;
			PtrCanRx->rxData[PtrCanRx->rxWriteIndex][1] = RxDL>>16;
			PtrCanRx->rxData[PtrCanRx->rxWriteIndex][2] = RxDH;
			PtrCanRx->rxData[PtrCanRx->rxWriteIndex][3] = RxDH>>16;

			PtrCanRx->rxWriteIndex=(++PtrCanRx->rxWriteIndex)%(CANRxLEN);
		}
		else//接收缓存满
		{
			//用户这里做缓存满的处理,
			PtrCanRx->Mark_Para.Status_Bits.rFifoFullFlag = 1;
		}

	 }
}

//CAN接收缓存处理函数
void handleCANRxFIFO()
{
	Uint16 wrIndex =0;
	if(PtrCanRx->Mark_Para.Status_Bits.rFifoDataflag == 1 )
	{
		PtrCanRx->Mark_Para.Status_Bits.rFifoDataflag =0;
		while(PtrCanRx->rxReadIndex != PtrCanRx->rxWriteIndex)
		{
			//这里处理接收到的数据
			wrIndex = (PtrCanRx->rxWriteIndex!=0)?(PtrCanRx->rxWriteIndex-1):(CANRxLEN-1); //获取写入FIFO指针，上传到PC
			//先显示低位 (B1B0 B3B2 B5B4 B7B6顺序显示)
			printf("\r\nCAN Rx Index :%d ,Data :0x%0.4x 0x%0.4x 0x%0.4x 0x%0.4x    wr Index :%d",PtrCanRx->rxReadIndex\
					,PtrCanRx->rxData[PtrCanRx->rxReadIndex][0]\
					,PtrCanRx->rxData[PtrCanRx->rxReadIndex][1]\
					,PtrCanRx->rxData[PtrCanRx->rxReadIndex][2]\
					,PtrCanRx->rxData[PtrCanRx->rxReadIndex][3]\
					,wrIndex);
			//点亮LED
			switch(PtrCanRx->rxData[PtrCanRx->rxReadIndex][0])
			{
			   case '1':  LED1_TOGGLE();break;
			   case '2':  LED2_TOGGLE();break;
			   default:break;
			}
			//数码管显示低2个字节的数据
			Running.showhex[1]= (PtrCanRx->rxData[PtrCanRx->rxReadIndex][0]>>4)&0x0f;
			Running.showhex[0]=  PtrCanRx->rxData[PtrCanRx->rxReadIndex][0]&0x0f;
			PtrCanRx->rxReadIndex=(++PtrCanRx->rxReadIndex)%(CANRxLEN);
		}

	}
	// Running.showhex[1]= 0x01;//(PtrCanRx->rxData[PtrCanRx->rxReadIndex][0]>>4)&0x0f;
	 //Running.showhex[0]= 0x02;//PtrCanRx->rxData[PtrCanRx->rxReadIndex][0]&0x0f;

	//这里做CAN缓存溢出处理
	if(PtrCanRx->Mark_Para.Status_Bits.rFifoFullFlag ==1)
	{
		PtrCanRx->Mark_Para.Status_Bits.rFifoFullFlag = 0;
		printf("\r\n--------------CAN Rx FIFO overflow-------------------------------------");

		PtrCanRx->rxReadIndex  = 0;
		PtrCanRx->rxWriteIndex = 0;
	}

	//这里做硬件CAN接收溢出处理
	if(PtrCanRx->Mark_Para.Status_Bits.HWOVFlag == 1)
	{
		PtrCanRx->Mark_Para.Status_Bits.HWOVFlag = 0;
		printf("\r\n---------------CAN Rx HW Overflow ISR Clear-------------------------");
	}

}




