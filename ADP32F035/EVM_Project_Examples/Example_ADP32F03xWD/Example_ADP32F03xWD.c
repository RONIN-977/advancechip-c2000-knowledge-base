//工程演示说明
//1.本示例演示ADP32F035 看门狗的基本使用方法,包括看门狗溢出后,软件中断和软件复位;
//2.本示例中正常情况下会定时喂狗,按键K1按下,执行长时间延时函数(共计约1s),不喂狗,ADP32F035看门狗将会产生溢出,
//  看门狗设置的溢出时间为839ms。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟;
//  3.3 然后是PIE中断向量表的初始化;
//  3.2 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,有按键扫描,并且通过CPU定时器0,每隔200ms对看门狗进行喂狗。
//4.其他说明
//  4.1 主要函数在 watchDog.c 里的WatchDogInit_HWRST(void),WatchDogInit_SWIsrRST(void)和interrupt void WatchDog_isr(void);
//  4.2 WatchDogInit_HWRST(void),看门狗初始化函数,采用WDRST触发硬件复位,不采用WDINT软件中断;
//  4.3 WatchDogInit_SWIsrRST(void)和interrupt void WatchDog_isr(void),
//      看门狗初始化函数采用WDINT软件中断函数，不采用直接硬件复位中断。
//  4.4 #define HWRST 1时,使能WDRST触发硬件复位,不采用WDINT软件中断;
//  4.5 #define HWRST 0时,使能WDINT软件中断函数,不采用直接硬件复位中断;
//  4.6 串口调试助手设置:115200 8N1。
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载,点击运行,LED1 LED2 LED3闪烁;
//  5.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根USB转串口线进行调试;
//  5.3 使能WDRST触发硬件复位时,按键K1按下,LED4由灭变亮,ADP32F035复位后,LED1 LED2 LED3继续闪烁,LED4由亮变灭,用示波器
//      测量ADP32F035的XRSn脚,可以看到有低电平出现。
//  5.4 使能WDINT软件中断函数时，按键K1按下LED4由灭变亮,串口打印两组信息,printf("\r\nWD Isr Cnt(%d)",WD_SW_IsrCnt)
//      printf("\r\nWD Isr SW Reset")后,强制跳转到入口地址，DSP复位;
//  5.5 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//宏定义区
#define HWRST 1

//函数声明区
void InitLED(void);
void InitBeep(void);
void InitKEY(void);

//预编译区
#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区
Uint16 Beepcnt=0;

//主函数入口处
void main(void) 
{
    Uint16 i;

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
     InitKEY();
     InitLED();
     LED1_ON();
     LED2_OFF();
     LED4_OFF();
     Timer0_init();
     SCI_Init();       //串口初始化，128000  8N1
     open_uart_debug();

#if  HWRST
     WatchDogInit_HWRST();//硬件直接复位
#else
     WatchDogInit_SWIsrRST();
#endif


     printf("\r\nADP32F035 is Ready!");
     printf("\r\n--------ADP32F035看门狗的基本使用方法-------");
     ServiceDog();
     printf("\r\n选择 WatchDogInit_SWIsrRST()进行看门狗溢出软件中断试验");
     printf("\r\n或者选择 WatchDogInit_HWRST()进行看门狗溢出硬件复位试验");
     printf("\r\n请按K1，进入长时间延时，看门狗将进行复位.....");
     ServiceDog();

     //上电或者复位后运行指示
     for(i=0;i<10;i++)
     {
         LED2_TOGGLE();
         ServiceDog();
         Delay_nMS(100);//100MS
     }


     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
     EINT;  // Enable Global interrupt INTM
     ERTM;

     while(1)
     {
         if(timer0Base.msCounter >= 200)//ms
         {
             timer0Base.msCounter = 0;

             ServiceDog();//正常喂狗

             //普通LED交互闪烁
             LED1_TOGGLE();
             LED2_TOGGLE();
             LED3_TOGGLE();

             //按键K1按下，这里做长时间延时，不喂狗，DSP看门狗将会产生溢出
             if(rK1_DAT()==0)
             {
                 Delay_nMS(500000);//单位us  500ms
                 LED4_TOGGLE();
                 Delay_nMS(500000);// 500ms  由于看门狗设置为 839ms 周期，故这里的delay函数执行时进行复位
                 Delay_nMS(500000);
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

 
