/******************************************************************************
文件名                     : AVP33x_Adc.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片ADC模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_ADC_H
#define AVP33x_ADC_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// ADC Individual Register Bit Definitions:
//----------------------------------------------------
/* ADC模块控制寄存器1（ADCTRL1）位定义 */
struct ADCTRL1_BITS {           // 位描述
	Uint16  LDO_CT:3; 			// 2:0 ADC内部1.5v模拟电源电压调节位。
								// 000:VLDO; 001:1.3*VLDO/1.5; 010:1.35*VLDO/1.5; 011:1.4*VLDO/1.5;
								// 100:1.45*VLDO/1.5; 101:1.55*VLDO/1.5; 110:1.6*VLDO/1.5; 111:1.65*VLDO/1.5。
	Uint16	LDO_EN:1; 			// 3: ADC内部1.5v模拟电源使能位。0：禁止；1：使能
    Uint16  SEQ_CASC:1;         // 4     排序器模式控制位（0：双排序模式；1：级联模式）
    Uint16  SEQ_OVRD:1;         // 5     排序器运行方式控制位
    Uint16  CONT_RUN:1;         // 6     运行方式控制位（0：启停模式；1：连续模式）
    Uint16  CPS:1;              // 7     ADC时钟预定标控制位（0：f=HSPCLK/1；1：f=HSPCLK/2）
    Uint16  ACQ_PS:4;           // 11:8  采样窗口宽度控制位
    Uint16  SUSMOD:2;           // 13:12 仿真暂停模式控制位
    Uint16  RESET:1;            // 14    ADC软件复位控制位
    Uint16  rsvd2:1;            // 15    预留
};

union ADCTRL1_REG {
   Uint16                all;
   struct ADCTRL1_BITS   bit;
};

//----------------------------------------------------
/* ADC模块控制寄存器2（ADCTRL2）位定义 */
struct ADCTRL2_BITS {           // 位描述
    Uint16  EPWM_SOCB_SEQ2:1;   // 0     SEQ2的ePWM的SOCB屏蔽控制位
    Uint16  rsvd1:1;            // 1     预留
    Uint16  INT_MOD_SEQ2:1;     // 2     SEQ2中断模式控制位
    Uint16  INT_ENA_SEQ2:1;     // 3     SEQ2中断使能
    Uint16  rsvd2:1;            // 4     预留
    Uint16  SOC_SEQ2:1;         // 5     SEQ2转换启动控制位
    Uint16  RST_SEQ2:1;         // 6     复位SEQ2
    Uint16  EXT_SOC_SEQ1:1;     // 7     SEQ1外部引脚（ADCSOC）启动ADC转换使能位
    Uint16  EPWM_SOCA_SEQ1:1;   // 8     SEQ1的ePWM的SOCA屏蔽控制位
    Uint16  rsvd3:1;            // 9     预留
    Uint16  INT_MOD_SEQ1:1;     // 10    SEQ1中断模式控制位
    Uint16  INT_ENA_SEQ1:1;     // 11    SEQ1中断使能控制位
    Uint16  rsvd4:1;            // 12    预留
    Uint16  SOC_SEQ1:1;         // 13    SEQ1转换启动控制位
    Uint16  RST_SEQ1:1;         // 14    复位SEQ1
    Uint16  EPWM_SOCB_SEQ:1;    // 15    级联模式下eEPWM的SOCB启动AD使能控制位
};

union ADCTRL2_REG {
    Uint16                all;
    struct ADCTRL2_BITS   bit;
};

//----------------------------------------------------
/* ADC自动排序状态寄存器（ADCASEQSR）位定义 */
struct ADCASEQSR_BITS {         // 位描述
    Uint16  SEQ1_STATE:4;       // 3:0    SEQ1状态位
    Uint16  SEQ2_STATE:3;       // 6:4    SEQ2状态位
    Uint16  rsvd1:1;            // 7      预留
    Uint16  SEQ_CNTR:4;         // 11:8   排序器计数器状态位
    Uint16  rsvd2:4;            // 15:12  预留
};

