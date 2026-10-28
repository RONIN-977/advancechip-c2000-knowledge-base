/******************************************************************************
文件名                     : AVP33x_DefaultIsr.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片默认的中断服务程序：
                                       本文件包含AVP33x的PIE向量表的内核中断服务程序。通常，这些内核中断服务程序可用于在芯片调试期间
                                       填充整个PIE向量表。这样的话，如果在固件开发期间产生了中断，将始终有中断服务程序来响应。
                                       随着开发工作的进行，用户可以将自己的中断服务程序来替换这些默认的中断服务程序。并且，这些中断服务程
                                       序中都包含死循环代码，故也不应该包含在最终的工程代码中。
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // AVP33x Headerfile Include File
#include "AVP33x_Examples.h"   // AVP33x Examples Include File

//---------------------------------------------------------------------------
// INT13_ISR:CPU的INT13中断服务程序
//
interrupt void INT13_ISR(void)      // INT13或CPU-Timer1
{
    // 添加自己的代码

    // 接下来的两行调试仅用于在此处停止处理器插入ISR代码后移除
    asm ("      ESTOP0");
    for(;;);
}

//---------------------------------------------------------------------------
// INT13_ISR:CPU的INT14中断服务程序
//
interrupt void INT14_ISR(void)      // CPU-Timer2
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void DATALOG_ISR(void)    // 数据记录中断
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void RTOSINT_ISR(void)    // 实时操作系统中断
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void EMUINT_ISR(void)     // 仿真中断
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void NMI_ISR(void)        // 不可屏蔽中断
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void ILLEGAL_ISR(void)    // 非法操作陷阱
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm("          ESTOP0");
    for(;;);

}

interrupt void USER1_ISR(void)      // 用户定义陷阱1
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER2_ISR(void)      // 用户定义陷阱2
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER3_ISR(void)      // 用户定义陷阱3
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER4_ISR(void)      // 用户定义陷阱4
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER5_ISR(void)      // 用户定义陷阱5
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER6_ISR(void)      // 用户定义陷阱6
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER7_ISR(void)      // 用户定义陷阱7
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER8_ISR(void)      // 用户定义陷阱8
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER9_ISR(void)      // 用户定义陷阱9
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER10_ISR(void)     // 用户定义陷阱10
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER11_ISR(void)     // 用户定义陷阱11
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

interrupt void USER12_ISR(void)     // 用户定义陷阱12
{
    // 添加自己的代码

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// -----------------------------------------------------------
// 第1组PIE中断 - 映射到CPU的INT1
// -----------------------------------------------------------

// INT1.1
interrupt void SEQ1INT_ISR(void)    // ADC排序器1(SEQ1)
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT1.2
interrupt void SEQ2INT_ISR(void)    // ADC排序器2(SEQ2)
{
  // 添加自己的代码

  // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

  // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
  asm("	  ESTOP0");
  for(;;);
}
// INT1.3 - 预留

// INT1.4
interrupt void  XINT1_ISR(void)     // 外部中断1（外部中断1）
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT1.5
interrupt void  XINT2_ISR(void)     // 外部中断1（外部中断2）
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT1.6
interrupt void  ADCINT_ISR(void)    // ADC中断
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT1.7
interrupt void  TINT0_ISR(void)     // CPU-Timer 0
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。

    asm ("      ESTOP0");
    for(;;);
}

// INT1.8
interrupt void  WAKEINT_ISR(void)   // 看门狗中断
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group1;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}


// -----------------------------------------------------------
// 第2组PIE中断 - 映射到CPU的INT2
// -----------------------------------------------------------

// INT2.1
interrupt void EPWM1_TZINT_ISR(void)    // EPWM-1中断
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group2;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT2.2
interrupt void EPWM2_TZINT_ISR(void)    // EPWM-2
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group2;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT2.3
interrupt void EPWM3_TZINT_ISR(void)    // EPWM-3
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group2;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}


// INT2.4
interrupt void EPWM4_TZINT_ISR(void)    // EPWM-4
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group2;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT2.5
interrupt void EPWM5_TZINT_ISR(void)    // EPWM-5
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group2;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT2.6
interrupt void EPWM6_TZINT_ISR(void)   // EPWM-6
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group2;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT2.7 - 预留
// INT2.8 - 预留


// -----------------------------------------------------------
// 第3组PIE中断 - 映射到CPU的INT3
// -----------------------------------------------------------

// INT 3.1
interrupt void EPWM1_INT_ISR(void)      // EPWM-1
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group3;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT3.2
interrupt void EPWM2_INT_ISR(void)      // EPWM-2
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group3;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT3.3
interrupt void EPWM3_INT_ISR(void)      // EPWM-3
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group3;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT3.4
interrupt void EPWM4_INT_ISR(void)      // EPWM-4
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group3;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT3.5
interrupt void EPWM5_INT_ISR(void)      // EPWM-5
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group3;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT3.6
interrupt void EPWM6_INT_ISR(void)    // EPWM-6
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group3;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT3.7 - 预留
// INT3.8 - 预留


// -----------------------------------------------------------
// 第4组PIE中断 - 映射到CPU的INT4
// -----------------------------------------------------------

// INT 4.1
interrupt void ECAP1_INT_ISR(void)      // ECAP-1
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group4;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT4.2
interrupt void ECAP2_INT_ISR(void)      // ECAP-2
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group4;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT4.3
interrupt void ECAP3_INT_ISR(void)      // ECAP-3
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group4;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT4.4
interrupt void ECAP4_INT_ISR(void)      // ECAP-4
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group4;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT4.5
interrupt void ECAP5_INT_ISR(void)      // ECAP-5
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group4;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}
// INT4.6
interrupt void ECAP6_INT_ISR(void)      // ECAP-6
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group4;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}
// INT4.7 - 预留
// INT4.8 - 预留


// -----------------------------------------------------------
// 第5组PIE中断 - 映射到CPU的INT5
// -----------------------------------------------------------

// INT 5.1
interrupt void EQEP1_INT_ISR(void)      // EQEP-1
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group5;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT5.2
interrupt void EQEP2_INT_ISR(void)      // EQEP-2
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group5;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT5.3 - 预留
// INT5.4 - 预留
// INT5.5 - 预留
// INT5.6 - 预留
// INT5.7 - 预留
// INT5.8 - 预留


// -----------------------------------------------------------
// 第6组PIE中断 - 映射到CPU的INT6
// -----------------------------------------------------------

// INT6.1
interrupt void SPIRXINTA_ISR(void)      // SPI-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group6;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT6.2
interrupt void SPITXINTA_ISR(void)     // SPI-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group6;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT6.3
interrupt void MRINTB_ISR(void)         // McBSP-B
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group6;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT6.4
interrupt void MXINTB_ISR(void)         // McBSP-B
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group6;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT6.5
interrupt void MRINTA_ISR(void)         // McBSP-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group6;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT6.6
interrupt void MXINTA_ISR(void)         // McBSP-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group6;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT6.7 - 预留
// INT6.8 - 预留


// -----------------------------------------------------------
// 第7组PIE中断 - 映射到CPU的INT7
// -----------------------------------------------------------

// INT7.1
interrupt void DINTCH1_ISR(void)        // DMA
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group7;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT7.2
interrupt void DINTCH2_ISR(void)        // DMA
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group7;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT7.3
interrupt void DINTCH3_ISR(void)        // DMA
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group7;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT7.4
interrupt void DINTCH4_ISR(void)        // DMA
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group7;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT7.5
interrupt void DINTCH5_ISR(void)        // DMA
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group7;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT7.6
interrupt void DINTCH6_ISR(void)        // DMA
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group7;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT7.7 - 预留
// INT7.8 - 预留


// -----------------------------------------------------------
// 第8组PIE中断 - 映射到CPU的INT8
// -----------------------------------------------------------

// INT8.1
interrupt void I2CINT1A_ISR(void)       // I2C-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group8;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT8.2
interrupt void I2CINT2A_ISR(void)       // I2C-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group8;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT8.3 - 预留
// INT8.4 - 预留

// INT8.5
interrupt void SCIRXINTC_ISR(void)      // SCI-C
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group8;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT8.6
interrupt void SCITXINTC_ISR(void)     // SCI-C
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group8;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT8.7 - 预留
// INT8.8 - 预留


// -----------------------------------------------------------
// 第9组PIE中断 - 映射到CPU的INT9
// -----------------------------------------------------------

// INT9.1
interrupt void SCIRXINTA_ISR(void)      // SCI-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.2
interrupt void SCITXINTA_ISR(void)      // SCI-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.3
interrupt void SCIRXINTB_ISR(void)      // SCI-B
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.4
interrupt void SCITXINTB_ISR(void)      // SCI-B
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.5
interrupt void ECAN0INTA_ISR(void)      // eCAN-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.6
interrupt void ECAN1INTA_ISR(void)      // eCAN-A
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.7
interrupt void ECAN0INTB_ISR(void)      // eCAN-B
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;


    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT9.8
interrupt void ECAN1INTB_ISR(void)      // eCAN-B
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group9;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}


// -----------------------------------------------------------
// 第10组PIE中断 - 映射到CPU的INT10
// -----------------------------------------------------------

// INT10.1 - 预留
// INT10.2 - 预留
// INT10.3 - 预留
// INT10.4 - 预留
// INT10.5 - 预留
// INT10.6 - 预留
// INT10.7 - 预留
// INT10.8 - 预留


// -----------------------------------------------------------
// 第11组PIE中断 - 映射到CPU的INT11
// -----------------------------------------------------------

// INT11.1 - 预留
// INT11.2 - 预留
// INT11.3 - 预留
// INT11.4 - 预留
// INT11.5 - 预留
// INT11.6 - 预留
// INT11.7 - 预留
// INT11.8 - 预留


// -----------------------------------------------------------
// 第12组PIE中断 - 映射到CPU的INT12
// -----------------------------------------------------------

// INT12.1
interrupt void XINT3_ISR(void)      // 外部中断XINT3
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT12.2
interrupt void XINT4_ISR(void)      // 外部中断XINT4
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT12.3
interrupt void XINT5_ISR(void)      // 外部中断XINT5
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}
// INT12.4
interrupt void XINT6_ISR(void)      // 外部中断XINT6
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT12.5
interrupt void XINT7_ISR(void)      // 外部中断XINT7
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

// INT12.6 - Reserved
// INT12.7
interrupt void LVF_ISR(void)        // 锁存溢出中断
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}
// INT12.8
interrupt void LUF_ISR(void)        // 锁存下溢中断
{
    // 添加自己的代码

    // 要从本PIE组接收更多中断，请确认此组PIE中断的piectrlegs.pieak.all=pieak_group12;

    // 下面的两行调试代码仅用于停止处理器，插入中断服务代码后移除可移除。
    asm ("      ESTOP0");
    for(;;);
}

//---------------------------------------------------------------------------
// 所有默认中断:
//
interrupt void PIE_RESERVED(void)       // 预留空间，调试用
{
  asm ("      ESTOP0");
  for(;;);
}

interrupt void rsvd_ISR(void)           // 调试用
{
  asm ("      ESTOP0");
  for(;;);
}

//===========================================================================
// End of file.
//===========================================================================
