//工程演示说明
//1.本示例演示RS485的收发功能,取下JP3 ADP32F035的GPIO28/SCIRXDA与SP3232EN的R-OUT1的连接的跳线帽,
//  短接J3P 485芯片(SP3485EN-L/TR)的485-RO与ADP32F035的GPIO28/SCIRXDA连接的跳线帽。
//2.代码执行流程:
//  2.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
//  2.2 然后是PIE中断向量表的初始化
//  2.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  2.4 最后是一个while(1)超级循环,LED1 LED2 LED3 闪灯程序,数码管按键操作程序,485接收处理程序,485发送按键信息程序;
//3.其他说明
//  3.1 RS485差分线不能直接短接做自收自发试验,否则可能损坏485收发器;
//  3.2 重要函数说明
//      函数 handle485RxFIFO()为处理485中断接收信息，并且通过数码管显示接收到的十六进制数,
//      函数 handleScanKey485()为主动发送按键信息。
//  3.3 以HEX（十六进制）发送,最大4个字节长度,每个字节的范围 00到0F
//  3.4 串口调试助手设置:115200 8N1
//4.观察实验现象
//  4.1 将仿真器与开发板连接后,编译程序并下载并运行;
//  4.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根
//      USB转串口线进行调试。
//  4.3 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"
#include "string.h"

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
     memset((void*)&SCI_Msg, 0, sizeof(SCI_Msg));
     InitLED();
     LED1_ON();
     LED2_OFF();
     Timer0_init();

     SCI_Init();
     open_uart_debug();

     Init485EN();
     start485Send(); //发送前使能控制，占用485总线

     printf("\r\n\r\nADP32F035 is Ready!");
     printf("\r\n---------------ADP32F035 485通信实验---------------");
     printf("\r\n开发板硬件配置：注意跳线帽的正确配置！！！！！");
     printf("\r\nJP2的跳线帽提供485芯片的485-RO与DSP的GPIO28/SCIRXDA连接");
     printf("\r\n请用串口调试助手，波特率%ld，数据格式8N1",SCIBaudRate);
     printf("\r\n请以HEX（十六进制）发送，最大4个字节长度，每个字节的范围 00 到 0F，数码管将依次显示接收到485总线的内容");
     printf("\r\n请按按键，485将按键信息上传");
     wait485Send(); //
     softResetIIC_BUS();         //软件复位IIC从设备
     InitI2C_Gpio();             //io 初始化为IIC
     I2CA_Init();                //HW IIC初始化，100KHz

     LigntVal = 0x71;                  //BIT6到BIT4为亮度调节，BIT0是  1 开启/0关闭
     AIP650_Send(CMD_SEG,LigntVal);   //1级亮度，开启显示
     AIP650_Send(DIG1,SEG7Table[0]);  //GID1
     AIP650_Send(DIG2,SEG7Table[0]);  //GID2
     AIP650_Send(DIG3,SEG7Table[0]);  //GID3
     AIP650_Send(DIG4,SEG7Table[0]);  //GID4

     Running.showNum =3485;//初始化数码管显示内容

     Running.showhex[3]=SEGHexBLK;
     Running.showhex[2]=SEGHexBLK;
     Running.showhex[1]=SEGHexBLK;
     Running.showhex[0]=SEGHexBLK;

     Running.key[0].ClickCNT = 0;
     Running.key[1].ClickCNT = 0;
     Running.key[2].ClickCNT = 0;
     Running.key[3].ClickCNT = 0;

     Running.key[0].pressDEF = 5;
     Running.key[1].pressDEF = 5;
     Running.key[2].pressDEF = 5;
     Running.key[3].pressDEF = 5;

     Running.key[0].pressTick = 0;
     Running.key[1].pressTick = 0;
     Running.key[2].pressTick = 0;
     Running.key[3].pressTick = 0;

     Running.nowScanKey = KEY1;


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
         SegScanTask(&Running);           //数码管按键操作函数
         handle485RxFIFO(&Running);      //485接收处理函数
         handleScanKey485(&Running);     //485发送按键信息
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

 
