
;-------------------------------------- 定义主向量段---------------------------------------------
 .SECT  ".VECTORS"              ;定义主向量段
RESET  B  _c_int0          ;地址0000H，复位，优先级1
INT1   B  PHANTOM        ;地址0002H，INT1，优先级4
INT2   B  _C_INT2          ;地址0004H，INT2，优先级5
INT3   B  PHANTOM        ;地址0006H，INT3，优先级6
INT4   B  PHANTOM        ;地址0008H，INT4，优先级7
INT5   B  PHANTOM        ;地址000AH，INT5，优先级8
INT6   B  PHANTOM        ;地址000CH，INT6，优先级9 
RESERVED  B  PHANTOM			;地址000EH，测试，优先级10
SW_INT8   B  PHANTOM		;地址0010H，自定义软中断
SW_INT9   B  PHANTOM		;地址0012H，自定义软中断
SW_INT10  B  PHANTOM		;地址0014H，自定义软中断
SW_INT11  B  PHANTOM		;地址0016H，自定义软中断
SW_INT12  B  PHANTOM		;地址0018H，自定义软中断
SW_INT13  B  PHANTOM		;地址001AH，自定义软中断
SW_INT14  B  PHANTOM		;地址001CH，自定义软中断
SW_INT15  B  PHANTOM		;地址001EH，自定义软中断
SW_INT16  B  PHANTOM		;地址0020H，自定义软中断
TRAP	   B  PHANTOM			;地址0022H，TRAP矢量
NMI	  	   B  PHANTOM			;地址0024H，NMI，优先级3
EMU_TRAP B  PHANTOM			;地址0026H，仿真Trap，优先级2
SW_INT20  B  PHANTOM		;地址0028H，自定义软中断
SW_INT21  B  PHANTOM		;地址002AH，自定义软中断
SW_INT22  B  PHANTOM		;地址002CH，自定义软中断
SW_INT23  B  PHANTOM		;地址002EH，自定义软中断
SW_INT24  B  PHANTOM		;地址0030H，自定义软中断
SW_INT25  B  PHANTOM		;地址0032H，自定义软中断
SW_INT26  B  PHANTOM		;地址0034H，自定义软中断
SW_INT27  B  PHANTOM		;地址0036H，自定义软中断
SW_INT28  B  PHANTOM		;地址0038H，自定义软中断
SW_INT29  B  PHANTOM		;地址003AH，自定义软中断
SW_INT30  B  PHANTOM		;地址003CH，自定义软中断
SW_INT31  B  PHANTOM		;地址003EH，自定义软中断
;--------------------------------------定义子向量段----------------------------------------------------
 .SECT  ".PVECS"                ;定义子向量段
PVECTORS	B	PHANTOM			;偏移地址0000H 
			B	PHANTOM  			;偏移地址0001H
			B	PHANTOM			;偏移地址0002H
			B	PHANTOM			;偏移地址0003H
			B	PHANTOM			;偏移地址0004H
			B	PHANTOM			;偏移地址0005H
			B	PHANTOM			;偏移地址0006H
			B	PHANTOM			;偏移地址0007H
			B	PHANTOM			;偏移地址0008H
			B	PHANTOM			;偏移地址0009H
			B	PHANTOM			;偏移地址000AH
			B	PHANTOM			;偏移地址000BH
			B	PHANTOM			;偏移地址000CH
			B	PHANTOM			;偏移地址000DH
			B	PHANTOM			;偏移地址000EH
			B	PHANTOM			;偏移地址000FH 
			B	PHANTOM			;偏移地址0010H
			B	PHANTOM			;偏移地址0011H
			B	PHANTOM			;偏移地址0012H
			B	PHANTOM			;偏移地址0013H
			B	PHANTOM			;偏移地址0014H
			B	PHANTOM			;偏移地址0015H
			B	PHANTOM			;偏移地址0016H
			B	PHANTOM			;偏移地址0017H 
			B	PHANTOM			;偏移地址0018H
			B	PHANTOM			;偏移地址0019H 
			B	PHANTOM			;偏移地址001AH
			B	PHANTOM			;偏移地址001BH
			B	PHANTOM			;偏移地址001CH
			B	PHANTOM			;偏移地址001DH
			B	PHANTOM			;偏移地址001EH
			B	PHANTOM			;偏移地址001FH
			B	PHANTOM			;偏移地址0020H
			B	PHANTOM			;偏移地址0021H 
			B	PHANTOM			;偏移地址0022H
			B	PHANTOM			;偏移地址0023H
			B	PHANTOM			;偏移地址0024H
			B	PHANTOM			;偏移地址0025H
			B	PHANTOM			;偏移地址0026H 
			B	PHANTOM			;偏移地址0027H 
			B	PHANTOM			;偏移地址0028H
			B	T1UF_ISR    	;偏移地址0029H  ;定时器1timer下溢中断
			B	PHANTOM			;偏移地址002AH
			B	PHANTOM			;偏移地址002BH
			B	PHANTOM			;偏移地址002CH
			B	PHANTOM			;偏移地址002DH
			B	PHANTOM			;偏移地址002EH
			B	PHANTOM			;偏移地址002FH 
			B	PHANTOM			;偏移地址0030H
			B	PHANTOM			;偏移地址0031H
			B	PHANTOM			;偏移地址0032H
			B	PHANTOM			;偏移地址0033H
			B	PHANTOM			;偏移地址0034H
			B	PHANTOM			;偏移地址0035H
			B	PHANTOM			;偏移地址0036H
			B	PHANTOM			;偏移地址0037H
			B	PHANTOM			;偏移地址0038H
			B	PHANTOM			;偏移地址0039H
			B	PHANTOM			;偏移地址003AH
			B	PHANTOM			;偏移地址003BH
			B	PHANTOM			;偏移地址003CH
			B	PHANTOM			;偏移地址003DH
			B	PHANTOM			;偏移地址003EH
			B	PHANTOM			;偏移地址003FH
			B	PHANTOM			;偏移地址0040H
			B	PHANTOM		    ;偏移地址0041H