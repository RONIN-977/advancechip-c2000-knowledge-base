/******************************************************************************
文件名                     : AVP33x_PieCtrl.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片PIE模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_PIE_CTRL_H
#define AVP33x_PIE_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* PIE控制寄存器（PIECTRL）位定义 */
struct PIECTRL_BITS {       // 位描述
    Uint16  ENPIE:1;        // 0    PIE中断向量表使能控制位
    Uint16  PIEVECT:15;     // 15:1 PIE中断向量
};

union PIECTRL_REG {
    Uint16               all;
    struct PIECTRL_BITS  bit;
};  

//----------------------------------------------------
/* PIE中断使能寄存器（PIEIER）位定义 */
struct PIEIER_BITS {        // 位描述
    Uint16 INTx1:1;         // 0    INTx.1
    Uint16 INTx2:1;         // 1    INTx.2
    Uint16 INTx3:1;         // 2    INTx.3
    Uint16 INTx4:1;         // 3    INTx.4
    Uint16 INTx5:1;         // 4    INTx.5
    Uint16 INTx6:1;         // 5    INTx.6
    Uint16 INTx7:1;         // 6    INTx.7
    Uint16 INTx8:1;         // 7    INTx.8
    Uint16 rsvd:8;          // 15:8 预留
};

union PIEIER_REG {
    Uint16              all;
    struct PIEIER_BITS  bit;
}; 

//----------------------------------------------------
/* PIE中断标志寄存器（PIEIFR）位定义 */
struct PIEIFR_BITS {        // 位描述
    Uint16 INTx1:1;         // 0    INTx.1
    Uint16 INTx2:1;         // 1    INTx.2
    Uint16 INTx3:1;         // 2    INTx.3
    Uint16 INTx4:1;         // 3    INTx.4
    Uint16 INTx5:1;         // 4    INTx.5
    Uint16 INTx6:1;         // 5    INTx.6
    Uint16 INTx7:1;         // 6    INTx.7
    Uint16 INTx8:1;         // 7    INTx.8
    Uint16 rsvd:8;          // 15:8 预留
};

union PIEIFR_REG {
    Uint16              all;
    struct PIEIFR_BITS  bit;
};

//----------------------------------------------------
/* PIE中断应答寄存器（PIEACK）位定义 */
struct PIEACK_BITS {        // 位描述
    Uint16 ACK1:1;          // 0    PIE中断向量组1响应标志位
    Uint16 ACK2:1;          // 1    PIE中断向量组2响应标志位
    Uint16 ACK3:1;          // 2    PIE中断向量组3响应标志位
    Uint16 ACK4:1;          // 3    PIE中断向量组4响应标志位
    Uint16 ACK5:1;          // 4    PIE中断向量组5响应标志位
    Uint16 ACK6:1;          // 5    PIE中断向量组6响应标志位
    Uint16 ACK7:1;          // 6    PIE中断向量组7响应标志位
    Uint16 ACK8:1;          // 7    PIE中断向量组8响应标志位
    Uint16 ACK9:1;          // 8    PIE中断向量组9响应标志位
    Uint16 ACK10:1;         // 9    PIE中断向量组10响应标志位
    Uint16 ACK11:1;         // 10   PIE中断向量组11响应标志位
    Uint16 ACK12:1;         // 11   PIE中断向量组12响应标志位
    Uint16 rsvd:4;          // 15:12 预留
};

union PIEACK_REG {
    Uint16              all;
    struct PIEACK_BITS  bit;
};

/**************************************/
/*            PIE控制寄存器文件                                  */
/**************************************/
struct PIE_CTRL_REGS {
    union PIECTRL_REG PIECTRL;      // PIE控制寄存器
    union PIEACK_REG  PIEACK;       // PIE应答寄存器
    union PIEIER_REG  PIEIER1;      // PIE中断组1使能寄存器
    union PIEIFR_REG  PIEIFR1;      // PIE中断组1标志寄存器
    union PIEIER_REG  PIEIER2;      // PIE中断组2使能寄存器
    union PIEIFR_REG  PIEIFR2;      // PIE中断组2标志寄存器
    union PIEIER_REG  PIEIER3;      // PIE中断组3使能寄存器
    union PIEIFR_REG  PIEIFR3;      // PIE中断组3标志寄存器
    union PIEIER_REG  PIEIER4;      // PIE中断组4使能寄存器
    union PIEIFR_REG  PIEIFR4;      // PIE中断组4标志寄存器
    union PIEIER_REG  PIEIER5;      // PIE中断组5使能寄存器
    union PIEIFR_REG  PIEIFR5;      // PIE中断组5标志寄存器
    union PIEIER_REG  PIEIER6;      // PIE中断组6使能寄存器
    union PIEIFR_REG  PIEIFR6;      // PIE中断组6标志寄存器
    union PIEIER_REG  PIEIER7;      // PIE中断组7使能寄存器
    union PIEIFR_REG  PIEIFR7;      // PIE中断组7标志寄存器
    union PIEIER_REG  PIEIER8;      // PIE中断组8使能寄存器
    union PIEIFR_REG  PIEIFR8;      // PIE中断组8标志寄存器
    union PIEIER_REG  PIEIER9;      // PIE中断组9使能寄存器
    union PIEIFR_REG  PIEIFR9;      // PIE中断组9标志寄存器
    union PIEIER_REG  PIEIER10;     // PIE中断组10使能寄存器
    union PIEIFR_REG  PIEIFR10;     // PIE中断组10标志寄存器
    union PIEIER_REG  PIEIER11;     // PIE中断组11使能寄存器
    union PIEIFR_REG  PIEIFR11;     // PIE中断组11标志寄存器
    union PIEIER_REG  PIEIER12;     // PIE中断组12使能寄存器
    union PIEIFR_REG  PIEIFR12;     // PIE中断组12标志寄存器
};     

#define PIEACK_GROUP1   0x0001
#define PIEACK_GROUP2   0x0002
#define PIEACK_GROUP3   0x0004
#define PIEACK_GROUP4   0x0008
#define PIEACK_GROUP5   0x0010
#define PIEACK_GROUP6   0x0020
#define PIEACK_GROUP7   0x0040
#define PIEACK_GROUP8   0x0080
#define PIEACK_GROUP9   0x0100
#define PIEACK_GROUP10  0x0200
#define PIEACK_GROUP11  0x0400
#define PIEACK_GROUP12  0x0800

//---------------------------------------------------------------------------
// PIE控制寄存器外部引用和函数声明:
//
extern volatile struct PIE_CTRL_REGS PieCtrlRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_PIE_CTRL_H definition

//===========================================================================
// End of file.
//===========================================================================
