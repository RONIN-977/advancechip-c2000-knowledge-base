// 使用LIN上位机软件设置波特率19200，ID0x23主机写，0x24主机读，运行程序后可看到从机数据读写;
//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "lin.h"

//宏定义区
//宏定义区
#define SLP_MUX       GpioCtrlRegs.GPBMUX1.bit.GPIO34
#define SLP_DIR       GpioCtrlRegs.GPBDIR.bit.GPIO34
#define SLP_ON();     (GpioDataRegs.GPBSET.bit.GPIO34=1)
#define SLP_OFF()     (GpioDataRegs.GPBCLEAR.bit.GPIO34=1)

Uint32 txdata = 0;
void Init_LinaGpio(void);         //LIN外设的GPIO初始化
void LinSetupSCIBuffer(void);     //初始化LIN为SCI功能

#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区
Uint16 sdataA[8];
Uint16 rdataA[8];                 // Received data for SCI-A
Uint32 LinL0IntVect;  //Vector for L0 interrupts
Uint16 i;             //Generic iterator
Uint16 ID_RX=0xA3;//23
Uint16 ID_TX=0x64;//24
Uint16 TXRXnum = 0;
Uint16 error = 0;
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
    // Init_KeyGpio();
    // Init_LedGpio();
     Init_LinaGpio();      //初始化LIN的GPIO作为外设功能
     LinSetupSCIBuffer();  //LIN作为 SCI module ,  采用buffer机制，8字节深度，  115200bps，8N1
     SLP_ON();

     InitCpuTimers();                       //调用官方库函数，初始化CPU定时器
     ConfigCpuTimer(&CpuTimer1, 60, 100000);//定时器1初始化，第二个参数为CPU SYSCLK = 60M， 第三个参数为设定的周期，us单位
     CpuTimer1Regs.TCR.bit.TSS = 0;         // To start or restart the CPU-timer, set TSS to 0

     IER = 0x100;
     EINT;                                 // Enable Global interrupt INTM
     ERTM;

     Delay_nMS(0x150);

     while(1)
     {
         if(CpuTimer1Regs.TCR.bit.TIF == 1)//定时器1查询周期
         {
             CpuTimer1Regs.TCR.bit.TIF = 1;// clear flag
         }

         error = ((LinaRegs.SCIFLR.all & 0xFF000000)>>24);
         if(error > 0)
         {
//             LinaRegs.SCIFLR.bit.BE = 1;
//             LinaRegs.SCIFLR.bit.PBE = 1;
//             LinaRegs.SCIFLR.bit.CE = 1;
//             LinaRegs.SCIFLR.bit.ISFE = 1;
//             LinaRegs.SCIFLR.bit.NRE = 1;
//             LinaRegs.SCIFLR.bit.FE = 1;
//             LinaRegs.SCIFLR.bit.OE = 1;
//             LinaRegs.SCIFLR.bit.PE = 1;
         }
     }
}

