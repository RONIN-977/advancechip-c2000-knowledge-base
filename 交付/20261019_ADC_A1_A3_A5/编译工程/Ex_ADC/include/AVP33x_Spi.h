/******************************************************************************
文件名                     : AVP33x_Spi.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片SPI模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_SPI_H
#define AVP33x_SPI_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* SPI发送FIFO寄存器（SPIFFTX）位定义 */
struct  SPIFFTX_BITS {          // 位描述
    Uint16 TXFFIL:5;            // 4:0    SPI发送FIFO中断级别控制位
    Uint16 TXFFIENA:1;          // 5      SPI发送FIFO中断使能控制位
    Uint16 TXFFINTCLR:1;        // 6      SPI发送FIFO中断标志清除控制位
    Uint16 TXFFINT:1;           // 7      SPI发送FIFO中断标志位
    Uint16 TXFFST:5;            // 12:8   SPI发送FIFO中数据个数
    Uint16 TXFIFO:1;            // 13     SPI发送FIFO复位控制位
    Uint16 SPIFFENA:1;          // 14     SPI发送FIFO增强使能控制位
    Uint16 SPIRST:1;            // 15     SPI复位控制位
};

union SPIFFTX_REG {
    Uint16               all;
    struct SPIFFTX_BITS  bit;
};

//----------------------------------------------------
/* SPI接收FIFO寄存器（SPIFFRX）位定义 */
struct  SPIFFRX_BITS {          // 位描述
    Uint16 RXFFIL:5;            // 4:0    SPI接收FIFO中断级别控制位
    Uint16 RXFFIENA:1;          // 5      SPI接收FIFO中断使能控制位
    Uint16 RXFFINTCLR:1;        // 6      SPI接收FIFO中断标志清除控制位
    Uint16 RXFFINT:1;           // 7      SPI接收FIFO中断标志位
    Uint16 RXFFST:5;            // 12:8   SPI接收FIFO中数据个数
    Uint16 RXFIFORESET:1;       // 13     SPI接收FIFO复位控制位
    Uint16 RXFFOVFCLR:1;        // 14     SPI接收FIFO溢出标志清除控制位
    Uint16 RXFFOVF:1;           // 15     SPI接收FIFO溢出标志位

};

union SPIFFRX_REG {
    Uint16               all;
    struct SPIFFRX_BITS  bit;
};

//----------------------------------------------------
/* SPI FIFO控制寄存器（SPIFFCT）位定义 */
struct  SPIFFCT_BITS {          // 位描述
    Uint16 TXDLY:8;             // 7:0    SPI FIFO发送延迟控制位
    Uint16 rsvd:8;              // 15:8   预留
};

union SPIFFCT_REG {
    Uint16               all;
    struct SPIFFCT_BITS  bit;
};

//----------------------------------------------------
/* SPI配置寄存器（SPICCR）位定义 */
struct  SPICCR_BITS {           // 位描述
    Uint16 SPICHAR:4;           // 3:0    SPI字符长度控制位
    Uint16 SPILBK:1;            // 4      SPI自测模式使能控制位
    Uint16 rsvd1:1;             // 5      预留
    Uint16 CLKPOLARITY:1;       // 6      SPI时钟极性控制位
    Uint16 SPISWRESET:1;        // 7      SPI软件复位控制位
    Uint16 rsvd2:8;             // 15:8   预留
};

union SPICCR_REG {
    Uint16              all;
    struct SPICCR_BITS  bit;
};

//----------------------------------------------------
/* SPI操作控制寄存器（SPICTL）位定义 */
struct  SPICTL_BITS {           // 位描述
    Uint16 SPIINTENA:1;         // 0      SPI中断使能控制位
    Uint16 TALK:1;              // 1      SPI主/从模式切换控制位
    Uint16 MASTER_SLAVE:1;      // 2      SPI网络模式控制位
    Uint16 CLK_PHASE:1;         // 3      SPI时钟相位选择控制位
    Uint16 OVERRUNINTENA:1;     // 4      SPI超时中断使能
    Uint16 rsvd:11;             // 15:5   预留
};

union SPICTL_REG {
    Uint16              all;
    struct SPICTL_BITS  bit;
};

//----------------------------------------------------
/* SPI状态寄存器（SPISTS）位定义 */
struct  SPISTS_BITS {           // 位描述
    Uint16 rsvd1:5;             // 4:0    预留
    Uint16 BUFFULL_FLAG:1;      // 5      SPI发送缓冲寄存器满标志位
    Uint16 INT_FLAG:1;          // 6      SPI中断标志位
    Uint16 OVERRUN_FLAG:1;      // 7      SPI接收溢出标志位
    Uint16 rsvd2:8;             // 15:8   预留
};

union SPISTS_REG {
    Uint16              all;
    struct SPISTS_BITS  bit;
};

//----------------------------------------------------
/* SPI优先级控制寄存器（SPIPRI）位定义 */
struct  SPIPRI_BITS {           // 位描述
    Uint16 rsvd1:4;             // 3:0    预留
    Uint16 FREE:1;              // 4      SPI仿真暂停模式控制位
    Uint16 SOFT:1;              // 5      SPI仿真暂停模式控制位
    Uint16 rsvd2:1;             // 6      预留
    Uint16 rsvd3:9;             // 15:7   预留
};

union SPIPRI_REG {
    Uint16              all;
    struct SPIPRI_BITS  bit;
};

/**************************************/
/* SPI控制和状态寄存器文件 */
/**************************************/
struct  SPI_REGS {
    union SPICCR_REG     SPICCR;        // SPI配置寄存器
    union SPICTL_REG     SPICTL;        // SPI操作控制寄存器
    union SPISTS_REG     SPISTS;        // SPI状态寄存器
    Uint16               rsvd1;         // 预留
    Uint16               SPIBRR;        // SPI波特率寄存器
    Uint16               rsvd2;         // 预留
    Uint16               SPIRXEMU;      // SPI仿真缓冲寄存器
    Uint16               SPIRXBUF;      // SPI接收缓冲寄存器
    Uint16               SPITXBUF;      // SPI发送缓冲寄存器
    Uint16               SPIDAT;        // SPI发送/接收移位寄存器
    union SPIFFTX_REG    SPIFFTX;       // SPI发送FIFO寄存器
    union SPIFFRX_REG    SPIFFRX;       // SPI接收FIFO寄存器
    union SPIFFCT_REG    SPIFFCT;       // SPI FIFO控制寄存器
    Uint16               rsvd3[2];      // 预留
    union SPIPRI_REG     SPIPRI;        // SPI FIFO优先级控制寄存器
};

//---------------------------------------------------------------------------
// SPI外部引用和函数声明:
//
extern volatile struct SPI_REGS SpiaRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_SPI_H definition

//===========================================================================
// End of file.
//===========================================================================
