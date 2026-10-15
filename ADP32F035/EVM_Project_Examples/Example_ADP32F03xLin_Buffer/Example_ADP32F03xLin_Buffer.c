//工程演示说明
//工程演示说明
//1.本示例ADP32F035的LIN作为SCI串口通信试验。
//2.代码执行流程:
//  2.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟;
//  2.2 然后是PIE中断向量表的初始化;
//  2.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  2.4 最后是一个while(1)超级循环。
//3.其他说明
//  3.1 重要函数说明;
//      Init_LinaGpio(); //初始化LIN的GPIO作为外设功能
//      LinSetupSCIBuffer();//,LIN作为 SCI module ,  采用buffer机制，8字节深度，  115200bps，8N1
//      测试接收：interrupt void Lina_Level0_ISR(void)  接收中断服务函数
//      测试发送:当BUFFER深度为8时的发送形式： 一次发送以8个字节为一帧
//      while(LinaRegs.SCIFLR.bit.TXEMPTY !=1);//先判断是否忙碌再发送
//      LinaRegs.LINTD0.bit.TD0 = sdataA[0];//依次将发送的数据填入BUFFER寄存器进行发送
//      LinaRegs.LINTD0.bit.TD1 = sdataA[1];
//      LinaRegs.LINTD0.bit.TD2 = sdataA[2];
//      LinaRegs.LINTD0.bit.TD3 = sdataA[3];
//      LinaRegs.LINTD1.bit.TD4 = sdataA[4];
//      LinaRegs.LINTD1.bit.TD5 = sdataA[5];
//      LinaRegs.LINTD1.bit.TD6 = sdataA[6];
//      LinaRegs.LINTD1.bit.TD7 = sdataA[7];
//  3.2 串口调试助手设置:115200 8N1。
//4.观察实验现象
//  4.1 将仿真器与开发板连接后,编译程序并下载并运行;
//  4.2 将EVM板的DB9接口通过串口延长线与电脑相连接,如果您的电脑没有串口接口,请使用一根
//      USB转串口线进行调试;
//  4.3 LED2快闪,上电自动发送一帧 8个字节:"F035Test",当LIN-SCI接收满8个字节时，进入中断服务函数 Lina_Level0_ISR
//      读取8个字节数据,LIN-SCI将接收到的数据原封不动返回到串口助手,LED1在中断服务函数里状态翻转;
//  4.4 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
//宏定义区
#define SLP_MUX       GpioCtrlRegs.GPAMUX2.bit.GPIO26
#define SLP_DIR       GpioCtrlRegs.GPADIR.bit.GPIO26
#define SLP_ON();     (GpioDataRegs.GPASET.bit.GPIO26=1)
#define SLP_OFF()     (GpioDataRegs.GPACLEAR.bit.GPIO26=1)

//函数声明区
void InitSLP(void);
void InitLED(void);
void InitBeep(void);
void InitKEY(void);
void Init_LinaGpio(void);         //LIN外设的GPIO初始化
void LinSetupSCIBuffer(void);     //初始化LIN为SCI功能
void LinTxCharBuffer(int a);

#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区
char sdataA[8]={"F035Test"};   // 建立8个发送字符
char rdataA[8];                 // Received data for SCI-A
Uint32 LinL0IntVect;  //Vector for L0 interrupts
Uint16 i;             //Generic iterator

