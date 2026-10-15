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


//---------------------------------------------------------------------------
// InitECan:
//---------------------------------------------------------------------------
// 初始化CAN模块
//
void InitECan(void)
{
    struct ECAN_REGS ECanbShadow;
	
		//配置GPIO引脚工作在eCAN功能
    EALLOW;

        GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;     // Enable pull-up for GPIO18 (CANRXA)
        GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;     // Enable pull-up for GPIO19 (CANTXA)
        GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;   // Asynch qual for GPIO18 (CANRXA)
        GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;    // Configure GPIO18 for CANRXA operation
        GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;    // Configure GPIO19 for CANTXA operation

	
		//配置eCAN的RX和TX分别为eCAN的接收和发送引脚
        ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
        ECanbShadow.CANTIOC.bit.TXFUNC = 1;
        ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;
	
		ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
		ECanbShadow.CANRIOC.bit.RXFUNC = 1;
		ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;
		EDIS;
	
	
		EALLOW;
		ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
		//工作在非测试模式
		ECanbShadow.CANMC.bit.STM = 0;
		//工作在ecan模式
		ECanbShadow.CANMC.bit.SCB = 1;
		ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
		EDIS;
	
		//初始化所有主设备控制区域为0，控制区域所有的位都初始化为0
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

		//清除所有的TA位
		ECanbRegs.CANTA.all = 0xFFFFFFFF;
   
   		//清除所有的RMP位
		ECanbRegs.CANRMP.all = 0xFFFFFFFF;
	
		//清除所有的中断标志位
		ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
		ECanbRegs.CANGIF1.all = 0xFFFFFFFF;
	
		//配置时钟参数
		EALLOW;
		ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
		ECanbShadow.CANMC.bit.CCR = 1;
		ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
		EDIS;
    	//CPU请求向CANBTC和CANGAM写配置信息，该位置1后必须等到CANED.CCE为1，才能
		//对CANBTC进行操作。
		do
		{
		    ECanbShadow.CANES.all = ECanbRegs.CANES.all;
		}
		while(ECanbShadow.CANES.bit.CCE != 1);
	
		EALLOW;
		//(BRPREG+1)=10 ，CAN时钟为15MHz
		ECanbShadow.CANBTC.bit.BRPREG = 9;
		//CAN通信的波特率为1MHz
		ECanbShadow.CANBTC.bit.TSEG2REG = 2;
		ECanbShadow.CANBTC.bit.TSEG1REG = 10;
		ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;
	
		//CPU请求正常操作
		ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
		ECanbShadow.CANMC.bit.CCR = 0;
		ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;
		EDIS;
		do
		{
		    ECanbShadow.CANES.all = ECanbRegs.CANES.all;
		}
		while(ECanbShadow.CANES.bit.CCE != 0);
	
		//屏蔽所有邮箱，在写MSGID之前要完成该操作
		ECanbRegs.CANME.all = 0;
	
    	//设置接收邮箱的ID，扩展帧
        ECanbMboxes.MBOX16.MSGID.all =  0x80C20000;

		//设置邮箱16为接收邮箱
		ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
		ECanbShadow.CANMD.bit.MD16 =1;
		ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;
	
		//数据长度 8个BYTE
        ECanbMboxes.MBOX16.MSGCTRL.bit.DLC = 8;

		//没有远方应答帧被请求*/
		ECanbMboxes.MBOX16.MSGCTRL.bit.RTR = 0;

		//邮箱使能
		ECanbShadow.CANME.all = ECanbRegs.CANME.all;
		ECanbShadow.CANME.bit.ME16 =1;
		ECanbRegs.CANME.all = ECanbShadow.CANME.all;
	
		//邮箱中断使能
   		 EALLOW;
   		ECanbRegs.CANMIM.all = 0xFFFFFFFF;
    	//邮箱中断将产生在ECAN0INT
   		ECanbRegs.CANMIL.all = 0;
   		ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
    	//ECAN0INT中断请求线被使能
   		ECanbRegs.CANGIM.bit.I0EN = 1;
    	EDIS;
}
void InitECanb_test(void)        // Initialize eCAN-A module
{
struct ECAN_REGS ECanbShadow;

    EALLOW;     // EALLOW enables access to protected bits
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;     // Enable pull-up for GPIO18 (CANRXA)
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;     // Enable pull-up for GPIO19 (CANTXA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;   // Asynch qual for GPIO18 (CANRXA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;    // Configure GPIO18 for CANRXA operation
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;
/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/
    ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
    ECanbShadow.CANTIOC.bit.TXFUNC = 1;
    ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;

    ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
    ECanbShadow.CANRIOC.bit.RXFUNC = 1;
    ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;
// Standard and extended frames can be received.
    ECanbShadow.CANGAM.all = ECanbRegs.CANGAM.all;
    ECanbShadow.CANGAM.bit.AMI=1;
    ECanbRegs.CANGAM.all = ECanbShadow.CANGAM.all;

/* Configure bit timing parameters for eCANA*/
    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.SCB = 1 ;
    ECanbShadow.CANMC.bit.PDR = 0 ;
    ECanbShadow.CANMC.bit.DBO = 0 ;
    ECanbShadow.CANMC.bit.WUBA = 0;
    ECanbShadow.CANMC.bit.CDR = 0 ;
    ECanbShadow.CANMC.bit.ABO = 1 ;
    ECanbShadow.CANMC.bit.STM = 0 ;
    ECanbShadow.CANMC.bit.SRES = 0 ;
    ECanbShadow.CANMC.bit.MBNR = 0 ;

    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

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
    ECanbMboxes.MBOX31.MSGCTRL.all = 0x00000000;

    //清除所有的TA位
    ECanbRegs.CANTA.all = 0xFFFFFFFF;

    //清除所有的RMP位
    ECanbRegs.CANRMP.all = 0xFFFFFFFF;

    //清除所有的中断标志位
    ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
    ECanbRegs.CANGIF1.all = 0xFFFFFFFF;

    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.CCR = 1;
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;            //发送错误计数器清零

    do
    {
        ECanbShadow.CANES.all = ECanbRegs.CANES.all;
    } while(ECanbShadow.CANES.bit.CCE != 1);       // Wait for CCE bit to be set..发送错误计数器清零
    ECanbShadow.CANBTC.all = 0;

    ECanbShadow.CANBTC.bit.BRPREG = 9;
    //CAN通信的波特率为1MHz
    ECanbShadow.CANBTC.bit.TSEG2REG = 2;
    ECanbShadow.CANBTC.bit.TSEG1REG = 10;
    ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;

    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
    ECanbShadow.CANMC.bit.CCR = 0;

    ECanbShadow.CANBTC.bit.SAM = 0;
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;


    do
    {
        ECanbShadow.CANES.all = ECanbRegs.CANES.all;
    } while(ECanbShadow.CANES.bit.CCE != 0 );       // Wait for CCE bit to be  cleared..*/
    ECanbShadow.CANME.all = ECanbRegs.CANME.all;
    ECanbShadow.CANME.all = 0xFFFFFFFF;
    ECanbRegs.CANME.all = ECanbShadow.CANME.all;
    ECanbRegs.CANME.all = 0;
    ECanbMboxes.MBOX16.MSGID.all =  0x80C20000;
    ECanbMboxes.MBOX29.MSGID.all =  0x80C30000;
    //设置邮箱16为接收邮箱
    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    ECanbShadow.CANMD.bit.MD16 =1;
    ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;
    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    ECanbShadow.CANMD.bit.MD29 =1;
    ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;
    ECanbShadow.CANMD.all = ECanbRegs.CANMD.all;
    ECanbShadow.CANMD.all = 0xFFFF0000;
    ECanbRegs.CANMD.all = ECanbShadow.CANMD.all;
    //数据长度 8个BYTE
    ECanbMboxes.MBOX16.MSGCTRL.bit.DLC = 8;
    ECanbMboxes.MBOX29.MSGCTRL.bit.DLC = 8;
    //没有远方应答帧被请求*/
    ECanbMboxes.MBOX16.MSGCTRL.bit.RTR = 0;
    //邮箱使能
    ECanbShadow.CANME.all = ECanbRegs.CANME.all;
    ECanbShadow.CANME.bit.ME29 =1;
    ECanbRegs.CANME.all = ECanbShadow.CANME.all;

    ECanbShadow.CANME.all = ECanbRegs.CANME.all;
    ECanbShadow.CANME.bit.ME16 =1;
    ECanbRegs.CANME.all = ECanbShadow.CANME.all;
    ECanbShadow.CANME.all = ECanbRegs.CANME.all;
    ECanbShadow.CANME.all = 0xFFFFFFFF;
    ECanbRegs.CANME.all = ECanbShadow.CANME.all;
    //邮箱中断使能
    ECanbRegs.CANMIM.all = 0xFFFFFFFF;
    //邮箱中断将产生在ECAN0INT
    ECanbRegs.CANMIL.all = 0;
    ECanbRegs.CANGIF0.all = 0xFFFFFFFF;
    //ECAN0INT中断请求线被使能
    ECanbRegs.CANGIM.bit.I0EN = 1;

    EDIS;
}
