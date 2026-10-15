/******************************************************************************
文件名:                    Example_AVP33xCAN_Rx.c
作者:                      湖南进芯电子科技有限公司
功能描述:  通过外部其他CAN设备给AVP32F335发送数据，了解CAN总线的设置和通讯过程，请尤其注意ID的设置
           将MBOX16设置为接收模式，并采用中断的方式接收数据,可以观察变量Rec_h，Rec_l,看看收到的数据    
          
创建时间:                    06-25-2019
版本:                        V1.0
修订时间:
修订内容:
*******************************************************************************/

#include "AVP33x_Device.h"
#include "AVP33x_Examples.h"

interrupt void eCAN0INTA_ISR(void); 

Uint32 Rec_l;
Uint32 Rec_h;


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

	EALLOW;
    PieVectTable.ECAN0INTA = &eCAN0INTA_ISR;
	EDIS;
	
	
    //使能PIE中断   
    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;
	
	//使能CPU中断
	IER |= M_INT9;
	
	EINT;   //开全局中断
	ERTM;	//开实时中断

   
    for(;;)
	{

	}
}

interrupt void eCAN0INTA_ISR(void) 
{
	while(ECanaRegs.CANRMP.all != 0x00010000 ) ;
	ECanaRegs.CANRMP.all = 0x00010000;

	//收到的数据在接收邮箱Mbox16
	Rec_l = ECanaMboxes.MBOX16.MDL.all;
	Rec_h = ECanaMboxes.MBOX16.MDH.all;

	PieCtrlRegs.PIEACK.bit.ACK9= 1;
    EINT;

}


//===========================================================================
// No more.
//===========================================================================
