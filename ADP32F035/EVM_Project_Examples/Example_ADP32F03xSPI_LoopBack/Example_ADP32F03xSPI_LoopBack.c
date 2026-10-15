//工程示例说明
//1.本示例演示DSP的SPI通信功能，使用的是SPI内部回环测试，即通过将
//	SpibRegs.SPICCR.bit.SPILBK位置1后，可以使SPIA的MOSI和MISO在内部相连接，
//	达到自发自收的目的，在回环测试模式下，SCK和MOSI也是有波形输出的，用户可以通
//	过示波器查看它们的波形，可以观察验证SPI配置的频率和通信长度；
//2.本示例封装的SPI初始化函数可以配置SPI的时钟极性,数据采样沿和数据通信长
//	度(1-16位可选)，SPI使用的低速时钟LSPCLK，如果系统配置的是15MHZ(主时钟4分频)
//	那么当BRR为4时，实际的SPI速度是15/(4+1) = 3MHZ，关于SPI速度的配置可以查阅
//	SPI部分的数据手册。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
// 	3.3 然后是PIE中断向量表的初始化
//  3.2 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里调用的是SPI读写函数，即SPI发送一个数据后再读回一个数据。
//		可以在变量观察窗口查看sdata和rdata的值。
//4.其他说明
//  4.1 该示例演示的是SPI自发自收功能。
//	4.2 本示例启用了FIFO的功能，SPI的FIFO具有最大4级深度,使用FIFO的好处是可以降低CPU在
//		发送和接收时等待的时间，如果这时还开启了中断的话，也可以降低中断触发的频率，这
//		是因为在没有使能FIFO的情况下，当SPI接收到一个数据后就会触发一次中断，而如果开启
//		了最大4级的接收FIFO的话，SPI就会在接收满4个数据后才触发中断，我们只需要在接收中断
//		程序里面一次读出4个数据即可。在本示例中我们并未使用SPI中断的功能，后面的应用也不
//		会开启中断功能，因为通常SPI做为主机时使用查询模式能够较好的实现SPI通信操作。但是
//		就如上所述，在查询模式下SPI的FIFO功能也能降低CPU查询等待的时间。
//6.观察实验现象
//  6.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED1指示灯以
//		一定频率闪烁，在“LED1_TOGGLE()”代码处设置断点观察rdata和sdata的值变化情况，
//		我们可以发现rdata和sdata的值在增加，且rdata的值总是比sdata的值小1。
//	6.2 可以使用示波器查看SCKA和MOSIA的波形，观察频率和数据长度是否如配置的一致；
//	6.3 测试一下SPIA_SpeedSet()函数，改几个不同的参数，看SPI的传输速度是否
//		变化一致。
//	6.4 我们用GPIO15来模拟片选,在很多应用中，片选都是用的是软件控制IO实现的，而非使用
//		SPI自己的硬件片选SPISTE。但是在本示例中，大家可以通过示波器捕捉到该硬件片选
//		信号(GPIO19)，软件片选相对于硬件片选来说具有较高的灵活性，比如有时我们要在一个
//		SPI读写过程中连续读写多个字节，而保持片选始终为低，如果使用硬件片选是无法做到的
//		(硬件片选只能保证读一次拉低一次然后拉高，下一次读再拉低，读完一个字节就拉高)。
//		下一个W25Q32JVSSIQ，就属于这种情况，所以我们将在那里禁用SPISTEA的功能，而是
//		将其当成一个普通IO模拟片选。
//  6.5 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
void InitLED(void);
void InitBeep(void);
void InitKEY(void);
void SPIA_MsterConfig(Uint16 spi_mode, Uint16 data_len);
void SPIB_MsterConfig(Uint16 spi_mode, Uint16 data_len);
Uint16 SPIA_WriteRead(Uint16 data);
Uint16 SPIB_WriteRead(Uint16 data);
void SPIA_SpeedSet(Uint16 brr);
void SPIB_SpeedSet(Uint16 brr);

