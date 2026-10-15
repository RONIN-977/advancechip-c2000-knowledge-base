/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :watchdog_config.c
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
#include "watchdog_config.h"

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
extern __interrupt void WatchDog_ISR(void);
/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:看门狗配置，采用WDRST触发硬件复位，不采用WDINT软件中断
*Input:
*Output:
-----------------------------------------------------------------------------*/
Uint16 WatchDog_Init_HWRST(void)
{
    Uint16 fnRst = 0;
    ServiceDog();

    EALLOW;
	//开启硬件 WDRST信号输出
	//采用WDRST触发硬件复位，不采用WDINT软件中断
	SysCtrlRegs.SCSR =0x0000;
	
	//WDPS[2:0] = 111 ， WDCLK = OSCCLK/512/64， (10000000/512/64/256)Hz，即为 839ms
	//看门狗选用10MHz时钟，固定512分频后，再根据WDPS[2:0]选择分频数，这里再分配64
	if(SysCtrlRegs.WDCR & 0x0080)//判断WDFLAG位，WDCR寄存器描述
	{
		//WDFLAG[7] 写1清除
		//WDCHK[5:3],固定写 b'101
		//WDPS[2:0] = b'1111
		//WDCR一次写入，否则可能在写期间引起复位
		SysCtrlRegs.WDCR = 0x00AF;
		fnRst = 1; //WD复位重启
	}
	else
	{
		SysCtrlRegs.WDCR = 0x002F;//没有WDFLAG，故不清除WDFLAG
	}
	EDIS;
    return fnRst;
}

/******************************************************************************
*Description:看门狗配置，采用WDINT软件中断函数，不采用直接硬件复位中断
-----------------------------------------------------------------------------*/
Uint16 WatchDog_Init_SWRST(void)
{
    Uint16 fnRst=0;
    ServiceDog();

    EALLOW; // This is needed to write to EALLOW protected registers
    PieVectTable.WAKEINT = &WatchDog_ISR;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    EALLOW;
	SysCtrlRegs.SCSR |=BIT1;//P60 采用WDINT软件中断 WDENINT[1]=1
	EDIS;
	
	//配置中断
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx8 = 1;   // Enable PIE Gropu 1 INT8; //HIKE  watchdog and the Low-Power-Mode module interrupt
    IER |= M_INT1;                       // Enable CPU INT1
    EALLOW;

    //WDPS[2:0] = 111 ， WDCLK = OSCCLK/512/64， (10000000/512/64/256)Hz，即为 839ms
	//看门狗选用10MHz时钟，固定512分频后，再根据WDPS[2:0]选择分频数，这里再分配64
	if(SysCtrlRegs.WDCR & 0x0080)//判断WDFLAG位，见P61，WDCR寄存器描述
	{
		//WDFLAG[7] 写1清除
		//WDCHK[5:3],固定写 b'101
		//WDPS[2:0] = b'1111
		//WDCR一次写入，否则可能在写期间引起复位
		SysCtrlRegs.WDCR = 0x00AF;
		fnRst = 1; //WD复位重启
	}
	else
	{
		SysCtrlRegs.WDCR = 0x002F;//没有WDFLAG，故不清除WDFLAG
	}
	EDIS;
    return fnRst;
}
















/* End of file */






