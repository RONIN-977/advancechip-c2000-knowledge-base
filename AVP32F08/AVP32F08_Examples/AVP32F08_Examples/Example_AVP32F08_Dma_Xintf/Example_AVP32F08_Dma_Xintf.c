/******************************************************************************
文件名			: Ex_Dma_Xintf.c
作者  			: 湖南进芯电子科技有限公司
功能描述      	: DMA例程：
                  本例实现的功能是：将1024个字的数据从Zone7搬到SARAM的L4。
                  由定时器0来启动搬移。
                  在例程的现有配置下整个搬移过程大约耗时30s。
                  注意：这里DMA我们设置的32位数据宽度，而XINTF数据宽度为16位。
创建时间      	: 07-25-2022
版本           	: V1.0
修订时间       	:
修订内容       	:
*******************************************************************************/

#include "AVP32F08_Project.h"     // Device Headerfile and Examples Include File

#define BUF_SIZE   1024         // 数组长度定义

#pragma DATA_SECTION(DMABuf1,"DMARAML4");   // 将DMABuf1数组放到“DMARAML4”内存空间段中
#pragma DATA_SECTION(DMABuf2,"ZONE7DATA");  // 将DMABuf2数组放到“ZONE7DATA”内存空间段中(即片外SRAM中)

volatile Uint16 DMABuf1[BUF_SIZE];
volatile Uint16 DMABuf2[BUF_SIZE];

volatile Uint16 *DMADest;
volatile Uint16 *DMASource;

__interrupt void local_DINTCH1_ISR(void);
void init_zone7(void);

void main(void)
{
    Uint16 i;

	// 注意：如遇到程序加载不成功等类似相关问题，可在此处增加提高
	// 内核LDO相关语句代码，操作的寄存器为：SysCtrlRegs.VDDCORE。
	// 配置方法详见：AVP33x_SysCtrl.h文件。

// 步骤1：系统初始化。设置PLL、看门狗、使能外设时钟等。
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();
    InitSysCtrl();


// 步骤2：初始化GPIO
    // InitGpio();          			// Skipped for this example


// 步骤3：清除所有中断标志，初始化PIE中断向量表
    DINT;                   			// 禁止CPU中断
    InitPieCtrl();          			// 初始化PIE
    IER = 0x0000;           			// 禁止CPU中断
    IFR = 0x0000;           			// 清除CPU中断标志
    InitPieVectTable();     			// 初始化PIE中断向量表

    // DMA通道1的中断向量重新映射
    EALLOW;
    PieVectTable.DINTCH1 = &local_DINTCH1_ISR;
    EDIS;

    IER = M_INT7 ;           			// 使能DMA1的PIE级中断
    EnableInterrupts();
    CpuTimer0Regs.TCR.bit.TSS  = 1;  	// 停止CPU定时器0


// 步骤4：初始化所有外设
    // InitPeripherals();   			// Not required for this example
    InitCpuTimers();        			// 为本例程的用到的CPU定时器进行初始化


// 步骤5：用户自己的代码

    DMAInitialize();        			// 初始化DMA
    init_zone7();          				// 初始化zone7

    // 初始化数组，同时DMABuf1中的数据存放在DMARAML4中，DMABuf2中的数据存放在片外RAM中
    for (i=0; i<BUF_SIZE; i++)
    {
     DMABuf1[i] = 0;
     DMABuf2[i] = i;
    }

    // 配置DMA通道
    DMADest   = &DMABuf1[0];            // 设置目标地址
    DMASource = &DMABuf2[0];            // 设置源地址
    DMACH1AddrConfig(DMADest,DMASource);// 配置DMA传送的源地址和目标地址
    DMACH1BurstConfig(31,2,2);          // 一次突发传送32个数据，每次突发结束后，源地址指针和目标地址指针都增加2（这个跟DMA模式配置的设置相关的，下面的模式设置里面设置的DMA通道数据宽度为32位的，故这里每次突发传送完地址是加2的）
    DMACH1TransferConfig(31,2,2);       // 一次外环传送突发传送32次，每次突发结束后，源地址指针和目标地址指针都增加2
    DMACH1WrapConfig(0xFFFF,0,0xFFFF,0);// 设置源地址和目标地址返回时突发传送的次数，这里设置为65536，但实际上只传送1024次，故不会返回。

    //Use timer0 to start the x-fer.
	//Since this is a static copy use one shot mode, so only one trigger is needed
	//Also using 32-bit mode to decrease x-fer time
    DMACH1ModeConfig(DMA_TINT0,PERINT_ENABLE,ONESHOT_ENABLE,CONT_DISABLE,SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,THIRTYTWO_BIT,CHINT_END,CHINT_ENABLE);

	StartDMACH1();                      // 启动DMA1

	// 初始化CPU Timer0
    CpuTimer0Regs.TIM.half.LSW = 512;   // 设置CPU Timer0计数寄存器的值(如果DMA模式设置为ONESHOT_ENABLE，则这个参数不影响传输的快慢；如果设置为ONESHOT_DISABLE，则这个参数越小，传输越快)
    CpuTimer0Regs.TCR.bit.SOFT = 1;     // 仿真暂停不影响定时器工作
    CpuTimer0Regs.TCR.bit.FREE = 1;
    CpuTimer0Regs.TCR.bit.TIE  = 1;     // 使能CPU Timer0中断
    CpuTimer0Regs.TCR.bit.TSS  = 0;     // 启动CPU Timer0
    for(;;){}
}

