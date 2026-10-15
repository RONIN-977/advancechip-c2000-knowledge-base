#include <stdio.h>
#include <file.h>
#include "ADP32F03x_Project.h"
#include "BspInc.h"

Uart_Msg SCI_Msg;//建立一个串口通信结构体

interrupt void uartRx_isr(void);

//串口初始化
//波特率 baud = LSPCLK/8/((BRR+1)
//baud @LSPCLK = 15MHz (60 MHz SYSCLK)，
//关于LSPCLK的初始化，在main函数系统初始化函数 InitSysCtrl  里的 InitPeripheralClocks 实现（默认 LSPCLK = SYSCLK/4 = 15MHz）
//115200   8N1
void SCI_Init()
{
	InitSciaGpio();
	SciaRegs.SCICTL1.bit.SWRESET = 0;

 	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
// baud = LSPCLK/8/((BRR+1) 
// baud @LSPCLK = 15MHz (60 MHz SYSCLK)
	SciaRegs.SCIHBAUD   =0x0000;
    SciaRegs.SCILBAUD    =194;  //   194-->9600 ;   97--> 19200 ;  48-->38400;  15--->115200    14-->128000

    SciaRegs.SCICTL1.bit.SWRESET = 1;     // Relinquish SCI from Reset
    SciaRegs.SCIFFTX.bit.SCIRST=1;

	SciaRegs.SCIFFRX.bit.RXFFIL  = SCI_FIFO_LEN;  //设置FIFO深度
	SciaRegs.SCICTL1.bit.TXENA = 1;       //使能发送
	SciaRegs.SCICTL1.bit.RXENA = 1;       //使能接收

//	SciaRegs.SCICTL2.bit.TXINTENA =1;
//	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
//  SciaRegs.SCIFFTX.bit.TXFFIENA = 0; //禁止发送中断使能

	//中断配置步骤-----1
	SciaRegs.SCIFFTX.bit.SCIFFENA = 1;      //使能FIFO中断
	SciaRegs.SCIFFRX.bit.RXFFIENA=1;
	EALLOW;
	PieVectTable.SCIRXINTA = &uartRx_isr; //中断配置步骤-----2
	EDIS;
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;           //中断配置步骤-----3
	IER |= M_INT9;						               //中断配置步骤-----4

	SciaRegs.SCIFFCT.all=0x00;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}

//发送一个字节/字符
void scia_xmit(int a)
{
	Uint32 WaitTimer = 0;

	//while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
	while(SciaRegs.SCICTL2.bit.TXEMPTY != 1)
	{
		WaitTimer++;
		if(WaitTimer > TIMEROUTSCI)break;
	}
	if(WaitTimer <= TIMEROUTSCI)
		SciaRegs.SCITXBUF=a;
}

//发送字符串
//Enter1 字符串前的回车换行次数   Enter2 字符串后的回车换行次数
void sciPutString(char Enter1,char *s, char Enter2)
{
	char i;

	for(i=0;i<Enter1;i++)
	{
		scia_xmit(0x0D);
		scia_xmit(0x0A);
	}

	while(*s!=0)
	{
		scia_xmit(*(s++));
	}

	for(i=0;i<Enter2;i++)
	{
		scia_xmit(0x0D);
		scia_xmit(0x0A);
	}
}

//串口接收中断函数
//采用FIFO机制（缓存）
//SCI_FIFO_LEN 定义为 1，最大为4
interrupt void uartRx_isr(void)
{
    if(SciaRegs.SCIFFRX.bit.RXFFOVF == 0)//接收FIFO未溢出
    {
        SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag = 1;

        if((SCI_Msg.rxWriteIndex + SCI_FIFO_LEN) != SCI_Msg.rxReadIndex )
        {
            //接收数据
            while(SciaRegs.SCIFFRX.bit.RXFFST>0)
            {
                SCI_Msg.rxData[SCI_Msg.rxWriteIndex] = SciaRegs.SCIRXBUF.all&0x00ff;
                SCI_Msg.rxWriteIndex=(++SCI_Msg.rxWriteIndex)%(UartRxLEN);
            }
        }
        else//接收缓存满
        {
            //用户这里做缓存满的处理,
            SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
            SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation

            SCI_Msg.Mark_Para.Status_Bits.rFifoFullflag = 1;
        }
    }
    else
    {
        //用户这里做串口硬件溢出的处理,可以完全读取出FIFO里的数据或者清空FIFO
        //这里清空FIFO操作
        SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear HW Overflow flag
        SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
        SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation
        SCI_Msg.Mark_Para.Status_Bits.HWOVFlag = 1;

    }
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

//以下都为 标准 printf函数连接
//printf 函数连接
void open_uart_debug (void)
{
	int status=0;
	status = add_device("uart", _MSA, my_open, my_close, my_read, my_write, my_lseek, my_unlink, my_rename);
	if (status == 0) {
		freopen("uart:", "w", stdout);		// open uart and redirect stdout to UART
		setvbuf(stdout, NULL, _IONBF, 0); 	// disable buffering for stdout
	}
}

int my_open(const char *path, unsigned flags, int fno)
{
	//scia_fifo_init();
	//scia_echoback_init();
	path = path;
	flags = flags;
	fno = fno;
	return 0;
}

int my_close(int fno)
{
	fno =fno;
	return 0;
}

int my_read(int fno, char *buffer, unsigned count)
{
	fno = fno;
	buffer = buffer;
	count = count;
	return 0;
}

int my_write(int fno, const char *buffer, unsigned count)
{
	int i=0;
	fno = fno;

	while(count-- > 0) {
		scia_xmit(buffer[i++]);
	}
	return count;
}

off_t my_lseek(int fno, off_t offset, int origin)
{
	fno = fno; offset = offset; origin = origin;
	return 0;
}

int my_unlink(const char *path)
{
	path = path;
	return 0;
}

int my_rename(const char *old_name, const char *new_name)
{
	old_name = old_name;
	new_name = new_name;
	return 0;
}




