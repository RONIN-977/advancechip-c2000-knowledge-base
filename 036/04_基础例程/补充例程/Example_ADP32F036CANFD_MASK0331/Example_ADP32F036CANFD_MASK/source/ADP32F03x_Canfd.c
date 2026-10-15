//###########################################################################
//
// FILE:    ADP32F03x_Canfd.c
//
// TITLE:  ADP32F03x Enhanced CANFD Initialization & Support Functions.

//###########################################################################

#include "ADP32F03x_Device.h"     // ADP32F03x Headerfile Include File
#include "ADP32F03x_Examples.h"   //ADP32F03xExamples Include File

//---------------------------------------------------------------------------
// InitCanfd:

//
Uint16 rdata;
void InitCanfd(void)
{

       CanfdRegs.CFG_STAT.bit.RESET = 1;

    // 选择CANFD控制器时钟源
       CanfdRegs.CANCFG.bit.CAN_CLK_SEL = 0;   // 时钟源选择：0=系统时钟(sysclk)；1=PLL时钟(pllclk)

       // 配置仲裁段（Arbitration Phase）波特率（目标：500KHz @ 100MHz CANHD_CLK）
       // 波特率计算公式：Baud = CANHD_CLK / [(Seg1 + Seg2 + 3) * (PRESC + 1)]
       // 计算验证：100MHz / [(72+25+3)*(1+1)] = 100MHz/(100*2) = 500KHz ✔️
       CanfdRegs.S_CFG.bit.S_PRESC = 1;        // 仲裁段预分频系数：PRESC=1 → 实际分频比=PRESC+1=2
       CanfdRegs.S_SEG.bit.S_Seg_1 = 72;       // 仲裁段时间段1（Seg1）：72个时间量子(TQ)
       CanfdRegs.S_SEG.bit.S_Seg_2 = 25;       // 仲裁段时间段2（Seg2）：25个时间量子(TQ)
       CanfdRegs.S_CFG.bit.S_SJW   = 25;       // 仲裁段同步跳转宽度(SJW)：25TQ，建议与Seg2值保持一致，提升同步容错性

       // 配置数据段（Data Phase）波特率（目标：2MHz @ 100MHz CANHD_CLK）
       // 计算验证：100MHz / [(18+4+3)*(1+1)] = 100MHz/(25*2) = 2MHz ✔️
       CanfdRegs.F_CFG.bit.F_PRESC = 1;        // 数据段预分频系数：PRESC=1 → 实际分频比=PRESC+1=2
       CanfdRegs.F_SEG.bit.F_Seg_1 = 18;       // 数据段时间段1（Seg1）：18个时间量子(TQ)
       CanfdRegs.F_SEG.bit.F_Seg_2 = 4;        // 数据段时间段2（Seg2）：4个时间量子(TQ)
       CanfdRegs.F_CFG.bit.F_SJW   = 4;        // 数据段同步跳转宽度(SJW)：4TQ，与Seg2值保持一致

       // 配置传输延迟补偿（高速CANFD通信必备）
       CanfdRegs.DELAY_EALCAP.bit.TDCEN = 1;   // 传输延迟补偿使能：数据段波特率>2MHz时建议开启，补偿TX/RX相位偏差
       // 采样点偏移(SSPOFF)配置规则说明：
       // 前提：time_delay=TXD与RXD的相位偏差，BT=数据段单个位时间（TQ总数）
       // 规则1：若(time_delay + (F_Seg1+2+F_Seg2+1) < 3*BT) → SSPOFF = (F_Seg1+2+F_Seg2+1)/2（范围1~(F_Seg1+2+F_Seg2+1)）
       // 规则2：若(time_delay + (F_Seg1+2+F_Seg2+1) ≥ 3*BT) → SSPOFF = (3*BT - time_delay)/2（需小于BT）
       // 本配置计算：(18+2+4+1)/2 = 12.5 → 取整12，符合1~25的有效范围
       CanfdRegs.DELAY_EALCAP.bit.SSPOFF = 12; // 采样点偏移值：12TQ，优化高速传输时的采样准确性

    #if 1
        /* TBUF0 */
        CanfdRegs.CFG_STAT.bit.TBSEL = 0;       // select PTB
        CanfdRegs.CFG_STAT.bit.TPE = 1;         // 1： 为高优先级TBUF0中的消息启用传输  0： 对TBUF0没有传输
        CanfdRegs.CFG_STAT.bit.TSONE = 0;
        CanfdRegs.CFG_STAT.bit.TSALL = 0;
        //CanfdRegs.CFG_STAT.bit.LBME = 1;      //外回环使能
        // CanfdRegs.CFG_STAT.bit.LBMI = 1;      // 0:disable internal loopback mode ; 1: enable internal loopback mode
    #else
        /* use TBUF1~15 mode */
        CanfdRegs.TCTRL.bit.TSMODE = 0;         /* 设置TBUF为次要缓冲区TBUF1~15 FIFO模式 */
        CanfdRegs.CFG_STAT.bit.TBSEL = 1;       /* 选择TBUF1~15缓冲区 */
        CanfdRegs.CFG_STAT.bit.TPE = 0;         // no transfer to PTB
        CanfdRegs.RTINTFE.bit.TSIE = 1;         /* 中断使能位 */
    #endif

        CanfdRegs.TBUF.TID0.bit.ID0 = 0x610;    // standard frame ID
        CanfdRegs.TBUF.TID0.bit.ID1 = 0x0;
        CanfdRegs.TBUF.TID1.bit.ID = 0;
        CanfdRegs.TBUF.TID1.bit.ESI = 0;

        CanfdRegs.TBUF.TIDST.bit.IDE = 0;        // 0: standard frame ID; 1: extended frame ID
        CanfdRegs.TBUF.TIDST.bit.RTR = 0;        // 0: data frame; 1: remote frame(for can)
        CanfdRegs.TBUF.TIDST.bit.FDF_EDL = 1;    // 0: can 2.0; 1:can_fd
        CanfdRegs.TBUF.TIDST.bit.BRS = 1;        //

        CanfdRegs.CFG_STAT.bit.RESET = 0;

}

void InitCanfdGpio(void)
{

    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;     // CANFD_Rx
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1;     // CANFD_Tx
    AnalogRegs.PIN_MUX_FLASH.bit.canfd_gpio_mux = 0x5;   //GPIO0 CANFDRXD ; GPIO32 CANFDTXD;

//    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 1;     // CANFD_Rx
//    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1;     // CANFD_Tx
//    AnalogRegs.PIN_MUX_FLASH.bit.canfd_gpio_mux = 0xA;   //GPIO28 CANFDRXD ; GPIO29 CANFDTXD;
    EDIS;;
}


//===========================================================================
// End of file.
//===========================================================================