//主函数入口处
void main(void) 
{
     // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
     InitSysCtrl();

     // 第二步: 清除所有中断标志并初始化PIE中断向量表
     DINT;           //先禁止CPU中断
     InitPieCtrl();  //给PIE寄存器赋初值
     IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
     IFR = 0x0000;
     InitPieVectTable();//初始化中断向量表
     // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,
     // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
     // RAM里面运行的代码拷贝到flash运行
     #if DEBUG_EN    > 0
     #else
     MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
     InitFlash();            //初始化flash
     #endif

     // 第三步：初始化其他使用的外设
     InitLED();
     InitBeep();
     Init_LinaGpio();      //初始化LIN的GPIO作为外设功能
     LinSetupSCIBuffer();  //LIN作为 SCI module ,  采用buffer机制，8字节深度，  115200bps，8N1

     SLP_ON();
     //测试发送
     // 当BUFFER深度为8时的发送形式： 一次发送以8个字节为一帧
     while(LinaRegs.SCIFLR.bit.TXEMPTY !=1);//先判断是否忙碌再发送
     LinaRegs.LINTD0.bit.TD0 = sdataA[0];   //依次将发送的数据填入BUFFER寄存器进行发送
     LinaRegs.LINTD0.bit.TD1 = sdataA[1];
     LinaRegs.LINTD0.bit.TD2 = sdataA[2];
     LinaRegs.LINTD0.bit.TD3 = sdataA[3];
     LinaRegs.LINTD1.bit.TD4 = sdataA[4];
     LinaRegs.LINTD1.bit.TD5 = sdataA[5];
     LinaRegs.LINTD1.bit.TD6 = sdataA[6];
     LinaRegs.LINTD1.bit.TD7 = sdataA[7];

     InitCpuTimers();                       //调用官方库函数，初始化CPU定时器
     ConfigCpuTimer(&CpuTimer1, 60, 100000);//定时器1初始化，第二个参数为CPU SYSCLK = 60M， 第三个参数为设定的周期，us单位
     CpuTimer1Regs.TCR.bit.TSS = 0;         // To start or restart the CPU-timer, set TSS to 0

     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
     EINT;                                 // Enable Global interrupt INTM
     ERTM;

     while(1)
     {
         if(CpuTimer1Regs.TCR.bit.TIF == 1)//定时器1查询周期
         {
             CpuTimer1Regs.TCR.bit.TIF = 1;// clear flag
             LED2_TOGGLE();//交互闪烁
          }

           /*
                                   当LIN-SCI接收满8个字节时，进入中断服务函数 Lina_Level0_ISR
                                  读取8个字节数据，
           LIN-SCI将接收到的数据原封不动返回到串口助手
           LED在中断服务函数里状态翻转
           */
       }
}

//中断服务函数0，高优先级（比 Lina_Level1_ISR优先级高）
//当LIN-SCI接收满8个字节时，进入该中断服务函数，  将数据读取
//这里作为测试：将接收到的8个字节数据，原封不动发送回串口助手
interrupt void Lina_Level0_ISR(void)
{
    //Read-clear the interrupt vector
    LinL0IntVect = LinaRegs.SCIINTVECT0.all;

    //RXINT向量值为11
    if(LinL0IntVect == 11)
    {
       //按byte格式接收，依次接收8个
       rdataA[0] = LinaRegs.LINRD0.bit.RD0;
       rdataA[1] = LinaRegs.LINRD0.bit.RD1;
       rdataA[2] = LinaRegs.LINRD0.bit.RD2;
       rdataA[3] = LinaRegs.LINRD0.bit.RD3;
       rdataA[4] = LinaRegs.LINRD1.bit.RD4;
       rdataA[5] = LinaRegs.LINRD1.bit.RD5;
       rdataA[6] = LinaRegs.LINRD1.bit.RD6;
       rdataA[7] = LinaRegs.LINRD1.bit.RD7;

       //将接收到的数据返回发送
       while(LinaRegs.SCIFLR.bit.TXEMPTY !=1);
       LinaRegs.LINTD0.bit.TD0 = rdataA[0];
       LinaRegs.LINTD0.bit.TD1 = rdataA[1];
       LinaRegs.LINTD0.bit.TD2 = rdataA[2];
       LinaRegs.LINTD0.bit.TD3 = rdataA[3];
       LinaRegs.LINTD1.bit.TD4 = rdataA[4];
       LinaRegs.LINTD1.bit.TD5 = rdataA[5];
       LinaRegs.LINTD1.bit.TD6 = rdataA[6];
       LinaRegs.LINTD1.bit.TD7 = rdataA[7];

       LED1_TOGGLE();//状态翻转
     }
     PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;//Acknowledge the PIE to be able to receive more interrupts
}

//LIN-SCI 的GPIO初始化
//GPIO22---TX;  GPIO23--RX
void Init_LinaGpio(void)
{
      EALLOW;
      /* Enable internal pull-up for the selected pins */
      // Pull-ups can be enabled or disabled by the user.
      // This will enable the pullups for the specified pins.
      GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;     // Enable pull-up for GPIO22 (LIN TX)
      GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;     // Enable pull-up for GPIO23 (LIN RX)

      /* Set qualification for selected pins to asynch only */
      // Inputs are synchronized to SYSCLKOUT by default.
      // This will select asynch (no qualification) for the selected pins.
      GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (LINRXA)

      /* Configure LIN-A pins using GPIO regs*/
      // This specifies which of the possible GPIO pins will be LIN pins.
      // Only one set of pins should be enabled at any time for LIN operation.
      GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO19 for LIN TX operation  (3-Enable,0-Disable)
      GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for LIN RX operation (3-Enable,0-Disable)

      EDIS;
}


