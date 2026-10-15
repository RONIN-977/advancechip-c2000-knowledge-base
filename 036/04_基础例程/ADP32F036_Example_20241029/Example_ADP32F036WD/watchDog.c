#include "ADP32F03x_Project.h"
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

void (*run) (void);//PC跳转函数

interrupt void WatchDog_isr(void);

Uint16 WD_SW_IsrCnt=0;


//看门狗初始化函数
//采用WDRST触发硬件复位，不采用WDINT软件中断
Uint16 WatchDogInit_HWRST()
{
	Uint16 fnRst=0;
	ServiceDog();

	EALLOW;
	//开启硬件 WDRST信号输出
	//采用WDRST触发硬件复位，不采用WDINT软件中断
	SysCtrlRegs.SCSR =0x0000;

   //WDPS[2:0] = 111 ， WDCLK = OSCCLK/512/64， (10000000/512/64/256)Hz ，即为 839ms
   //看门狗选用10M时钟，固定512分频后，再根据 WDPS[2:0] 选择 分频数，这里再分配 64
	if(SysCtrlRegs.WDCR & 0x0080)//判断 WDFLAG位,WDCR寄存器描述
	{
		//WDFLAG[7] 写1清除
		//WDCHK[5:3],固定写 b'101
		//WDPS[2:0] = b'1111
		//WDCR一次写入，否则可能在写期间引起复位
		SysCtrlRegs.WDCR = 0x00AF;
		fnRst = 1; //WD复位重启
	}
	else
		SysCtrlRegs.WDCR = 0x002F;//没有 WDFLAG ，故不清除WDFLAG
   EDIS;

   return fnRst;
}

//看门狗初始化函数
//采用WDINT软件中断函数，不采用直接硬件复位中断
//WDPS[2:0] = 111 ， WDCLK = OSCCLK/512/64， (10000000/512/64/256)Hz ，即为 839ms
//与看门狗溢出硬件复位的配置区别在于，  SysCtrlRegs.SCSR |=BIT1 ，关闭WDRST信号输出。
Uint16 WatchDogInit_SWIsrRST()
{
	Uint16 fnRst=0;
	ServiceDog();

	EALLOW;	// This is needed to write to EALLOW protected registers
	PieVectTable.WAKEINT = &WatchDog_isr;
	EDIS;   // This is needed to disable write to EALLOW protected registers

	EALLOW;
	SysCtrlRegs.SCSR |=BIT1;//P60 采用WDINT软件中断  WDENINT[1]=1，
	EDIS;

	//配置中断
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx8 = 1;   // Enable PIE Gropu 1 INT8; //HIKE  watchdog and the Low-Power-Mode module interrupt
	IER |= M_INT1;                       // Enable CPU INT1
	EALLOW;

   //WDPS[2:0] = 111 ， WDCLK = OSCCLK/512/64， (10000000/512/64/256)Hz ，即为 839ms
   //看门狗选用10M时钟，固定512分频后，再根据 WDPS[2:0] 选择 分频数，这里再分配 64
	if(SysCtrlRegs.WDCR & 0x0080)//判断 WDFLAG 位，见P61  WDCR寄存器描述
	{
		//WDFLAG[7] 写1清除
		//WDCHK[5:3],固定写 b'101
		//WDPS[2:0] = b'1111
		//WDCR一次写入，否则可能在写期间引起复位
		SysCtrlRegs.WDCR = 0x00AF;
		fnRst = 1; //WD复位重启
	}
	else
		SysCtrlRegs.WDCR = 0x002F;//没有 WDFLAG ，故不清除WDFLAG

   EDIS;

   return fnRst;
}

//不及时喂狗，进入中断函数函数
interrupt void WatchDog_isr(void)
{
	printf("WD Isr Cnt(%d)",WD_SW_IsrCnt);
	WD_SW_IsrCnt++;
	if(WD_SW_IsrCnt > 2)//2次之后，进入强制跳转，DSP复位
	{
		WD_SW_IsrCnt =0;
		printf("\r\nWD Isr SW Reset");
		//强制跳转到入口地址，DSP复位
		run = ( void(*)() ) ENTRYPOINT ;
		(*run)();
	}

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
