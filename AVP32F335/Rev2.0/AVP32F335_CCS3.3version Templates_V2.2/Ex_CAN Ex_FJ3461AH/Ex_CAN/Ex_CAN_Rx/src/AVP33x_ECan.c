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
	
		//配置GPIO引脚工作在eCAN功能
		EALLOW;
		GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)

		//	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
		GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)

		/* Set qualification for selected CAN pins to asynch only */
		// Inputs are synchronized to SYSCLKOUT by default.
		// This will select asynch (no qualification) for the selected pins.

		//   GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
	  	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)


		/* Configure eCAN-A pins using GPIO regs*/
		// This specifies which of the possible GPIO pins will be eCAN functional pins.

	 	//	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANRXA operation
	  	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
	 	//	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANTXA operation
	  	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;
	
		//配置eCAN的RX和TX分别为eCAN的接收和发送引脚
		ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
		ECanaShadow.CANTIOC.bit.TXFUNC = 1;
		ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;
	
		ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
		ECanaShadow.CANRIOC.bit.RXFUNC = 1;
		ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;
		EDIS;
	
	
		EALLOW;
		ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
		//工作在非测试模式
		ECanaShadow.CANMC.bit.STM = 0;
		//工作在ecan模式
		ECanaShadow.CANMC.bit.SCB = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
	
		//初始化所有主设备控制区域为0，控制区域所有的位都初始化为0
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

		//清除所有的TA位
		ECanaRegs.CANTA.all = 0xFFFFFFFF;
   
   		//清除所有的RMP位
		ECanaRegs.CANRMP.all = 0xFFFFFFFF;
	
		//清除所有的中断标志位
		ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
		ECanaRegs.CANGIF1.all = 0xFFFFFFFF;
	
		//配置时钟参数
		EALLOW;
		ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
		ECanaShadow.CANMC.bit.CCR = 1;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
    	//CPU请求向CANBTC和CANGAM写配置信息，该位置1后必须等到CANED.CCE为1，才能
		//对CANBTC进行操作。
		do
		{
	  		ECanaShadow.CANES.all = ECanaRegs.CANES.all;
		}
		while(ECanaShadow.CANES.bit.CCE != 1);
	
		EALLOW;
		//(BRPREG+1)=10 ，CAN时钟为15MHz
		ECanaShadow.CANBTC.bit.BRPREG = 9;
		//CAN通信的波特率为1MHz
		ECanaShadow.CANBTC.bit.TSEG2REG = 2;
		ECanaShadow.CANBTC.bit.TSEG1REG = 10;
		ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;
	
		//CPU请求正常操作
		ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
		ECanaShadow.CANMC.bit.CCR = 0;
		ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
		EDIS;
		do
		{
	  		ECanaShadow.CANES.all = ECanaRegs.CANES.all;
		}
		while(ECanaShadow.CANES.bit.CCE != 0);
	
		//屏蔽所有邮箱，在写MSGID之前要完成该操作
		ECanaRegs.CANME.all = 0;
	
    	//设置接收邮箱的ID，扩展帧
		ECanaMboxes.MBOX16.MSGID.all =  0x80C20000;

		//设置邮箱16为接收邮箱
		ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    	ECanaShadow.CANMD.bit.MD16 =1;
		ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	
		//数据长度 8个BYTE
		ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;

		//没有远方应答帧被请求*/
		ECanaMboxes.MBOX16.MSGCTRL.bit.RTR = 0;

		//邮箱使能
		ECanaShadow.CANME.all = ECanaRegs.CANME.all;
		ECanaShadow.CANME.bit.ME16 =1;
		ECanaRegs.CANME.all = ECanaShadow.CANME.all;
	
		//邮箱中断使能
   		 EALLOW;
   		 ECanaRegs.CANMIM.all = 0xFFFFFFFF;
    	//邮箱中断将产生在ECAN0INT
    	ECanaRegs.CANMIL.all = 0;
    	ECanaRegs.CANGIF0.all = 0xFFFFFFFF;
    	//ECAN0INT中断请求线被使能
    	ECanaRegs.CANGIM.bit.I0EN = 1;
    	EDIS;
}
