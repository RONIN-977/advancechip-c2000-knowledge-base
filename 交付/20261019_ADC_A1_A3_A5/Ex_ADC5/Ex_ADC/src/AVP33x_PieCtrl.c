/******************************************************************************
文件名                     : AVP33x_PieCtrl.c
作者                         : 湖南进芯电子科技有限公司
功能描述                 : PIE控制寄存器初始化
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // AVP33x Headerfile Include File
#include "AVP33x_Examples.h"   // AVP33x Examples Include File

//---------------------------------------------------------------------------
// InitPieCtrl: 
//---------------------------------------------------------------------------
// 初始化PIE控制寄存器
//
void InitPieCtrl(void)
{
    // 禁止CPU级中断
    DINT;

    // 禁止PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

	// 清除所有PIE中断使能寄存器PIEIER
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;	
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	// 清除所有PIE中断标志寄存器PIEIFR
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;	
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;
}	

//---------------------------------------------------------------------------
// EnableInterrupts: 
//---------------------------------------------------------------------------
// 中断使能函数
//
void EnableInterrupts()
{
    // 使能PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    		
	// 让PIE模块产生一个脉冲给CPU中断，使得CPU可以响应相应的中断
	PieCtrlRegs.PIEACK.all = 0xFFFF;  

	// 使能CPU中断
    EINT;
}

//===========================================================================
// End of file.
//===========================================================================
