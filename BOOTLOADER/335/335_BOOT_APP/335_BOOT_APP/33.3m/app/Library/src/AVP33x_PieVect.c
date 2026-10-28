/******************************************************************************
文件名                     : AVP33x_PieVect.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片PIE向量表初始化函数
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // AVP33x Headerfile Include File
#include "AVP33x_Examples.h"   // AVP33x Examples Include File


const struct PIE_VECT_TABLE PieVectTableInit = {

    PIE_RESERVED,       // 0  预留空间
    PIE_RESERVED,       // 1  预留空间
    PIE_RESERVED,       // 2  预留空间
    PIE_RESERVED,       // 3  预留空间
    PIE_RESERVED,       // 4  预留空间
    PIE_RESERVED,       // 5  预留空间
    PIE_RESERVED,       // 6  预留空间
    PIE_RESERVED,       // 7  预留空间
    PIE_RESERVED,       // 8  预留空间
    PIE_RESERVED,       // 9  预留空间
    PIE_RESERVED,       // 10 预留空间
    PIE_RESERVED,       // 11 预留空间
    PIE_RESERVED,       // 12 预留空间

// 非外设中断
    INT13_ISR,          // XINT13或CPU定时器1
    INT14_ISR,          // CPU定时器2
    DATALOG_ISR,        // 数据记录中断
    RTOSINT_ISR,        // 实时操作系统中断
    EMUINT_ISR,         // 仿真中断
    NMI_ISR,            // 不可屏蔽中断
    ILLEGAL_ISR,        // 非法操作陷阱中断
    USER1_ISR,          // 用户定义陷阱1
    USER2_ISR,          // 用户定义陷阱2
    USER3_ISR,          // 用户定义陷阱3
    USER4_ISR,          // 用户定义陷阱4
    USER5_ISR,          // 用户定义陷阱5
    USER6_ISR,          // 用户定义陷阱6
    USER7_ISR,          // 用户定义陷阱7
    USER8_ISR,          // 用户定义陷阱8
    USER9_ISR,          // 用户定义陷阱9
    USER10_ISR,         // 用户定义陷阱10
    USER11_ISR,         // 用户定义陷阱11
    USER12_ISR,         // 用户定义陷阱12

// 第1组PIE中断向量
    SEQ1INT_ISR,        // 1.1 ADC SEQ1中断
    SEQ2INT_ISR,        // 1.2 ADC SEQ2中断
    rsvd_ISR,           // 1.3 预留
    XINT1_ISR,          // 1.4 外部中断XINT1
    XINT2_ISR,          // 1.5 外部中断XINT2
    ADCINT_ISR,         // 1.6 ADC中断
    TINT0_ISR,          // 1.7 CPU定时器0中断
    WAKEINT_ISR,        // 1.8 看门狗中断

// 第2组PIE中断向量
    EPWM1_TZINT_ISR,    // 2.1 EPWM-1 TZ中断
    EPWM2_TZINT_ISR,    // 2.2 EPWM-2 TZ中断
    EPWM3_TZINT_ISR,    // 2.3 EPWM-3 TZ中断
    EPWM4_TZINT_ISR,    // 2.4 EPWM-4 TZ中断
    EPWM5_TZINT_ISR,    // 2.5 EPWM-5 TZ中断
    EPWM6_TZINT_ISR,    // 2.6 EPWM-6 TZ中断
    rsvd_ISR,           // 2.7 预留
    rsvd_ISR,           // 2.8 预留
      
// 第3组PIE中断向量
    EPWM1_INT_ISR,      // 3.1 EPWM-1中断
    EPWM2_INT_ISR,      // 3.2 EPWM-2中断
    EPWM3_INT_ISR,      // 3.3 EPWM-3中断
    EPWM4_INT_ISR,      // 3.4 EPWM-4中断
    EPWM5_INT_ISR,      // 3.5 EPWM-5中断
    EPWM6_INT_ISR,      // 3.6 EPWM-6中断
    rsvd_ISR,           // 3.7 预留
    rsvd_ISR,           // 3.8 预留
      
// 第4组PIE中断向量
    ECAP1_INT_ISR,      // 4.1 ECAP-1中断
    ECAP2_INT_ISR,      // 4.2 ECAP-2中断
    ECAP3_INT_ISR,      // 4.3 ECAP-3中断
    ECAP4_INT_ISR,      // 4.4 ECAP-4中断
    ECAP5_INT_ISR,      // 4.5 ECAP-5中断
    ECAP6_INT_ISR,      // 4.6 ECAP-6中断
    rsvd_ISR,           // 4.7 预留
    rsvd_ISR,           // 4.8 预留
      
// 第5组PIE中断向量
    EQEP1_INT_ISR,      // 5.1 EQEP-1中断
    EQEP2_INT_ISR,      // 5.2 EQEP-2中断
    rsvd_ISR,           // 5.3 预留
    rsvd_ISR,           // 5.4 预留
    rsvd_ISR,           // 5.5 预留
    rsvd_ISR,           // 5.6 预留
    rsvd_ISR,           // 5.7 预留
    rsvd_ISR,           // 5.8 预留

// 第6组PIE中断向量
    SPIRXINTA_ISR,      // 6.1 SPI-A中断
    SPITXINTA_ISR,      // 6.2 SPI-A中断
    MRINTA_ISR,         // 6.3 McBSP-A中断
    MXINTA_ISR,         // 6.4 McBSP-A中断
    MRINTB_ISR,         // 6.5 McBSP-B中断
    MXINTB_ISR,         // 6.6 McBSP-B中断
    rsvd_ISR,           // 6.7 预留
    rsvd_ISR,           // 6.8 预留
      
// 第7组PIE中断向量
    DINTCH1_ISR,        // 7.1 DMA通道1中断
    DINTCH2_ISR,        // 7.2 DMA通道2中断
    DINTCH3_ISR,        // 7.3 DMA通道3中断
    DINTCH4_ISR,        // 7.4 DMA通道4中断
    DINTCH5_ISR,        // 7.5 DMA通道5中断
    DINTCH6_ISR,        // 7.6 DMA通道6中断
    rsvd_ISR,           // 7.7 预留
    rsvd_ISR,           // 7.8 预留

// 第8组PIE中断向量
    I2CINT1A_ISR,       // 8.1 I2C中断
    I2CINT2A_ISR,       // 8.2 I2C中断
    rsvd_ISR,           // 8.3 预留
    rsvd_ISR,           // 8.4 预留
    SCIRXINTC_ISR,      // 8.5 SCI-C中断
    SCITXINTC_ISR,      // 8.6 SCI-C中断
    rsvd_ISR,           // 8.7 预留
    rsvd_ISR,           // 8.8 预留
      
// 第9组PIE中断向量
    SCIRXINTA_ISR,      // 9.1 SCI-A中断
    SCITXINTA_ISR,      // 9.2 SCI-A中断
    SCIRXINTB_ISR,      // 9.3 SCI-B中断
    SCITXINTB_ISR,      // 9.4 SCI-B中断
    ECAN0INTA_ISR,      // 9.5 eCAN-A中断
    ECAN1INTA_ISR,      // 9.6 eCAN-A中断
    ECAN0INTB_ISR,      // 9.7 eCAN-B中断
    ECAN1INTB_ISR,      // 9.8 eCAN-B中断
      
// 第10组PIE中断向量
    rsvd_ISR,           // 10.1 预留
    rsvd_ISR,           // 10.2 预留
    rsvd_ISR,           // 10.3 预留
    rsvd_ISR,           // 10.4 预留
    rsvd_ISR,           // 10.5 预留
    rsvd_ISR,           // 10.6 预留
    rsvd_ISR,           // 10.7 预留
    rsvd_ISR,           // 10.8 预留
            
// 第11组PIE中断向量
    rsvd_ISR,           // 11.1 预留
    rsvd_ISR,           // 11.2 预留
    rsvd_ISR,           // 11.3 预留
    rsvd_ISR,           // 11.4 预留
    rsvd_ISR,           // 11.5 预留
    rsvd_ISR,           // 11.6 预留
    rsvd_ISR,           // 11.7 预留
    rsvd_ISR,           // 11.8 预留
        
// 第12组PIE中断向量
    XINT3_ISR,          // 12.1 外部中断XINT3
    XINT4_ISR,          // 12.2 外部中断XINT4
    XINT5_ISR,          // 12.3 外部中断XINT5
    XINT6_ISR,          // 12.4 外部中断XINT6
    XINT7_ISR,          // 12.5 外部中断XINT7
    rsvd_ISR,           // 12.6 预留
    LVF_ISR,            // 12.7 预留
    LUF_ISR,            // 12.8 预留
};

//---------------------------------------------------------------------------
// InitPieVectTable: 
//---------------------------------------------------------------------------
// 初始化PIE中断向量表函数（必须在boot完成后执行）
//
void InitPieVectTable(void)
{
    int16	i;
    Uint32 *Source = (void *) &PieVectTableInit;
    volatile Uint32 *Dest = (void *) &PieVectTable;

    EALLOW;
    for(i=0; i < 128; i++)
        *Dest++ = *Source++;
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;  // 使能PIE中断向量表
			
}

//===========================================================================
// End of file.
//===========================================================================
