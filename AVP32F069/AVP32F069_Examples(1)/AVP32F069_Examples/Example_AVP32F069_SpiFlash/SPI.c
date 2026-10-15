#include "SPI.h"
#include "OnChip_Driver_Inc.h"

Uint16 SPIA_WriteRead_8bit(Uint16 data)
{
	while(SpiaRegs.SPIFFTX.bit.TXFFST >= 3)
	{	//如果发送FIFO里面还有3个或3个以上的数据待发送就等待
		;
	}
	SpiaRegs.SPITXBUF = (data << 8);

	while(SpiaRegs.SPIFFRX.bit.RXFFST == 0)
	{	//如果接收FIFO里面已经没有数据了就等待,直到接收到数据为止
		;
	}
	
	data = SpiaRegs.SPIRXBUF;
	return (data & 0x00FF);
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
    SpiaRegs.SPIFFCT.all=0;
}
static void SPIB_FifoInit(void)
{
	SpibRegs.SPIFFTX.all=0xE040;	//使能SPI FIFO功能,FIFO最大深度4级
    SpibRegs.SPIFFRX.all=0x2044;
    SpibRegs.SPIFFCT.all=0x0;
}

static void SPIA_GpioInit(void)
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (SPISOMIA)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
    //GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
//  GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;  // Configure GPIO5 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
//  GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 2;  // Configure GPIO3 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
    //GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    EDIS;
}

// Input		    : Uint16 spi_mode		->	配置SPI的模式(时钟和采样)
//					0					->	上升沿采集发送数据(时钟空闲为低)
//				 	1					->	下降沿采集数据(时钟空闲为低)
//				 	2					->	下降沿采集数据(时钟空闲为高)
//				 	3					->	上升沿采集发送数据(时钟空闲为高)
//				  Uint16 data_len		-> SPI数据长度,1-16可选

void SPIA_MsterConfig(Uint16 spi_mode, Uint16 data_len)
{
	//主机模式,双线全双工,数据位可选(8/16bit),MSB
	//CPOL和CPHA由参数spi_mode决定
	SPIA_GpioInit();
	SPIA_FifoInit();
	switch(spi_mode)
	{
		case 0:		//上升沿采集发送数据(时钟空闲为低)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 1; 
			break;
		case 1:		//下降沿采集数据(时钟空闲为低)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
			break;
		case 2:		//下降沿采集数据(时钟空闲为高)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		case 3:		//上升沿采集发送数据(时钟空闲为高)
			SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
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
	SpiaRegs.SPICCR.bit.SPILBK = 0;		// 1:使能内部回环测试功能,调试时使用
										// 0:关闭回环测试,正常使用
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;	// 启动SPI发送和接收
}


// Input		    : Uint16 spi_mode		->	配置SPI的模式(时钟和采样)
//					0					->	上升沿采集发送数据(时钟空闲为低)
//				 	1					->	下降沿采集数据(时钟空闲为低)
//				 	2					->	下降沿采集数据(时钟空闲为高)
//				 	3					->	上升沿采集发送数据(时钟空闲为高)
//				  Uint16 data_len		-> SPI数据长度,1-16可选
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
			SpibRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		case 1:		//下降沿采集数据(时钟空闲为低)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
			SpibRegs.SPICTL.bit.CLK_PHASE = 0;
			break;
		case 2:		//下降沿采集数据(时钟空闲为高)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpibRegs.SPICTL.bit.CLK_PHASE = 1;
			break;
		case 3:		//上升沿采集发送数据(时钟空闲为高)
			SpibRegs.SPICCR.bit.CLKPOLARITY = 1;
			SpibRegs.SPICTL.bit.CLK_PHASE = 0;
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
