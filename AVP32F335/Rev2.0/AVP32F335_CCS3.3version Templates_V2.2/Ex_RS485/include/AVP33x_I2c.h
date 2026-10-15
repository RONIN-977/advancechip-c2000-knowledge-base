/******************************************************************************
文件名                     : AVP33x_I2c.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片I2C模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_I2C_H
#define AVP33x_I2C_H


#ifdef __cplusplus
extern "C" {
#endif


//----------------------------------------------------
/* I2C中断向量寄存器（I2CISRC）位定义 */
struct I2CISRC_BITS {           // 位描述
    Uint16 INTCODE:3;           // 2:0    中断源
    Uint16 rsvd1:13;            // 15:3   预留
};

union I2CISRC_REG {
    Uint16                 all;
    struct I2CISRC_BITS    bit;
};

//----------------------------------------------------
/* I2C中断使能寄存器（I2CIER）位定义 */
struct I2CIER_BITS {            // 位描述
    Uint16 ARBL:1;              // 0      总线仲裁失败中断使能控制位
    Uint16 NACK:1;              // 1      无应答信号中断使能控制位
    Uint16 ARDY:1;              // 2      寄存器访问准备就绪中断使能控制位
    Uint16 RRDY:1;              // 3      接收数据就绪中断使能控制位
    Uint16 XRDY:1;              // 4      发送数据就绪中断使能控制位
    Uint16 SCD:1;               // 5      停止信号中断使能控制位
    Uint16 AAS:1;               // 6      从机地址中断使能控制位
    Uint16 rsvd:9;              // 15:7   预留
};

union I2CIER_REG {
    Uint16                 all;
    struct I2CIER_BITS     bit;
};

//----------------------------------------------------
/* I2C状态寄存器（I2CSTR）位定义 */
struct I2CSTR_BITS {            // 位描述
    Uint16 ARBL:1;              // 0      仲裁失败中断标志位
    Uint16 NACK:1;              // 1      无应答中断标志位
    Uint16 ARDY:1;              // 2      寄存器访问准备就绪中断标志位
    Uint16 RRDY:1;              // 3      接收数据就绪中断标志位
    Uint16 XRDY:1;              // 4      发送数据就绪中断标志位
    Uint16 SCD:1;               // 5      停止信号标志位
    Uint16 rsvd1:2;             // 7:6    预留
    Uint16 AD0:1;               // 8      全0地址标志位
    Uint16 AAS:1;               // 9      从机地址标志位
    Uint16 XSMT:1;              // 10     发送移位寄存器空标志位
    Uint16 RSFULL:1;            // 11     接收移位寄存器满标志位
    Uint16 BB:1;                // 12     总线忙标志位
    Uint16 NACKSNT:1;           // 13     发送无应答标志位
    Uint16 SDIR:1;              // 14     从节点方向标志位
    Uint16 rsvd2:1;             // 15     预留
};

union I2CSTR_REG {
    Uint16                 all;
    struct I2CSTR_BITS     bit;
};

//----------------------------------------------------
/* I2C模式寄存器（I2CMDR）位定义 */
struct I2CMDR_BITS {            // 位描述
    Uint16 BC:3;                // 2:0    I2C收发数据位数控制位
    Uint16 FDF:1;               // 3      全数据格式使能控制位
    Uint16 STB:1;               // 4      起始字节模式控制位
    Uint16 IRS:1;               // 5      I2C模块复位控制位
    Uint16 DLB:1;               // 6      I2C自测模式使能控制位
    Uint16 RM:1;                // 7      I2C循环模式控制位（仅限于I2C主节点使用）
    Uint16 XA:1;                // 8      扩充地址使能控制位
    Uint16 TRX:1;               // 9      发送/接收模式控制位
    Uint16 MST:1;               // 10     主/从模式切换控制位
    Uint16 STP:1;               // 11     停止位（仅限于I2C主节点使用）
    Uint16 rsvd1:1;             // 12     预留
    Uint16 STT:1;               // 13     起始位（仅限于I2C主节点使用）
    Uint16 FREE:1;              // 14     仿真模式控制位
    Uint16 NACKMOD:1;           // 15     无应答模式控制位
};

