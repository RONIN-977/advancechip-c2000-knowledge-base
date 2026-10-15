//工程示例说明
//1.本示例演示ADP32F036的外中断配置和使用方法。ADP32F036一共有3个外中断，这三个外中断的
//	的中断触发源可配置为GPIO0-GPIO31之间的任意一个；本示例中，我便将GPIO16作为XINT1
//	的中断触发沿，如果按键按下，一个下降沿将触发进入中断，在中断中，我们将让LED的状态
//	翻转一次；
//2.外中断XINT可以配置为上升沿触发，下降沿触，双边触发(上升沿和下降沿都触发)。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
// 	3.3 然后是PIE中断向量表的初始化
//		还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里我们什么都不做，LED的状态反正将在外中断执行程序中实现。
//4.其他说明
//  4.1 注意XINT1-XINT3只能分配从GPIO0-GPIO31之间的引脚，如果用户自行设计硬件时，欲使用外中断
//		功能的引脚必须安排在这之间。
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载,点击运行;
//	5.2 按下K1后可以看到LED的状态会翻转一次，D2之前是亮的，那么会熄，
//		反之亦然。
//  5.3 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
void InitLED(void);
void InitKEY(void);
interrupt void Xint1_IsrHander(void);
interrupt void Xint2_IsrHander(void);
interrupt void Xint3_IsrHander(void);
void Xint_Init(Uint16 Xintx, Uint16 gpiox, Uint16 polarity);

//预编译区
#if DEBUG_EN	> 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区


//主函数入口处
void main(void)
{
    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
   	InitSysCtrl();

    // 第二步: 清除所有中断标志并初始化PIE中断向量表
   	DINT;			//先禁止CPU中断
   	InitPieCtrl();	//给PIE寄存器赋初值
   	IER = 0x0000;	//禁止CPU的中断并清楚所有相关的中断标志
   	IFR = 0x0000;
   	InitPieVectTable();//初始化中断向量表
    // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,否则进入Flash固化模式,
    // 即要执行flash的初始化并将设定的部分在RAM里面运行的代码拷贝到flash运行
#if DEBUG_EN	> 0
	//
#else
 	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   	InitFlash();			//初始化flash
#endif

    // 第三步：初始化其他使用的外设
   	InitLED();

   	InitKEY();
	Xint_Init(1, 16, 0);	//初始化XINT1,中断引脚GPIO16,下降沿触发

    Timer0_init();//定时器0初始化，1ms周期，采用定时器周期中断方式

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
    EINT;                                 // Enable Global interrupt INTM
    ERTM;

    // 第四步: 执行系统任务
	while(1)
	{
	    if(timer0Base.msCounter >= 200)//ms 这里只是做测试选取200ms为一个周期
	    {
	       timer0Base.msCounter = 0;
	      // LED1_TOGGLE();
	     }
	}
}

void InitKEY(void)
{
    EALLOW;
    K1_MUX = 0;//GPIO功能
    K1_DIR = 0;//输入
    K1_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉



    EDIS;

}

void InitLED(void)
{
    EALLOW;
    //4个普通LED
    LED1_MUX = 0;//GPIO功能
    LED1_DIR = 1;//输出

    EDIS;
}



//外中断初始化函数
//Xintn:	欲初始化的中断(1-3)
//gpiox:	中断引脚(0-31之间)
//polarity：触发极性:
//			0->下降沿;
//			1->上升沿; 
//			3->双边触发
void Xint_Init(Uint16 Xintx, Uint16 gpiox, Uint16 polarity)
{
	if((Xintx > 3) || (Xintx < 1))
	{
		return;
	}

	if(gpiox > 31)
	{	//外中断只能选择GPIO0-GPIO31之间作为中断引脚
		return;
	}
	//重新分配中断入口函数
	EALLOW;
	PieVectTable.XINT1 = &Xint1_IsrHander;
   	PieVectTable.XINT2 = &Xint2_IsrHander;
	PieVectTable.XINT3 = &Xint3_IsrHander;
   	EDIS;
	
	if(Xintx == 3)
	{
		IER |= M_INT12;                       	// 使能INT12分组(XINT3在该组中)
	}
	else
	{
		IER |= M_INT1;                      	// 使能INT1分组(XINT1和XINT2在该组中)
	}
   	//分别配置各外中断                          		
	switch(Xintx)
	{
		case 1:
			PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          	// Enable PIE Gropu 1 INT4
			
			EALLOW;
   			GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = gpiox;   // 配置中断引脚
			EDIS;

			XIntruptRegs.XINT1CR.bit.POLARITY = polarity;	//配置触发极性
			
			XIntruptRegs.XINT1CR.bit.ENABLE = 1;        	// 使能中断
		break;

		case 2:
			PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          	// Enable PIE Gropu 1 INT5
			
			EALLOW;
   			GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = gpiox;   // 配置中断引脚
			EDIS;

			XIntruptRegs.XINT2CR.bit.POLARITY = polarity;	//配置触发极性
			
			XIntruptRegs.XINT2CR.bit.ENABLE = 1;        	// 使能中断
		break;

		case 3:
			PieCtrlRegs.PIEIER12.bit.INTx1 = 1;          	// Enable PIE Gropu 12 INT1
				
			EALLOW;
			GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = gpiox;	// 配置中断引脚
			EDIS;

			XIntruptRegs.XINT3CR.bit.POLARITY = polarity;	//配置触发极性
			
			XIntruptRegs.XINT3CR.bit.ENABLE = 1;        	// 使能中断
		break;

		default:
		break;
	}

}
//中断服务程序
interrupt void Xint1_IsrHander(void)
{
	Delay_nMS(10);								//延时10MS，按键消抖
	if(rK2_DAT() == 0)
	{
	  	LED1_TOGGLE();
	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void Xint2_IsrHander(void)
{
	//添加用户代码


	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
interrupt void Xint3_IsrHander(void)
{
	//添加用户代码

	
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
//===========================================================================
// No more.
//===========================================================================

