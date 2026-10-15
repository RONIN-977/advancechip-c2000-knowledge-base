/******************************************************************************
文件名:                    Example_AVP32F08_CAN_Tx.c
作者:                      湖南进芯电子科技有限公司
功能描述:  通过AVP32F08发送数据给其他外部CAN设备，了解CAN总线的设置和通讯过程，请尤其注意ID的设置。
           将MBOX0设置为发送模式，MBOX16设置为接收模式,通过CAN Tools工具软件，可以观察收到的数据              
          
创建时间:                    07-25-2022
版本:                        V1.0
修订时间:
修订内容:
*******************************************************************************/

#include "AVP32F08_Device.h"
#include "AVP32F08_Examples.h"

Uint32 MessageSendCount;
Uint32 MessageReceiveCount;

unsigned char senddata[]="HelloAdvancechip";

void Can_Tx_Test(void)
{
	int temp=0;
	while(1)
	{
		GpioDataRegs.GPBTOGGLE.bit.GPIO58 = 1;
		//判断是否发送完毕
		ECanbRegs.CANTRS.all = 0x00000001;
		 
		while(ECanbRegs.CANTA.all != 0x00000001);
		ECanbRegs.CANTA.all = 0x00000001;
		MessageSendCount++;				//在这里设断点，观察

		ECanbMboxes.MBOX0.MDL.all =senddata[temp];
		ECanbMboxes.MBOX0.MDH.all =senddata[temp+1];
		temp=temp+2;
		if(temp>16)
		  temp=0;
	}
}


void main(void)
{

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

   // 系统初始化。设置PLL、看门狗、使能外设时钟等。
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


   	/*关中断*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

	/*初始化PIE中断*/
	InitPieCtrl();

	/*初始化PIE中断矢量表*/
	InitPieVectTable();	

	/*初始化CAN寄存器*/
    InitECan();

	MessageSendCount = 0;
	MessageReceiveCount = 0;

    Can_Tx_Test();
}

//===========================================================================
// No more.
//===========================================================================
