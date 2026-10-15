/******************************************************************************
文件名                     : AVP32F08_ECan.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP32F08芯片eCAN模块初始化及函数定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP32F08_Device.h"     // AVP32F08 Headerfile Include File
#include "AVP32F08_Examples.h"   // AVP32F08 Examples Include File
// InitECan:
//---------------------------------------------------------------------------
// 初始化CAN模块
//
void InitECan(void)
{   
	struct ECAN_REGS ECanbShadow;
	EALLOW;
			
	GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;	    // Enable pull-up for GPIO18 (CANRXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;	    // Enable pull-up for GPIO19 (CANTXA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;   // Asynch qual for GPIO18 (CANRXA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;	// Configure GPIO18 for CANRXA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;	// Configure GPIO19 for CANTXA operation
	
	//配置eCAN的RX和TX分别为eCAN的接收和发送引脚
	ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
	ECanbShadow.CANTIOC.bit.TXFUNC = 1;
	ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;
	
	ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
	ECanbShadow.CANRIOC.bit.RXFUNC = 1;
	ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;
	EDIS;
	
//初始化所有主设备控制区域为0，MCF所有的位都初始化为0
	ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX1.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX2.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX3.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX4.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX5.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX6.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX7.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX8.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX9.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX10.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX11.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX12.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX13.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX14.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX15.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX16.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX17.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX18.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX19.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX20.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX21.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX22.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX23.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX24.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX25.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX26.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX27.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX28.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX29.MSGCTRL.all = 0x00000000;
	ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;

	/* Clear all TAn bits */
	ECanbRegs.CANTA.all = 0xFFFFFFFF;
	
	/* Clear all RMPn bits */
	ECanbRegs.CANRMP.all = 0xFFFFFFFF;
	
	/* Clear all interrupt flag bits */
	ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
	ECanbRegs.CANGIF1.all = 0xFFFFFFFF;
	
	/* Configure bit timing parameters */
	EALLOW;
	//ECanaRegs.CANMIM.all = 0xFFFFFFFF;		/*邮箱中断使能*/
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 1;
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	EDIS;
	
	//CPU请求向CANBTC和CANGAM写配置信息，该位置1后必须等到CANED.CCE为1，才能
	//对CANBTC进行操作。
	do
	{
	    ECanbShadow.CANES.all = ECanbRegs.CANES.all;
	}while(ECanbShadow.CANES.bit.CCE != 1);
	
	EALLOW;
	/*(BRPREG+1)=10 feeds a 15MHz CAN clock*/
	ECanbShadow.CANBTC.bit.BRPREG = 9;
	/*150/10=15*/
	ECanbShadow.CANBTC.bit.TSEG2REG = 2;
	ECanbShadow.CANBTC.bit.TSEG1REG = 10;
	ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;
	
	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 0;
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
	EDIS;
	do
	{
	    ECanbShadow.CANES.all = ECanbRegs.CANES.all;
	}while(ECanbShadow.CANES.bit.CCE != 0);		//等待CCE位清零
	
	//屏蔽所有邮箱
	ECanbRegs.CANME.all = 0;		//在写MSGID之前要完成该操作
	
	/*发送邮箱的ID号*/
	ECanbMboxes.MBOX0.MSGID.all = 0x80C20000;
	
	
	/*接收邮箱的ID号*/
    ECanbMboxes.MBOX16.MSGID.all = 0x00C80000;
	
	//设置邮箱16为接收邮箱
    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    ECanbShadow.CANMD.bit.MD16 =1;
    ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;

    //设置邮箱0为发送邮箱
    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    ECanbShadow.CANMD.bit.MD0 =0;
	ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;
	
	
	/*数据长度 8个BYTE*/
	ECanbMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
	ECanbMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
			
	//设置发送优先级
	ECanbMboxes.MBOX0.MSGCTRL.bit.TPL = 0;
	ECanbMboxes.MBOX16.MSGCTRL.bit.TPL = 0;
			
			
	/*没有远方应答帧被请求*/
	ECanbMboxes.MBOX0.MSGCTRL.bit.RTR = 0;
	ECanbMboxes.MBOX16.MSGCTRL.bit.RTR = 0;

	
	//向邮箱RAM区写数据
	ECanbMboxes.MBOX0.MDL.all = 0x01234567;
	ECanbMboxes.MBOX0.MDH.all = 0x89ABCDEF;
	
	
    //邮箱使能Mailbox0
	
	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME0 =1;
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;

	ECanbShadow.CANME.all = ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME16 =1;
	ECanbRegs.CANME.all = ECanbShadow.CANME.all;

}
