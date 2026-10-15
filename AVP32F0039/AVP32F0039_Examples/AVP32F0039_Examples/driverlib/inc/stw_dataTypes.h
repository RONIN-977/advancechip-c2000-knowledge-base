/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   stw_dataTypes.h
*******************************************************************************************/

#ifndef STW_DATATYPES_H_
#define STW_DATATYPES_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

typedef uint16_t      Bool; /* In sync with xdc/std.h */
typedef uint32_t      UInt32;
typedef int32_t       Int32;
typedef uint16_t      UInt16;
typedef int16_t       Int16;
typedef char          Char;
typedef void *        Ptr;
typedef uint32_t      UArg;
typedef double        Double;
typedef uint64_t      UInt64;
typedef unsigned char tBoolean;
typedef float         Float32;

/* To fix Misra-C errors */
#ifndef TRUE
#define TRUE  ((Bool) 1)
#endif
#ifndef FALSE
#define FALSE ((Bool) 0)
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef STW_DATATYPES_H_ */
