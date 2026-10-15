; Originator:		Texas Instruments
; Description:		F240 Header file containing all peripheral register 
;			declarations as well as other useful definitions.
; Last Updated:   	27 May 1997
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; On Chip Periperal Register Definitions (All registers mapped into data 
; space unless otherwise noted)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;全局变量寄存器和CPU中断寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IMR		.set	0004h		;中断屏蔽寄存器
GREG	.set	0005h		;全局变量定位寄存器
IFR		.set	0006h		;中断标志寄存器
;System Registers
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PIRQR0		.set	07010h
PIRQR1		.set	07011h
PIRQR2		.set	07012h
PIACKR0		.set	07014h
PIACKR1		.set	07015h
PIACKR2		.set	07016h
SCSR1		.set	07018h		;系统模块控制寄存器1
SCSR2		.set	07019h		;系统模块控制寄存器2
DINR		.set	0701Ch		;系统模块状态寄存器
PIVR		.set	0701Eh		;系统中断矢量寄存器
;WD 程序监视控制寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
WDCNTR 	.set	07023h		;WD 计数器寄存器
WDKEY		.set	07025h		;WDKey 寄存器
WDCR		.set	07029h		;WD 控制寄存器
;串行外围接口 (SPI) 寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPICCR		.set	07040h		;SPI 配置控制寄存器
SPICTL		.set	07041h		;SPI 操作控制寄存器
SPISTS		.set	07042h		;SPI 状态寄存器
SPIBRR		.set	07044h		;SPI 波特率寄存器
SPIEMU 		.set	07046h		;SPI 仿真缓冲寄存器
SPIRXBUF 	.set	07047h		;SPI 串行输入缓冲寄存器
SPITXBUF 	.set	07048h		;SPI 串行输入缓冲寄存器
SPIDAT 		.set	07049h		;SPI 串行数据寄存器
SPIPRI 		.set	0704Fh		;SPI 中断优先级控制寄存器
;串行通讯接口 (SCI) 寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SCICCR		.set	07050h		;SCI 通讯控制寄存器
SCICTL1		.set	07051h		;SCI 控制寄存器 1
SCIHBAUD	.set	07052h		;SCI 波特率寄存器高位
SCILBAUD	.set	07053h		;SCI 波特率寄存器低位
SCICTL2		.set	07054h		;SCI 控制寄存器 2
SCIRXST		.set	07055h		;SCI 接受状态寄存器
SCIRXEMU	.set	07056h		;SCI 仿真数据缓冲寄存器
SCIRXBUF	.set	07057h		;SCI 接受数据缓冲寄存器
SCITXBUF	.set	07059h		;SCI 发送数据缓冲寄存器
SCIPRI 		.set	0705Fh		;SCI 中断优先级控制寄存器
;外部中断寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
XINT1CR		.set	07070h		;中断 1 控制寄存器
XINT2CR 	.set	07071h		;中断 2 控制寄存器
;数据 I/O 控制寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCRA		.set	07090h		;Output Control Reg A
MCRB		.set	07092h		;Output Control Reg B
MCRC		.set	07094h		;Output Control Reg C
PEDATDIR	.set	07095h		;I/O port E Data & Direction reg.
PFDATDIR	.set	07096h		;I/O port F Data & Direction reg.
PADATDIR	.set	07098h		;I/O port A Data & Direction reg.
PBDATDIR	.set	0709Ah		;I/O port B Data & Direction reg.
PCDATDIR	.set	0709Ch		;I/O port C Data & Direction reg.
PDDATDIR	.set	0709Eh		;I/O port D Data & Direction reg.
;模数转换(ADC) 寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADCCTRL1	.set	070A0h		;ADC 控制寄存器 1
ADCCTRL2	.set	070A1h		;ADC 控制寄存器 2
MAXCONV		.set	070A2h		
CHSELSEQ1	.set	070A3h		
CHSELSEQ2	.set	070A4h		
CHSELSEQ3	.set	070A5h		
CHSELSEQ4	.set	070A6h		
AUTO_SEQ_SR	.set	070A7h		
RESULT0		.set	070A8h		;A/D转换结果寄存器0
RESULT1		.set	070A9h		;A/D转换结果寄存器1
RESULT2		.set	070AAh		;A/D转换结果寄存器2
RESULT3		.set	070ABh		;A/D转换结果寄存器3
RESULT4		.set	070ACh		;A/D转换结果寄存器4
RESULT5		.set	070ADh		;A/D转换结果寄存器5
RESULT6		.set	070AEh		;A/D转换结果寄存器6
RESULT7		.set	070AFh		;A/D转换结果寄存器7
RESULT8		.set	070B0h		;A/D转换结果寄存器8
RESULT9		.set	070B1h		;A/D转换结果寄存器9
RESULT10	.set	070B2h		;A/D转换结果寄存器10
RESULT11	.set	070B3h		;A/D转换结果寄存器11
RESULT12	.set	070B4h		;A/D转换结果寄存器12
RESULT13	.set	070B5h		;A/D转换结果寄存器13
RESULT14	.set	070B6h		;A/D转换结果寄存器14
RESULT15	.set	070B7h		;A/D转换结果寄存器15
CALIBRATION	.set 070B8h		
;CAN 配置控制寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MDER	.set	07100h		
TCR		.set	07101h		
RCR		.set	07102h		
MCR		.set	07103h		
BCR2	.set	07104h		
BCR1	.set	07105h		
ESR		.set	07106h		
GSR		.set	07107h		
CEC		.set	07108h		
CAN_IFR		.set    07109h		
CAN_IMR		.set    0710Ah		
LAM0_H		.set	0701Bh		
LAM0_L		.set	0701Ch		
LAM1_H		.set	0701Dh		
LAM1_L		.set	0701Eh		
;邮包#0
MSGID0L		.set	07200h		
MSGID0H		.set	07201h		
MSGCTRL0	.set	07202h		
MBX0A		.set	07204h		
MBX0B		.set	07205h		
MBX0C		.set	07206h		
MBX0D		.set	07207h		
;邮包#1
MSGID1L		.set	07208h		
MSGID1H		.set	07209h		
MSGCTRL1	.set	0720Ah		
MBX1A		.set	0720Ch		
MBX1B		.set	0720Dh		
MBX1C		.set	0720Eh		
MBX1D		.set	0720Fh		
;邮包#2
MSGID2L		.set	07210h		
MSGID2H		.set	07211h		
MSGCTRL2	.set	07212h		
MBX2A		.set	07214h		
MBX2B		.set	07215h		
MBX2C		.set	07216h		
MBX2D		.set	07217h		
;邮包#3
MSGID3L		.set	07218h		
MSGID3H		.set	07219h		
MSGCTRL3	.set	0721Ah		
MBX3A		.set	0721Ch		
MBX3B		.set	0721Dh		
MBX3C		.set	0721Eh		
MBX3D		.set	0721Fh		
;邮包#4
MSGID4L		.set	07220h		
MSGID4H		.set	07221h		
MSGCTRL4	.set	07222h		
MBX4A		.set	07224h		
MBX4B		.set	07225h		
MBX4C		.set	07226h		
MBX4D		.set	07227h		
;邮包#5
MSGID5L		.set	07228h		
MSGID5H		.set	07229h		
MSGCTRL5	.set	0722Ah		
MBX5A		.set	0722Ch		
MBX5B		.set	0722Dh		
MBX5C		.set	0722Eh		
MBX5D		.set	0722Fh		
;通用定时器 -- 事件管理器A (EVA)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GPTCONA	 .set	7400h		;通用定时控制寄存器
T1CNT		 .set	7401h		;通用定时器 1 计数寄存器
T1CMPR	 .set	7402h		;通用定时器 1 比较寄存器
T1PR		 .set	7403h		;通用定时器 1 周期寄存器
T1CON		 .set	7404h		;通用定时器 1 控制寄存器
T2CNT		 .set	7405h		;通用定时器 2 计数寄存器
T2CMPR	 .set	7406h		;通用定时器 2 比较寄存器
T2PR		 .set	7407h		;通用定时器 2 周期寄存器
T2CON		 .set	7408h		;通用定时器 2 控制寄存器
;Full & Simple Compare Unit Registers --事件管理器A (EVA)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
COMCONA		.set	7411h		;比较控制寄存器 A
ACTRA		  .set	7413h		;全比较动作控制寄存器 A
DBTCONA		.set	7415h		;死区时间控制寄存器 A
CMPR1		  .set	7417h		;全比较单元 1 比较寄存器
CMPR2		  .set	7418h		;全比较单元 2 比较寄存器
CMPR3		  .set	7419h		;全比较单元 3 比较寄存器
;捕捉和正交编码寄存器事件管理器(EVA)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CAPCONA		.set	7420h		;捕捉控制寄存器 A
CAPFIFOA	.set	7422h		;捕捉FIFO 状态寄存器 A
CAP1FIFO	.set	7423h		;捕捉 1 二级 FIFO 寄存器
CAP2FIFO	.set	7424h		;捕捉 2 二级 FIFO 寄存器
CAP3FIFO	.set	7425h		;捕捉 3 二级 FIFO 寄存器
CAP1FBOT	.set	7427h		
CAP2FBOT	.set	7428h		
CAP3FBOT	.set	7429h		
;事件管理器 (EVA) 中断控制寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EVAIMRA		.set	742Ch		;事件管理器中断屏蔽寄存器 A
EVAIMRB		.set	742Dh		;事件管理器中断屏蔽寄存器 B
EVAIMRC		.set	742Eh		;事件管理器中断屏蔽寄存器 C
EVAIFRA		.set	742Fh		;事件管理器中断标志寄存器 A
EVAIFRB		.set	7430h		;事件管理器中断标志寄存器 B
EVAIFRC		.set	7431h		;事件管理器中断标志寄存器 C
;通用(GP)定时器配置控制寄存器--EVB
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GPTCONB		.set	7500h			
T3CNT		  .set	7501h			
T3CMPR		.set	7502h			
T3PR		  .set	7503H			
T3CON		  .set	7504h			
T4CNT		  .set	7505h			
T4CMPR		.set	7506h			
T4PR		  .set	7507H			
T4CON		  .set	7508h			
;比较单元寄存器--EVB
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
COMCONB		.set	07511h			
ACTRB		  .set	07513h			
DBTCONB		.set	07515h			
CMPR4		  .set	07517h			
CMPR5		  .set	07518h			
CMPR6		  .set	07519h			
;捕捉单元寄存器--EVB
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CAPCONB		.set	7520h			
CAPFIFOB	.set	7522h			
CAP4FIFO	.set	7523h			
CAP5FIFO	.set	7524h			
CAP6FIFO	.set	7525h			
CAP4FBOT	.set	7527h			
CAP5FBOT	.set	7528h			
CAP6FBOT	.set	7529h			
;事件管理器 (EVB) 中断控制寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EVBIMRA		.set	742Ch		;事件管理器中断屏蔽寄存器 A
EVBIMRB		.set	742Dh		;事件管理器中断屏蔽寄存器 B
EVBIMRC		.set	742Eh		;事件管理器中断屏蔽寄存器 C
EVBIFRA		.set	742Fh		;事件管理器中断标志寄存器 A
EVBIFRB		.set	7430h		;事件管理器中断标志寄存器 B
EVBIFRC		.set	7431h		;事件管理器中断标志寄存器 C
;程序存储器空间 --Flash 寄存器
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;PMPC		.set	0h		;Flash 段控制寄存器
;CTRL		.set	01h		
;WADDR	.set	2h		;Flash 写地址寄存器
;WDATA	.set	3h		;Flash 写数据寄存器
;TCR		.set	4h		
;ENAB		.set	5h		
;SETC		.set	6h		
;I/O 存储空间
FCMR		.set	0FF0Fh		
;等待状态产生寄存器 (映射到 I/O 空间)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
WSGR		.set	0FFFFh		;等待状态产生寄存器
;数据存储器块地址
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
B0_SADDR	.set	00200h		;块 B0 开始地址
B0_EADDR	.set	002FFh		;块 B0 结束地址
B1_SADDR	.set	00400h		;块 B1 开始地址
B1_EADDR	.set	004FFh		;块 B1 结束地址
B2_SADDR	.set	00060h		;块 B2 开始地址
B2_EADDR	.set	0007Fh		;块 B2 结束地址
XDATA_SADDR	.set	08000h		;外部数据空间开始地址
XDATA_EADDR	.set  	0FFFFh		;外部数据空间结束地址
;经常使用的数据页
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DP_B2     .set 	0		;页 0 数据空间
DP_B01		.set	4		;页 4  B0(200H/80H)	
DP_B02		.set	5		;页 5  B0(280H/80H)	
DP_B11		.set	6		;页 6  B1(300H/80H)	
DP_B12		.set	7		;页 7  AD(380H/80H)	
DP_SARAM1	.set	16		;页 1  SARAM(800h/80h)
DP_SARAM2	.set	26		;页 2  SARAM(0D00h/80h)
DP_SARAM3	.set	18		;页 3  SARAM(900h/80h)
DP_SARAM4	.set	19		;页 4  SARAM(980h/80h)
DP_PF1    .set 	224		;页 1  外设帧文件 (7000h/80h)(0XE0)
DP_PF2    .set 	225		;页 2  外设帧文件  (7080h/80h)(0XE1)
DP_PF3    .set 	226		;页 3  外设帧文件  (7100h/80h)(0XE2)
DP_PF4    .set 	227		;页 4  外设帧文件(7080h/80h)(0XE3)
DP_PF5    .set 	228		;页 5  外设帧文件(7200h/80h)(0XE4)
DP_EVA		.set	232		;页 0 事件管理器-EVA 文件 (7400h/80h)(0xE8)
DP_EVB		.set	234		;页 0 事件管理器-EVB 文件 (7500h/80h)(0xE9)
;位测试指令的位代码(BIT)
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BIT15		.set	0000h		;位代码15
BIT14		.set	0001h		;位代码14
BIT13		.set	0002h		;位代码13
BIT12		.set	0003h		;位代码12
BIT11		.set	0004h		;位代码11
BIT10		.set	0005h		;位代码10
BIT9		.set	0006h		;位代码9
BIT8		.set	0007h		;位代码8
BIT7		.set	0008h		;位代码7
BIT6		.set	0009h		;位代码6
BIT5		.set	000Ah		;位代码5
BIT4		.set	000Bh		;位代码4
BIT3		.set	000Ch		;位代码3
BIT2		.set	000Dh		;位代码2
BIT1		.set	000Eh		;位代码1
BIT0		.set	000Fh		;位代码 0
;用 SBIT0 和 SBIT1 宏屏蔽位
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
B15_MSK 	.set	8000h		;位屏蔽15
B14_MSK 	.set	4000h		;位屏蔽 14
B13_MSK 	.set	2000h		;位屏蔽 13
B12_MSK 	.set	1000h		;位屏蔽12
B11_MSK 	.set	0800h		;位屏蔽11
B10_MSK 	.set	0400h		;位屏蔽 10
B9_MSK		.set	0200h		;位屏蔽 9
B8_MSK		.set	0100h		;位屏蔽 8
B7_MSK		.set	0080h		;位屏蔽 7
B6_MSK		.set	0040h		;位屏蔽 6
B5_MSK		.set	0020h		;位屏蔽 5
B4_MSK		.set	0010h		;位屏蔽 4
B3_MSK		.set	0008h		;位屏蔽3
B2_MSK		.set	0004h		;位屏蔽 2
B1_MSK		.set	0002h		;位屏蔽 1
B0_MSK		.set	0001h		;位屏蔽 0
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;宏定义
; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SBIT0		.macro	DMA, MASK	;清位宏定义  DMA_direct memory address  macro 是宏的意思
		LACC	DMA				
		AND	#(0FFFFh-MASK)		
		SACL	DMA				
		.endm
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SBIT1		.macro	DMA, MASK	;置位宏定义
		LACC	DMA
		OR	#(MASK)
		SACL	DMA
		.endm
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
KICK_DOG	.macro			;程序监视器复位宏定义
		LDP	#00E0h		;DP→7000h～707Fh
		SPLK	#05555h, WDKEY	;WDCNTR由下一步复位被使能
	SPLK	#0AAAAh, WDKEY	;WDCNTR 被复位
		.endm
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DELAY_S 	.macro	delay_value	;延时 = 0.05 μs × 延时计数
		RPT #delay_value
		NOP
		.endm
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SAVE           .macro
	SETC	INTM
	MAR	*,	AR1
	SST	#1,	*+		;1
	SST	#0,	*+		;2
	SACL	*+			;3
	SACH	*+			;4	SAVE ACC
	PAC
	SPM	#0
	MPY	#1			;
	SPL	*+			;5	SAVE TREG
	SACH	*+			;6	SAVE PREGH
	SACL	*+			;7	SAVE PREGL
	LDP	#IMR/128		;
	LACL	IMR			;
	SACL	*+			;8  
	.endm
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
RESUME         .macro
	SETC	INTM
	MAR	*,	AR1
	LACL	*-
	LACL	*-
	LDP		#IMR/128
	SACL	IMR	
	SPM	#0		;8
	LT		*-
	MPY	#1			;7	RE WR PREGL
	LPH		*-			;6	RE WR PREGH
	LT		*-			;5	RE WR TREG	
	LACC	*-,	16		;4
	ADDS	*-			;3
	LST		#0,	*-		;2
	LST		#1,	*		;1
               .endm	
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~