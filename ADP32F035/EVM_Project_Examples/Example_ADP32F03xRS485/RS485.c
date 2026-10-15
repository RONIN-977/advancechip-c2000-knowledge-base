#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//初始化485的收发控制引脚
void Init485EN()
{
	EALLOW;
    TEN485MUX = 0;
	TEN485DIR = 1;
	EDIS;
	TEN485OFF();
}

//485接收缓存处理函数
//测试函数，合法范围为 0--0x0f
void handle485RxFIFO(Scan * PtrSegTask)
{
	if(SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag == 1 )
	{
		SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag =0;
		//显示4个字节的数据
		while(SCI_Msg.rxReadIndex != SCI_Msg.rxWriteIndex)
		{
			PtrSegTask->showhex[3] = PtrSegTask->showhex[2];
			PtrSegTask->showhex[2] = PtrSegTask->showhex[1];
			PtrSegTask->showhex[1] = PtrSegTask->showhex[0];
			PtrSegTask->showhex[0] = 0;
			//判断是否写入为 0--0X0F
			if((SCI_Msg.rxData[SCI_Msg.rxReadIndex]>=0)&&(SCI_Msg.rxData[SCI_Msg.rxReadIndex]<=0x0f))
			{
				Running.showhex[0] =SCI_Msg.rxData[SCI_Msg.rxReadIndex];
			}
			SCI_Msg.rxReadIndex=(++SCI_Msg.rxReadIndex)%(UartRxLEN);
		 }
    }

	//这里做缓存溢出处理
	if(SCI_Msg.Mark_Para.Status_Bits.rFifoFullflag ==1)
	{
		SCI_Msg.Mark_Para.Status_Bits.rFifoFullflag = 0;
		SCI_Msg.rxReadIndex  = 0;
		SCI_Msg.rxWriteIndex  = 0;//SCI_Msg.rxReadIndex;
	}

	//这里做硬件串口接收溢出处理
	if(SCI_Msg.Mark_Para.Status_Bits.HWOVFlag == 1)
	{
		SCI_Msg.Mark_Para.Status_Bits.HWOVFlag = 0;
	}

	//扫描硬件FIFO溢出处理
	if(SciaRegs.SCIFFRX.bit.RXFFOVF == 1)
	{
		SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;
		SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
		SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation
	}
}

//启动485发送
void start485Send()
{
	TEN485ON();     //-------------------------发送前使能控制，占用485总线--------------------
}

//等待485发送完毕
//判断移位寄存器为空TXEMPTY 
#define TIMEROUT485 (Uint16)10*(SYSCLK/SCIBaudRate)
void wait485Send()
{
	Uint32 WaitTimer=0;

	//while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
	while(SciaRegs.SCICTL2.bit.TXEMPTY != 1)
	{
		WaitTimer++;
		if(WaitTimer > TIMEROUT485) break;
	}
//	while(WaitTimer < TIMEROUT485)
//		WaitTimer++;
	TEN485OFF();     //发送完毕释放485总线
}

//处理扫描按键,通过RS485发送按键信息
void handleScanKey485(Scan * PtrSegTask)
{
	Uint16 i;
	//这里做485显示哪个按键按下处理
	for(i=0;i<KeyNUM;i++)
	{
		if((PtrSegTask ->key[i].ClickCNT == 1)  && (PtrSegTask ->key[i].pressTick ==0))
		{
			PtrSegTask ->key[i].ClickCNT = 0;
			start485Send(); //发送前使能控制，占用485总线
			printf("\r\nRS485 Send %s",keyStr[i]);
			wait485Send(); //发送完毕释放485总线
		}
	}

}



