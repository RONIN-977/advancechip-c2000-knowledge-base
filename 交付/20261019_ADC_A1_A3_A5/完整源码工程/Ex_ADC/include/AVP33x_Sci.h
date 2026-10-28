/******************************************************************************
文件名                     : AVP33x_Sci.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片SCI模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_SCI_H
#define AVP33x_SCI_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* SCI通信控制寄存器（SCICCR）位定义 */
struct  SCICCR_BITS {           // 位描述
    Uint16 SCICHAR:3;           // 2:0    SCI通信数据长度控制位
    Uint16 ADDRIDLE_MODE:1;     // 3      SCI多级模式控制位
    Uint16 LOOPBKENA:1;         // 4      SCI自测模式使能控制位
    Uint16 PARITYENA:1;         // 5      SCI奇偶校验使能控制位
    Uint16 PARITY:1;            // 6      奇/偶校验选择控制位
    Uint16 STOPBITS:1;          // 7      停止位位数控制位
    Uint16 rsvd1:8;             // 15:8   预留
}; 


union SCICCR_REG {
    Uint16              all;
    struct SCICCR_BITS  bit;
};

//----------------------------------------------------
/* SCI控制寄存器1（SCICTL1）位定义 */
struct  SCICTL1_BITS {          // 位描述
    Uint16 RXENA:1;             // 0      SCI接收使能控制位
    Uint16 TXENA:1;             // 1      SCI发送使能控制位
    Uint16 SLEEP:1;             // 2      SCI睡眠模式使能控制位
    Uint16 TXWAKE:1;            // 3      SCI发送唤醒方式选择控制位
    Uint16 rsvd:1;              // 4      预留
    Uint16 SWRESET:1;           // 5      SCI软件复位位
    Uint16 RXERRINTENA:1;       // 6      SCI接收中断时能控制位
    Uint16 rsvd1:9;             // 15:7   预留
}; 

union SCICTL1_REG {
    Uint16               all;
    struct SCICTL1_BITS  bit;
};

//----------------------------------------------------
/* SCI控制寄存器2（SCICTL2）位定义 */
struct  SCICTL2_BITS {          // 位描述
    Uint16 TXINTENA:1;          // 0      SCI发送中断时能控制位
    Uint16 RXBKINTENA:1;        // 1      SCI接收/抑制中断使能控制位
    Uint16 rsvd:4;              // 5:2    预留
    Uint16 TXEMPTY:1;           // 6      SCI发送空标志位
    Uint16 TXRDY:1;             // 7      SCI发送缓冲寄存器准备就绪标志位
    Uint16 rsvd1:8;             // 15:8   预留
}; 

union SCICTL2_REG {
    Uint16               all;
    struct SCICTL2_BITS  bit;
};

//----------------------------------------------------
/* SCI接收状态寄存器（SCIRXST）位定义 */
struct  SCIRXST_BITS {          // 位描述
    Uint16 rsvd:1;              // 0      预留
    Uint16 RXWAKE:1;            // 1      SCI接收到一个唤醒信号的标志位
    Uint16 PE:1;                // 2      SCI奇偶校验错误标志位
    Uint16 OE:1;                // 3      SCI数据溢出标志位
    Uint16 FE:1;                // 4      SCI数据帧格式错误标志位
    Uint16 BRKDT:1;             // 5      SCI间断检测标志位
    Uint16 RXRDY:1;             // 6      SCI接收就绪标志位
    Uint16 RXERROR:1;           // 7      SCI接收错误标志位
}; 

union SCIRXST_REG {
    Uint16               all;
    struct SCIRXST_BITS  bit;
};

//----------------------------------------------------
/* SCI接收数据缓冲寄存器（SCIRXBUF）位定义 */
struct  SCIRXBUF_BITS {         // 位描述
    Uint16 RXDT:8;              // 7:0    接收到的数据位
    Uint16 rsvd:6;              // 13:8   预留
    Uint16 SCIFFPE:1;           // 14     SCIFIFO奇偶校验错误标志位
    Uint16 SCIFFFE:1;           // 15     SCIFIFO帧格式错误标志位
};

union SCIRXBUF_REG {
    Uint16                all;
    struct SCIRXBUF_BITS  bit;
};

//----------------------------------------------------
/* SCI优先级控制寄存器（SCIPRI）位定义 */
struct  SCIPRI_BITS {           // 位描述
    Uint16 rsvd:3;              // 2:0    预留
    Uint16 FREE:1;              // 3      仿真暂停模式控制位
    Uint16 SOFT:1;              // 4      仿真暂停模式控制位
    Uint16 rsvd1:3;             // 7:5    预留
}; 

