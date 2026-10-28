/******************************************************************************
文件名                     : AVP33x_DMA.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x DMA模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_DMA_H
#define AVP33x_DMA_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
// 通道模式寄存器位定义：
struct MODE_BITS {          // 位描述
    Uint16 PERINTSEL:5;     // 4:0    外设中断源和同步选择位(R/W):
                            //        0     无中断源
						    //        1               中断源为SEQ1INT，同步信号为ADCSYNC
                            //        2     中断源为SEQ2INT，无同步信号
							//        3               中断源为XINT1，无同步信号
							//        4               中断源为XINT2，无同步信号
							//        5               中断源为XINT3，无同步信号
							//        6               中断源为XINT4，无同步信号
							//        7               中断源为XINT5，无同步信号
							//        8               中断源为XINT6，无同步信号
							//        9               中断源为XINT7，无同步信号
							//        10            中断源为XINT13，无同步信号
							//        11            中断源为TINT0，无同步信号
						    //        12            中断源为TINT1，无同步信号
						    //        13            中断源为TINT2，无同步信号
							//        14            中断源为MXEVTA，无同步信号
							//        15            中断源为MREVTA，无同步信号
							//        16            中断源为MXEVTB，无同步信号
							//        17            中断源为MREVTB，无同步信号
	                        //        18            中断源为ePWM1SOCA，无同步信号
	                        //        19            中断源为ePWM1SOCB，无同步信号
	                        //        20            中断源为ePWM2SOCA，无同步信号
	                        //        21            中断源为ePWM2SOCB，无同步信号
	                        //        22            中断源为ePWM3SOCA，无同步信号
	                        //        23            中断源为ePWM3SOCB，无同步信号
	                        //        24            中断源为ePWM4SOCA，无同步信号
	                        //        25            中断源为ePWM4SOCB，无同步信号
	                        //        26            中断源为ePWM5SOCA，无同步信号
	                        //        27            中断源为ePWM5SOCB，无同步信号
	                        //        28            中断源为ePWM6SOCA，无同步信号
	                        //        29            中断源为ePWM6SOCB，无同步信号
	                        //        30:31    保留（无中断源）
    Uint16 rsvd1:2;         // 6:5    保留(R=0)
    Uint16 OVRINTE:1;       // 7      溢出中断使能位(R/W):
                            //        0     禁止溢出中断
                            //        1     使能溢出中断
    Uint16 PERINTE:1;       // 8      外设中断触发使能位(R/W):
                            //        0     禁止外设中断触发信号
                            //        1     使能外设中断触发信号
    Uint16 CHINTMODE:1;     // 9      通道中断模式选择位(R/W):
                            //        0     在一次传送开始产生中断事件
                            //        1     在一次传送结束产生中断事件
    Uint16 ONESHOT:1;       // 10     单次触发控制位(R/W):
                            //        0     每次中断触发信号启动一次突发传送
                            //        1     一次中断触发信号完成所有突发传送
    Uint16 CONTINUOUS:1;    // 11     连续触发控制位(R/W):
                            //        0     当TRANSFER_COUNT归零时，DMA停止，并将RUNSTS位清零
                            //        1     当TRANSFER_COUNT归零时，DMA重新初始化，并等待下次触发信号
    Uint16 SYNCE:1;         // 12     同步信号使能位(R/W):
                            //        0     ADCSYNC被忽略
                            //        1     如果通过外设中断源选择位（PERINTSEL）选择了ADC，则ADCSYNC被用来同步ADC中断信号与DMA返回计数器
    Uint16 SYNCSEL:1;       // 13     同步模式选择位(R/W):
                            //        0     SRC返回计数器被控制
                            //        1     DST返回计数器被控制
    Uint16 DATASIZE:1;      // 14     DMA通道数据宽度控制位(R/W):
                            //        0     16位数据宽度
                            //        1     32位数据宽度
    Uint16 CHINTE:1;        // 15     通道中断使能位(R/W):
                            //        0     禁止DMA中断
                            //        1     使能DMA中断
};

union MODE_REG {
    Uint16                all;
    struct MODE_BITS      bit;
};

//----------------------------------------------------
// 通道控制寄存器（CONTROL）位定义:
struct CONTROL_BITS {       // 位描述
    Uint16 RUN:1;           // 0      通道运行位(R=0/W=1)
    Uint16 HALT:1;          // 1      通道停止位(R=0/W=1)
    Uint16 SOFTRESET:1;     // 2      通道软件复位位(R=0/W=1)
    Uint16 PERINTFRC:1;     // 3      外设中断强制位(R=0/W=1)
    Uint16 PERINTCLR:1;     // 4      外设中断信号清除位(R=0/W=1)
    Uint16 SYNCFRC:1;       // 5      同步信号强制位(R=0/W=1)
    Uint16 SYNCCLR:1;       // 6      同步信号清除位(R=0/W=1)
    Uint16 ERRCLR:1;        // 7      错误清除位(R=0/W=1)
    Uint16 PERINTFLG:1;     // 8      外设中断触发事件标志位(R):
                            //        0     无外设中断事件发生
                            //        1     有外设中断事件发生
    Uint16 SYNCFLG:1;       // 9      同步信号标志位(R):
                            //        0     无同步事件
                            //        1     有同步事件发生
    Uint16 SYNCERR:1;       // 10     同步错误标志位(R):
                            //        0     无同步错误标志
                            //        1     有同步错误发生
    Uint16 TRANSFERSTS:1;   // 11     传送状态标志位(R):
                            //        0     无有效的传送过程
                            //        1     DMA正在执行一次传送过程
    Uint16 BURSTSTS:1;      // 12     突发传送状态标志位(R):
                            //        0     无有效的突发传送
                            //        1     DMA正在执行或挂起一次突发传送
    Uint16 RUNSTS:1;        // 13     运行状态标志位(R):
                            //        0     通道被禁止
                            //        1     通道被使能
    Uint16 OVRFLG:1;        // 14     溢出标志位(R)
                            //        0     无溢出事件
                            //        1     有溢出事件
    Uint16 rsvd1:1;         // 15     保留(R=0)
};

union CONTROL_REG {
    Uint16                 all;
    struct CONTROL_BITS    bit;
};

//----------------------------------------------------
// DMA控制寄存器（DMACTRL）位定义（与CONTROL是有区别的）:
struct DMACTRL_BITS {       // 位描述
    Uint16 HARDRESET:1;     // 0      硬件复位位(R=0/W=1)
    Uint16 PRIORITYRESET:1; // 1      优先级复位位(R=0/W=1)
    Uint16 rsvd1:14;        // 15:2   预留(R=0:0)
};

union DMACTRL_REG {
   Uint16                 all;
   struct DMACTRL_BITS    bit;
};

//----------------------------------------------------
// DMA仿真控制寄存器（DEBUGCTRL）位定义:
struct DEBUGCTRL_BITS {     // 位描述
    Uint16 rsvd1:15;        // 14:0   预留(R=0:0)
    Uint16 FREE:1;          // 15     仿真模式控制位(R/W):
                            //        0     DMA继续运行知道当前DMA读操作完成并且当前DMA被冻结
                            //        1     DMA不受仿真挂起影响
};

union DEBUGCTRL_REG {
   Uint16                 all;
   struct DEBUGCTRL_BITS  bit;
};


//----------------------------------------------------
// DMA优先级控制寄存器（PRIORITYCTRL1）位定义:
struct PRIORITYCTRL1_BITS { // 位描述
    Uint16 CH1PRIORITY:1;   // 0      通道1优先级控制位(R/W):
                            //        0     所有通道有相同优先级
                            //        1     通道1相对其他通道有较高优先级
    Uint16 rsvd1:15;        // 15:1   预留(R=0:0)
};

union PRIORITYCTRL1_REG {
   Uint16                     all;
   struct PRIORITYCTRL1_BITS  bit;
};


//----------------------------------------------------
// DMA优先级状态寄存器（PRIORITYSTAT位定义:
struct PRIORITYSTAT_BITS {      // 位描述
    Uint16 ACTIVESTS:3;         // 2:0    通道激活状态位(R):
                                //        0,0,0  无通道被激活
                                //        0,0,1  通道1被激活
                                //        0,1,0  通道2被激活
                                //        0,1,1  通道3被激活
                                //        1,0,0  通道4被激活
                                //        1,0,1  通道5被激活
                                //        1,1,0  通道6被激活
    Uint16 rsvd1:1;             // 3      预留(R=0)
    Uint16 ACTIVESTS_SHADOW:3;  // 6:4    通道状态映射位(R):（只有通道1被设为高优先级时才用到此位段）
                                //        0,0,0  无通道被挂起
                                //        0,0,1  不会发生（通道1不会被自己挂起）
                                //        0,1,0  通道2正在工作，但是被通道1打断并被挂起
                                //        0,1,1  通道3正在工作，但是被通道1打断并被挂起
                                //        1,0,0  通道4正在工作，但是被通道1打断并被挂起
                                //        1,0,1  通道5正在工作，但是被通道1打断并被挂起
                                //        1,1,0  通道6正在工作，但是被通道1打断并被挂起
    Uint16 rsvd2:9;             // 15:7   (R=0:0)
};

union PRIORITYSTAT_REG {
   Uint16                     all;
   struct PRIORITYSTAT_BITS   bit;
};

// 突发传送长度控制寄存器（BURST_SIZE）位定义：
struct BURST_SIZE_BITS {    // 位描述
    Uint16 BURSTSIZE:5;     // 4:0   突发传送长度控制位。0~31(k)：一次突发传送传递k+1个字。
    Uint16 rsvd1:11;        // 15:5  预留
};

union BURST_SIZE_REG {
   Uint16                  all;
   struct BURST_SIZE_BITS  bit;
};

// 突发传送计数寄存器（BURST_COUNT）位定义：
struct BURST_COUNT_BITS {   // 位描述
    Uint16 BURSTCOUNT:5;    // 4:0   一次突发传送中未被传送的数据个数。0~31(k)：一次突发传送中还剩余k个字未被传送。
    Uint16 rsvd1:11;        // 15:5  预留
};

union BURST_COUNT_REG {
   Uint16                   all;
   struct BURST_COUNT_BITS  bit;
};


//----------------------------------------------------
// DMA通道寄存器:
struct CH_REGS {
    union  MODE_REG            MODE;                    // DMA模式寄存器
    union  CONTROL_REG         CONTROL;                 // DMA控制寄存器

    union  BURST_SIZE_REG      BURST_SIZE;              // 突发长度寄存器
    union  BURST_COUNT_REG     BURST_COUNT;             // 突发传送计数寄存器
    int16                      SRC_BURST_STEP;          // 源突发传送步长寄存器
    int16                      DST_BURST_STEP;          // 目标突发传送步长寄存器

    Uint16                     TRANSFER_SIZE;           // 外环传送长度寄存器
    Uint16                     TRANSFER_COUNT;          // 外环传送计数寄存器
    int16                      SRC_TRANSFER_STEP;       // 源传送步长寄存器
    int16                      DST_TRANSFER_STEP;       // 目标传送步长寄存器

    Uint16                     SRC_WRAP_SIZE;           // 源返回长度寄存器
    Uint16                     SRC_WRAP_COUNT;          // 源返回计数寄存器
    int16                      SRC_WRAP_STEP;           // 源返回步长寄存器

    Uint16                     DST_WRAP_SIZE;           // 目标返回长度寄存器
    Uint16                     DST_WRAP_COUNT;          // 目标返回计数寄存器
    int16                      DST_WRAP_STEP;           // 目标返回步长寄存器

    Uint32                     SRC_BEG_ADDR_SHADOW;     // 源开始地址指针映射寄存器
    Uint32                     SRC_ADDR_SHADOW;         // 源地址指针映射寄存器
    Uint32                     SRC_BEG_ADDR_ACTIVE;     // 源开始地址指针当前寄存器
    Uint32                     SRC_ADDR_ACTIVE;         // 源地址指针当前寄存器

    Uint32                     DST_BEG_ADDR_SHADOW;     // 目标开始地址指针映射寄存器
    Uint32                     DST_ADDR_SHADOW;         // 目标地址指针映射寄存器
    Uint32                     DST_BEG_ADDR_ACTIVE;     // 目标开始地址指针当前寄存器
    Uint32                     DST_ADDR_ACTIVE;         // 目标地址指针当前寄存器
};

//----------------------------------------------------
// DMA寄存器:
struct DMA_REGS {
    union  DMACTRL_REG         DMACTRL;                 // DMA控制寄存器
    union  DEBUGCTRL_REG       DEBUGCTRL;               // DMA仿真控制寄存器
    Uint16                     rsvd0;                   // 预留
    Uint16                     rsvd1;                   //
    union  PRIORITYCTRL1_REG   PRIORITYCTRL1;           // DMA优先级控制寄存器
    Uint16                     rsvd2;                   //
    union  PRIORITYSTAT_REG    PRIORITYSTAT;            // DMA优先级状态寄存器
    Uint16                     rsvd3[25];               //
    struct CH_REGS             CH1;                     // DMA通道1寄存器
    struct CH_REGS             CH2;                     // DMA通道2寄存器
    struct CH_REGS             CH3;                     // DMA通道3寄存器
    struct CH_REGS             CH4;                     // DMA通道4寄存器
    struct CH_REGS             CH5;                     // DMA通道5寄存器
    struct CH_REGS             CH6;                     // DMA通道6寄存器
};


//---------------------------------------------------------------------------
// DMA的外部引用和函数声明:
//
extern volatile struct DMA_REGS DmaRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_DMA_H definition

//===========================================================================
// End of file.
//===========================================================================
