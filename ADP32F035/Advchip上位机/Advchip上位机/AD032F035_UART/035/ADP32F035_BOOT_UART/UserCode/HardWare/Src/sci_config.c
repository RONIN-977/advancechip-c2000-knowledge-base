/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :sci_config.c
*Author     :Wei Youfu
*Version    :v1.0
*Date       :2025.3.5
*Description:
*Others     :
*History    :<date>,<author>,<modify>
******************************************************************************/
/******************************************************************************
Includes
-----------------------------------------------------------------------------*/
#include "sci_config.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern __interrupt void SCIA_ISR(void);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
*Input:
*Output:
-----------------------------------------------------------------------------*/
void SCI_A_Init(void)
{
    //RXD -> GPIO28
    //TXD -> GPIO29
    InitSciaGpio();

    //初始化SCI状态机和操作标志
    SciaRegs.SCICTL1.bit.SWRESET = 0;

    //配置通信控制寄存器
    SciaRegs.SCICCR.bit.SCICHAR = 0x07;//数据位长度为8
    SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0x0;//0:Idle-line mode
    SciaRegs.SCICCR.bit.LOOPBKENA = 0x0;//0:关闭回环测试模式
    SciaRegs.SCICCR.bit.PARITYENA = 0x0;//0：关闭奇偶校验
    SciaRegs.SCICCR.bit.PARITY = 0x0;//0：奇校验，1：偶校验
    SciaRegs.SCICCR.bit.STOPBITS = 0x0;//0:1个停止位

    //波特率寄存器(H/L)不为0的时候，使用：
    // BRR = (SCIHBAUD << 8) + (SCILBAUD)
    // Asynchronous Baud = LSPCLK / ((BRR + 1) *8)

    //波特率寄存器(H/L)为0的时候(BRR=0)，使用：
    // Asynchronous Baud = LSPCLK / 16

    // LOSPCP.bit.LSPCLK = 0x02; 即(60MHz)SYSCLKOUT/4 = 15MHz，即低速外设时钟为15MHz
    //计算BRR方法：BRR = (15*1000000 / TargetBaudRate / 8) - 1 ;
    SciaRegs.SCIHBAUD = 0x0000;
    SciaRegs.SCILBAUD = 15;  // 194-->9600 ;   97--> 19200 ;  48-->38400;  15--->115200    14-->128000

    SciaRegs.SCICTL1.bit.SWRESET = 1; //使能SCI
    SciaRegs.SCIFFTX.bit.SCIRST = 1; //恢复发送和接收
    SciaRegs.SCICTL1.bit.TXENA = 1;//使能发送
    SciaRegs.SCICTL1.bit.RXENA = 1;//使能接收

    SciaRegs.SCIFFRX.bit.RXFFIL = 1;//设置接收FIFO深度为1，即接收到1个字节数据产生中断
    //SciaRegs.SCIFFTX.bit.TXFFIL = 1;
    SciaRegs.SCIFFTX.bit.SCIFFENA = 1;
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1; // RX FIFO interrupt is enabled

    EALLOW;
    PieVectTable.SCIRXINTA = &SCIA_ISR;
    EDIS;
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
    IER |= M_INT9;

    SciaRegs.SCIFFCT.all = 0x00; //不使能自动波特率检测
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1; // 复位发送FIFO
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1; // 复位接收FIFO
}

/******************************************************************************
*Description:调用一次发送1个字节
-----------------------------------------------------------------------------*/
void SCI_A_Send_Byte(Uint16 data)
{
    SciaRegs.SCITXBUF = data;
}
















/* End of file */
