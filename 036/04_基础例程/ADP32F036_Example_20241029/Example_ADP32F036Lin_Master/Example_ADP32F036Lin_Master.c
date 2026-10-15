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
//      LinSetupSCIBuffer();//,LIN作为 SCI module ,  采用buffer机制，8字节深度，  9600bps，8N1
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
//4.观察实验现象
// 4.1  使用LIN上位机软件设置波特率9600，ID0x24从机写，0x23从机读，运行程序后可看到从机数据读写;
//  4.2  用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "lin.h"

//宏定义区
#define SLP_MUX       GpioCtrlRegs.GPBMUX1.bit.GPIO34
#define SLP_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO34
#define SLP_ON();     (GpioDataRegs.GPBSET.bit.GPIO34=1)
#define SLP_OFF()     (GpioDataRegs.GPBCLEAR.bit.GPIO34=1)


//函数声明区
//void InitSLP(void);
void InitLED(void);
void InitKEY(void);
void InitSLP(void);
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
Uint16 sdataA[8];
char rdataA[8];                 // Received data for SCI-A
Uint32 LinL0IntVect;  //Vector for L0 interrupts
Uint16 i;             //Generic iterator
Uint16 ID_TX1=0x24,ID_TX2=0x23;
Uint16 TXRXnum = 0;

