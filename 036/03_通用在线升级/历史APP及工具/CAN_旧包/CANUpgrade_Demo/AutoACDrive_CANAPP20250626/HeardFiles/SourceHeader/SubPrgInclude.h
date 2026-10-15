/************************************************************
------------该文件是通用子程序模块程序的头文件---------------
************************************************************/
#ifndef SUBPRG_INCLUDE_H
#define SUBPRG_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Define.h"

/************************************************************
	结构定义
************************************************************/
typedef struct PID_STRUCT_DEF {
	long 	Total;			//积分累加值
	long 	Out;			//输出值
	int  	Max;			//最大值限制
	int  	Min;			//最小值限制
	int  	Deta;			//偏差值
	int  	KP;				//KP增益
	int  	KI;				//KI增益
	int  	KD;				//KD增益
}PID_STRUCT;//PID计算用的数据结构(无量纲数据结构)
typedef struct PID_STRUCT_DEF_MD {
	long 	Total;			//积分累加值
	long 	Out;			//输出值
	int  	Max;			//最大值限制
	int  	Min;			//最小值限制
	int  	Deta;			//偏差值
	int  	KP;				//KP增益
	int  	KI;				//KI增益
	int  	KD;				//KD增益

    Uint    QP;             // KP的放大倍数 KP = KP << QP
    Uint    QI;             // KI的放大倍数
    Uint    QD;             // KD的放大倍数
}PID_STRUCT_MD;//PID计算用的数据结构(无量纲数据结构)
typedef struct BURR_FILTER_STRUCT_DEF{					
	int 	Input;			//本次采样数据
	int		Output;			//当前使用数据
	int 	Err;			//偏差限值
	int  	Max;			//最大偏差值
	int  	Min;			//最小偏差值
}BURR_FILTER_STRUCT;//去除毛刺滤波处理的数据结构

/************************************************************
	基本函数定义和引用
************************************************************/
extern Uint swap(Uint);				//
extern int 	abs(int);				//
extern int 	absl(long int);			//
extern int 	qsin(int);				//正弦函数
extern int 	qatan(long int);		//反正切函数
extern int  atan(int x, int y);		//四象限反正切函数
extern Uint qsqrt(Ulong);			//开方函数

extern Uint GetInvCurrent(Uint);	//根据机型查询变频器额定电流函数

extern void PID(PID_STRUCT * );
extern void PID_MD(PID_STRUCT_MD * pid);


#define	Filter2(x,total)  (( (((Ulong)total)<<16) + (((Ulong)x)<<15) - (((Ulong)total)<<15) )>>16)
#define	Filter4(x,total)  (( (((Ulong)total)<<16) + (((Ulong)x)<<14) - (((Ulong)total)<<14) )>>16)
#define	Filter8(x,total)  (( (((Ulong)total)<<16) + (((Ulong)x)<<13) - (((Ulong)total)<<13) )>>16)
#define	Filter16(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<12) - (((Ulong)total)<<12) )>>16)
#define	Filter32(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<11) - (((Ulong)total)<<11) )>>16)
#define Filter128(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<9) - (((Ulong)total)<<9) )>>16)
#define Filter256(x,total) (( (((Ulong)total)<<16) + (((Ulong)x)<<8) - (((Ulong)total)<<8) )>>16)


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of definition

//===========================================================================
// End of file.
//===========================================================================
