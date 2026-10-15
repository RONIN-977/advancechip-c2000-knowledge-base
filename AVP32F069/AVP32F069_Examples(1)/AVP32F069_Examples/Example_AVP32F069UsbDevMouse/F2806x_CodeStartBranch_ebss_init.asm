;//###########################################################################
;//
;// FILE:  F2806x_CodeStartBranch.asm	
;//
;// TITLE: Branch for redirecting code execution after boot. 
;//
;// For these examples, code_start is the first code that is executed after
;// exiting the boot ROM code. 
;//
;// The codestart section in the linker cmd file is used to physically place
;// this code at the correct memory location.  This section should be placed 
;// at the location the BOOT ROM will re-direct the code to.  For example, 
;// for boot to FLASH this code will be located at 0x3f7ff6. 
;//
;// In addition, the example F2806x projects are setup such that the codegen
;// entry point is also set to the code_start label.  This is done by linker 
;// option -e in the project build options.  When the debugger loads the code,
;// it will automatically set the PC to the "entry point" address indicated by
;// the -e linker option.  In this case the debugger is simply assigning the PC, 
;// it is not the same as a full reset of the device. 
;// 
;// The compiler may warn that the entry point for the project is other then
;//  _c_init00.  _c_init00 is the C environment setup and is run before 
;// main() is entered. The code_start code will re-direct the execution 
;// to _c_init00 and thus there is no worry and this warning can be ignored. 
;// 
;//###########################################################################
;// $TI Release: F2806x C/C++ Header Files and Peripheral Examples V151 $ 
;// $Release Date: February  2, 2016 $ 
;// $Copyright: Copyright (C) 2011-2016 Texas Instruments Incorporated -
;//             http://www.ti.com/ ALL RIGHTS RESERVED $
;//###########################################################################

***********************************************************************

WD_DISABLE	.set	1		;set to 1 to disable WD, else set to 0
init_ebss	.set 	1		; initialise .ebss section

    .ref _c_int00
    .global code_start

; region related symbols from linker command file
	.global	_ebss_start
	.global _ebss_size

***********************************************************************
* Function: codestart section
*
* Description: Branch to code starting point
***********************************************************************

    .sect "codestart"

code_start:
    .if WD_DISABLE == 1
        LB wd_disable       ;Branch to watchdog disable code
    .else
        LB _preboot         ;Branch to start of boot.asm in RTS library
    .endif

;end codestart section

***********************************************************************
* Function: wd_disable
*
* Description: Disables the watchdog timer
***********************************************************************
    .if WD_DISABLE == 1

    .text
wd_disable:
    SETC OBJMODE        ;Set OBJMODE for 28x object code
    EALLOW              ;Enable EALLOW protected register access
    MOVZ DP, #7029h>>6  ;Set data page for WDCR register
    MOV @7029h, #0068h  ;Set WDDIS bit in WDCR to disable WD
    EDIS                ;Disable EALLOW protected register access
    LB _preboot         ;Branch to start of boot.asm in RTS library

    .endif

;end wd_disable

***********************************************************************
* Function: _preboot
*
* Description: initializes .ebss section
***********************************************************************

		.text
_preboot:

		; initialise contents of .ebss section with 0
		.if (init_ebss)
		MOVL 	XAR2, #_ebss_start		; load .ebss section start address
		MOV 	AR1, #(_ebss_size - 1)	; load section size
		RPT		AR1
||		MOV 	*XAR2++, #0x0000		; zero memory
		.endif

		; restore registers & call "boot.obj"
		SETC OBJMODE                    ; Set OBJMODE for 28x object code
		ZAPA							; ACC:P = 0
		MOVL	XAR1, ACC				; XAR1 = 0
		MOVL	XAR2, ACC				; XAR2 = 0
    	LB 		_c_int00				; jump to boot.obj
; end _preboot

	.end
	
;//===========================================================================
;// End of file.
;//===========================================================================
