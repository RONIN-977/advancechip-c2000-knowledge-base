

***********************************************************************

WD_DISABLE	.set	1		;set to 1 to disable WD, else set to 0

    .ref _c_int00
    .ref _AppFlashErase
    .global code_start

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
        LB _c_int00         ;Branch to start of boot.asm in RTS library
    .endif


    .sect "flash_erase_in"
    LB _AppFlashErase

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

    MOVL    XAR4, #0x3f5f70
    MOV     @AL, #0x55aa
    CMP     AL, *+XAR4[0]
    SB      GOTO_BOOTMIAN, NEQ

    MOVL    XAR4, #0x3f5f71
    MOV     @AL, #0xaa55
    CMP     AL, *+XAR4[0]
    SB      GOTO_BOOTMIAN, NEQ

    MOVL    XAR4, #0x3f5ff6
    MOV     @AL, #0xFFFF
    CMP     AL, *+XAR4[0]
    SB      GOTO_BOOTMIAN, EQ

    MOVL    XAR7, #0x3F5FF6
    LB      *XAR7

GOTO_BOOTMIAN:

     LB _c_int00         ;Branch to start of boot.asm in RTS library

    .endif

;end wd_disable

	.end
	
;//===========================================================================
;// End of file.
;//===========================================================================