union ADCASEQSR_REG {
    Uint16                 all;
    struct ADCASEQSR_BITS  bit;
};

//----------------------------------------------------
/* ADC最大转换通道数寄存器（ADCMAXCONV）位定义 */
struct ADCMAXCONV_BITS {        // 位描述
    Uint16  MAX_CONV1:4;        // 3:0   最大转换通道数
    Uint16  MAX_CONV2:3;        // 6:4   最大转换通道数
    Uint16  rsvd1:9;            // 15:7  预留
};

union ADCMAXCONV_REG {
    Uint16                  all;
    struct ADCMAXCONV_BITS  bit;
};

//----------------------------------------------------
/* ADC输入通道选择排序控制寄存器1（ADCCHSELSEQ1）位定义 */
struct ADCCHSELSEQ1_BITS {      // 位描述
    Uint16  CONV00:4;           // 3:0    将转换结果保存到结果寄存器00
    Uint16  CONV01:4;           // 7:4    将转换结果保存到结果寄存器01
    Uint16  CONV02:4;           // 11:8   将转换结果保存到结果寄存器02
    Uint16  CONV03:4;           // 15:12  将转换结果保存到结果寄存器03
};

union  ADCCHSELSEQ1_REG{
    Uint16                    all;
    struct ADCCHSELSEQ1_BITS  bit;
};

//----------------------------------------------------
/* ADC输入通道选择排序控制寄存器2（ADCCHSELSEQ2）位定义 */
struct ADCCHSELSEQ2_BITS {      // 位描述
    Uint16  CONV04:4;           // 3:0    将转换结果保存到结果寄存器04
    Uint16  CONV05:4;           // 7:4    将转换结果保存到结果寄存器05
    Uint16  CONV06:4;           // 11:8   将转换结果保存到结果寄存器06
    Uint16  CONV07:4;           // 15:12  将转换结果保存到结果寄存器07
};

union  ADCCHSELSEQ2_REG{
    Uint16                    all;
    struct ADCCHSELSEQ2_BITS  bit;
};

//----------------------------------------------------
/* ADC输入通道选择排序控制寄存器3（ADCCHSELSEQ3）位定义 */
struct ADCCHSELSEQ3_BITS {      // 位描述
    Uint16  CONV08:4;           // 3:0    将转换结果保存到结果寄存器08
    Uint16  CONV09:4;           // 7:4    将转换结果保存到结果寄存器09
    Uint16  CONV10:4;           // 11:8   将转换结果保存到结果寄存器10
    Uint16  CONV11:4;           // 15:12  将转换结果保存到结果寄存器11
};

union  ADCCHSELSEQ3_REG{
    Uint16                    all;
    struct ADCCHSELSEQ3_BITS  bit;
};

//----------------------------------------------------
/* ADC输入通道选择排序控制寄存器4（ADCCHSELSEQ4）位定义 */
struct ADCCHSELSEQ4_BITS {      // 位描述
    Uint16  CONV12:4;           // 3:0    将转换结果保存到结果寄存器12
    Uint16  CONV13:4;           // 7:4    将转换结果保存到结果寄存器13
    Uint16  CONV14:4;           // 11:8   将转换结果保存到结果寄存器14
    Uint16  CONV15:4;           // 15:12  将转换结果保存到结果寄存器15
};

union  ADCCHSELSEQ4_REG {
    Uint16                    all;
    struct ADCCHSELSEQ4_BITS  bit;
};