union I2CMDR_REG {
    Uint16                 all;
    struct I2CMDR_BITS     bit;
};

//----------------------------------------------------
/* I2C扩展模式控制寄存器（I2CEMDR）位定义 */
struct I2CEMDR_BITS {           // 位描述
    Uint16 BCM:1;               // 0      向后兼容模式控制位
    Uint16 rsvd:15;             // 15     预留
};

union I2CEMDR_REG {
    Uint16                 all;
    struct I2CEMDR_BITS     bit;
};

//----------------------------------------------------
/* I2C预分频寄存器（I2CPSC）位定义 */
struct I2CPSC_BITS {            // 位描述
    Uint16 IPSC:8;              // 7:0    预分频系数控制位
    Uint16 rsvd1:8;             // 15:8   预留
};

union I2CPSC_REG {
    Uint16                 all;
    struct I2CPSC_BITS     bit;
};

//----------------------------------------------------
/* I2C发送FIFO控制寄存器（I2CFFTX）位定义 */
struct I2CFFTX_BITS {           // 位描述
    Uint16 TXFFIL:5;            // 4:0    发送FIFO深度控制位
    Uint16 TXFFIENA:1;          // 5      发送FIFO中断使能控制位
    Uint16 TXFFINTCLR:1;        // 6      发送FIFO中断标志清除控制位
    Uint16 TXFFINT:1;           // 7      发送FIFO中断标志位
    Uint16 TXFFST:5;            // 12:8   发送FIFO中数据个数
    Uint16 TXFFRST:1;           // 13     发送FIFO复位控制位
    Uint16 I2CFFEN:1;           // 14     发送FIFO模块使能控制位
    Uint16 rsvd1:1;             // 15     预留

};

union I2CFFTX_REG {
    Uint16                 all;
    struct I2CFFTX_BITS    bit;
};

//----------------------------------------------------
/* I2C接收FIFO控制寄存器（I2CFFRX）位定义 */
struct I2CFFRX_BITS {           // 位描述
    Uint16 RXFFIL:5;            // 4:0    接收FIFO深度控制位
    Uint16 RXFFIENA:1;          // 5      接收FIFO中断使能控制位
    Uint16 RXFFINTCLR:1;        // 6      接收FIFO中断标志清除控制位
    Uint16 RXFFINT:1;           // 7      接收FIFO中断标志位
    Uint16 RXFFST:5;            // 12:8   接收FIFO中数据个数
    Uint16 RXFFRST:1;           // 13     接收FIFO模块使能控制位
    Uint16 rsvd1:2;             // 15:14  预留
};

union I2CFFRX_REG {
    Uint16                 all;
    struct I2CFFRX_BITS    bit;
};

/**************************************/
/* I2C控制和状态寄存器文件 */
/**************************************/
struct I2C_REGS {
    Uint16              I2COAR;     // I2C自身地址寄存器
    union  I2CIER_REG   I2CIER;     // I2C中断使能控制寄存器
    union  I2CSTR_REG   I2CSTR;     // I2C中断状态控制寄存器
    Uint16              I2CCLKL;    // I2C时钟分频寄存器（低字）
    Uint16              I2CCLKH;    // I2C时钟分频寄存器（高字）
    Uint16              I2CCNT;     // I2C数据计数寄存器
    Uint16              I2CDRR;     // I2C数据接收寄存器
    Uint16              I2CSAR;     // I2C从地址寄存器
    Uint16              I2CDXR;     // I2C数据发送寄存器
    union  I2CMDR_REG   I2CMDR;     // I2C模式寄存器
    union  I2CISRC_REG  I2CISRC;    // I2C中断源寄存器
    union  I2CEMDR_REG  I2CEMDR;    // I2C扩展模式寄存器
    union  I2CPSC_REG   I2CPSC;     // I2C预分频寄存器
    Uint16              rsvd2[19];  // 预留
    union  I2CFFTX_REG  I2CFFTX;    // I2C发送FIFO寄存器
    union  I2CFFRX_REG  I2CFFRX;    // I2C接收FIFO寄存器
};

//---------------------------------------------------------------------------
// I2C外部引用和函数声明:
//
extern volatile struct I2C_REGS I2caRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_I2C_H definition

//===========================================================================
// End of file.
//===========================================================================
