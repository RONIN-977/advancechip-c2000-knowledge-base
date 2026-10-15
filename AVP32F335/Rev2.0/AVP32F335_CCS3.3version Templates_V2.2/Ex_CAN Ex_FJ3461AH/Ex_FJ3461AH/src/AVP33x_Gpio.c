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
void InitGpio(void)
{
   EALLOW;
   
   
   //GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;
   GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;
   GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
   //GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;
   GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;
   GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;

   //GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1;
   GpioCtrlRegs.GPBPUD.bit.GPIO35 = 1;
   GpioCtrlRegs.GPAPUD.bit.GPIO30 = 1;


   EDIS;

}	
	
//===========================================================================
// End of file.
//===========================================================================