//----------------------------------------------------
/* ADC模块控制寄存器3（ADCTRL3）位定义 */
struct ADCTRL3_BITS {           // 位描述
    Uint16   SMODE_SEL:1;       // 0      采样模式选择控制位（0：顺序采样；1：同步采样）
    Uint16   ADCCLKPS:4;        // 4:1    ADC内核时钟分频系数配置为
    Uint16   ADCPWDN:1;         // 5      给ADC除带隙与参考电路外的模拟电路供电控制位
    Uint16   ADCBGRFDN:2;       // 7:6    ADC带隙和参考电路供电控制位
	Uint16   rsvd1:1;       	// 8    预留
	Uint16   CT_SPEED:1;    	// 9     ADC转换位数精度选择。0：ADC转换精度12位模式；1：ADC转换精度16位模式。
	Uint16   ADC_TEST:1;    	// 10    ADC增益校正使能位。0：禁止；1：使能。
	Uint16   GAINTRIM_SEL:1;   	// 11    ADC输出结果整体偏移选择位。0：ADC输出结果整体相减一个固定值；1：ADC输出结果整体相加一个固定值。  
	Uint16   OFFTRIM_SEL:1; 	// 12    ADC直接偏移修调精度切换位。0：ADC直接偏移修调精度12位；1：ADC直接偏移修调精度16位。
	Uint16   SH_TRIM:1;     	// 13     ADC采保电路基准电压的差值大小控制位。0：ADC采保电路使用内部默认基准电压差值；1：ADC采保电路基准电压差值缩小一个固定比例。
	Uint16   rsvd2:2;       	// 14-15  预留
}; 

union  ADCTRL3_REG {
    Uint16                all;
    struct ADCTRL3_BITS   bit;
};

//----------------------------------------------------
/* ADC状态和标志寄存器（ADCST）位定义 */
struct ADCST_BITS {             // 位描述
    Uint16   INT_SEQ1:1;        // 0      SEQ1中断标志位（R=0）
    Uint16   INT_SEQ2:1;        // 1      SEQ2中断标志位（R=0）
    Uint16   SEQ1_BSY:1;        // 2      SEQ1忙状态标志位（R=0）
    Uint16   SEQ2_BSY:1;        // 3      SEQ2忙状态标志位（R=0）
    Uint16   INT_SEQ1_CLR:1;    // 4      SEQ1中断清除控制位（R/W=0）
    Uint16   INT_SEQ2_CLR:1;    // 5      SEQ2中断清除控制位（R/W=0）
    Uint16   EOS_BUF1:1;        // 6      SEQ1的排序缓冲结束位（R=0）
    Uint16   EOS_BUF2:1;        // 7      SEQ1的排序缓冲结束位（R=0）
    Uint16   rsvd1:8;           // 15:8   预留（R=0）
};
                             
union  ADCST_REG {            
    Uint16             all;
    struct ADCST_BITS  bit;
};                           

//----------------------------------------------------
/* ADC参考电压选择寄存器（ADCREFSEL）位定义 */
struct ADCREFSEL_BITS {         // 位描述
	Uint16   rsvd1:14;          // 13:0         预留
	Uint16   REF_SEL:2;         // 15:14      参考电压选择控制位
};
union ADCREFSEL_REG {
	Uint16		all;
	struct ADCREFSEL_BITS bit;
};

//----------------------------------------------------
/* ADC矫正寄存器（ADCOFFTRIM）位定义 */
struct ADCOFFTRIM_BITS{         // 位描述
	int16	OFFSET_TRIM:9;      // 8:0            偏置
	Uint16	rsvd1:7;            // 15:9         预留
};

union ADCOFFTRIM_REG{
	Uint16		all;
	struct ADCOFFTRIM_BITS bit;
};