union SCIPRI_REG {
    Uint16              all;
    struct SCIPRI_BITS  bit;
};

//----------------------------------------------------
/* SCI发送FIFO寄存器（SCIFFTX）位定义 */
struct  SCIFFTX_BITS {          // 位描述
    Uint16 TXFFIL:5;            // 4:0    SCI发送FIFO深度控制位
    Uint16 TXFFIENA:1;          // 5      SCI发送FIFO中断使能控制位
    Uint16 TXFFINTCLR:1;        // 6      SCI发送FIFO中断标志清除控制位
    Uint16 TXFFINT:1;           // 7      SCI发送FIFO中断标志位
    Uint16 TXFFST:5;            // 12:8   SCI发送FIFO中数据个数
    Uint16 TXFIFOXRESET:1;      // 13     SCI发送FIFO复位控制位
    Uint16 SCIFFENA:1;          // 14     SCI发送FIFO模块功能使能控制位
    Uint16 SCIRST:1;            // 15     SCI模块复位控制位
}; 

union SCIFFTX_REG {
    Uint16               all;
    struct SCIFFTX_BITS  bit;
};

//----------------------------------------------------
/* SCI接收FIFO寄存器（SCIFFRX）位定义 */
struct  SCIFFRX_BITS {          // 位描述
    Uint16 RXFFIL:5;            // 4:0    SCI接收FIFO深度控制位
    Uint16 RXFFIENA:1;          // 5      SCI接收FIFO中断使能控制位
    Uint16 RXFFINTCLR:1;        // 6      SCI接收FIFO中断标志清除控制位
    Uint16 RXFFINT:1;           // 7      SCI接收FIFO中断标志位
    Uint16 RXFFST:5;            // 12:8   SCI接收FIFO中数据个数
    Uint16 RXFIFORESET:1;       // 13     SCI接收FIFO复位控制位
    Uint16 RXFFOVRCLR:1;        // 14     SCI接收FIFO溢出标志清除控制位
    Uint16 RXFFOVF:1;           // 15     SCI接收FIFO溢出标志位
}; 

union SCIFFRX_REG {
    Uint16               all;
    struct SCIFFRX_BITS  bit;
};

// SCI FIFO control register bit definitions:
//----------------------------------------------------
/* SCI FIFO控制寄存器（SCIFFCT）位定义 */
struct  SCIFFCT_BITS {          // 位描述
    Uint16 FFTXDLY:8;           // 7:0    SCI发送FIFO延时控制位
    Uint16 rsvd:5;              // 12:8   预留
    Uint16 CDC:1;               // 13     SCI波特率自动校准使能控制位
    Uint16 ABDCLR:1;            // 14     ABD位清除使能位
    Uint16 ABD:1;               // 15     SCI波特率自动检测控制位
};

union SCIFFCT_REG {
    Uint16               all;
    struct SCIFFCT_BITS  bit;
};

/**************************************/
/* SCI控制和状态寄存器文件 */
/**************************************/
struct  SCI_REGS {
    union SCICCR_REG     SCICCR;    // SCI通信控制寄存器
    union SCICTL1_REG    SCICTL1;   // SCI控制寄存器1
    Uint16               SCIHBAUD;  // SCI波特率寄存器（高）
    Uint16               SCILBAUD;  // SCI波特率寄存器（低）
    union SCICTL2_REG    SCICTL2;   // SCI控制寄存器2
    union SCIRXST_REG    SCIRXST;   // SCI状态寄存器
    Uint16               SCIRXEMU;  // SCI接收方真缓冲寄存器
    union SCIRXBUF_REG   SCIRXBUF;  // SCI接收数据缓冲寄存器
    Uint16               rsvd1;     // 预留
    Uint16               SCITXBUF;  // SCI发送数据缓冲寄存器
    union SCIFFTX_REG    SCIFFTX;   // SCI发送FIFO寄存器
    union SCIFFRX_REG    SCIFFRX;   // SCI接收FIFO寄存器
    union SCIFFCT_REG    SCIFFCT;   // SCI FIFO控制寄存器
    Uint16               rsvd2;     // 预留
    Uint16               rsvd3;     // 预留
    union SCIPRI_REG     SCIPRI;    // SCI优先级控制寄存器
};

//---------------------------------------------------------------------------
// SCI外部引用和函数声明:
//
extern volatile struct SCI_REGS SciaRegs;
extern volatile struct SCI_REGS ScibRegs;
extern volatile struct SCI_REGS ScicRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_SCI_H definition

//===========================================================================
// End of file.
//===========================================================================
