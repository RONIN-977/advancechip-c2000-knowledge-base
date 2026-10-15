/* ==============================================================================
File Name:	sysvecs.h

Originator:	Digital control systems Group - Texas Instruments

Description: 	This file contains the interrupt vector table for Mathfunctions demo

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 16-12-2000	Release	Rev 1.0    dd/mm/yyyy format
=================================================================================  */

#ifndef __SYSVECS_H__
#define __SYSVECS_H__

//asm("	.include   \"c200mnrt.i\"                         ");
//asm("	.include   \"LF2407REGS.h\"                         ");

asm("	.sect   \"vectors\"                         ");
asm("	.ref    _c_int0                             ");
asm("	.ref    _phantom                             ");
asm("	.ref    _pwm_int2                            ");

asm("          B       _c_int0        ;00h reset    ");
asm("          B       _phantom       ;02h INT1     ");
asm("          B       _pwm_int2        ;04h INT2     ");
asm("          B       _phantom    ;06h INT3     ");
asm("          B       _phantom       ;08h INT4     ");
asm("          B        _phantom       ;0Ah INT5     ");
asm("          B       _phantom       ;0Ch INT6     ");
//asm("	.include   \"rtvecs.h\"                         ");
#endif


