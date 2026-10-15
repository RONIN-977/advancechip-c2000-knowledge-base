/******************************************************************************
文件名                     : AVP33x_Gpio.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片通用IO模块初始化及函数定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // AVP33x Headerfile Include File
#include "AVP33x_Examples.h"   // AVP33x Examples Include File

//---------------------------------------------------------------------------
// InitGpio: 
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example. 
void InitGpio(void)
{
   EALLOW;
   
   EALLOW;
   
   GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;
   GpioCtrlRegs.GPBPUD.bit.GPIO58 = 1;
   GpioDataRegs.GPBDAT.bit.GPIO58 = 0;

   EDIS;

   EDIS;

}	
	
//===========================================================================
// End of file.
//===========================================================================
