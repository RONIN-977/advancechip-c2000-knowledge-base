/******************************************************************************
文件名                     : DSP2833x_Headers_nonBIOS.cmd
作者                         : 湖南进芯电子科技有限公司
功能描述                 : 外设寄存器链接器命令文件
			  针对Non-BIOS的应用。
			  用于将AVP33X头文件中使用的外设结构体放置到正确的内存映射位置。
			  此版本的文件包含PievectorTable结构体；对于BIOS应用程序，请使用不包含pievectortable
			  结构的AVP33x_Headers_nonBIOS.cmd文件。
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/


MEMORY
{
 PAGE 0:    /* 程序空间  */

 PAGE 1:    /* 数据空间  */
 
   DEV_EMU     : origin = 0x000880, length = 0x000180     /* 设备仿真寄存器 */
   FLASH_REGS  : origin = 0x000A80, length = 0x000060     /* FLASH寄存器 */
   CSM         : origin = 0x000AE0, length = 0x000010     /* 加密模块寄存器 */
  
   ADC_MIRROR  : origin = 0x000B00, length = 0x000010     /* AD结果寄存器镜像 */

   XINTF       : origin = 0x000B20, length = 0x000020     /* 外部总线接口寄存器 */
   
   CPU_TIMER0  : origin = 0x000C00, length = 0x000008     /* CPU Timer0寄存器  */
   CPU_TIMER1  : origin = 0x000C08, length = 0x000008     /* CPU Timer1寄存器 */
   CPU_TIMER2  : origin = 0x000C10, length = 0x000008     /* CPU Timer2寄存器 */

   PIE_CTRL    : origin = 0x000CE0, length = 0x000020     /* PIE控制寄存器 */
   PIE_VECT    : origin = 0x000D00, length = 0x000100     /* PIE向量表 */

   DMA         : origin = 0x001000, length = 0x000200     /* DMA寄存器 */

   MCBSPA      : origin = 0x005000, length = 0x000040     /* McBSP-A寄存器 */
   MCBSPB      : origin = 0x005040, length = 0x000040     /* McBSP-B寄存器 */

   ECANA       : origin = 0x006000, length = 0x000040     /* eCAN-A控制和状态寄存器 */
   ECANA_LAM   : origin = 0x006040, length = 0x000040     /* eCAN-A本地接收屏蔽寄存器 */
   ECANA_MOTS  : origin = 0x006080, length = 0x000040     /* eCAN-A消息对象时间戳寄存器 */
   ECANA_MOTO  : origin = 0x0060C0, length = 0x000040     /* eCAN-A消息对象超时寄存器 */
   ECANA_MBOX  : origin = 0x006100, length = 0x000100     /* eCAN-A邮箱寄存器 */

   ECANB       : origin = 0x006200, length = 0x000040     /* eCAN-B控制和状态寄存器 */
   ECANB_LAM   : origin = 0x006240, length = 0x000040     /* eCAN-B本地接收屏蔽寄存器 */
   ECANB_MOTS  : origin = 0x006280, length = 0x000040     /* eCAN-B消息对象时间戳寄存器 */
   ECANB_MOTO  : origin = 0x0062C0, length = 0x000040     /* eCAN-B消息对象超时寄存器 */
   ECANB_MBOX  : origin = 0x006300, length = 0x000100     /* eCAN-B邮箱寄存器 */

   EPWM1       : origin = 0x006800, length = 0x000022     /* 增强型PWM1寄存器 */
   EPWM2       : origin = 0x006840, length = 0x000022     /* 增强型PWM2寄存器 */
   EPWM3       : origin = 0x006880, length = 0x000022     /* 增强型PWM3寄存器 */
   EPWM4       : origin = 0x0068C0, length = 0x000022     /* 增强型PWM4寄存器 */
   EPWM5       : origin = 0x006900, length = 0x000022     /* 增强型PWM5寄存器 */
   EPWM6       : origin = 0x006940, length = 0x000022     /* 增强型PWM6寄存器 */

   ECAP1       : origin = 0x006A00, length = 0x000020     /* 增强型捕获1寄存器 */
   ECAP2       : origin = 0x006A20, length = 0x000020     /* 增强型捕获2寄存器 */
   ECAP3       : origin = 0x006A40, length = 0x000020     /* 增强型捕获3寄存器 */
   ECAP4       : origin = 0x006A60, length = 0x000020     /* 增强型捕获4寄存器 */
   ECAP5       : origin = 0x006A80, length = 0x000020     /* 增强型捕获5寄存器 */
   ECAP6       : origin = 0x006AA0, length = 0x000020     /* 增强型捕获6寄存器 */
 
   EQEP1       : origin = 0x006B00, length = 0x000040     /* 增强型QEP1寄存器 */
   EQEP2       : origin = 0x006B40, length = 0x000040     /* 增强型QEP2寄存器 */

   GPIOCTRL    : origin = 0x006F80, length = 0x000040     /* GPIO控制寄存器 */
   GPIODAT     : origin = 0x006FC0, length = 0x000020     /* GPIO数据寄存器 */
   GPIOINT     : origin = 0x006FE0, length = 0x000020     /* GPIO中断及低功耗模式唤醒选择寄存器 */
                 
   SYSTEM      : origin = 0x007010, length = 0x000020     /* 系统控制寄存器 */
   SPIA        : origin = 0x007040, length = 0x000010     /* SPI-A寄存器 */
   SCIA        : origin = 0x007050, length = 0x000010     /* SCI-A寄存器 */
   XINTRUPT    : origin = 0x007070, length = 0x000010     /* 外部中断寄存器 */

   ADC         : origin = 0x007100, length = 0x000020     /* ADC寄存器 */

   SCIB        : origin = 0x007750, length = 0x000010     /* SCI-B寄存器 */

   SCIC        : origin = 0x007770, length = 0x000010     /* SCI-C寄存器 */
   
   I2CA        : origin = 0x007900, length = 0x000040     /* I2C-A寄存器 */
   
   CSM_PWL     : origin = 0x33FFF8, length = 0x000008     /* 部分FLASHA. CSM密码存放位置. */

   PARTID      : origin = 0x380090, length = 0x000001     /* 芯片ID寄存器位置 */
}

 
SECTIONS
{
   PieVectTableFile : > PIE_VECT,   PAGE = 1

/*** 外设帧0寄存器结构体 ***/
   DevEmuRegsFile    : > DEV_EMU,     PAGE = 1
   FlashRegsFile     : > FLASH_REGS,  PAGE = 1
   CsmRegsFile       : > CSM,         PAGE = 1
   AdcMirrorFile     : > ADC_MIRROR,  PAGE = 1 
   XintfRegsFile     : > XINTF,       PAGE = 1
   CpuTimer0RegsFile : > CPU_TIMER0,  PAGE = 1
   CpuTimer1RegsFile : > CPU_TIMER1,  PAGE = 1
   CpuTimer2RegsFile : > CPU_TIMER2,  PAGE = 1  
   PieCtrlRegsFile   : > PIE_CTRL,    PAGE = 1     
   DmaRegsFile       : > DMA,         PAGE = 1 

/*** 外设帧3寄存器结构体 ***/
   McbspaRegsFile    : > MCBSPA,      PAGE = 1
   McbspbRegsFile    : > MCBSPB,      PAGE = 1

/*** 外设帧1寄存器结构体 ***/
   ECanaRegsFile     : > ECANA,       PAGE = 1
   ECanaLAMRegsFile  : > ECANA_LAM    PAGE = 1   
   ECanaMboxesFile   : > ECANA_MBOX   PAGE = 1
   ECanaMOTSRegsFile : > ECANA_MOTS   PAGE = 1
   ECanaMOTORegsFile : > ECANA_MOTO   PAGE = 1
   
   ECanbRegsFile     : > ECANB,       PAGE = 1
   ECanbLAMRegsFile  : > ECANB_LAM    PAGE = 1   
   ECanbMboxesFile   : > ECANB_MBOX   PAGE = 1
   ECanbMOTSRegsFile : > ECANB_MOTS   PAGE = 1
   ECanbMOTORegsFile : > ECANB_MOTO   PAGE = 1
   
   EPwm1RegsFile     : > EPWM1        PAGE = 1   
   EPwm2RegsFile     : > EPWM2        PAGE = 1   
   EPwm3RegsFile     : > EPWM3        PAGE = 1   
   EPwm4RegsFile     : > EPWM4        PAGE = 1   
   EPwm5RegsFile     : > EPWM5        PAGE = 1   
   EPwm6RegsFile     : > EPWM6        PAGE = 1
   
   ECap1RegsFile     : > ECAP1        PAGE = 1   
   ECap2RegsFile     : > ECAP2        PAGE = 1   
   ECap3RegsFile     : > ECAP3        PAGE = 1   
   ECap4RegsFile     : > ECAP4        PAGE = 1
   ECap5RegsFile     : > ECAP5        PAGE = 1   
   ECap6RegsFile     : > ECAP6        PAGE = 1

   EQep1RegsFile     : > EQEP1        PAGE = 1   
   EQep2RegsFile     : > EQEP2        PAGE = 1               

   GpioCtrlRegsFile  : > GPIOCTRL     PAGE = 1
   GpioDataRegsFile  : > GPIODAT      PAGE = 1
   GpioIntRegsFile   : > GPIOINT      PAGE = 1
   
/*** 外设帧2寄存器结构体 ***/
   SysCtrlRegsFile   : > SYSTEM,      PAGE = 1
   SpiaRegsFile      : > SPIA,        PAGE = 1
   SciaRegsFile      : > SCIA,        PAGE = 1
   XIntruptRegsFile  : > XINTRUPT,    PAGE = 1
   AdcRegsFile       : > ADC,         PAGE = 1
   ScibRegsFile      : > SCIB,        PAGE = 1
   ScicRegsFile      : > SCIC,        PAGE = 1
   I2caRegsFile      : > I2CA,        PAGE = 1
              
/*** 代码安全模块寄存器结构体 ***/
   CsmPwlFile        : > CSM_PWL,     PAGE = 1

/*** 芯片ID寄存器结构体 ***/
   PartIdRegsFile    : > PARTID,      PAGE = 1

}
