/******************************************************************************
文件名                     : AVP33x_ECap.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x增强型捕获模块寄存器位定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_ECAP_H
#define AVP33x_ECAP_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
// eCAP控制寄存1（ECCTL1）位定义  */
struct ECCTL1_BITS {            // 位描述
    Uint16 CAP1POL:1;           // 0      CAP1捕捉沿选择控制位
    Uint16 CTRRST1:1;           // 1      在CAP1事件中是否重置计数器控制位位
    Uint16 CAP2POL:1;           // 2      CAP2捕捉沿选择控制位
    Uint16 CTRRST2:1;           // 3      在CAP2事件中是否重置计数器控制位位
    Uint16 CAP3POL:1;           // 4      CAP3捕捉沿选择控制位
    Uint16 CTRRST3:1;           // 5      在CAP3事件中是否重置计数器控制位位
    Uint16 CAP4POL:1;           // 6      CAP4捕捉沿选择控制位
    Uint16 CTRRST4:1;           // 7      在CAP4事件中是否重置计数器控制位位
    Uint16 CAPLDEN:1;           // 8      在捕获事件中CAP1-4寄存器的加载使能控制位
    Uint16 PRESCALE:5;          // 13:9   输入信号分频选择控制位
    Uint16 FREE_SOFT:2;         // 15:14  仿真模式控制位
};

union ECCTL1_REG {
    Uint16              all;
    struct ECCTL1_BITS  bit;
};

//----------------------------------------------------
// eCAP控制寄存2（ECCTL2）位定义 */
struct ECCTL2_BITS {            // 位描述
    Uint16 CONT_ONESHT:1;       // 0      捕获模式控制位（0：连续模式；1：单次模式）
    Uint16 STOP_WRAP:2;         // 2:1    单次/连续模式下的停止值
    Uint16 REARM:1;             // 3      单次模式下，重新加载控制位
    Uint16 TSCTRSTOP:1;         // 4      计数器启停控制位
    Uint16 SYNCI_EN:1;          // 5      计数器（TSCTR）同步输入选择模式
    Uint16 SYNCO_SEL:2;         // 7:6    同步信号输出条件选择控制位
    Uint16 SWSYNC:1;            // 8      软件强制计数器同步控制位
    Uint16 CAP_APWM:1;          // 9      CAP与APWM模式选择控制位
    Uint16 APWMPOL:1;           // 10     APWM输出极性选择控制位
    Uint16 rsvd1:5;             // 15:11  预留
};

union ECCTL2_REG {
    Uint16              all;
    struct ECCTL2_BITS  bit;
};

//----------------------------------------------------
// eCAP中断使能寄存（ECEINT）位定义 */
struct ECEINT_BITS {            // 位描述
    Uint16 rsvd1:1;             // 0      预留
    Uint16 CEVT1:1;             // 1      捕获1中断使能控制位
    Uint16 CEVT2:1;             // 2      捕获2中断使能控制位
    Uint16 CEVT3:1;             // 3      捕获3中断使能控制位
    Uint16 CEVT4:1;             // 4      捕获4中断使能控制位
    Uint16 CTROVF:1;            // 5      计数溢出中断使能控制位
    Uint16 CTR_EQ_PRD:1;        // 6      计数器周期匹配中断使能控制位
    Uint16 CTR_EQ_CMP:1;        // 7      计数器比较匹配中断使能控制位
    Uint16 rsvd2:8;             // 15:8   预留
};

union ECEINT_REG {
    Uint16              all;
    struct ECEINT_BITS  bit;
};

//----------------------------------------------------
// eCAP中断标志寄存（ECFLG）位定义 */
struct ECFLG_BITS {             // 位描述
    Uint16 INT:1;               // 0      eCAP全局中断标志
    Uint16 CEVT1:1;             // 1      捕获1中断标志位
    Uint16 CEVT2:1;             // 2      捕获2中断标志位
    Uint16 CEVT3:1;             // 3      捕获3中断标志位
    Uint16 CEVT4:1;             // 4      捕获4中断标志位
    Uint16 CTROVF:1;            // 5      计数溢出中断标志位
    Uint16 CTR_EQ_PRD:1;        // 6      计数器周期匹配中断标志位
    Uint16 CTR_EQ_CMP:1;        // 7      计数器比较匹配中断标志位
    Uint16 rsvd2:8;             // 15:8   预留
};

union ECFLG_REG {
    Uint16              all;
    struct ECFLG_BITS   bit;
};

/**************************************/
/* eCAP控制和状态寄存器文件 */
/**************************************/
struct ECAP_REGS {
    Uint32              TSCTR;      // eCAP计数器寄存器
    Uint32              CTRPHS;     // eCAP相位寄存器
    Uint32              CAP1;       // eCAP1寄存器
    Uint32              CAP2;       // eCAP2寄存器
    Uint32              CAP3;       // eCAP3寄存器
    Uint32              CAP4;       // eCAP4寄存器
    Uint16              rsvd1[8];   // 预留
    union   ECCTL1_REG  ECCTL1;     // eCAP控制寄存器1
    union   ECCTL2_REG  ECCTL2;     // eCAP控制寄存器2
    union   ECEINT_REG  ECEINT;     // eCAP中断使能控制寄存
    union   ECFLG_REG   ECFLG;      // eCAP中断标志寄存器
    union   ECFLG_REG   ECCLR;      // eCAP中断标志清除寄存器
    union   ECEINT_REG  ECFRC;      // eCAP强制中断控制寄存器
    Uint16              rsvd2[6];   // 预留
};


//---------------------------------------------------------------------------
// eCAP外部引用和函数声明:
//
extern volatile struct ECAP_REGS ECap1Regs;
extern volatile struct ECAP_REGS ECap2Regs;
extern volatile struct ECAP_REGS ECap3Regs;
extern volatile struct ECAP_REGS ECap4Regs;
extern volatile struct ECAP_REGS ECap5Regs;
extern volatile struct ECAP_REGS ECap6Regs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_ECAP_H definition

//===========================================================================
// End of file.
//===========================================================================
