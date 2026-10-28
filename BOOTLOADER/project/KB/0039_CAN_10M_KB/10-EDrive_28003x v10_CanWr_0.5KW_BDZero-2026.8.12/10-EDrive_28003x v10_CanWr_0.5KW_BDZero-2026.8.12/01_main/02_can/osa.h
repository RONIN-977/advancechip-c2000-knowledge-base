/*******************************************************************************
 * osa.h
 *
 * Modification: 
 *    Date     : 
 *    Revision :
 *    Author   :
 *    Contents :
 *******************************************************************************/



#ifndef _OSA_H_
#define _OSA_H_


/* ========================================================================== */
/*                             头文件区                                       */
/* ========================================================================== */

//#include <string.h>
//#include <stdint.h>
#include <stdbool.h>
#include "DataTypeDef.h"
//#include "Platform_Types.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/* ========================================================================== */
/*                           宏和类型定义区                                   */
/* ========================================================================== */


#define OSA_SOK (0)
#define OSA_EFAIL    (-1)   /* 通用错误 */
#define OSA_ETIMEOUT (-2)   /* 超时错误 */
#define OSA_EALIGN   (-3)   /* 对齐错误 */
#define OSA_EINVAL   (-4)   /* 参数错误 */
#define OSA_EBUSY    (-5)   /* 硬件工作繁忙错误 */
#define OSA_ENODATA  (-6)   /* 无可用数据 */

#ifndef NULL
#define NULL ((void*)0)
#endif

/* 超时参数 */
#define OSA_TIMEOUT_NONE         (0)    /* 不等待，立即返回。*/ 
#define OSA_TIMEOUT_FOREVER      (~0U)  /* 一直等待直到返回 */

#ifdef true
#define OSA_TRUE true
#else
#define OSA_TRUE 1
#endif

#ifdef false
#define OSA_FALSE false
#else
#define OSA_FALSE 0
#endif

typedef u64 Uint64;
typedef u32 Uint32;
typedef u16 Uint16;
typedef u8 Uint8;
typedef s32 Int32;
typedef u16 Int16;
typedef s8 Int8;
typedef bool Bool;

#define OSA_clear(pointer) memset(pointer, 0, sizeof(*pointer))
#define OSA_clearSize(pointer, size) memset(pointer, 0, size)

#define OSA_max(X,Y) (((X)>(Y))?(X):(Y))
#define OSA_min(X,Y) (((X)<(Y))?(X):(Y))


/* ========================================================================== */
/*                          数据结构定义区                                    */
/* ========================================================================== */


/*******************************************************************************
* 函数名  : OSA_callbackFunc
* 描  述  : 定义基础回调函数，无参数，无返回值
* 输  入  : - pPrm: 参数
* 输  出  : 无
* 返回值  : 无
*******************************************************************************/
typedef void (* OSA_callbackFunc)(void);

/* ========================================================================== */
/*                          函数声明区                                        */
/* ========================================================================== */



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