/**************************************/
/* ADC控制和状态寄存器文件 */
/**************************************/
struct ADC_REGS {
    union ADCTRL1_REG      ADCTRL1;         // ADC模块控制寄存器1
    union ADCTRL2_REG      ADCTRL2;         // ADC模块控制寄存器2
    union ADCMAXCONV_REG   ADCMAXCONV;      // ADC最大转换通道数控制寄存器
    union ADCCHSELSEQ1_REG ADCCHSELSEQ1;    // ADC输入通道选择排序控制寄存器1
    union ADCCHSELSEQ2_REG ADCCHSELSEQ2;    // ADC输入通道选择排序控制寄存器2
    union ADCCHSELSEQ3_REG ADCCHSELSEQ3;    // ADC输入通道选择排序控制寄存器3
    union ADCCHSELSEQ4_REG ADCCHSELSEQ4;    // ADC输入通道选择排序控制寄存器4
    union ADCASEQSR_REG    ADCASEQSR;       // ADC自动排序状态寄存器
    Uint16                 ADCRESULT0;      // ADC结果寄存器0
    Uint16                 ADCRESULT1;      // ADC结果寄存器1
    Uint16                 ADCRESULT2;      // ADC结果寄存器2
    Uint16                 ADCRESULT3;      // ADC结果寄存器3
    Uint16                 ADCRESULT4;      // ADC结果寄存器4
    Uint16                 ADCRESULT5;      // ADC结果寄存器5
    Uint16                 ADCRESULT6;      // ADC结果寄存器6
    Uint16                 ADCRESULT7;      // ADC结果寄存器7
    Uint16                 ADCRESULT8;      // ADC结果寄存器8
    Uint16                 ADCRESULT9;      // ADC结果寄存器9
    Uint16                 ADCRESULT10;     // ADC结果寄存器10
    Uint16                 ADCRESULT11;     // ADC结果寄存器11
    Uint16                 ADCRESULT12;     // ADC结果寄存器12
    Uint16                 ADCRESULT13;     // ADC结果寄存器13
    Uint16                 ADCRESULT14;     // ADC结果寄存器14
    Uint16                 ADCRESULT15;     // ADC结果寄存器15
    union ADCTRL3_REG      ADCTRL3;         // ADC模块控制寄存器3
    union ADCST_REG        ADCST;           // ADC状态寄存器
    Uint16				   rsvd1;
    Uint16                 rsvd2;
    union ADCREFSEL_REG    ADCREFSEL;       // ADC参考选择寄存器
    union ADCOFFTRIM_REG   ADCOFFTRIM;      // ADC校正寄存器
};

//----------------------------------------------------
/* ADC结果寄存器（ADC_RESULT）位定义 */
struct ADC_RESULT_MIRROR_REGS
{
    Uint16                 ADCRESULT0;      // ADC结果寄存器0
    Uint16                 ADCRESULT1;      // ADC结果寄存器1
    Uint16                 ADCRESULT2;      // ADC结果寄存器2
    Uint16                 ADCRESULT3;      // ADC结果寄存器3
    Uint16                 ADCRESULT4;      // ADC结果寄存器4
    Uint16                 ADCRESULT5;      // ADC结果寄存器5
    Uint16                 ADCRESULT6;      // ADC结果寄存器6
    Uint16                 ADCRESULT7;      // ADC结果寄存器7
    Uint16                 ADCRESULT8;      // ADC结果寄存器8
    Uint16                 ADCRESULT9;      // ADC结果寄存器9
    Uint16                 ADCRESULT10;     // ADC结果寄存器10
    Uint16                 ADCRESULT11;     // ADC结果寄存器11
    Uint16                 ADCRESULT12;     // ADC结果寄存器12
    Uint16                 ADCRESULT13;     // ADC结果寄存器13
    Uint16                 ADCRESULT14;     // ADC结果寄存器14
    Uint16                 ADCRESULT15;     // ADC结果寄存器15
};

//---------------------------------------------------------------------------
// ADC外部引用和函数声明:
//
extern volatile struct ADC_REGS AdcRegs;
extern volatile struct ADC_RESULT_MIRROR_REGS AdcMirror;


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of AVP33x_ADC_H definition

//===========================================================================
// End of file.
//===========================================================================
