//工程演示说明
//工程演示说明
//1.本示例演示ADP32F035通过IIC对AIP650进行驱动来驱动数码管 显示。
//2.代码执行流程:
//  2.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
//  2.2 然后是PIE中断向量表的初始化
//  2.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  2.4 接着是SCI GPIO IIC等的初始化;
//  2.5 最后是while(1)中IIC操作AIP650,LED1 LED2 LED3 LED4交替闪烁。
//3.其他说明
//  3.1 AIP650为IIC接口的数码管专用芯片,关于标准IIC的硬件地址和AIP650的控制字（操作地址）的理解:
//      标准IIC,将地址赋予ADP32F035的IIC控制器时,IIC控制器先发送7bit的物理地址,再 读/写 位,
//      故看起来是7bit的物理地址即为左移了一位，BIT7到BIT1为物理地址位，BIT0为 读写位
//      AIP650的几个操作地址，需要右移一位处理，即可实现硬件IIC的操作。
//      #define CMD_SEG  (0X48)>>1  //显示模式 ADDR
//      #define DIG1  (0X68)>>1 //DIG1 ADDR
//      #define DIG2  (0X6A)>>1 //DIG2 ADDR
//      #define DIG3  (0X6C)>>1 //DIG3 ADDR
//      #define DIG4  (0X6E)>>1 //DIG4 ADDR
//  3.2 重要函数
//      写入操作AIP650_Send()
//  3.3 串口调试助手设置：115200 8N1
//4.观察实验现象
//  4.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED3在以一定的频率闪烁;
//  4.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根
//      USB转串口线进行调试。
//  4.3 LED1 LED2 LED3 LED4交替闪烁，4位SEG上的数字由9999每隔180ms减1,当减至0时,
//      4位SEG上的数字又由9999每隔180ms减1,依次往复。
//  4.4 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
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
#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区
int Tmp=0;
char keyReg=0;
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
     InitLED();
     SCI_Init();     //初始化SCIA（占用GPIO28---SCI_RX ,  GPIO29_SCI_TX），波特率115200bps，接收采用中断机制
     open_uart_debug(); //printf 函数连接，固定用法，将底层发送函数放置于 my_write 函数内即可； 串口助手采用  ASCII 格式显示

     //采用标准C语言printf函数  发送字符串测试
     printf("\r\n\r\nADP32F035 is Ready!");
     printf("\r\n--------ADP32F035 IIC控制AIP650驱动数码管例程-------\r\n");

     Timer0_init();//定时器0初始化，1ms周期，采用定时器周期中断方式

     //---------------------AIP650初始化  IIC驱动--------------------------------
     softResetIIC_BUS(); //软件复位IIC从设备
     InitI2C_Gpio(); //IO 初始化为IIC，GPIO32-SDA   GPIO33-SCL
     I2CA_Init();    //硬件 IIC初始化，100KHz


     printf("\r\nAIP650 init...OK");
     printf("\r\nAIP650控制4位数码管显示");

     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
     EINT;                                 // Enable Global interrupt INTM
     ERTM;

     LigntVal = 0x71; //BIT6到BIT4为亮度调节，BIT0是  1 开启/0关闭
     AIP650_Send(CMD_SEG, LigntVal); //设定亮度
     Tmp = 0;
     AIP650_Send(DIG4,SEG7Table[Tmp/1000]);   //DIG4
     AIP650_Send(DIG3,SEG7Table[Tmp/100%10]); //DIG2
     AIP650_Send(DIG2,SEG7Table[(Tmp/10)%10]);//DIG3
     AIP650_Send(DIG1,SEG7Table[Tmp%10]);     //DIG4


     while(1)
     {
         if(timer0Base.msCounter >= 180)//ms 这里只是做测试选取180ms为一个周期
         {
            timer0Base.msCounter = 0;

            //测试变量，后三个数码管显示 Tmp的内容
            Tmp++;
            if(Tmp>9999) Tmp=0;
            //根据 Tmp 变量，刷新数码管1-4位显示内容
            AIP650_Send(DIG4,SEG7Table[Tmp/1000]);     //DIG4
            AIP650_Send(DIG3,SEG7Table[Tmp/100%10]);   //DIG3
            AIP650_Send(DIG2,SEG7Table[(Tmp/10)%10]);  //DIG2
            AIP650_Send(DIG1,SEG7Table[Tmp%10]);       //DIG1

            printf("\r\nAIP650控制4位数码管显示数字为:%d",Tmp);

            //LED交替闪烁
            if(Tmp%3==0)
            {
               LED1_ON();
               LED2_OFF();
               LED3_OFF();
               LED4_OFF();
             }
             else if(Tmp%3==1)
             {
                LED1_OFF();
                LED2_ON();
                LED3_OFF();
                LED4_OFF();
              }
              else if(Tmp%3==2)
              {
                LED1_OFF();
                LED2_OFF();
                LED3_ON();
                LED4_OFF();
               }
               else if(Tmp%3==3)
               {
                 LED1_OFF();
                 LED2_OFF();
                 LED3_OFF();
                 LED4_ON();
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
