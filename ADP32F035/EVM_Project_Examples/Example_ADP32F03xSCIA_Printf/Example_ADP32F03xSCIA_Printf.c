//工程演示说明
//1.本示例演示如何将printf重定向到串口,实现串口的打印功能;
//2.本示例中使用DSP的SICA,其中发送使用查询模式,接收使用中断模式,相对于查询接收模式可以降低CPU
//  消耗。本示例中串口使用8位长度数据,1位停止位,无奇偶校验,波特率115200bps。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
//  3.2 然后是PIE中断向量表的初始化
//  3.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里演示的是DSP将从上位机收到的数据发回上位机,如果按下K1键,则发送字符串变量'K1'。
//4.其他说明
//  4.1 本示例中的串口配置使用FIFO深度为1级;
//  4.2 串口调试助手设置:115200 8N1
//  4.3 该例程重要函数说明
//      SCI_Init();           初始化SCIA，波特率115200bps，接收采用中断机制
//      open_uart_debug(); //printf 函数连接，固定用法，将底层发送函数放置于 my_write 函数内即可； 串口助手采用  ASCII 格式显示
//      //定义printf输出测试变量
//      char  test_hex = 0x55;//定义一个 十六进制数
//      Uint16 test_dec = 1005;//定义一个十进制数
//      long  test_long = 0x12345678;//定义一个long型
//      float  test_float = 99.06;//定义一个浮点数
//      char *test_str={"S100"}; //定义字符串
        //采用标准C语言printf函数  发送字符串测试
//      printf("\r\n\r\nDSP is Ready!");
//      printf("\r\nM新动力原创例程--------硬件串口标准C语言的printf函数使用例程-------\r\n");
//      printf("\r\n\r\n开发板硬件配置：注意：跳线帽正确配置为左右连接！！！！！\r\n");
//      printf("\r\nprintf测试输出十六进制 0x%x",test_hex);  //发送十六进制测试
//      printf("\r\nprintf测试输出十六进制 0x%lx",test_long);//发送超过16bit长度的long型数据， 格式为  %lx
//      printf("\r\nprintf测试输出十进制 %d",test_dec);//测试发送十进制
//      printf("\r\nprintf测试输出浮点数 %0.2f",test_float); //输出2位小数点
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载,点击运行可以观察到LED3在以一定的频率闪烁;
//  5.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根
//      USB转串口线进行调试。
//  5.3 在主循环里,ADP32F035串口发送测试:KEY1按下,ADP32FO35通过串口发送 字符串变量 printf("\r\n %s按键",test_str),
//      中断函数里,ADP32F035串口接收测试：  串口助手ASCII格式，发送 '0'，LED1状态翻转；发送 '1'，LED2状态翻转;
//      将接收到的数据原封不动返回串口助手，测试收发。如果串口助手不停往ADP32F035发送字符'0'或者'1',可以观察到LED1或LED2不停闪烁。
//  5.4 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"

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
char  test_hex = 0x55;//定义一个 十六进制数
Uint16 test_dec = 1005;//定义一个十进制数
long  test_long = 0x12345678;//定义一个long型
float  test_float = 99.06;//定义一个浮点数
char *test_str={"KEY1"}; //定义字符串

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
       LED1_OFF();
       LED2_OFF();
       LED4_OFF();

       SCI_Init();             //初始化SCIA（占用GPIO28---SCI_RX ,  GPIO29_SCI_TX），波特率115200bps，接收采用中断机制
       open_uart_debug();      //printf 函数连接，固定用法，将底层发送函数放置于 my_write 函数内即可； 串口助手采用  ASCII 格式显示

       // 第四步: 执行系统任务
       //采用标准C语言printf函数  发送字符串测试
       printf("\r\n\r\nADP32F035 is Ready!");
       printf("\r\n--------printf重定向到ADP32F035的串口实验-------\r\n");

       printf("\r\nprintf测试输出十六进制 0x%x",test_hex);         //发送十六进制测试
       printf("\r\nprintf测试输出long型的十六进制 0x%lx",test_long);//发送超过16bit长度的long型数据， 格式为  %lx
       printf("\r\nprintf测试输出十进制 %d",test_dec);            //测试发送十进制
       printf("\r\nprintf测试输出浮点数 %0.2f",test_float);       //输出2位小数点

       InitCpuTimers();                                     //调用官方库函数，初始化CPU定时器
       ConfigCpuTimer(&CpuTimer1, 60, 100000);              //定时器1初始化，第二个参数为CPU SYSCLK = 60M， 第三个参数为设定的周期，us单位
       CpuTimer1Regs.TCR.bit.TSS = 0;// To start or restart the CPU-timer, set TSS to 0

       PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
       EINT;                                 // Enable Global interrupt INTM
       ERTM;

       while(1)
       {
           if(CpuTimer1Regs.TCR.bit.TIF == 1)//定时器1查询周期
           {
               CpuTimer1Regs.TCR.bit.TIF = 1;// clear flag
               LED3_TOGGLE();//交互闪烁

               //按键K1按下
               if(rK1_DAT()==0)
               {
                   printf("\r\n %s按键",test_str); //测试 发送字符串变量
               }
               //串口接收的操作，见中断服务函数  uartRx_isr
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

