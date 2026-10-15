#include "OnChip_Driver_Inc.h"

//中断接收用的标志和缓存变量定义
static Uint16 Index=0;
static Uint16 Frame_Length = 0;
static Uint16 COM1_RxBuff[15]={0};

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
    SciaRegs.SCILBAUD    =15;  //   194-->9600 ;   97--> 19200 ;  48-->38400;  15--->115200    14-->128000

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
	Uint16 revData;

    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;      // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

	if(SciaRegs.SCIFFRX.bit.RXFFOVF == 0)//接收FIFO未溢出
	{
		//接收串口数据
		while(SciaRegs.SCIFFRX.bit.RXFFST)
		{
			revData = SciaRegs.SCIRXBUF.all;//从接收寄存器中取出数据

			COM1_RxBuff[Index] = revData;
			if(Index < Frame_Length)
			{
			        Index ++;
			}

			scia_xmit(revData);//将接收到的数据原封不动发送回串口助手，进行收发测试

			//接收到字符 '0'LED1状态翻转；接收到字符 '1'，LED2翻转
			if(revData == '0')
		    {
			    LED1_TOGGLE();
		    }

		}
	}
	else
	{
		//用户这里做串口硬件溢出的处理,可以完全读取出FIFO里的数据或者清空FIFO
		//这里清空FIFO操作
		SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear HW Overflow flag
		SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
		SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation
	}
}

Uint16 scia_GetChar(Uint16* p_array, Uint16 frame_len)
{
    Uint16 i = 0;
    if(Frame_Length != frame_len)
    {
        Frame_Length = frame_len;
    }
    if(Index == Frame_Length)
    {
        for(i=0; i< Frame_Length; i++)
        {
            p_array[i] = COM1_RxBuff[i];
        }
        Index = 0;
        return 1;
    }
    return 0;
}

Uint16 scia_GetRate(int32* p_rate)
{
    Uint16 temp_array[9] = {0};
    int32 temp_rate = 0;
    Uint16 ch_cmd = 0;
    if(scia_GetChar(temp_array, 9))        //如果收到新数据才进行数据校验与恢复
    {                                       //这样提高了查询速度
        if((temp_array[0] == 0xaa)&&(temp_array[8] == 0x55))
        {
            temp_rate = temp_array[6];
            temp_rate <<= 24;
            *p_rate = temp_rate;

            temp_rate = temp_array[5];
            temp_rate <<= 16;
            *p_rate |= temp_rate;

            temp_rate = temp_array[4];
            temp_rate <<= 8;
            *p_rate |= temp_rate;

            temp_rate = temp_array[3];
            *p_rate |= temp_rate;

            ch_cmd = temp_array[2];         //获取命令位
        }
    }
    return ch_cmd;
}

void scia_SendLine(int32 Data,Uint16 Is_H,Uint16 Line_ID)
{
    Uint16 Temp_A_L;
    Uint16 Temp_A_H;
    Uint16 Temp_B_L;
    int16 Temp_B_H;

    Uint16 ch_XOR = 0;

    Temp_A_L = Data;
    Temp_A_H = Data>>8;
    Temp_B_L = Data >> 16;
    Temp_B_H = Data >> 24;

    ch_XOR ^= 0x09;
    ch_XOR ^= Line_ID;

    ch_XOR ^= Temp_B_H;
    ch_XOR ^= Temp_B_L;
    ch_XOR ^= Temp_A_H;
    ch_XOR ^= Temp_A_L;

    scia_xmit(0xaa);
    scia_xmit(0x09);
    scia_xmit(Line_ID);

    if(Is_H)                            //是否是先发高字节？
    {
        scia_xmit(Temp_B_H);       //是
        scia_xmit(Temp_B_L);
        scia_xmit(Temp_A_H);
        scia_xmit(Temp_A_L);
    }
    else
    {
        scia_xmit(Temp_A_L);       //不是
        scia_xmit(Temp_A_H);
        scia_xmit(Temp_B_L);
        scia_xmit(Temp_B_H);
    }

    scia_xmit(ch_XOR);
    scia_xmit(0x55);
}


