/**************************************************************************************************
 *          文件：      rs422.h
 *          创造者：    张永琪
 *          修改记录：  2026.05.11
 **************************************************************************************************/

#include "rs422.h"
#include "gpio.h"
#include "ccdl.h"

__interrupt void serial_RXA_isr(void);
__interrupt void serial_RXB_isr(void);

RS422_err  CCDL_RX = {0};
RS422_err  VMC_RX = {0};

extern Uint16 CCDL_RX_BUF[CCDL_BUFF_LEN];
extern Uint16 CCDL_Int_Flag;
extern Uint16 CCDL_RX_CNT;

extern Uint16 VMC_Int_Flag;
extern Uint16 VMC_RX_BUF[VMC_BUFF_LEN];
extern Uint16 VMC_RX_CNT;


StSCI g_StSCI = {0};
/********************************************************
函数名: scia_init
功能描述: scic-通讯初始化
参数: baud-波特率
返回: 无
其它: RX-io28、 TX-io29  用于CCDL通讯  12-TX使能   11-RX使能
********************************************************/
void scia_init(Uint32 baud)
{
    Uint16 scihbaud=0U;
    Uint16 scilbaud=0U;
    Uint16 scibaud=0U;

    scibaud = (Uint16)(37500000/(8*baud)-1);
    scihbaud = scibaud>>8;
    scilbaud = scibaud&0xff;

    SciaRegs.SCIHBAUD = scihbaud;  /* 9600 baud @LSPCLK = 37.5MHz.    */
    SciaRegs.SCILBAUD = scilbaud;

//    EALLOW;
//    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   /* SCI-A   */
//    EDIS;
//
//    EALLOW;
//    PieVectTable.SCIRXINTA = &serial_RXA_isr;       /*  映射中断函数      */
//    EDIS;
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK=1;
    PieVectTable.SCIRXINTA=&serial_RXA_isr;
    EDIS;

    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;      /*  开启SCIRXC中断  */
    IER |= M_INT9;

    InitSciaGpio();

    // in the InitSysCtrl() function
    SciaRegs.SCICCR.all =0x0067;   // 1 stop bit,  No loopback
                                   // EVEN,8 char bits, 0x0067-even
                                   // async mode, idle-line protocol
    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all =0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 0;  //1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    SciaRegs.SCICTL1.bit.SWRESET = 1;
    SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

    /*  Initalize the SCI FIFO  */
    SciaRegs.SCIFFTX.all=0x400F;        //0xC02F;    //0x401F
    SciaRegs.SCIFFRX.all=0x002F;        //0x204f;
    SciaRegs.SCIFFCT.all=0x00;
    SciaRegs.SCIFFRX.bit.RXFFIL = 1;
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET=0;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
    SciaRegs.SCIFFTX.bit.SCIRST = 1;
}


/********************************************************
函数名: scib_init
功能描述: scib-通讯初始化
参数: baud-波特率
返回: 无
其它: TX-io14、 RX-io15  用于BCU板间通讯  16-TX使能   17-RX使能
********************************************************/
void scib_init(Uint32 baud)
{
    Uint16 scihbaud=0U;
    Uint16 scilbaud=0U;
    Uint16 scibaud=0U;

    scibaud = (Uint16)(37500000/(8*baud)-1);
    scihbaud = scibaud>>8;
    scilbaud = scibaud&0xff;

    ScibRegs.SCIHBAUD = scihbaud;  /* 9600 baud @LSPCLK = 37.5MHz.    */
    ScibRegs.SCILBAUD = scilbaud;


    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   /* SCI-B   */
    EDIS;

    EALLOW;
    PieVectTable.SCIRXINTB = &serial_RXB_isr;       /*  映射中断函数      */
    EDIS;

    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;      /*  开启SCIRXB中断  */
    IER |= M_INT9;

    InitScibGpio();

    EALLOW;
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    ScibRegs.SCICCR.all =0x0067;   // 1 stop bit,  No loopback
                                   // EVEN,8 char bits,
                                   // async mode, idle-line protocol
    ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScibRegs.SCICTL2.all =0x0003;
    ScibRegs.SCICTL2.bit.TXINTENA = 0;  //1
    ScibRegs.SCICTL2.bit.RXBKINTENA = 1;

    ScibRegs.SCICTL1.bit.SWRESET =1;
    ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset

    //Initalize the SCI FIFO
    ScibRegs.SCIFFTX.all=0x400F;
    ScibRegs.SCIFFRX.all=0x002F;
    ScibRegs.SCIFFCT.all=0x00;
    ScibRegs.SCIFFRX.bit.RXFFIL = 1;
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET=0;
    ScibRegs.SCIFFRX.bit.RXFIFORESET = 0;
    ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;
    ScibRegs.SCIFFTX.bit.SCIRST = 1;
}