// INT7.1
__interrupt void local_DINTCH1_ISR(void)// DMA Channel 1
{
    // 响应同组中断应答
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;

	    __asm ("      ESTOP0");
    for(;;);
}

//---------------------------------------------------------------------------
// init_zone7:
//---------------------------------------------------------------------------
// zone7的配置函数
// zone7的配置函数不能放在XINTF中，各个时序需要根据外部应用做相应调整
void init_zone7(void)
{
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1; // 使能XINTF时钟

    InitXintf16Gpio();                      // 配置相应的GPIO为XINTF脚
    EALLOW;

    // 配置所有区域的时钟为：XTIMCLK = SYSCLKOUT
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;     // XINTF模块的时钟为XTIMCLK = SYSCLKOUT
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;      // 写缓冲深度为0
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;      // 使能XCLKOUT时钟输出
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;     // 外部总线时钟输出XCLKOUT = XTIMCLK
    XintfRegs.XINTCNF2.bit.HOLD = 1;        // 禁止外部中断设备请求XINTF

    // Zone 7配置
    // 当使用异步模式（对XREADY信号采样）时，ACTIVE和LEAD两个参数都不能小于1个XIMCLK
    // 区域写时序配置
    XintfRegs.XTIMING7.bit.XWRLEAD = 1;     // 写访问的建立时间：X2TIMING=0时为1个XTIMCLK；X2TIMING=1时为2个XTIMCLK
    XintfRegs.XTIMING7.bit.XWRACTIVE = 2;   // 写访问的有效时间：X2TIMING=0时为2个XTIMCLK；X2TIMING=1时为4个XTIMCLK
    XintfRegs.XTIMING7.bit.XWRTRAIL = 1;    // 写方位的跟踪时间：X2TIMING=0时为1个XTIMCLK；X2TIMING=1时为2个XTIMCLK
    // 区域读时序配置
    XintfRegs.XTIMING7.bit.XRDLEAD = 1;     // 读访问的建立时间：X2TIMING=0时为1个XTIMCLK；X2TIMING=1时为2个XTIMCLK
    XintfRegs.XTIMING7.bit.XRDACTIVE = 3;   // 读访问的有效时间：X2TIMING=0时为3个XTIMCLK；X2TIMING=1时为6个XTIMCLK
    XintfRegs.XTIMING7.bit.XRDTRAIL = 0;    // 读访问的跟踪时间：0个XTIMCLK

    XintfRegs.XTIMING7.bit.X2TIMING = 0;    // 读/写访问的建立、有效和跟踪时间的实际值与寄存器中的设定值的比值为1:1.

    // 不对XREADY采样
    XintfRegs.XTIMING7.bit.USEREADY = 0;    // 忽略XREADY信号
    XintfRegs.XTIMING7.bit.READYMODE = 0;   // 当USEREADY=0时，不起作用。当USEREADY=1时，0：同步采样；1：异步采样。

    XintfRegs.XTIMING7.bit.XSIZE = 3;       // 数据总线宽度：0:2：无效；1：32位总线方式；3:16位总线方式，此时XA0/XWE1引脚工作在XA0功能。
    EDIS;

    //Force a pipeline flush to ensure that the write to
    //the last register configured occurs before returning.
    __asm(" RPT #7 || NOP");
}

//===========================================================================
// No more.
//===========================================================================
