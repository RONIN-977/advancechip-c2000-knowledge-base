/******************************************************************************
文件名                     : AVP33x_XIntrupt.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片外部中断模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_XINTRUPT_H
#define AVP33x_XINTRUPT_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* 外部中断控制寄存器（XINTCR）位定义 */
struct XINTCR_BITS {
    Uint16   ENABLE:1;      // 0      XINTn（n=1~7）外部中断使能控制位
    Uint16   rsvd1:1;       // 1      预留
    Uint16   POLARITY:2;    // 3:2    输入信号选择中断触发边沿（00：下降沿触发中断；01：上升沿触发中断；10：下降沿触发中断；11：上升沿和下降沿都触发中断。）
    Uint16   rsvd2:12;      //15:4    预留
};

union XINTCR_REG {
    Uint16               all;
    struct XINTCR_BITS   bit;
};  

//----------------------------------------------------
/* 外部中断控制寄存器（XNMICR）位定义 */
struct XNMICR_BITS {
    Uint16   ENABLE:1;      // 0      XNMI中断使能控制位
    Uint16   SELECT:1;      // 1      CPU的INT13选择中断源（0：Timer1中断请求信号连接到INT13；1：XNMI_XINT13中断请求信号连接到INT13）
    Uint16   POLARITY:2;    // 3:2    输入信号XNMI选择中断触发沿
    Uint16   rsvd2:12;      // 15:4   预留
};

union XNMICR_REG {
    Uint16               all;
    struct XNMICR_BITS   bit;
};  

/**************************************/
/*            外部中断寄存器文件                                    */
/**************************************/
struct XINTRUPT_REGS {
    union XINTCR_REG XINT1CR;
    union XINTCR_REG XINT2CR;
    union XINTCR_REG XINT3CR;
    union XINTCR_REG XINT4CR;
    union XINTCR_REG XINT5CR;
    union XINTCR_REG XINT6CR;
    union XINTCR_REG XINT7CR;
    union XNMICR_REG XNMICR;
    Uint16           XINT1CTR;
    Uint16           XINT2CTR;
    Uint16           rsvd[5];
    Uint16           XNMICTR;
};

//----------------------------------------------------
// 外部中断外部引用和函数声明:
//
extern volatile struct XINTRUPT_REGS XIntruptRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_XINTF_H definition

//===========================================================================
// End of file.
//===========================================================================
