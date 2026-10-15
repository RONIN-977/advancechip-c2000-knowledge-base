//工程示例说明
//1.本示例演示GPIO的操作，LED1每隔200ms闪烁一次，并且通过对一个按键状态的检测,并通过相应的按键状态来控制LED2灯的亮灭；
//2.本示例中使用的按键是K1,它的GPIO端口是GPIO39，属于GPIOB。GPIO39硬件上有一个上拉电阻，所以按键未按下时GPIO39为高电平，
//  按下后为低电平。GPIO13控制LED1，GPIO14控制LED2，属于GPIOA。更多 GPIO方面的知识请参考GPIO阐述部分的数据手册。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
// 	3.2 然后是PIE中断向量表的初始化
//  3.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里实现按键的检测和LED的亮灭控制。
//4.其他说明
//  本例程中使用了了DSP的 CPU定时器和SCI操作。
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED1以一定的频率闪烁;
//	5.2 按下K1后可以看到LED2`点亮,释放按键后灯熄灭。
//	5.3 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//		如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//		固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
void delay_loop(void);
void LED_example(void);
void InitLED(void);
void InitBeep(void);
void InitKEY(void);

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

    // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,否则进入Flash固化模式,
    // 即要执行flash的初始化并将设定的部分在RAM里面运行的代码拷贝到flash运行
#if DEBUG_EN    > 0
    //
#else
    MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
    //InitFlash();        //初始化flash
#endif

    // 第二步: 清除所有中断标志并初始化PIE中断向量表
   	DINT;			//先禁止CPU中断
   	InitPieCtrl();	//给PIE寄存器赋初值
   	IER = 0x0000;	//禁止CPU的中断并清楚所有相关的中断标志
   	IFR = 0x0000;
   	InitPieVectTable();//初始化中断向量表

    // 第三步：初始化其他使用的外设
   	InitLED();
   	InitKEY();

    SCI_Init();     //初始化SCIA（占用GPIO28---SCI_RX ,  GPIO29_SCI_TX），波特率115200bps，接收采用中断机制
    open_uart_debug(); //printf 函数连接，固定用法，将底层发送函数放置于 my_write 函数内即可； 串口助手采用  ASCII 格式显示

    //采用标准C语言printf函数  发送字符串测试
    printf("\r\n\r\nADP32F035 is Ready!");
    printf("\r\n--------ADP32F035 GPIO实验例程-------\r\n");
    printf("\r\n当按下K1键时，LED2状态亮,松开K1键时，LED2灭 。");


    Timer0_init();//定时器0初始化，1ms周期，采用定时器周期中断方式

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
    EINT;                                 // Enable Global interrupt INTM
    ERTM;

	LED1_ON();
	Delay_nMS(100);
	LED1_OFF();
	Delay_nMS(100);

	LED1_ON();
	Delay_nMS(100);
	LED1_OFF();

    // 第四步: 执行系统任务
	while(1)
	{
	    if(timer0Base.msCounter >= 200)//ms 这里只是做测试选取200ms为一个周期
	    {
	       timer0Base.msCounter = 0;
	       LED1_TOGGLE();

	       if(rK1_DAT() == 0)
		   {
			   LED2_ON();
		   }
		   else
		   {
		       LED2_OFF();
		   }
		}
	}
} 	

void delay_loop(void)
{	//延时
	Uint32 i;
    for (i = 0; i < 100000; i++) 
    {
    	;//执行空操作
    }
}

void InitKEY(void)
{
    EALLOW;
    K1_MUX = 0;//GPIO功能
    K1_DIR = 0;//输入
    K1_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉

    K2_MUX = 0;//GPIO功能
    K2_DIR = 0;//输入
    K2_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉

    K3_MUX = 0;//GPIO功能
    K3_DIR = 0;//输入
    K4_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉

    K4_MUX = 0;//GPIO功能
    K4_DIR = 0;//输入
    K4_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉

    EDIS;

}

void InitLED(void)
{
    EALLOW;
    //4个普通LED
    LED1_MUX = 0;//GPIO功能
    LED1_DIR = 1;//输出

    LED2_MUX = 0;
    LED2_DIR = 1;

    LED3_MUX = 0;
    LED3_DIR = 1;

    LED4_MUX = 0;
    LED4_DIR = 1;

    EDIS;
}

void InitBeep(void)
{
    BEEP_OFF();//默认关闭蜂鸣器输出
    EALLOW;
    BEEP_MUX = 0;//普通IO功能
    BEEP_DIR = 1;   //输出
    EDIS;
}

//===========================================================================
// No more.
//===========================================================================

