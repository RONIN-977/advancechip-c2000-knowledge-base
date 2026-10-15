//###########################################################################
//
// FILE:   hw_types.h
//
// TITLE:  Type definitions used in ALL driverlib functions.
//
//###########################################################################
// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $
// $Release Date: February  2, 2016 $
// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __HW_TYPES_H__
#define __HW_TYPES_H__

//*****************************************************************************
//
// Define a boolean type, and values for true and false.
//
//*****************************************************************************
typedef unsigned char tBoolean;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef uint16_t uint8_t;
typedef int16_t int8_t;

//*****************************************************************************
//
// Macros for hardware access, both direct and via the bit-band region.
//
//*****************************************************************************
#define HWREG(x)                                                              \
        (*((volatile unsigned long *)(x)))
#define HWREGH(x)                                                             \
        (*((volatile unsigned short *)(x)))
#define HWREGB(x)                                                            \
        __byte((int *)(x),0)
//Emulated Bitbanded write        
#define HWREGBITW(address, mask, value)                                       \
        (*(volatile unsigned long *)(address)) =                              \
       ((*(volatile unsigned long *)(address)) & ~((unsigned long)1 << mask)) \
       | ((unsigned long)value << mask)
//Emulated Bitbanded read      
#define HWREGBITR(address, mask)                                              \
        (((*(volatile unsigned long *)(address)) & ((unsigned long)1 << mask)) >> mask)        

//Emulated Bitbanded write
#define HWREGBITHW(address, mask, value)                                       \
        (*(volatile unsigned short *)(address)) =                              \
       ((*(volatile unsigned short *)(address)) & ~((unsigned short)1 << mask)) \
       | ((unsigned short)value << mask)
//Emulated Bitbanded read
#define HWREGBITHR(address, mask)                                              \
        (((*(volatile unsigned short *)(address)) & ((unsigned short)1 << mask)) >> mask)


#endif // __HW_TYPES_H__


