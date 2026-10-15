#include "AVP32F069_Project.h"
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"
#include "timer0_base.h"

timer0 timer0Base={0,0};

interrupt void cpu_timer0_isr(void);


//---------------------定时器0初始化------------------------
//1ms 中断
void Timer0_init()
{
    InitCpuTimers();

    //中断配置步骤-----1,开启模块中断使能，位于 Timer->RegsAddr->TCR.bit.TIE = 1;
    ConfigCpuTimer(&CpuTimer0, 90, 1000);//90MHz CPU Freq, 1 millisecond Period (in uSeconds)
    CpuTimer0Regs.TCR.all = 0x4001;        // Use write-only instruction to set TSS bit = 0

    //中断配置步骤-----2，重映射中断服务函数
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    EDIS;
    //中断配置步骤-----3，连接CPU中断Y
    IER |= M_INT1;
    //中断配置步骤-----4，连接Y中断里的第几位
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}


interrupt void cpu_timer0_isr(void)
{
   char i;
   timer0Base.msCounter++;
   timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 1;

   Running.scanTick++;//数码管芯片AIP650扫描计时

   //串口通讯处理
   if(SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag == 1)
   {
       SCI_Msg.timerOut++;
   }

   //IIC判断超时的计时操作
   for(i=0;i<2;i++)//开发板共计2个IIC的器件，AIP650和AT24C02
   {
       PtrMsg[i]->IIC_TimerOUT  = (PtrMsg[i]->MasterStatus == IIC_IDLE)?0:(PtrMsg[i]->IIC_TimerOUT+1);
   }

   // Acknowledge this interrupt to receive more interrupts from group 1
    EALLOW;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    EDIS;

}
