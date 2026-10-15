//工程示例说明
//1.本示例实现片上AD的基本使用方法,AD采样的电压值通过SCIA打印出来;
//2.本示例中使用片上AD端口是ADCINB6,通过外部可调电阻改变ADCINB6输入口的电压。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟;
//  3.2 然后是PIE中断向量表的初始化;
//  3.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里通过CPU定时器0,每隔1s软件触发ADC和驱动数码管显示采样的电压值。
//4.其他说明
//  4.1 主要函数在 ad.c 里的  void ADC_Config(void)  和 interrupt void adc_isr(void);
//  4.2 AD配置为软件触发;
//  4.3 在AD中断里打印采样的值;
//  4.4 串口调试助手设置:115200 8N1。
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到数码管显示采样的电压值，调节外部可调电阻，数码管显示的电压值也随之改变，LED1、LED2、LED3以大约1Hz的频率闪烁;
//  5.2 将EVM板上的sci与USB转串口线进行调试;
//  5.3 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
void InitLED(void);
void InitKEY(void);

//预编译区
#if DEBUG_EN    > 0
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

     // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,
     // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
     // RAM里面运行的代码拷贝到flash运行
#if DEBUG_EN    > 0
     //
#else
     MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
     //InitFlash();            //初始化flash
#endif

     // 第二步: 清除所有中断标志并初始化PIE中断向量表
     DINT;           //先禁止CPU中断
     InitPieCtrl();  //给PIE寄存器赋初值
     IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
     IFR = 0x0000;
     InitPieVectTable();//初始化中断向量表


    // 第三步：初始化其他使用的外设
     InitLED();
     LED1_ON();
     Timer0_init();

     SCI_Init();
     open_uart_debug();
     printf("\r\n\r\nADP32F036 is ready!");
     printf("\r\n--------ADP32F036片上AD的基本使用方法-------\r\n");

     ADC_Config();

     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
     EINT;  // Enable Global interrupt INTM
     ERTM;

     while(1)
     {
          if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
          {
              timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;

              if(timer0Base.msCounter >= 1000)//ms
              {
                  timer0Base.msCounter = 0;
                  LED1_TOGGLE();
                  AdcRegs.ADCSOCFRC1.all = 0X000F; //软件触发AD 的 SOC0--SOC3采样
              }
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
    //1个普通LED
    LED1_MUX = 0;//GPIO功能
    LED1_DIR = 1;//输出



    EDIS;
}



//===========================================================================
// No more.
//===========================================================================

 
