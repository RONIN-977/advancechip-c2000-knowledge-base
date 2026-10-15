/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   f28003x_device.h
*                         TITLE:  f28003x Device Definitions.
*******************************************************************************************/

#ifndef f28003x_DEVICE_H
#define f28003x_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif


#ifndef __TMS320C28XX__
#define __cregister
#endif  //__TMS320C28xx__

extern __cregister volatile unsigned int IFR;
extern __cregister volatile unsigned int IER;

#define  EINT   __asm(" clrc INTM")
#define  DINT   __asm(" setc INTM")
#define  ERTM   __asm(" clrc DBGM")
#define  DRTM   __asm(" setc DBGM")
#ifndef  EALLOW
#ifndef _TMS320C28XX_CLA_
#define  EALLOW __eallow()
#else
#define  EALLOW __meallow()
#endif // _TMS320C28XX_CLA_
#endif // EALLOW
 
#ifndef  EDIS
#ifndef _TMS320C28XX_CLA_
#define  EDIS   __edis()
#else
#define  EDIS   __medis()
#endif // _TMS320C28XX_CLA_
#endif // EDIS
#define  ESTOP0 __asm(" ESTOP0")

#define M_INT1  0x0001
#define M_INT2  0x0002
#define M_INT3  0x0004
#define M_INT4  0x0008
#define M_INT5  0x0010
#define M_INT6  0x0020
#define M_INT7  0x0040
#define M_INT8  0x0080
#define M_INT9  0x0100
#define M_INT10 0x0200
#define M_INT11 0x0400
#define M_INT12 0x0800
#define M_INT13 0x1000
#define M_INT14 0x2000
#define M_DLOG  0x4000
#define M_RTOS  0x8000

#define BIT0    0x00000001
#define BIT1    0x00000002
#define BIT2    0x00000004
#define BIT3    0x00000008
#define BIT4    0x00000010
#define BIT5    0x00000020
#define BIT6    0x00000040
#define BIT7    0x00000080
#define BIT8    0x00000100
#define BIT9    0x00000200
#define BIT10   0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13   0x00002000
#define BIT14   0x00004000
#define BIT15   0x00008000
#define BIT16   0x00010000
#define BIT17   0x00020000
#define BIT18   0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21   0x00200000
#define BIT22   0x00400000
#define BIT23   0x00800000
#define BIT24   0x01000000
#define BIT25   0x02000000
#define BIT26   0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000

//
// For Portability, User Is Recommended To Use the C99 Standard integer types
//
#if !defined(__TMS320C28XX_CLA__)
#include <assert.h>
#include <stdarg.h>
#endif //__TMS320C28XX_CLA__
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//
// C++ Bool Compatibility
//
#if defined(__cplusplus)
typedef bool _Bool;
#endif

//
// C99 defines boolean type to be _Bool, but this doesn't match the format of
// the other standard integer types.  bool_t has been defined to fill this gap.
//
typedef _Bool bool_t;

//
// Used for a bool function return status
//
typedef _Bool status_t;

#ifndef SUCCESS
#define SUCCESS     true
#endif

#ifndef FAIL
#define FAIL        false
#endif

//
// The following data types are included for compatibility with legacy code,
// they are not recommended for use in new software.  Please use the C99
// types included above
//
#if (!defined(DSP28_DATA_TYPES) && !defined(F28_DATA_TYPES))
#define DSP28_DATA_TYPES
#define F28_DATA_TYPES

#ifdef __TMS320C28XX_CLA__
typedef short                                   int16;
typedef long                                    int32;
typedef unsigned char                           Uint8;
typedef unsigned short                          Uint16;
typedef unsigned long                           Uint32;
typedef float                                   float32;
typedef long double                             float64;
typedef struct { Uint32 low32; Uint32 high32; } Uint64;
typedef struct { int32  low32; int32  high32; } int64;
#else // __TMS320C28XX__
typedef int                                     int16;
typedef long                                    int32;
typedef long long                               int64;
typedef unsigned int                            Uint16;
typedef unsigned long                           Uint32;
typedef unsigned long long                      Uint64;
#ifndef C2000_IEEE754_TYPES
#define C2000_IEEE754_TYPES
#ifdef __TI_EABI__
typedef float         float32_t;
typedef double        float64_t;
#else // TI COFF
typedef float         float32_t;
typedef long double   float64_t;
#endif // __TI_EABI__
#endif // C2000_IEEE754_TYPES
#endif //__TMS320C28XX_CLA__

#endif //(!defined(DSP28_DATA_TYPES) && !defined(F28_DATA_TYPES))

//
// The following data types are for use with byte addressable peripherals.
// See compiler documentation on the byte_peripheral type attribute.
//
#ifndef __TMS320C28XX_CLA__
typedef unsigned int bp_16 __attribute__((byte_peripheral));
typedef unsigned long bp_32 __attribute__((byte_peripheral));
#endif

//
// Include All Peripheral Header Files:
//
#include "f28003x_adc.h"
#include "f28003x_aes.h"
#include "f28003x_aes_ss.h"
#include "f28003x_analogsubsys.h"
#include "f28003x_bgcrc.h"
#include "f28003x_cla.h"
#include "f28003x_clb.h"
#include "f28003x_clbxbar.h"
#include "f28003x_cmpss.h"
#include "f28003x_cputimer.h"
#include "f28003x_dac.h"
#include "f28003x_dcsm.h"
#include "f28003x_dma.h"
#include "f28003x_ecap.h"
#include "f28003x_epg.h"
#include "f28003x_epwm.h"
#include "f28003x_epwm_xbar.h"
#include "f28003x_eqep.h"
#include "f28003x_erad.h"
#include "f28003x_flash.h"
#include "f28003x_fsi.h"
#include "f28003x_gpio.h"
#include "f28003x_hic.h"
#include "f28003x_i2c.h"
#include "f28003x_input_xbar.h"
#include "f28003x_memconfig.h"
#include "f28003x_nmiintrupt.h"
#include "f28003x_otp.h"
#include "f28003x_output_xbar.h"
#include "f28003x_piectrl.h"
#include "f28003x_pievect.h"
#include "f28003x_pmbus.h"
#include "f28003x_sci.h"
#include "f28003x_sdfm.h"
#include "f28003x_spi.h"
#include "f28003x_sysctrl.h"
#include "f28003x_wwd.h"
#include "f28003x_xbar.h"
#include "f28003x_xint.h"
#include "f28003x_tempsensorconv.h"

//
// byte_peripheral attribute is only supported on the C28
//
#ifndef __TMS320C28XX_CLA__
#include "f28003x_can.h"
#include "f28003x_dcc.h"
#include "f28003x_lin.h"
//
// Include MCAN bitfield header only if driverlib header is not already included
//
#ifndef HW_MCANSS_H_
#include "f28003x_mcan.h"       
#endif
#endif

#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif                                  // end of f28003x_DEVICE_H definition

//===========================================================================
// End of file.
//===========================================================================