//中断服务函数0，高优先级（比 Lina_Level1_ISR优先级高）
interrupt void Lina_Level0_ISR(void)
{
    LinL0IntVect = LinaRegs.SCIINTVECT0.all;
    if(LinL0IntVect == 4)
    {
        if(LinaRegs.LINID.bit.RECEIVEDID == ID_TX)
        {
            Delay_nUS(400);
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
            LinaRegs.LINTD1.all = txdata;
            LinaRegs.LINTD0.all = txdata;
           /* LinaRegs.LINTD1.bit.TD7 = TXRXnum;//rdataA[7];
            LinaRegs.LINTD1.bit.TD6 = TXRXnum;//rdataA[6];
            LinaRegs.LINTD1.bit.TD5 = TXRXnum;//rdataA[5];
            LinaRegs.LINTD1.bit.TD4 = TXRXnum;//rdataA[4];
            LinaRegs.LINTD0.bit.TD3 = TXRXnum;//rdataA[3];
            LinaRegs.LINTD0.bit.TD2 = TXRXnum;//rdataA[2];
            LinaRegs.LINTD0.bit.TD1 = TXRXnum;//rdataA[1];
            LinaRegs.LINTD0.bit.TD0 = TXRXnum;//rdataA[0];*/
        }
        else if(LinaRegs.LINID.bit.RECEIVEDID == 0x25)
        {
            Delay_nUS(400);
            LinaRegs.SCIFORMAT.bit.LENGTH = 1;
            LinaRegs.LINTD1.bit.TD7 = 0;//rdataA[7];
            LinaRegs.LINTD1.bit.TD6 = 0;
            LinaRegs.LINTD1.bit.TD5 = 0;
            LinaRegs.LINTD1.bit.TD4 = 0;
            LinaRegs.LINTD0.bit.TD3 = 0;
            LinaRegs.LINTD0.bit.TD2 = 0;
            LinaRegs.LINTD0.bit.TD1 = 0x02;
            LinaRegs.LINTD0.bit.TD0 = 0x14;
        }
        else
        {
            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
        }
    }
    else if(LinL0IntVect == 11)
    {
        if(LinaRegs.LINID.bit.RECEIVEDID == ID_RX)
        {
//            LinaRegs.SCIFORMAT.bit.LENGTH = 7;
            rdataA[0] = LinaRegs.LINRD0.bit.RD0;
            rdataA[1] = LinaRegs.LINRD0.bit.RD1;
            rdataA[2] = LinaRegs.LINRD0.bit.RD2;
            rdataA[3] = LinaRegs.LINRD0.bit.RD3;
            rdataA[4] = LinaRegs.LINRD1.bit.RD4;
            rdataA[5] = LinaRegs.LINRD1.bit.RD5;
            rdataA[6] = LinaRegs.LINRD1.bit.RD6;
            rdataA[7] = LinaRegs.LINRD1.bit.RD7;
            TXRXnum+=2;
            txdata += 16843009;
        }
        else
        {
            sdataA[0] = LinaRegs.LINRD0.bit.RD0;
            sdataA[1] = LinaRegs.LINRD0.bit.RD1;
            sdataA[2] = LinaRegs.LINRD0.bit.RD2;
            sdataA[3] = LinaRegs.LINRD0.bit.RD3;
            sdataA[4] = LinaRegs.LINRD1.bit.RD4;
            sdataA[5] = LinaRegs.LINRD1.bit.RD5;
            sdataA[6] = LinaRegs.LINRD1.bit.RD6;
            sdataA[7] = LinaRegs.LINRD1.bit.RD7;
        }
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

//LIN-SCI 的GPIO初始化
//GPIO22---TX;  GPIO23--RX
void Init_LinaGpio(void)
{
      EALLOW;
#if 0
      /* Enable internal pull-up for the selected pins */
      // Pull-ups can be enabled or disabled by the user.
      // This will enable the pullups for the specified pins.
      GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO22 (LIN TX)
      GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;     // Enable pull-up for GPIO23 (LIN RX)

      /* Set qualification for selected pins to asynch only */
      // Inputs are synchronized to SYSCLKOUT by default.
      // This will select asynch (no qualification) for the selected pins.
      GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO23 (LINRXA)

      /* Configure LIN-A pins using GPIO regs*/
      // This specifies which of the possible GPIO pins will be LIN pins.
      // Only one set of pins should be enabled at any time for LIN operation.
      GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;   // Configure GPIO19 for LIN TX operation  (3-Enable,0-Disable)
      GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO23 for LIN RX operation (3-Enable,0-Disable)

#else
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;     // Enable pull-up for GPIO18 (LIN TX)

    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;     // Enable pull-up for GPIO19 (LIN RX)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (LINRXA)


    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for LIN TX operation (2-Enable,0-Disable)
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for LIN RX operation (2-Enable,0-Disable)
#endif
      EDIS;
}

void LinSetupSCIBuffer(void)
{
     //Allow write to protected registers
     EALLOW;

     LinaRegs.SCIGCR0.bit.RESET = 0; //Into reset
     LinaRegs.SCIGCR0.bit.RESET = 1; //Out of reset

     LinaRegs.SCIGCR1.bit.SWnRST = 0; //Into software reset

     LinaRegs.SCIGCR1.bit.CLK_MASTER = 0; //Enable SCI Clock     The node is in master mode
     LinaRegs.SCIGCR1.bit.LINMODE = 1;    //LIN mode is enabled
     //SCI Configurations
     LinaRegs.SCIGCR1.bit.COMMMODE = 0;   //Idle-Line Mode    ID4 and ID5 are not used for length control.
//     LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; //Asynchronous Timing
     LinaRegs.SCIGCR1.bit.PARITYENA = 0;  //No Parity Check    ID-parity verification is Unenabled
//     LinaRegs.SCIGCR1.bit.PARITY = 0;     //Odd Parity
//     LinaRegs.SCIGCR1.bit.STOP = 0;       //One Stop Bit
//     LinaRegs.SCIGCR1.bit.SLEEP = 0;      //Ensure Out of Sleep
     LinaRegs.SCIGCR1.bit.ADAPT = 1;       //Automatic baudrate adjustment is disabled
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

     LinaRegs.SCIGCR2.bit.CC = 0;           // compare checksum
     LinaRegs.SCIGCR2.bit.SC = 1;           // send checksum
//     LinaRegs.LINID.bit.IDSLAVETASKBYTE = 0x64;

     //Ensure IODFT is disabled
//     LinaRegs.IODFTCTRL.bit.IODFTENA = 0x0;
//     LinaRegs.IODFTCTRL.bit.LPBENA   = 0;   // IODFT loopback disabled
     //Set transmission length
//     LinaRegs.SCIFORMAT.bit.CHAR = 7;        //8个数据位
     LinaRegs.SCIFORMAT.bit.LENGTH = 7;   // （LENGTH+1）个字节深度的BUFFER， 意味着ADP32F035的 LIN-SCI接收到（LENGTH+1） 个字节之后，进入中断服务函数

     // 当LinaRegs.BRSR.bit.M = 0 时     LinaRegs.BRSR.bit.SCI_LIN_PSL取值  194-->9600bps ;  97--> 19200bps
     // 当LinaRegs.BRSR.bit.M = 4 时     LinaRegs.BRSR.bit.SCI_LIN_PSL取值  15-->115200bps ;
     // P13页波特率计算     P67 波特率表格
     LinaRegs.BRSR.bit.SCI_LIN_PSL = 194;  //96
     LinaRegs.BRSR.bit.M = 0;  //11
     LinaRegs.MBRSR = 92; //20kHz (max autobaud rate)


     LinaRegs.LINMASK.bit.RXIDMASK = 0xFF;
     LinaRegs.LINMASK.bit.TXIDMASK = 0xFF;      //When HGENCTRL is set to 1, this field must be set to 0xFF

     LinaRegs.LINCOMP.bit.SBREAK = 0;     //TSYNBRK = 13Tbit + SBREAK x Tbit
     LinaRegs.LINCOMP.bit.SDEL = 0;       //TSDEL = (SDEL + 1)Tbit

     //Set interrupt priority
     LinaRegs.SCICLEARINTLVL.all = 0xFFFFFFFF; //Set Int level of all interrupts to LVL 0 设置中断级别

     //中断配置步骤-----1,开启模块中断使能，
//     LinaRegs.SCICLEARINT.all        = 0xFFFFFFFF;    //Set all interrupts to disabled 中断禁用设置

     LinaRegs.SCISETINT.bit.SETRXINT = 1; //Enable RX interrupt
//     LinaRegs.SCISETINT.bit.SETTXINT = 1;
     LinaRegs.SCISETINT.bit.SETIDINT = 1;            //使能中断

     LinaRegs.SCISETINTLVL.bit.SETRXINTOVO = 0;
//     LinaRegs.SCISETINTLVL.bit.SETTXINTLVL = 0;
     LinaRegs.SCISETINTLVL.bit.SETIDINTLVL = 0;      //清除中断级别

     LinaRegs.SCIGCR1.bit.SWnRST = 1;  //bring out of software reset

     //中断配置步骤-----2，重映射中断服务函数
     PieVectTable.LIN0INTA  = &Lina_Level0_ISR;
//     PieVectTable.LIN1INTA  = &Lina_Level1_ISR;
     //Enable interrupts required for this example
     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block

     //中断配置步骤-----3，连接CPU中断矩阵Y
     IER |= M_INT9;// Enable CPU INT

     //中断配置步骤-----4，连接CPU中断矩阵X
     PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3
     PieCtrlRegs.PIEIER9.bit.INTx4=1;
     EDIS;   // This is needed to disable write to EALLOW protected registers

//     while(LinaRegs.SCIFLR.bit.IDLE == 1);   //Wait for SCI to be idle and ready for transmission
}

//===========================================================================
// No more.
//===========================================================================
