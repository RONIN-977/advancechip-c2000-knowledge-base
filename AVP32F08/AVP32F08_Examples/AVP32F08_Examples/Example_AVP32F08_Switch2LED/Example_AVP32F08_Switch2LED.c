/******************************************************************************
文件名			: Ex_Switch2LED.c
作者      		: 湖南进芯电子科技有限公司
功能描述     	: 开关控制LED例程：
                  本例实现的功能是：通过开发板上的开关控制LED灯的亮灭。
                  按键与GPIO的对应关系：
                	- S3:GPIO60
                	- S4:GPIO61
                	- S5:GPIO12
                	- S6:GPIO13
             	  LED灯与GPIO的对应关系：
                	- DS3:GPIO27
                	- DS4:GPIO26
                	- DS5:GPIO25
                	- DS6:GPIO24
                  按键与LED的控制关系：
                	- S3控制DS3
                	- S4控制DS4
                	- S5控制DS5
                	- S6控制DS6
创建时间		: 07-25-2022
版本			: V1.0
修订时间    	:
修订内容    	:
*******************************************************************************/

#include "AVP32F08_Project.h"     // Device Headerfile and Examples Include File

void main(void)
{

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

// 步骤1：系统初始化。设置PLL、看门狗、使能外设时钟等。
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          // Skipped for this example
    EALLOW;
    // 配置为通用IO口
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;

    // 配置输入输出方向。0：输入；1：输出。
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0;
    EDIS;


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   // 禁止CPU中断
    InitPieCtrl();          // 初始化PIE
    IER = 0x0000;           // 禁止CPU中断
    IFR = 0x0000;           // 清除CPU中断标志
    InitPieVectTable();     // 初始化PIE中断向量表


// 步骤4：初始化所有外设
    // InitPeripherals();   // Not required for this example


// 步骤5：用户自己的代码


// 步骤6：空循环 （可选）
    for(;;)
    {
        if(GpioDataRegs.GPADAT.bit.GPIO12 == 1)
            GpioDataRegs.GPASET.bit.GPIO25 = 1;
        else
            GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;

        if(GpioDataRegs.GPADAT.bit.GPIO13 == 1)
            GpioDataRegs.GPASET.bit.GPIO24 = 1;
        else
            GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;

        if(GpioDataRegs.GPBDAT.bit.GPIO60 == 1)
            GpioDataRegs.GPASET.bit.GPIO27 = 1;
        else
            GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;

        if(GpioDataRegs.GPBDAT.bit.GPIO61 == 1)
            GpioDataRegs.GPASET.bit.GPIO26 = 1;
        else
            GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;

   /* GpioDataRegs.GPATOGGLE.bit.GPIO25 = 1;
    DELAY_US(1L);
    GpioDataRegs.GPATOGGLE.bit.GPIO24 = 1;
    DELAY_US(1L);
    GpioDataRegs.GPATOGGLE.bit.GPIO26 = 1;
    DELAY_US(1L);
    GpioDataRegs.GPATOGGLE.bit.GPIO27 = 1;
        DELAY_US(500000L);*/
    }
}


//===========================================================================
// No more.
//===========================================================================
