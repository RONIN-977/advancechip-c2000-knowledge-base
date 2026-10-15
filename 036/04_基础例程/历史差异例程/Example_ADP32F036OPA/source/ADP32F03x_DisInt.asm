;//###########################################################################
;//
;// FILE:  ADP32F03x_DisInt.asm	
;//
;// TITLE: Disable and Restore INTM and DBGM
;// 
;// Function Prototypes:
;//
;//      Uint16 ADP32F03x_DisableInt();
;// and  void ADP32F03x_RestoreInt(Uint16 Stat0);
;//
;// Usage:
;//
;//      ADP32F03x_DisableInt() sets both the INTM and DBGM
;//      bits to disable maskable interrupts.  Before doing
;//      this, the current value of ST1 is stored on the stack
;//      so that the values can be restored later.  The value
;//      of ST1 before the masks are set is returned to the
;//      user in AL.  This is then used to restore their state
;//      via the ADP32F03x_RestoreInt(Uint16 ST1) function.
;//
;// Example
;//
;//   Uint16 StatusReg1    
;//   StatusReg1 = ADP32F03x_DisableInt();
;//
;//   ... May also want to disable INTM here
;// 
;//   ... code here
;//
;//   ADP32F03x_RestoreInt(StatusReg1);
;//
;//   ... Restore INTM enable
;//
;//###########################################################################

   .def _ADP32F03x_DisableInt
   .def _ADP32F03x_RestoreInt

_ADP32F03x_DisableInt:
    PUSH  ST1
    SETC  INTM,DBGM
    MOV   AL, *--SP
    LRETR

_ADP32F03x_RestoreInt:
    MOV   *SP++, AL
    POP   ST1
    LRETR

;//===========================================================================
;// End of file.
;//===========================================================================

   
