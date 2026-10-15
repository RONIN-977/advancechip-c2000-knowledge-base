//工程演示说明
//1.本示例演示通过比较器COPM1A/1B来控制PWM输出, 1A连接外部输入，1B设置为内部连接到DAC，通过设置DAC值作为比较值。

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

#if DEBUG_EN    > 0
     //
#else
     MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
     InitFlash();            //初始化flash
#endif

     // 第二步: 清除所有中断标志并初始化PIE中断向量表
     DINT;           //先禁止CPU中断
     InitPieCtrl();  //给PIE寄存器赋初值
     IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
     IFR = 0x0000;
     InitPieVectTable();//初始化中断向量表
     // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,
     // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
     // RAM里面运行的代码拷贝到flash运行

    // 第三步：初始化其他使用的外设
     InitLED();//初始化普通LED和三色LED
     InitKEY();
     LED1_ON();

     Timer0_init();

     SCI_Init();
     open_uart_debug();

     printf("\r\n\r\nADP32F036 is Ready!");
     printf("\r\n--------ADP32F036 PWM AQ强制输出试验-------\r\n");

     printf("\r\n\r\nPWM频率(%ldHz),中间对其模式",60000000/2/frequency);
     printf("\r\n按键KEY1 禁止AQ功能");
     // 初始化COMP1
     EALLOW;
        Comp1Regs.DACVAL.bit.DACVAL       =213;   //DACOUT = DACVAL∗（VDDA−VSSA）/256  vdda=3 vssa = 0 ,213对应电压值为2.5V
        Comp1Regs.COMPCTL.bit.SYNCSEL     = 0;  // Asynchronous version of Comparator output is passed
        Comp1Regs.COMPCTL.bit.CMPINV      = 1;  // Inverted output of comparator is passed
        Comp1Regs.COMPCTL.bit.COMPSOURCE  = 0;  // input of comparator connected to internal DAC
        Comp1Regs.COMPCTL.bit.QUALSEL     = 10; // Qualification Period for synchronized output of the comparator
        Comp1Regs.COMPCTL.bit.COMPDACEN   = 1;  // Comparator/DAC Enable
     EDIS;

     InitComp1Gpio();

     InitEPwm5();//初始化PWM7外设功能以及GPIO41作为PWM7B引脚输出
     PWM5B.dutyDir = UP;       //PWM增计数
     PWM5B.PWMstep = 5;                     //PWM占空比改变步进值，在 ChangePWMDutyCycle()函数中改变

     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
     EINT;  // Enable Global interrupt INTM
     ERTM;

     while(1)
     {
         if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
         {
             timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;

             if(timer0Base.msCounter >= 500)//ms
             {
                 timer0Base.msCounter = 0;
                 LED1_TOGGLE();

                 ChangePWMDutyCycle();//改变占空比，进而改变PWM输出

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
    //4个普通LED
    LED1_MUX = 0;//GPIO功能
    LED1_DIR = 1;//输出



    EDIS;
}


//===========================================================================
// No more.
//===========================================================================

 
