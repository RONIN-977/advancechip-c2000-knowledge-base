;//###########################################################################
;//
;// FILE:  ADP32F03x_usDelay.asm
;//
;// TITLE: Simple delay function
;//
;// DESCRIPTION:
;//          Microsecond delay function, must execute in zero-waitstate SARAM for accuracy
;//
;//###########################################################################

; Delay function definition - must execute in zero-waitstate RAM
       .def _ADP32F03x_usDelay
       .sect "ramfuncs"

        .global  __ADP32F03x_usDelay
_ADP32F03x_usDelay:
        SUB    ACC,#1
        BF     _ADP32F03x_usDelay,GEQ    ;; Loop if ACC >= 0
        LRETR 

; Cycle calculation:
;   Total CPU cycles = 9 + 5 ¡Á LoopCount
;   LoopCount = (Required CPU cycles - 9) / 5
;   DELAY_US(A) macro automatically performs this calculation
;
;//===========================================================================
;// End of file.
;//===========================================================================
