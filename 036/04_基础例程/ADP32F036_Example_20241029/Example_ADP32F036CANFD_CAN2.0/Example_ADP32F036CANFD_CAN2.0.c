/*************************************工程示例说明 *************************************/
// 该例程使用CANFD配置实现CAN2.0的数据传输。使用GPIO0作rx,GPIO32作tx
//发送ID为0x123  接收ID0x015A 通讯波特率为500K
//程序接收到数据后 将收到数据发送出去
/**********************************************************************************/
//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "ECanFunc.h"

//函数声明区
void delay_loop(void);
void LED_example(void);
void InitLED(void);
void InitBeep(void);
void InitKEY(void);
Uint16 rxData[32];
Uint16 txData[32];
//预编译区
#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif
int temp;

//变量声明区

//主函数入口处
void main(void)
{
    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
    MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
    InitFlash();        //初始化flash
    InitSysCtrl();

    // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,否则进入Flash固化模式,
    // 即要执行flash的初始化并将设定的部分在RAM里面运行的代码拷贝到flash运行
#if DEBUG_EN    > 0
    //
#else

#endif
    InitGpio();
    // 第二步: 清除所有中断标志并初始化PIE中断向量表
    DINT;           //先禁止CPU中断
    InitPieCtrl();  //给PIE寄存器赋初值
    IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
    IFR = 0x0000;
    InitPieVectTable();//初始化中断向量表


    // 第三步：初始化其他使用的外设
    InitCanfd();
    InitCanfdGpio();

    InitLED();
    InitKEY();

   // Timer0_init();//定时器0初始化，1ms周期，采用定时器周期中断方式

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
    PieCtrlRegs.PIEACK.all = 0xFFFF;

    EINT;                                 // Enable Global interrupt INTM
    ERTM;

    /*LED1_ON();
    Delay_nMS(100);
    LED1_OFF();
    Delay_nMS(100);

    LED1_ON();
    Delay_nMS(100);
    LED1_OFF();*/
    int i;
    for(i=0; i<32; i++)
    {
        rxData[i] = 0x00;
        txData[i] = 0x1200+i;
    }
   sCANFDDataTx(&txData[0],8);  //发送数据
    while(sCANFDTxStateGet()==0);
    // 第四步: 执行系统任务
    while(1)
    {
        temp = 0;
        while(temp==0)
        {
            temp = sCANFDDataRx(rxData);  //接收数据
        }
        sCANFDDataTx(&rxData[0],8);     // 发送接收到数据
        while(sCANFDTxStateGet()==0);
        LED1_TOGGLE();   //收到一次数据翻转一次led
    }
}

void delay_loop(void)
{   //延时
    Uint32 i;
    for (i = 0; i < 100000; i++) 
    {
        ;//执行空操作
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

