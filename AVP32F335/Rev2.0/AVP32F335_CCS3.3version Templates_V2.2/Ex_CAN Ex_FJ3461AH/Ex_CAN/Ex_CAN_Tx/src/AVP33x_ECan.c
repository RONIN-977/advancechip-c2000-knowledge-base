/******************************************************************************
文件名                     : AVP33x_ECan.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片eCAN模块初始化及函数定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // AVP33x Headerfile Include File
#include "AVP33x_Examples.h"   // AVP33x Examples Include File


//---------------------------------------------------------------------------
// InitECan:
//---------------------------------------------------------------------------
// 初始化CAN模块
//
void InitECan(void)
{   
	struct ECAN_REGS ECanaShadow;
	EALLOW;
			
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA operation
	
	//配置eCAN的RX和TX分别为eCAN的接收和发送引脚
	ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
	ECanaShadow.CANTIOC.bit.TXFUNC = 1;
	ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;
	
	ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
	ECanaShadow.CANRIOC.bit.RXFUNC = 1;
	ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;
	EDIS;
	
//初始化所有主设备控制区域为0，MCF所有的位都初始化为0
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

	/* Clear all TAn bits */
	ECanaRegs.CANTA.all = 0xFFFFFFFF;
	
	/* Clear all RMPn bits */
	ECanaRegs.CANRMP.all = 0xFFFFFFFF;
	
	/* Clear all interrupt flag bits */
	ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
	ECanaRegs.CANGIF1.all = 0xFFFFFFFF;
	
	/* Configure bit timing parameters */
	EALLOW;
	//ECanaRegs.CANMIM.all = 0xFFFFFFFF;		/*邮箱中断使能*/
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
	EDIS;
	
	//CPU请求向CANBTC和CANGAM写配置信息，该位置1后必须等到CANED.CCE为1，才能
	//对CANBTC进行操作。
	do
	{
	 	ECanaShadow.CANES.all = ECanaRegs.CANES.all;
	}while(ECanaShadow.CANES.bit.CCE != 1);
	
	EALLOW;
	/*(BRPREG+1)=10 feeds a 15MHz CAN clock*/
	ECanaShadow.CANBTC.bit.BRPREG = 9;
	/*150/10=15*/
	ECanaShadow.CANBTC.bit.TSEG2REG = 2;
	ECanaShadow.CANBTC.bit.TSEG1REG = 10;
	ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;
	
	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0;
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
	EDIS;
	do
	{
	 ECanaShadow.CANES.all = ECanaRegs.CANES.all;
	}while(ECanaShadow.CANES.bit.CCE != 0);		//等待CCE位清零
	
	//屏蔽所有邮箱
	ECanaRegs.CANME.all = 0;		//在写MSGID之前要完成该操作
	
	/*发送邮箱的ID号*/
	ECanaMboxes.MBOX0.MSGID.all = 0x80C20000;
	
	
	/*接收邮箱的ID号*/
	ECanaMboxes.MBOX16.MSGID.all = 0x00C80000;
	
	//设置邮箱16为接收邮箱
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.bit.MD16 =1;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    //设置邮箱0为发送邮箱
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD0 =0;
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	
	
	/*数据长度 8个BYTE*/
	ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
	ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
			
	//设置发送优先级
	ECanaMboxes.MBOX0.MSGCTRL.bit.TPL = 0;
	ECanaMboxes.MBOX16.MSGCTRL.bit.TPL = 0;
			
			
	/*没有远方应答帧被请求*/
	ECanaMboxes.MBOX0.MSGCTRL.bit.RTR = 0;
	ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 0;

	
	//向邮箱RAM区写数据
	ECanaMboxes.MBOX0.MDL.all = 0x01234567;
	ECanaMboxes.MBOX0.MDH.all = 0x89ABCDEF;
	
	
    //邮箱使能Mailbox0
	
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME0 =1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME16 =1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

}
