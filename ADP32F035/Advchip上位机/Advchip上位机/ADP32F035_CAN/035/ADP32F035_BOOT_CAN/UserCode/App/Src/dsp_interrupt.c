/******************************************************************************
*Copyright(C),2025,Shenzhen Bestchain Technology CO.,LTD.
*FileName   :dsp_interrupt.c
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
#include "dsp_interrupt.h"
#include "main.h"

/******************************************************************************
External declarations
-----------------------------------------------------------------------------*/

/******************************************************************************
Defines and macros
-----------------------------------------------------------------------------*/

/******************************************************************************
Typedefs and structures
-----------------------------------------------------------------------------*/
void (*run)(void);//PC跳转函数

/******************************************************************************
Global variable definitions
-----------------------------------------------------------------------------*/

/******************************************************************************
Function prototypes
-----------------------------------------------------------------------------*/
extern void Can_up_receive_write(Uint32 frame_id, Uint16 frame_type, Uint16 *buf);

/******************************************************************************
Function implementations
-----------------------------------------------------------------------------*/
/******************************************************************************
*Description:
*Input:
*Output:
-----------------------------------------------------------------------------*/
__interrupt void WatchDog_isr(void)
{
    static Uint16 WD_SW_IsrCnt = 0;
    WD_SW_IsrCnt++;
	if(WD_SW_IsrCnt >= 2)//2次之后，进入强制跳转，DSP复位
	{
		WD_SW_IsrCnt =0;
		//强制跳转到入口地址，DSP复位
		run = ( void(*)() ) 0x003F7FF6;
        (*run)();
	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
__interrupt void CPU_Timer0_ISR(void)
{
    TICK0_SET();
    SYSTICK_SET();

	//清除定时器0中断标志位
    EALLOW;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    EDIS;
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
__interrupt void Can_ISR0(void)
{
    struct ECAN_REGS ECanaShadow;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    EALLOW;
    ECanaShadow.CANGIF0.all = ECanaRegs.CANGIF0.all;//CANGIF0
    EDIS;
	
	if((ECanaShadow.CANGIF0.all & 0x00037f00) != 0)//有错误中断产生
	{
		//添加 处理错误代码
		//添加 清除错误产生的标记 P76 Table3-4
		ECanaRegs.CANGIF0.all = ECanaShadow.CANGIF0.all  & 0x00013700;
		ECanaRegs.CANTOS.all = 0x0000FFFF; //bits17,MTOF 低十六位用于发送，一次性全清除超时中断
		ECanaRegs.CANAA.all =  0x0000FFFF; //P29 Bit14 AAIF
        ECanaRegs.CANRMP.all=  0xFFFF0000; //Bit11,RMLIF
	}
	else if(ECanaShadow.CANGIF0.bit.GMIF0 == 1)
	{
		//注意是ECanaShadow，而ECanaRegs.CANRMP已被清除 == 0xFFFF0000
		//例程的中断只做了mailbox16的接收中断，在16~32的mailbox中，优先级最低
		//则认为mailbox16接收到数据，其他mailbox也接收到数据
		if(ECanaShadow.CANRMP.all != 0)
        {
            ;
        }
        else
        {
            ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
            ECanaShadow.CANRMP.all &= 0xFFFF0000;
			ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;//清除中断标志
		}
	}
}

/******************************************************************************
*Description:
-----------------------------------------------------------------------------*/
__interrupt void Can_ISR1(void)
{
    struct ECAN_REGS ECanaShadow;
	Uint16 data[8];
    Uint32 RxID;
    Uint16 len;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    EALLOW;
    ECanaShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;//CANGIF0
    EDIS;
	
	if((ECanaShadow.CANGIF1.all & 0x00037f00) != 0)//有错误中断产生
	{
		//添加 处理错误代码
		//添加 清除错误产生的标记 P76 Table3-4
		ECanaRegs.CANGIF1.all = ECanaShadow.CANGIF1.all  & 0x00013700;
		ECanaRegs.CANTOS.all = 0x0000FFFF; //bits17,MTOF 低十六位用于发送，一次性全清除超时中断
		ECanaRegs.CANAA.all =  0x0000FFFF; //P29 Bit14 AAIF
        ECanaRegs.CANRMP.all=  0xFFFF0000;
	}
	else if(ECanaShadow.CANGIF1.bit.GMIF1 == 1)//接收或者发送中断，读取相应的mainbox数据
	{
		if(ECanaRegs.CANRMP.bit.RMP16 == 1)
        {
            // 0x7A0
            RxID = ECanaMboxes.MBOX16.MSGID.bit.STDMSGID;
            len = ECanaMboxes.MBOX16.MSGCTRL.bit.DLC;

            data[0] = ECanaMboxes.MBOX16.MDL.byte.BYTE3;
            data[1] = ECanaMboxes.MBOX16.MDL.byte.BYTE2;
            data[2] = ECanaMboxes.MBOX16.MDL.byte.BYTE1;
            data[3] = ECanaMboxes.MBOX16.MDL.byte.BYTE0;

            data[4] = ECanaMboxes.MBOX16.MDH.byte.BYTE7;
            data[5] = ECanaMboxes.MBOX16.MDH.byte.BYTE6;
            data[6] = ECanaMboxes.MBOX16.MDH.byte.BYTE5;
            data[7] = ECanaMboxes.MBOX16.MDH.byte.BYTE4;
			
			//把数据存储到消息队列
            Can_up_receive_write(RxID, 0, (Uint16*)&data);
			
			ECanaRegs.CANRMP.bit.RMP16 = 1; //写1清除邮箱接收中断标志位
        }
	}
}




























/* End of file */
