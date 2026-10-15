//工程示例说明
//1.本示例演示ADP32F035通过SPI对W25Q32进行读写。
//2.代码执行流程:
//  2.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
// 	2.2 然后是PIE中断向量表的初始化
//  2.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  2.4 接着是W25Q32的初始化;
//	2.5 最后是while(1)中,串口操作SPI Flash W25Q32,LED1 LED2 LED3闪烁程序,数码管驱动程序。
//3.其他说明
//	3.1 本示例启用了FIFO的功能，SPI的FIFO具有最大4级深度,使用FIFO的好处是可以降低CPU在
//		发送和接收时等待的时间，如果这时还开启了中断的话，也可以降低中断触发的频率，这
//		是因为在没有使能FIFO的情况下，当SPI接收到一个数据后就会触发一次中断，而如果开启
//		了最大4级的接收FIFO的话，SPI就会在接收慢4个数据后才触发中断，我们只需要在接收中断
//		程序里面一次读出4个数据即可。在本示例中我们并未使用SPI中断的功能，后面的应用也不
//		会开启中断功能，因为通常SPI做为主机时使用查询模式能够较好的实现SPI通信操作。但是
//		就如上所述，在查询模式下SPI的FIFO功能也能降低CPU查询等待的时间。
//  3.2 串口调试助手设置:115200 8N1
//4.观察实验现象
//  4.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED3在以一定的频率闪烁;
//  4.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根
//      USB转串口线进行调试。
//  4.3 写入 通信格式:  0x64 + 0x06 + 3byte FLASH地址 + 3byte 数据长度 + 1byte 数据（）
//  4.4 读出 通信格式:  0x64 + 0x03 + 3byte FLASH地址 + 3byte 数据长度
//  4.5 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
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
Uint16 Beepcnt;

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
     //InitFlash();       //初始化flash
#endif

     // 第二步: 清除所有中断标志并初始化PIE中断向量表
   	 DINT;			//先禁止CPU中断
   	 InitPieCtrl();	//给PIE寄存器赋初值
   	 IER = 0x0000;	//禁止CPU的中断并清楚所有相关的中断标志
   	 IFR = 0x0000;
   	 InitPieVectTable();//初始化中断向量表

     // 第三步：初始化其他使用的外设
     InitLED();
     LED1_ON();
     LED2_OFF();
     LED4_OFF();
     Timer0_init();

     SCI_Init();
     open_uart_debug();
     printf("\r\n\r\nADP32F035 is Ready!");
     printf("\r\n--------ADP32F035 SPI操作外扩FLASH芯片W25Q32实验-------\r\n");


     printf("\r\n\r\n串口调试助手，HEX 发送命令，对FLASH进行操作");
     printf("\r\n一次发送完整一帧数据，DSP在接收最后一个字节5ms后进行通信包解析");
     printf("\r\n写入 通信格式:  0x64 + 0x06 + 3byte FLASH地址 + 3byte 数据长度 + 1byte 数据");
     printf("\r\n读出 通信格式:  0x64 + 0x03 + 3byte FLASH地址 + 3byte 数据长度");
     printf("\r\n擦除整个flash芯片 通信格式：0x64 + 0xFF");
     printf("\r\n具体操作看 handleCommFlash()函数");
     printf("\r\nFLASH 芯片有擦写的寿命，请不要频繁进行擦除操作； 读出操作不影响寿命");
     printf("\r\n32Mbit FLASH 地址最大为0x400000");


     //AIP650 芯片初始化-------------------------------------
     softResetIIC_BUS();         //软件复位IIC从设备
     InitI2C_Gpio();             //io 初始化为IIC
     I2CA_Init();                //HW IIC初始化，100KHz

     LigntVal = 0x71;                 //BIT6到BIT4为亮度调节，BIT0是  1 开启/0关闭
     AIP650_Send(CMD_SEG,LigntVal);   //1级亮度，开启显示
     AIP650_Send(DIG1,SEG7Table[0]);  //GID1
     AIP650_Send(DIG2,SEG7Table[1]);  //GID2
     AIP650_Send(DIG3,SEG7Table[2]);  //GID3
     AIP650_Send(DIG4,SEG7Table[3]);  //GID4

     Running.showNum = 2532;//初始化数码管显示内容

     Running.showhex[3]=SEGHexBLK;
     Running.showhex[2]=SEGHexBLK;
     Running.showhex[1]=SEGHexBLK;
     Running.showhex[0]=SEGHexBLK;

	 W25Q32_HardwareInit();

	 //W25Q32 ID参数
	 //ID 指令表格与读取指令的通信格式
	 SPIFlash.Jedec_ID=SpiFlashReadID();
	 printf("\r\nflash ID is 0x%x",SPIFlash.Jedec_ID);

	 if(SPIFlash.Jedec_ID==W25Q32)
	 {
	    printf("\r\n读取的flash ID与W25Q32 ID一致");
	 }
	 else
	 {
	     printf("\r\n读取的flash ID与W25Q32 ID不一致");
	 }

	 SPIFlash.SReg = SpiFlashReadSR();
	 printf("\r\nPowr ON the W25Q32 REG is %x",SPIFlash.SReg&0xff);

	 //W25Q32芯片初始化
	 SpiFlashWriteEnable();
	 SpiFlashWriteSR(0x00);//上点要设置状态，取消写保护才能写入
	 SPIFlash.SReg = SpiFlashReadSR();
	 printf("\r\nAfter Write the W25Q32 REG is %x",SPIFlash.SReg&0xff);

	 PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	 EINT;  // Enable Global interrupt INTM
	 ERTM;

	 while(1)
	 {
	     if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
	     {
	            timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;
	            if(timer0Base.msCounter >= 200)//ms
	            {
	                timer0Base.msCounter = 0;
	                LED1_TOGGLE();
	                LED2_TOGGLE();
	                LED3_TOGGLE();
	            }
	      }
	      /************************************************************************/
	      //串口操作SPI Flash W25Q32
	      //写入 通信格式:  0x64 + 0x06 + 3byte FLASH地址 + 3byte 数据长度 + 1byte 数据（）
	      //读出 通信格式:  0x64 + 0x03 + 3byte FLASH地址 + 3byte 数据长度
	      /************************************************************************/
	      if(SCI_Msg.timerOut >= RTU_TIMEROUT)
	      {
	           SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag = 0;
	           SCI_Msg.timerOut = 0;
	           SCI_Msg.Mark_Para.Status_Bits.DISRevflag = 1;

	           handleCommFlash();//FLASH芯片的操作函数

	           SCI_Msg.Mark_Para.Status_Bits.DISRevflag = 0;
	      }
	      SegScanTask(&Running);  //数码管按键操作函数------关键函数
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
