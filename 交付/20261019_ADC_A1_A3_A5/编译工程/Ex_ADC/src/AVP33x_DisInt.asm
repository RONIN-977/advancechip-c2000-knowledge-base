;;###########################################################################
;;文件名                     : AVP33x_DisInt.asm
;;作者                         : 湖南进芯电子科技有限公司
;;功能描述                 : 禁止和还原INTM和DBGM：
;;             AVP33x_DisableInt()函数通过置位INTM和DBGM来禁止可屏蔽中断。在禁止之前当前的ST1
;;             的值会被保存到堆栈里面去，以便后续能够还原ST1的值。
;;             屏蔽之前的ST1的值用户可通过AL读取，也就是由函数AVP33x_RestoreInt(Uint16 ST1)
;;             来实现的。

;;             eg:
;;                 Uint16 StatusReg1
;;                 StatusReg1 = AVP33x_DisableInt();    ----禁止INTM和DBGM
;;                 ...
;;                 AVP33x_RestoreInt(StatusReg1);       ----还原INTM和DBGM
;;
;;创建时间                 : 06-25-2019
;;版本                         : V1.0
;;修订时间                 :
;;修订内容                 :
;;###########################################################################


   .def _AVP33x_DisableInt
   .def _AVP33x_RestoreInt


_AVP33x_DisableInt:
    PUSH  ST1
    SETC  INTM,DBGM
    MOV   AL, *--SP
    LRETR

_AVP33x_RestoreInt:
    MOV   *SP++, AL
    POP   ST1
    LRETR

;//===========================================================================
;// End of file.
;//===========================================================================
