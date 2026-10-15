//工程示例说明
//1.本示例演示了CPU定时器的配置和使用方法,DSP有三个CPU定时器，分别是Timer0,
//	Timer1和Timer2。三个定时器相互独立。
//2.本示例中定时器的最小定时单位为1uS,我们在Timer0和Timer1的定时中断中分别
//  让LED1和LED2以不同的频率时间闪烁，定时时间不同，闪烁频率就不同，用户可以使
//	用示波器观察LED的闪烁频率是否和定时频率一致。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
// 	3.3 然后是PIE中断向量表的初始化
//  3.2 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里什么也不做，LED的闪烁控制代码放在定时函数中。
//4.其他说明
//  4.1 该示例开启的是三个定时器，用户可以根据需要只开启一个或两个，定时器的的
//		的启动是由这句代码实现的，如Timer0的启动：CpuTimer0Regs.TCR.bit.TSS = 0;
//	4.2 用户也可以根据需要配置定时器的定时周期，定时最小时间为1US。
//	4.3 该示例同时也演示了DSP中断的配置方法,有关PIE中断更多的描述请查阅相关手册
//		用户也可以在此基础上配置自己需要的中断。
//6.观察实验现象
//  6.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED1和LED2
//		以不同的频率在闪烁,其中LED1每隔1S闪一次,LED2每隔100MS闪一次；
//	6.2 修改定时器的定时时间，查看LED的闪烁频率变化；
//  6.3 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
void InitLED(void);
void InitBeep(void);
void InitKEY(void);
void CPU_TimerInit(void);
interrupt void Timer0_IsrHandler(void);
interrupt void Timer1_IsrHandler(void);
interrupt void Timer2_IsrHandler(void);


 //条件编译区
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
#if DEBUG_EN > 0
    //
#else
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
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
    LED3_OFF();
    LED4_OFF();
	CPU_TimerInit();//定时器初始化

    SCI_Init();
    open_uart_debug();
    printf("\r\nADP32F035 is ready!\r\n");
    printf("\r\n--------ADP32F035 CPU定时器实验例程-------\r\n");

    // 第四步: 执行系统任务
	while(1)
	{	
		//LED的闪烁控制在定时器中断中
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

static void CpuTimer_IntHandlerConfig(void)
{
	EALLOW;
 	PieVectTable.TINT0 = &Timer0_IsrHandler;
   	PieVectTable.TINT1 = &Timer1_IsrHandler;
   	PieVectTable.TINT2 = &Timer2_IsrHandler;
   	EDIS;
}

static void CpuTimer_CpuIntEn(void)
{
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   	// Enable the PIE block
  
	// Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
	// which is connected to CPU-Timer 1, and CPU int 14, which is connected
	// to CPU-Timer 2:
	IER |= M_INT1;
	IER |= M_INT13;
	IER |= M_INT14;

	// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM  									//使能全局中断
}
void CPU_TimerInit(void)
{
	CpuTimer_IntHandlerConfig();
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 60, 1000000);
   	ConfigCpuTimer(&CpuTimer1, 60, 100000);
  	ConfigCpuTimer(&CpuTimer2, 60, 1000);
	//本示例中我们使能并启动了三个定时器(Timer0-2),如果不想用某个定时器,
	//注释掉对应的使能代码即可
	CpuTimer0Regs.TCR.bit.TSS = 0; 		// 启动定时器(TSS = 0)
   	CpuTimer1Regs.TCR.bit.TSS = 0;; 	// 启动定时器(TSS = 0)
   	CpuTimer2Regs.TCR.bit.TSS = 0;; 	// 启动定时器(TSS = 0)

	CpuTimer_CpuIntEn();
}

interrupt void Timer0_IsrHandler(void)
{
   	LED1_TOGGLE();
   	CpuTimer0.InterruptCount++;
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;
	
}

interrupt void Timer1_IsrHandler(void)
{
   LED2_TOGGLE();
   CpuTimer1.InterruptCount++;
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

interrupt void Timer2_IsrHandler(void)
{ 
   CpuTimer2.InterruptCount++;
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}
//===========================================================================
// No more.
//===========================================================================

