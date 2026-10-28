/******************************************************************************
文件名                     : AVP33x_EQep.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x增强型正交编码模块寄存器位定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_EQEP_H
#define AVP33x_EQEP_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* QEP解码控制寄存器（QDECCTL）位定义 */
struct QDECCTL_BITS {           // 位描述
    Uint16 rsvd1:5;             // 4:0    预留
    Uint16 QSP:1;               // 5      QEPS输入极性控制位
    Uint16 QIP:1;               // 6      QEPI输入极性控制位
    Uint16 QBP:1;               // 7      QEPB输入极性控制位
    Uint16 QAP:1;               // 8      QEPA输入极性控制位
    Uint16 IGATE:1;             // 9      索引脉冲选通使能控制位
    Uint16 SWAP:1;              // 10     正交时钟变换输入使能控制位
    Uint16 XCR:1;               // 11     外部时钟计数模式选择控制位（0：上下边沿都计数；1：上升沿计数）
    Uint16 SPSEL:1;             // 12     同步信号引脚选择控制位（0：Index引脚用于输出；1：Strobe引脚用于输出）
    Uint16 SOEN:1;              // 13     同步信号输出使能控制位（0：禁止位置比较同步信号输出；1：使能位置比较同步信号输出）
    Uint16 QSRC:2;              // 15:14  位置计数器源选择控制位（00：正交计数模式；01：直接计数模式；10：向上计数模式；11：向下计数模式）
};

union QDECCTL_REG {
    Uint16              all;
    struct QDECCTL_BITS   bit;
};

//----------------------------------------------------
/* QEP控制寄存器（QEPCTL）位定义 */
struct QEPCTL_BITS {            // 位描述
    Uint16 WDE:1;               // 0      QEP看门狗使能控制位（0：禁止QEP看门狗；1：使能QEP看门狗）
    Uint16 UTE:1;               // 1      QEP单位定时器使能控制位（0：禁止定时器；1：使能定时器）
    Uint16 QCLM:1;              // 2      QEP捕获锁存模式控制位
    Uint16 QPEN:1;              // 3      正交位置计数器使能控制位
    Uint16 IEL:2;               // 5:4    索引事件锁存位置计数器
    Uint16 SEL:1;               // 6      索引事件锁存位置计数器
    Uint16 SWI:1;               // 7      软件初始化位置计数器
    Uint16 IEI:2;               // 9:8    索引事件初始位置计数器
    Uint16 SEI:2;               // 11:10  索引事件初始位置计数器
    Uint16 PCRM:2;              // 13:12  位置计数器复位模式
    Uint16 FREE_SOFT:2;         // 15:14  仿真模式控制位
};

union QEPCTL_REG {
    Uint16               all;
    struct QEPCTL_BITS   bit;
};         

//----------------------------------------------------
/* QEP捕获控制寄存器（QCAPCTL）位定义 */
struct QCAPCTL_BITS {           // 位描述
    Uint16 UPPS:4;              // 3:0    单位位置事件分频系数控制位
    Uint16 CCPS:3;              // 6:4    QEP捕获时钟分频系数
    Uint16 rsvd1:8;             // 14:7   预留
    Uint16 CEN:1;               // 15     QEP捕获功能使能控制位
};

union QCAPCTL_REG {
    Uint16               all;
    struct QCAPCTL_BITS  bit;
}; 

//----------------------------------------------------
/* QEP捕获位置比较控制寄存器（QCAPCTL）位定义 */
struct QPOSCTL_BITS {           // 位描述
    Uint16 PCSPW:12;            // 11:0   选择位置比较同步输出脉冲宽度控制位
    Uint16 PCE:1;               // 12     位置比较功能使能控制位（0：禁止；1：使能）
    Uint16 PCPOL:1;             // 13     同步信号输出极性
    Uint16 PCLOAD:1;            // 14     位置比较影子映射加载模式控制位
    Uint16 PCSHDW:1;            // 15     位置比较影子映射使能控制位
};

union QPOSCTL_REG {
    Uint16               all;
    struct QPOSCTL_BITS  bit;
};         

//----------------------------------------------------
/* QEP中断使能寄存器（QEINT）位定义 */
struct QEINT_BITS {             // 位描述
    Uint16 rsvd1:1;             // 0      预留
    Uint16 PCE:1;               // 1      位置计数器错误中断使能控制位
    Uint16 QPE:1;               // 2      正交相位错误中断使能控制位
    Uint16 QDC:1;               // 3      看门狗方向改变中断使能控制位
    Uint16 WTO:1;               // 4      看门狗超时中断使能控制位
    Uint16 PCU:1;               // 5      位置计数下溢中断使能控制位
    Uint16 PCO:1;               // 6      位置计数上溢中断使能控制位
    Uint16 PCR:1;               // 7      位置比较准备中断使能控制位
    Uint16 PCM:1;               // 8      位置比较匹配中断使能控制位
    Uint16 SEL:1;               // 9      位置事件锁存中断使能控制位
    Uint16 IEL:1;               // 10     索引事件锁存中断使能控制位
    Uint16 UTO:1;               // 11     单位时间事件中断使能控制位
    Uint16 rsvd2:4;             // 15:12  预留
};

union QEINT_REG {
    Uint16               all;
    struct QEINT_BITS    bit;
};

