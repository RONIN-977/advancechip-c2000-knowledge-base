;//###########################################################################
* This file is the entry file of the DSP program. When programming in C language,
* this file can perform operations before the MAIN function is executed.
;//###########################################################################
    .ref 	_c_int00
    .global code_start
	.global _MovePrgFrFlashToRam

***********************************************************************
* Description: Branch to code starting point
***********************************************************************

    .sect "codestart"

code_start:
    LB main_init       ;Branch User Code:One instruction only

***********************************************************************
* Here, initialize all the RAM areas to 0.
***********************************************************************
	.text
main_init:
    SETC 	OBJMODE        		;Set OBJMODE for 28x object code
    EALLOW              		;Enable EALLOW protected register access
    MOVZ 	DP, #7029h>>6  		;Set data page for WDCR register
    MOV 	@7029h, #0068h  	;Set WDDIS bit in WDCR to disable WD
    EDIS                		;Disable EALLOW protected register access

	MOV		ACC,	#00H	
	MOVL 	XAR5,	#0000H		;Clear M0
	MOVL 	XAR4,	#(400H-1)
	RPT		@AR4
	|| MOV	*XAR5++,	ACC

	MOVL 	XAR5,	#0400H		;Clear M1
	MOVL 	XAR4,	#(400H-1)
	RPT		@AR4
	|| MOV	*XAR5++,	ACC

	MOVL 	XAR5,	#8000H		;Clear L0, L2, L3
	MOVL 	XAR4,	#(1000H-1)
	RPT		@AR4
	|| MOV	*XAR5++,	ACC
	
	MOVL 	XAR5,	#9000H		;Clear L1
	MOVL 	XAR4,	#(3000H-1)
	RPT		@AR4
	|| MOV	*XAR5++,	ACC
 
	LB _c_int00					;Here, we jump to the main function of the C language.

***********************************************************************
* MovePrgFrFlashToRamIt is a function for moving the program from FLASH to RAM. The calling method in C language is:
* MovePrgFrFlashToRam(src_addr, dst_addr, size)
***********************************************************************
_MovePrgFrFlashToRam:
	MOVL XAR7, ACC
	MOVL XAR6, *-SP[4]
	RPT	 @AR4
	|| PREAD *XAR6++, *XAR7
	LRETR

	.sect "UserAppFlag"

   .int	0x55AA		;
   .int	0xAA55		;
   .int	0xFFFF		;
   .int	0xFFFF		;
   .int	0xFFFF		;
   .int	0xFFFF		;
   .int	0xFFFF		;
   .int	0xFFFF		;


	.end
	
;//===========================================================================
;// End of file.
;//===========================================================================