//主函数入口处
void main(void) 
{
     // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
     InitSysCtrl();

     // 第二步: 清除所有中断标志并初始化PIE中断向量表hall.pole = 1;
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
     Init_LinaGpio();      //初始化LIN的GPIO作为外设功能
     LinSetupSCIBuffer();  //LIN作为 SCI module ,  采用buffer机制，8字节深度，  115200bps，8N1
//     LinaRegs.SCIGCR2.bit.CC = 1;
     InitSLP();
     SLP_ON();

     InitCpuTimers();                       //调用官方库函数，初始化CPU定时器
     ConfigCpuTimer(&CpuTimer1, 60, 100000);//定时器1初始化，第二个参数为CPU SYSCLK = 60M， 第三个参数为设定的周期，us单位
     CpuTimer1Regs.TCR.bit.TSS = 0;         // To start or restart the CPU-timer, set TSS to 0

     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
     EINT;                                 // Enable Global interrupt INTM
     ERTM;
//     LinaRegs.LINTD0.all = 0x01234567;
//     LinaRegs.LINTD1.all = 0x89abcdef;
//     LinaRegs.LINTD0.bit.TD0 = 0x11;
    // Delay_nMS(0x160);
     TXRXnum = 0;
     while(1)
     {
         if(CpuTimer1Regs.TCR.bit.TIF == 1)//定时器1查询周期
         {
             CpuTimer1Regs.TCR.bit.TIF = 1;// clear flag
            // LED1_TOGGLE();//交互闪烁
         }
         Delay_nMS(0x10);
        // if((linTxData.RxFlag == 1)||(linTxData.TxFlag == 1))
         {
             if(TXRXnum < 100)
             {
                 TXRXnum++;
               //  if(linTxData.RxFlag == 1)  //主发从收


                 LinaRegs.LINID.bit.IDBYTE = 0xA3;
                 LinaRegs.LINTD1.bit.TD7 = 0x5C;//0x06;//sdataA[7];
                 LinaRegs.LINTD1.bit.TD6 = (TXRXnum & 0x00FF);//0x05;//sdataA[6];
                 LinaRegs.LINTD1.bit.TD5 = 0x00;//0x04;//sdataA[5];
                 LinaRegs.LINTD1.bit.TD4 = 0x00;//0x03;//linTxData.speed;//sdataA[4];
                 LinaRegs.LINTD0.bit.TD3 = 0x00;//0x02;//linTxData.Direction;//sdataA[3];
                 LinaRegs.LINTD0.bit.TD2 = 0x00;//0x01;//sdataA[2];//speed & 0x00ff;
                 LinaRegs.LINTD0.bit.TD1 = 0x00;//(linTxData.speed & 0x00FF);//FramHead2;//speed >> 8;
                 LinaRegs.LINTD0.bit.TD0 = 0x02;//(linTxData.speed & 0xFF00)>>8;//FramHead1;//sleep_cmd;
                 Delay_nMS(0x10);
                //接收

                 LinaRegs.LINID.bit.IDBYTE = 0x64;
                 sdataA[0] = LinaRegs.LINRD0.bit.RD0;
                 sdataA[1] = LinaRegs.LINRD0.bit.RD1;
                 sdataA[2] = LinaRegs.LINRD0.bit.RD2;
                 sdataA[3] = LinaRegs.LINRD0.bit.RD3;
                 sdataA[4] = LinaRegs.LINRD1.bit.RD4;
                 sdataA[5] = LinaRegs.LINRD1.bit.RD5;
                 sdataA[6] = LinaRegs.LINRD1.bit.RD6;
                 sdataA[7] = LinaRegs.LINRD1.bit.RD7;
                 Delay_nMS(0x10);

             }
         }
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
//GPIO18---TX;  GPIO19--RX
void Init_LinaGpio(void)
{
      EALLOW;
      /* Enable internal pull-up for the selected pins */
      // Pull-ups can be enabled or disabled by the user.
      // This will enable the pullups for the specified pins.
    //  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO22 (LIN TX)
    //  GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;     // Enable pull-up for GPIO23 (LIN RX)

      /* Set qualification for selected pins to asynch only */
      // Inputs are synchronized to SYSCLKOUT by default.
      // This will select asynch (no qualification) for the selected pins.
   //   GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO23 (LINRXA)

      /* Configure LIN-A pins using GPIO regs*/
      // This specifies which of the possible GPIO pins will be LIN pins.
      // Only one set of pins should be enabled at any time for LIN operation.
    //  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;   // Configure GPIO19 for LIN TX operation  (3-Enable,0-Disable)
  //    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO23 for LIN RX operation (3-Enable,0-Disable)




        GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;     // Enable pull-up for GPIO18 (LIN TX)

        GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;     // Enable pull-up for GPIO19 (LIN RX)

        GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (LINRXA)


        GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for LIN TX operation (2-Enable,0-Disable)
        GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for LIN RX operation (2-Enable,0-Disable)


      EDIS;
}


//LIN初始化为SCI兼容模式
//采用buffer机制，8字节深度（1-8字节），
//9600bps， 8N1
//接收采用中断
void LinSetupSCIBuffer(void)
{
     //Allow write to protected registers
     EALLOW;

     LinaRegs.SCIGCR0.bit.RESET = 0; //Into reset
     LinaRegs.SCIGCR0.bit.RESET = 1; //Out of reset

     LinaRegs.SCIGCR1.bit.SWnRST = 0; //Into software reset
 //    LinaRegs.SCIGCR2.bit.CC = 1;           // Validate checksum
     //SCI Configurations
     LinaRegs.SCIGCR1.bit.COMMMODE = 0;   //Idle-Line Mode    ID4 and ID5 are not used for length control.
//     LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; //Asynchronous Timing
     LinaRegs.SCIGCR1.bit.PARITYENA = 0;  //No Parity Check    ID-parity verification is Unenabled
//     LinaRegs.SCIGCR1.bit.PARITY = 0;     //Odd Parity
//     LinaRegs.SCIGCR1.bit.STOP = 0;       //One Stop Bit
     LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; //Enable SCI Clock     The node is in master mode
     LinaRegs.SCIGCR1.bit.LINMODE = 1;    //LIN mode is enabled
//     LinaRegs.SCIGCR1.bit.SLEEP = 0;      //Ensure Out of Sleep
     LinaRegs.SCIGCR1.bit.ADAPT = 0;       //Automatic baudrate adjustment is disabled
     LinaRegs.SCIGCR1.bit.MBUFMODE = 1;   //Buffered Mode
     LinaRegs.SCIGCR1.bit.CTYPE = 1;      //LIN 2.0 Enhanced checksum used
     LinaRegs.SCIGCR1.bit.HGENCTRL = 1;   //ID filtering using ID-SlaveTask byte
     LinaRegs.SCIGCR1.bit.STOPEXTFRAME = 0;
     LinaRegs.SCIGCR1.bit.LOOPBACK = 0;   //External Loopback 正常模式，非自测模式
     LinaRegs.SCIGCR1.bit.CONT = 1;       //Continue on Suspend
     LinaRegs.SCIGCR1.bit.RXENA = 1;      //Enable RX
     LinaRegs.SCIGCR1.bit.TXENA = 1;      //Enable TX
//     LinaRegs.SCIPIO2.bit.RXIN = 1;
//     LinaRegs.SCIPIO2.bit.TXIN = 1;

     LinaRegs.LINID.bit.IDSLAVETASKBYTE = 0x3a;

     //Ensure IODFT is disabled
     LinaRegs.IODFTCTRL.bit.IODFTENA = 0x0;
     LinaRegs.IODFTCTRL.bit.LPBENA   = 0;   // IODFT loopback disabled
     //Set transmission length
//     LinaRegs.SCIFORMAT.bit.CHAR = 7;        //8个数据位
     LinaRegs.SCIFORMAT.bit.LENGTH = 7;   // （LENGTH+1）个字节深度的BUFFER， 意味着ADP32F035的 LIN-SCI接收到（LENGTH+1） 个字节之后，进入中断服务函数

     // 当LinaRegs.BRSR.bit.M = 0 时     LinaRegs.BRSR.bit.SCI_LIN_PSL取值  194-->9600bps ;  97--> 19200bps
     // 当LinaRegs.BRSR.bit.M = 4 时     LinaRegs.BRSR.bit.SCI_LIN_PSL取值  15-->115200bps ;
     // P13页波特率计算     P67 波特率表格
     LinaRegs.BRSR.bit.SCI_LIN_PSL = 194;  //96
     LinaRegs.BRSR.bit.M = 0;  //11
     LinaRegs.MBRSR = 92; //20kHz (max autobaud rate)


     LinaRegs.LINMASK.bit.RXIDMASK = 0xff;
     LinaRegs.LINMASK.bit.TXIDMASK = 0xff;      //When HGENCTRL is set to 1, this field must be set to 0xFF

     LinaRegs.LINCOMP.bit.SBREAK = 5;     //TSYNBRK = 13Tbit + SBREAK x Tbit
     LinaRegs.LINCOMP.bit.SDEL = 3;       //TSDEL = (SDEL + 1)Tbit

     //Set interrupt priority
     LinaRegs.SCICLEARINTLVL.all = 0xFFFFFFFF; //Set Int level of all interrupts to LVL 0




     //中断配置步骤-----1,开启模块中断使能，
//     LinaRegs.SCISETINT.bit.SETRXINT = 1; //Enable RX interrupt


      LinaRegs.SCICLEARINT.all        = 0xFFFFFFFF;    //Set all interrupts to disabled

     LinaRegs.SCIGCR1.bit.SWnRST = 1;  //bring out of software reset

     //中断配置步骤-----2，重映射中断服务函数
     PieVectTable.LIN0INTA  = &Lina_Level0_ISR;

     //Enable interrupts required for this example
     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block

     //中断配置步骤-----3，连接CPU中断矩阵Y
     IER |= M_INT9;// Enable CPU INT

     //中断配置步骤-----4，连接CPU中断矩阵X
     PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3

     EDIS;   // This is needed to disable write to EALLOW protected registers

//     while(LinaRegs.SCIFLR.bit.IDLE == 1);   //Wait for SCI to be idle and ready for transmission
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

void InitSLP(void)
{
    EALLOW;
    SLP_MUX=0;
    SLP_DIR=1;
    EDIS;
}

//===========================================================================
// No more.
//===========================================================================