//条件编译区
#if DEBUG_EN	> 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区
Uint16 sdata=0;  // send data
Uint16 rdata=0;  // received data

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

	SPIA_MsterConfig(0,16);
	SPIA_SpeedSet(14);			//SPI速度 = 15/(14+1) = 1MHZ

    SCI_Init();
    open_uart_debug();
    printf("\r\nADP32F035 is ready!\r\n");
    printf("\r\n--------ADP32F035 SPI_LoopBack实验例程-------\r\n");

    // 第四步: 执行系统任务
	sdata = 0x0000;	
	while(1)
	{	
		LED2_ON();	//模拟片选信号拉低
		rdata = SPIA_WriteRead(sdata++);
		printf("sdata:%d--------------",sdata);
		printf("rdata:%d",rdata);
		LED2_OFF();	//模拟片选信号拉高
		LED1_TOGGLE();			//在这里设置断点，观察rdata和sdata的值变化情况
		Delay_nMS(200);
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

Uint16 SPIA_WriteRead(Uint16 data)
{
	while(SpiaRegs.SPIFFTX.bit.TXFFST >= 3)
	{	//如果发送FIFO里面还有3个或3个以上的数据待发送就等待
		;
	}
	SpiaRegs.SPITXBUF = data;

	while(SpiaRegs.SPIFFRX.bit.RXFFST == 0)
	{	//如果接收FIFO里面已经没有数据了就等待,直到接收到数据为止
		;
	}
	
	data = SpiaRegs.SPIRXBUF;
	return data;
}


Uint16 SPIB_WriteRead(Uint16 data)
{
	while(SpibRegs.SPIFFTX.bit.TXFFST >= 3)
	{	//如果发送FIFO里面还有3个或3个以上的数据待发送就等待
		;
	}
	SpibRegs.SPITXBUF = data;

	while(SpibRegs.SPIFFRX.bit.RXFFST == 0)
	{	//如果接收FIFO里面已经没有数据了就等待,直到接收到数据为止
		;
	}
	
	data = SpibRegs.SPIRXBUF;
	return data;
}

static void SPIA_FifoInit(void)
{
	SpiaRegs.SPIFFTX.all=0xE040;	//使能SPI FIFO功能,FIFO最大深度4级
    SpiaRegs.SPIFFRX.all=0x2044;	
    SpiaRegs.SPIFFCT.all=0x0;
}
static void SPIB_FifoInit(void)
{
	SpibRegs.SPIFFTX.all=0xE040;	//使能SPI FIFO功能,FIFO最大深度4级
    SpibRegs.SPIFFRX.all=0x2044;
    SpibRegs.SPIFFCT.all=0x0;
}

void SPIA_MsterConfig(Uint16 spi_mode, Uint16 data_len)
{
	//主机模式,双线全双工,数据位可选(8/16bit),MSB
	//CPOL和CPHA由参数spi_mode决定
	InitSpiaGpio();	//SPIA 使用引脚初始化
	SPIA_FifoInit();
	switch(spi_mode)
	{
		case 0:		//上升沿采集发送数据(时钟空闲为低)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
			break;
		case 1:		//下降沿采集数据(时钟空闲为低)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		case 2:		//下降沿采集数据(时钟空闲为高)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
			break;
		case 3:		//上升沿采集发送数据(时钟空闲为高)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		default:
			break;
	}
	
	SpiaRegs.SPIPRI.bit.TRIWIRE = 0;		// 4线全双工模式
	SpiaRegs.SPIPRI.bit.FREE = 1;			// 断点不影响SPI传输
	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;	// 1:主机模式;0:从机
	SpiaRegs.SPICTL.bit.TALK = 1;			// 使能发送(否则MOSI将为高阻态)
	//数据长度配置1-16可选
	SpiaRegs.SPICCR.bit.SPICHAR = (data_len&0x000F) - 1;
				
	SpiaRegs.SPIBRR = 100;		// BRR >= 3:		速度 = LSPCLK/(BRR+1)
								// BRR = 0\1\2：	速度 = LSPCLK/4
	SpiaRegs.SPICCR.bit.SPILBK = 1;		// 1:使能内部回环测试功能,调试时使用
										// 0:关闭回环测试,正常使用
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;	// 启动SPI发送和接收
}



void SPIB_MsterConfig(Uint16 spi_mode, Uint16 data_len)
{
	//主机模式,双线全双工,数据位可选(8/16bit),MSB
	//CPOL和CPHA由参数spi_mode决定
	InitSpibGpio();	//SPIB 使用引脚初始化
	SPIB_FifoInit();
	switch(spi_mode)
	{
		case 0:		//上升沿采集发送数据(时钟空闲为低)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpibRegs.SPICTL.bit.CLK_PHASE = 0;
			break;
		case 1:		//下降沿采集数据(时钟空闲为低)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpibRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		case 2:		//下降沿采集数据(时钟空闲为高)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpibRegs.SPICTL.bit.CLK_PHASE = 0;
			break;
		case 3:		//上升沿采集发送数据(时钟空闲为高)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpibRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		default:
			break;
	}
	
	SpibRegs.SPIPRI.bit.TRIWIRE = 0;		// 4线全双工模式
	SpibRegs.SPIPRI.bit.FREE = 1;			// 断点不影响SPI传输
	SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;	// 1:主机模式;0:从机
	SpibRegs.SPICTL.bit.TALK = 1;			// 使能发送(否则MOSI将为高阻态)
	//数据长度配置1-16可选
	SpibRegs.SPICCR.bit.SPICHAR = (data_len&0x000F) - 1;
	
	SpibRegs.SPIBRR = 100;		// BRR >= 3:		速度 = LSPCLK/(BRR+1)
								// BRR = 0\1\2：	速度 = LSPCLK/4
	SpibRegs.SPICCR.bit.SPILBK = 1;		// 1:使能内部回环测试功能,调试时使用
										// 0:关闭回环测试,正常使用
	SpibRegs.SPICCR.bit.SPISWRESET = 1;	// 启动SPI发送和接收
}

// 速度设置函数,注意传入的是分频项
// 当BRR >= 3:		SPI实际速度 = LSPCLK/(brr+1)
// 当BRR = 0\1\2：	SPI实际速度 = LSPCLK/4
void SPIA_SpeedSet(Uint16 brr)
{
	SpiaRegs.SPIBRR = (brr & 0x7E);	//最大分频数127
}
// 速度设置函数,注意传入的是分频项
// 当BRR >= 3:		SPI实际速度 = LSPCLK/(brr+1)
// 当BRR = 0\1\2：	SPI实际速度 = LSPCLK/4
void SPIB_SpeedSet(Uint16 brr)
{
	SpibRegs.SPIBRR = (brr & 0x7E);	//最大分频数127
}
//===========================================================================
// No more.
//===========================================================================