//LIN初始化为SCI兼容模式
//采用buffer机制，8字节深度（1-8字节），
//115200bps， 8N1
//接收采用中断
void LinSetupSCIBuffer(void)
{
     //Allow write to protected registers
     EALLOW;

     LinaRegs.SCIGCR0.bit.RESET = 0; //Into reset
     LinaRegs.SCIGCR0.bit.RESET = 1; //Out of reset

     LinaRegs.SCIGCR1.bit.SWnRST = 0; //Into software reset

     //SCI Configurations
     LinaRegs.SCIGCR1.bit.COMMMODE = 0;   //Idle-Line Mode
     LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; //Asynchronous Timing
     LinaRegs.SCIGCR1.bit.PARITYENA = 0;  //No Parity Check
     LinaRegs.SCIGCR1.bit.PARITY = 0;     //Odd Parity
     LinaRegs.SCIGCR1.bit.STOP = 0;       //One Stop Bit
     LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; //Enable SCI Clock
     LinaRegs.SCIGCR1.bit.LINMODE = 0;    //SCI Mode 兼容SCI串口模式
     LinaRegs.SCIGCR1.bit.SLEEP = 0;      //Ensure Out of Sleep
     LinaRegs.SCIGCR1.bit.MBUFMODE = 1;   //Buffered Mode
     LinaRegs.SCIGCR1.bit.LOOPBACK = 0;   //External Loopback 正常模式，非自测模式
     LinaRegs.SCIGCR1.bit.CONT = 1;       //Continue on Suspend
     LinaRegs.SCIGCR1.bit.RXENA = 1;      //Enable RX
     LinaRegs.SCIGCR1.bit.TXENA = 1;      //Enable TX

     //Ensure IODFT is disabled
     LinaRegs.IODFTCTRL.bit.IODFTENA = 0x0;

     //Set transmission length
     LinaRegs.SCIFORMAT.bit.CHAR = 7;        //8个数据位
     LinaRegs.SCIFORMAT.bit.LENGTH = 7;   // （LENGTH+1）个字节深度的BUFFER， 意味着ADP32F035的 LIN-SCI接收到（LENGTH+1） 个字节之后，进入中断服务函数

     // 当LinaRegs.BRSR.bit.M = 0 时     LinaRegs.BRSR.bit.SCI_LIN_PSL取值  194-->9600bps ;  97--> 19200bps
     // 当LinaRegs.BRSR.bit.M = 4 时     LinaRegs.BRSR.bit.SCI_LIN_PSL取值  15-->115200bps ;
     // P13页波特率计算     P67 波特率表格
     LinaRegs.BRSR.bit.SCI_LIN_PSL = 15;
     LinaRegs.BRSR.bit.M = 4;

     //Set interrupt priority
     LinaRegs.SCICLEARINTLVL.all = 0xFFFFFFFF; //Set Int level of all interrupts to LVL 0

     LinaRegs.SCIGCR1.bit.SWnRST = 1;  //bring out of software reset


     //中断配置步骤-----1,开启模块中断使能，
     LinaRegs.SCISETINT.bit.SETRXINT = 1; //Enable RX interrupt

     //中断配置步骤-----2，重映射中断服务函数
     PieVectTable.LIN0INTA  = &Lina_Level0_ISR;

     //Enable interrupts required for this example
     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block

     //中断配置步骤-----3，连接CPU中断矩阵Y
     IER |= M_INT9;// Enable CPU INT

     //中断配置步骤-----4，连接CPU中断矩阵X
     PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3

     EDIS;   // This is needed to disable write to EALLOW protected registers

     while(LinaRegs.SCIFLR.bit.IDLE == 1);   //Wait for SCI to be idle and ready for transmission
}


void InitSLP(void)
{
    EALLOW;
    SLP_MUX=0;
    SLP_DIR=1;
    EDIS;
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
