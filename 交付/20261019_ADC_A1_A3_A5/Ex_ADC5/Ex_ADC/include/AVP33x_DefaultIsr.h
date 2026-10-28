/******************************************************************************
文件名                     : AVP33x_DefaultIsr.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x默认的中断服务程序定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_DEFAULT_ISR_H
#define AVP33x_DEFAULT_ISR_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// 默认的中断服务程序声明：
// 
// 以下函数原型用于与默认的PIE向量表一起使用的默认ISR例程。在avp33x_pievect.h文件中可以找到此向量表。
//

// 非外设中断:
interrupt void INT13_ISR(void);         // XINT13或CPU-Timer 1
interrupt void INT14_ISR(void);         // CPU-Timer2
interrupt void DATALOG_ISR(void);       // 数据记录中断
interrupt void RTOSINT_ISR(void);       // 实时操作系统中断
interrupt void EMUINT_ISR(void);        // 仿真中断
interrupt void NMI_ISR(void);           // 不可屏蔽中断
interrupt void ILLEGAL_ISR(void);       // 非法操作陷阱中断
interrupt void USER1_ISR(void);         // 用户自定义中断1
interrupt void USER2_ISR(void);         // 用户自定义中断2
interrupt void USER3_ISR(void);         // 用户自定义中断3
interrupt void USER4_ISR(void);         // 用户自定义中断4
interrupt void USER5_ISR(void);         // 用户自定义中断5
interrupt void USER6_ISR(void);         // 用户自定义中断6
interrupt void USER7_ISR(void);         // 用户自定义中断7
interrupt void USER8_ISR(void);         // 用户自定义中断8
interrupt void USER9_ISR(void);         // 用户自定义中断9
interrupt void USER10_ISR(void);        // 用户自定义中断10
interrupt void USER11_ISR(void);        // 用户自定义中断11
interrupt void USER12_ISR(void);        // 用户自定义中断12

// 第1组PIE中断服务程序:
interrupt void  SEQ1INT_ISR(void);      // ADC排序器1中断
interrupt void  SEQ2INT_ISR(void);      // ADC排序器2中断
interrupt void  XINT1_ISR(void);        // 外部中断1
interrupt void  XINT2_ISR(void);        // 外部中断2
interrupt void  ADCINT_ISR(void);       // ADC中断
interrupt void  TINT0_ISR(void);        // Timer0中断
interrupt void  WAKEINT_ISR(void);      // 看门狗中断

// 第2组PIE中断服务程序:
interrupt void EPWM1_TZINT_ISR(void);   // EPWM-1中断
interrupt void EPWM2_TZINT_ISR(void);   // EPWM-2中断
interrupt void EPWM3_TZINT_ISR(void);   // EPWM-3中断
interrupt void EPWM4_TZINT_ISR(void);   // EPWM-4中断
interrupt void EPWM5_TZINT_ISR(void);   // EPWM-5中断
interrupt void EPWM6_TZINT_ISR(void);   // EPWM-6中断
      
// 第3组PIE中断服务程序:
interrupt void EPWM1_INT_ISR(void);     // EPWM-1中断
interrupt void EPWM2_INT_ISR(void);     // EPWM-2中断
interrupt void EPWM3_INT_ISR(void);     // EPWM-3中断
interrupt void EPWM4_INT_ISR(void);     // EPWM-4中断
interrupt void EPWM5_INT_ISR(void);     // EPWM-5中断
interrupt void EPWM6_INT_ISR(void);     // EPWM-6中断
      
// 第4组PIE中断服务程序:
interrupt void ECAP1_INT_ISR(void);     // ECAP-1中断
interrupt void ECAP2_INT_ISR(void);     // ECAP-2中断
interrupt void ECAP3_INT_ISR(void);     // ECAP-3中断
interrupt void ECAP4_INT_ISR(void);     // ECAP-4中断
interrupt void ECAP5_INT_ISR(void);     // ECAP-5中断
interrupt void ECAP6_INT_ISR(void);     // ECAP-6中断
     
// 第5组PIE中断服务程序:
interrupt void EQEP1_INT_ISR(void);     // EQEP-1中断
interrupt void EQEP2_INT_ISR(void);     // EQEP-2中断

// 第6组PIE中断服务程序:
interrupt void SPIRXINTA_ISR(void);     // SPI-A中断
interrupt void SPITXINTA_ISR(void);     // SPI-A中断
interrupt void MRINTA_ISR(void);        // McBSP-A中断
interrupt void MXINTA_ISR(void);        // McBSP-A中断
interrupt void MRINTB_ISR(void);        // McBSP-B中断
interrupt void MXINTB_ISR(void);        // McBSP-B中断

// 第7组PIE中断服务程序:
interrupt void DINTCH1_ISR(void);       // DMA-通道1中断
interrupt void DINTCH2_ISR(void);       // DMA-通道2中断
interrupt void DINTCH3_ISR(void);       // DMA-通道3中断
interrupt void DINTCH4_ISR(void);       // DMA-通道4中断
interrupt void DINTCH5_ISR(void);       // DMA-通道5中断
interrupt void DINTCH6_ISR(void);       // DMA-通道6中断

// 第8组PIE中断服务程序:
interrupt void I2CINT1A_ISR(void);      // I2C-A中断
interrupt void I2CINT2A_ISR(void);      // I2C-A中断
interrupt void SCIRXINTC_ISR(void);     // SCI-C中断
interrupt void SCITXINTC_ISR(void);     // SCI-C中断

// 第9组PIE中断服务程序:
interrupt void SCIRXINTA_ISR(void);     // SCI-A中断
interrupt void SCITXINTA_ISR(void);     // SCI-A中断
interrupt void SCIRXINTB_ISR(void);     // SCI-B中断
interrupt void SCITXINTB_ISR(void);     // SCI-B中断
interrupt void ECAN0INTA_ISR(void);     // eCAN-A中断
interrupt void ECAN1INTA_ISR(void);     // eCAN-A中断
interrupt void ECAN0INTB_ISR(void);     // eCAN-B中断
interrupt void ECAN1INTB_ISR(void);     // eCAN-B中断

// 第10组PIE中断服务程序（预留）:
 
// 第11组PIE中断服务程序（预留）:

// 第12组PIE中断服务程序:
interrupt void  XINT3_ISR(void);        // 外部中断3
interrupt void  XINT4_ISR(void);        // 外部中断4
interrupt void  XINT5_ISR(void);        // 外部中断5
interrupt void  XINT6_ISR(void);        // 外部中断6
interrupt void  XINT7_ISR(void);        // 外部中断7
interrupt void  LVF_ISR(void);          // 锁存上溢中断
interrupt void  LUF_ISR(void);          // 锁存下溢中断

// 为测试而保留的中断:
interrupt void PIE_RESERVED(void);      // 预留作测试用
interrupt void rsvd_ISR(void);          // 测试用的中断
interrupt void INT_NOTUSED_ISR(void);   // 未用到的中断

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif    // end of AVP33x_DEFAULT_ISR_H definition

//===========================================================================
// End of file.
//===========================================================================
