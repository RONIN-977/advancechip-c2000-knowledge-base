//工程演示说明
//1.本示例演示PWM波形输出，通过配置，ADP32F035的PWM7A和PWM7B输出10KHZ的PWM波,采用中央对其PWM模式（即计数器增减模式）
//2.ADP32F035有多达7个通道14路PWM波输出，在本实验中，我们的PWM7A和PWM7B分别是GPIO40和GPIO41。按键K1暂停或者启动PWM,
//  按键K2增加PWM变化频率,按键K3减少PWM变化频率。并且PWM输出时，数码管显示当前占空比的该变量,PWM停止输出时，数码管显示当前占空比的百分比。
//3.代码执行流程:
//  3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
//  3.2 然后是PIE中断向量表的初始化
//  3.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  3.4 最后是一个while(1)超级循环,这里演示的是通过按键K1控制PWM的输出,通过K2 K3,来调节PWM输出的占空比。
//4.其他说明
//  4.1 在本示例中PWM频率 = CPU主频/period，period是PWM初始化时传入的参数，如果：
//      period = 6000，那么频率就是60 000KHZ/6000 = 10KHZ;
//      PWM占空比 = 输入占空比值pwm/周期值period，比如：period = 6000,pwm = 3000时，
//      占空比即为50%，所以在10KHZ的频率下，占空比的细分数可以达到6000份。
//  4.2 PWM试验重要函数说明
//      InitEPwm7() 初始化外设 PWM7以及GPIO41作为PWM7B引脚输出
//      ChangePWMDutyCycle();//改变占空比，进而改变PWM输出，控制D400
//      handleScanKeyforPWM(&Running);//PWM例程的扫描按键处理函数
//  4.3 串口调试助手设置:115200 8N1
//5.观察实验现象
//  5.1 将仿真器与开发板连接后,编译程序并下载并运行;
//  5.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根
//      USB转串口线进行调试。
//  5.3 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
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
     LED2_OFF();
     InitKEY();
     Timer0_init();

     SCI_Init();
     open_uart_debug();

     printf("\r\n\r\nADP32F035 is Ready!");
     printf("\r\n--------ADP32F035 PWM试验-------\r\n");
     printf("\r\n\r\nPWM频率(%ldHz),中间对其模式",60000000/2/(frequency+1));
     printf("\r\n按键K1停止或者启动PWMstep");
     printf("\r\n按键K2增加PWMstep");
     printf("\r\n按键K3减少PWMstep");

     printf("\r\n启动PWMstep变化时，数码管显示当前占空比的该变量");
     printf("\r\n停止PWMstep变化时，数码管显示当前占空比的百分比");

     softResetIIC_BUS();         //软件复位IIC从设备
     InitI2C_Gpio();             //io 初始化为IIC
     I2CA_Init();                //HW IIC初始化，100KHz

     LigntVal = 0x71;//BIT6到BIT4为亮度调节，BIT0是  1 开启/0关闭
     AIP650_Send(CMD_SEG,LigntVal);   //1级亮度，开启显示
     AIP650_Send(DIG1,SEG7Table[0]);  //GID1
     AIP650_Send(DIG2,SEG7Table[0]);  //GID2
     AIP650_Send(DIG3,SEG7Table[0]);  //GID3
     AIP650_Send(DIG4,SEG7Table[0]);  //GID4

     Running.showhex[3]=SEGHexBLK;
     Running.showhex[2]=SEGHexBLK;
     Running.showhex[1]=SEGHexBLK;
     Running.showhex[0]=SEGHexBLK;

     Running.key[0].pressDEF = 5;
     Running.key[1].pressDEF = 5;
     Running.key[2].pressDEF = 5;
     Running.key[3].pressDEF = 5;

     Running.key[0].ClickCNT = 0;
     Running.key[1].ClickCNT = 0;
     Running.key[2].ClickCNT = 0;
     Running.key[3].ClickCNT = 0;

     Running.key[0].pressTick = 0;
     Running.key[1].pressTick = 0;
     Running.key[2].pressTick = 0;
     Running.key[3].pressTick = 0;

     Running.nowScanKey = KEY1;

     InitEPwm7();                //初始化PWM7外设功能以及GPIO41作为PWM7B引脚输出
     PWM7B.dutyDir = UP;
     InitEPWM_AQ_DB();

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

                 ChangePWMDutyCycle();//改变占空比，进而改变PWM输出
                 EPwm1Regs.CMPA.half.CMPA = 500;
                 EPwm2Regs.CMPA.half.CMPA = 500;
                 EPwm3Regs.CMPA.half.CMPA = 500;
             }
           }
         SegScanTask(&Running);  //数码管显示函数
         handleScanKeyforPWM(&Running);//PWM例程的扫描按键处理函数
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

 