//----------------------------------------------------
/* QEP中断标志寄存器（QFLG）位定义 */
struct QFLG_BITS {              // 位描述
    Uint16 INT:1;               // 0      QEP全局中断标志位
    Uint16 PCE:1;               // 1      位置计数器错误中断标志位
    Uint16 PHE:1;               // 2      正交相位错误中断标志位
    Uint16 QDC:1;               // 3      看门狗方向改变中断标志位
    Uint16 WTO:1;               // 4      看门狗超时中断标志位
    Uint16 PCU:1;               // 5      位置计数下溢中断标志位
    Uint16 PCO:1;               // 6      位置计数上溢中断标志位
    Uint16 PCR:1;               // 7      位置比较准备中断标志位
    Uint16 PCM:1;               // 8      位置比较匹配中断标志位
    Uint16 SEL:1;               // 9      位置事件锁存中断标志位
    Uint16 IEL:1;               // 10     索引事件锁存中断标志位
    Uint16 UTO:1;               // 11     单位时间事件中断标志位
    Uint16 rsvd2:4;             // 15:12  预留
};

union QFLG_REG {
    Uint16               all;
    struct QFLG_BITS     bit;
};

//----------------------------------------------------
/* QEP中断强制寄存器（QFRC）位定义 */
struct QFRC_BITS {              // 位描述
    Uint16 reserved:1;          // 0      预留
    Uint16 PCE:1;               // 1      强制位置计数器错误中断控制位
    Uint16 PHE:1;               // 2      强制正交相位错误中断控制位
    Uint16 QDC:1;               // 3      强制看门狗方向改变中断控制位
    Uint16 WTO:1;               // 4      强制看门狗超时中断控制位
    Uint16 PCU:1;               // 5      强制位置计数下溢中断控制位
    Uint16 PCO:1;               // 6      强制位置计数上溢中断控制位
    Uint16 PCR:1;               // 7      强制位置比较准备中断控制位
    Uint16 PCM:1;               // 8      强制位置比较匹配中断控制位
    Uint16 SEL:1;               // 9      强制位置事件锁存中断控制位
    Uint16 IEL:1;               // 10     强制索引事件锁存中断控制位
    Uint16 UTO:1;               // 11     强制单位时间事件中断控制位
    Uint16 rsvd2:4;             // 15:12  预留
};

union QFRC_REG {
    Uint16               all;
    struct QFRC_BITS     bit;
};

//----------------------------------------------------
/* QEP状态寄存器（QEPSTS）位定义 */
struct QEPSTS_BITS {            // 位描述
    Uint16 PCEF:1;              // 0      位置计数器错误状态标志位
    Uint16 FIMF:1;              // 1      第一个索引标记标志位
    Uint16 CDEF:1;              // 2      捕获方向错误标志位
    Uint16 COEF:1;              // 3      捕获溢出错误标志位
    Uint16 QDLF:1;              // 4      QEP方向锁存标志位
    Uint16 QDF:1;               // 5      正交方向标志位
    Uint16 FIDF:1;              // 6      第一个索引标记方向标志位
    Uint16 UPEVNT:1;            // 7      单位位置事件标志位
    Uint16 rsvd1:8;             // 15:8   预留
};

union QEPSTS_REG {
    Uint16               all;
    struct QEPSTS_BITS   bit;
};

/**************************************/
/* QEP控制和状态寄存器文件 */
/**************************************/
struct EQEP_REGS {
    Uint32              QPOSCNT;    // QEP位置计数器寄存器
    Uint32              QPOSINIT;   // QEP位置计数器初始化寄存器
    Uint32              QPOSMAX;    // QEP最大位置计数器寄存器
    Uint32              QPOSCMP;    // QEP位置比较寄存器
    Uint32              QPOSILAT;   // QEP索引位置加载寄存器
    Uint32              QPOSSLAT;   // QEP标记位置加载寄存器
    Uint32              QPOSLAT;    // QEP位置加载寄存器
    Uint32              QUTMR;      // QEP单位时间寄存器
    Uint32              QUPRD;      // QEP单位周期寄存器
    Uint16              QWDTMR;     // QEP看门狗定时机器寄存器
    Uint16              QWDPRD;     // QEP看门狗周期寄存器
    union  QDECCTL_REG  QDECCTL;    // QEP解码控制寄存器
    union  QEPCTL_REG   QEPCTL;     // QEP控制寄存器
    union  QCAPCTL_REG  QCAPCTL;    // QEP捕获控制寄存器
    union  QPOSCTL_REG  QPOSCTL;    // QEP位置比较控制寄存器
    union  QEINT_REG    QEINT;      // QEP中断控制寄存器
    union  QFLG_REG     QFLG;       // QEP中断标志寄存器
    union  QFLG_REG     QCLR;       // QEP中断标志清除寄存器
    union  QFRC_REG     QFRC;       // QEP中断强制寄存器
    union  QEPSTS_REG   QEPSTS;     // QEP状态寄存器
    Uint16              QCTMR;      // QEP捕获定时器寄存器
    Uint16              QCPRD;      // QEP捕获周期寄存器
    Uint16              QCTMRLAT;   // QEP捕获定时锁存寄存器
    Uint16              QCPRDLAT;   // QEP捕获周期锁存寄存器
    Uint16              rsvd1[30];  // 预留
};

//---------------------------------------------------------------------------
// QEP外部引用和函数声明:
//
extern volatile struct EQEP_REGS EQep1Regs;
extern volatile struct EQEP_REGS EQep2Regs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_EQEP_H definition

//===========================================================================
// End of file.
//===========================================================================