/********************************************************
函数名: RS422_Init
功能描述: RS422-通讯初始化
参数: baud-波特率
返回: 无
其它: 初始化scia scib
********************************************************/
void RS422_Init(Uint32 baud)
{
    scia_init(baud);
    scib_init(baud);

    /* VMC与CCDL变量初始化 */
    InitCCDLSCI();
    InitVMCSCI();

    /* VMC通讯状态 */
    g_StSCI.RxVMC422Fault = 0;
    g_StSCI.NoRxVMCDataFault = 0;
    g_StSCI.RxVMCChecksumFault = 0;
    g_StSCI.RxVMCDataNoUpdata = 0;
    g_StSCI.RxVMCLengthFault = 0;
    g_StSCI.OppRxVMCFault = 0;
    g_StSCI.RxCCDLFault = 0;

    /* CCDL(对侧阀)通讯状态 */
    g_StSCI.RxCCDL422Fault = 0;
    g_StSCI.NoRxCCDLDataFault = 0;
    g_StSCI.RxCCDLLengthFault = 0;
    g_StSCI.RxCCDLDataNoUpdata = 0;
    g_StSCI.RxCCDLChecksumFault = 0;

    SCI_EN_Gpio_Init();/*打开422使能*/
}



/********************************************************
函数名: scia_xmit
功能描述: RS422-CCDL-利用FIFO发送底层函数
参数: a-发送字节
返回: 无
其它: scia            调试sci接口
********************************************************/
void scia_xmit(Uint16 a)
{
    SciaRegs.SCITXBUF = a & 0x00FF;
}



/********************************************************
函数名: RS422_DSP_SendByte
功能描述: RS422-DSP-利用FIFO发送底层函数
参数: a-发送字节
返回: 无
其它: scib
********************************************************/
void scib_xmit(Uint16 a)
{
    ScibRegs.SCITXBUF = a & 0x00FF;
}




/********************************************************
函数名: serial_RXC_isr
功能描述: RS422-scic-CCDL数据接收中断函数
参数: 无
返回: 无
其它: 32个接收字节，放置接收缓存数组中
********************************************************/
__interrupt void serial_RXA_isr(void)
{
    VMC_RX_BUF[VMC_RX_CNT] = SciaRegs.SCIRXBUF.all;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    VMC_RX_CNT++;
    if(VMC_RX_CNT >= VMC_BUFF_LEN)
    {
        VMC_RX_CNT = 0U;
    }

    VMC_Int_Flag = TURE;

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

/********************************************************
函数名: serial_RXB_isr
功能描述: RS422-scia-DIU数据接收中断函数
参数: 无
返回: 无
其它: 32个接收字节，放置接收缓存数组中
********************************************************/
__interrupt void serial_RXB_isr(void)
{
    CCDL_RX_BUF[CCDL_RX_CNT] = ScibRegs.SCIRXBUF.all;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    CCDL_RX_CNT++;
    if(CCDL_RX_CNT >= CCDL_BUFF_LEN)
    {
        CCDL_RX_CNT = 0U;
    }

    CCDL_Int_Flag = TURE;

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}




extern Uint16 send_flag;



