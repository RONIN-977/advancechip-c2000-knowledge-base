/******************************************************************************
文件名                     : AVP33x_Dma_defines.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : DMA模块用到的宏定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_DMA_DEFINES_H
#define AVP33x_DMA_DEFINES_H


#ifdef __cplusplus
extern "C" {
#endif

// 模式寄存器
//==========================
// 外设中断源选择位：为给定的DMA通道选择合适的外部中断信号触发源。
#define DMA_SEQ1INT 1
#define DMA_SEQ2INT 2
#define DMA_XINT1   3
#define DMA_XINT2   4
#define DMA_XINT3   5
#define DMA_XINT4   6
#define DMA_XINT5   7
#define DMA_XINT6   8
#define DMA_XINT7   9
#define DMA_XINT13  10
#define DMA_TINT0   11
#define DMA_TINT1   12
#define DMA_TINT2   13
#define DMA_MXEVTA  14
#define DMA_MREVTA  15
#define DMA_MXREVTB 16
#define DMA_MREVTB  17
// 溢出中断使能位
#define	OVRFLOW_DISABLE	0x0         // 禁止溢出中断
#define	OVEFLOW_ENABLE	0x1         // 使能溢出中断
// 外设中断触发使能位
#define	PERINT_DISABLE	0x0         // 禁止外设中断触发信号
#define	PERINT_ENABLE   0x1         // 使能外设中断触发信号
// 通道中断模式选择位
#define	CHINT_BEGIN		0x0         // 在一次传送开始产生中断事件
#define	CHINT_END     	0x1         // 在一次传送结束产生中断事件
// 单次触发控制位
#define	ONESHOT_DISABLE	0x0         // 每次中断触发信号启动一次突发传送
#define	ONESHOT_ENABLE	0x1         // 一次中断触发信号完成所有突发传送
// 连续触发控制位
#define	CONT_DISABLE	0x0         // 当TRANSFER_COUNT归零时，DMA停止，并将RUNSTS位清零
#define	CONT_ENABLE 	0x1         // 当TRANSFER_COUNT归零时，DMA重新初始化，并等待下次触发信号
// 同步信号使能位
#define	SYNC_DISABLE	0x0         // ADCSYNC被忽略
#define	SYNC_ENABLE     0x1         // 如果通过外设中断源选择位（PERINTSEL）选择了ADC，则ADCSYNC被用来同步ADC中断信号与DMA返回计数器
// 同步模式选择位
#define	SYNC_SRC		0x0         // SRC返回计数器被控制
#define	SYNC_DST        0x1         // DST返回计数器被控制
// DMA通道数据宽度控制位
#define	SIXTEEN_BIT    	0x0         // 16位数据宽度
#define	THIRTYTWO_BIT   0x1         // 32位数据宽度
// 通道中断使能位
#define	CHINT_DISABLE	0x0         // 禁止DMA中断
#define	CHINT_ENABLE   	0x1         // 使能DMA中断


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of AVP33x_EPWM_DEFINES_H

//===========================================================================
// End of file.
//===========================================================================
