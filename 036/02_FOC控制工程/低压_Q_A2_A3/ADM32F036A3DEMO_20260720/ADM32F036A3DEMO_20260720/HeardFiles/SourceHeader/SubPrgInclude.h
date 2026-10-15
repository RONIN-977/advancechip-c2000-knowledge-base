/************************************************************
------------该文件是通用子程序模块程序的头文件---------------
************************************************************/
#ifndef SUBPRG_INCLUDE_H
#define SUBPRG_INCLUDE_H

#include "Define.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
	结构定义
************************************************************/
typedef struct PID_STRUCT_DEF {
	int32 	Total;			//积分累加值
	int32 	Out;			//输出值
	int16  	Max;			//最大值限制
	int16  	Min;			//最小值限制
	int16  	Deta;			//偏差值
	int16  	KP;				//KP增益
	int16  	KI;				//KI增益
	int16  	KD;				//KD增益
}PID_STRUCT;//PID计算用的数据结构(无量纲数据结构)
typedef struct PID_STRUCT_DEF_MD38 {
	int32 	Total;			//积分累加值
	int32 	Out;			//输出值
	int16  	Max;			//最大值限制
	int16  	Min;			//最小值限制
	int16  	Deta;			//偏差值
	int16  	KP;				//KP增益
	int16  	KI;				//KI增益
	int16  	KD;				//KD增益

    Uint16    QP;             // KP的放大倍数 KP = KP << QP
    Uint16    QI;             // KI的放大倍数
    Uint16    QD;             // KD的放大倍数
}PID_STRUCT_2;//PID计算用的数据结构(无量纲数据结构)

typedef struct TIME_STRUCT_DEF {
    Uint32 count_10ms;   // 10ms单位计数
    Uint32 count_1s;     // 1秒单位计数
    Uint32 count_1min;   // 1分钟单位计数
    Uint32 count_1hour;  // 1小时单位计数

    // 内部计数变量（不对外暴露）
    Uint32 _ms_counter;  // 毫秒计数器
    Uint32 _s_counter;   // 秒计数器
    Uint32 _min_counter; // 分钟计数器
} TIME_STRUCT;

extern TIME_STRUCT TimeAccumulator;
/************************************************************
	基本函数定义和引用
************************************************************/
// .global  _swap
// .global  _abs
// .global  _absl
// .global  _qsin
// .global  _qatan
// .global  _qsqrt
extern Uint16 swap(Uint16);				// ASM 文件里
extern Uint16 qsqrt(Uint32);			// 开方函数
extern int16  abs(int16);				//
extern int32  absl(int32);		        //
extern int16  qsin(int16);				// 正弦函数
extern int16  qatan(int32);		        // 反正切函数

int16 atan(int16 x, int16 y);		//四象限反正切函数

extern void PID(PID_STRUCT * );
extern void UpdateTimeAccumulator(TIME_STRUCT* time_acc);
extern void ResetTimeAccumulator(TIME_STRUCT* time_acc);
extern void init_random_freq(void);
extern int16 random_frequency(int16 center, int16 amplitude);
extern void sort_min_mid(Uint16 a, Uint16 b, Uint16 c, Uint16 *min, Uint16 *mid);

#define	Filter2(x,total)  (( (((Uint32)total)<<16) + (((Uint32)x)<<15) - (((Uint32)total)<<15) )>>16)
#define	Filter4(x,total)  (( (((Uint32)total)<<16) + (((Uint32)x)<<14) - (((Uint32)total)<<14) )>>16)
#define	Filter8(x,total)  (( (((Uint32)total)<<16) + (((Uint32)x)<<13) - (((Uint32)total)<<13) )>>16)
#define	Filter16(x,total) (( (((Uint32)total)<<16) + (((Uint32)x)<<12) - (((Uint32)total)<<12) )>>16)
#define	Filter32(x,total) (( (((Uint32)total)<<16) + (((Uint32)x)<<11) - (((Uint32)total)<<11) )>>16)
#define Filter128(x,total) (( (((Uint32)total)<<16) + (((Uint32)x)<<9) - (((Uint32)total)<<9) )>>16)
#define Filter256(x,total) (( (((Uint32)total)<<16) + (((Uint32)x)<<8) - (((Uint32)total)<<8) )>>16)


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================
