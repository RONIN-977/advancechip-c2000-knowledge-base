;/******************************* Copyright(C) *********************************************
;*
;*                           湖南进芯电子科技有限公司
;*                         http://www.advancechip.com/
;*
;*                         FILE:   f28003x_dbgier.asm
;*                         TITLE:  Set the DBGIER register
;*******************************************************************************************/

        .if __TI_EABI__
        .asg SetDBGIER, _SetDBGIER
        .endif

        .global _SetDBGIER
        .text

_SetDBGIER:
        MOV     *SP++,AL
        POP     DBGIER
        LRETR

;//
;// End of file
;//
