/******************************************************************************
文件名                     : AVP33x_EPwm.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x增强型PWM模块寄存器位定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_EPWM_H
#define AVP33x_EPWM_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* 时基控制寄存器（TBCTL）位定义 */
struct TBCTL_BITS {             // 位描述
    Uint16 CTRMODE:2;           // 1:0    技术模式控制位
    Uint16 PHSEN:1;             // 2      计数寄存器装载相位寄存器使能位
    Uint16 PRDLD:1;             // 3      周期寄存器装载周期寄存器的影子寄存器使能位
    Uint16 SYNCOSEL:2;          // 5:4    同步信号输出时刻选择位
    Uint16 SWFSYNC:1;           // 6      软件强制同步脉冲产生控制位
    Uint16 HSPCLKDIV:3;         // 9:7    高速时钟分频系数设置位
    Uint16 CLKDIV:3;            // 12:10  时基时钟分频系数设置位
    Uint16 PHSDIR:1;            // 13     相位方向控制位
    Uint16 FREE_SOFT:2;         // 15:14  仿真模式控制位
};

union TBCTL_REG {
    Uint16              all;
    struct TBCTL_BITS   bit;
};

//----------------------------------------------------
/* 时基状态寄存器位（TBSTS）定义   */
struct TBSTS_BITS {             // 位描述
    Uint16 CTRDIR:1;            // 0      时基计数方向状态位
    Uint16 SYNCI:1;             // 1      同步输入信号状态位
    Uint16 CTRMAX:1;            // 2      时基计数器是否计数到最大值（0xFFFF）标识位
    Uint16 rsvd1:13;            // 15:3   预留
};

union TBSTS_REG {
   Uint16              all;
   struct TBSTS_BITS   bit;
};

//----------------------------------------------------
/* 比较控制寄存器（CMPCTL）位定义   */
struct CMPCTL_BITS {            // 位描述
    Uint16 LOADAMODE:2;         // 0:1    CMPA影子装载模式下，装载条件选择控制位
    Uint16 LOADBMODE:2;         // 3:2    CMPB影子装载模式下，装载条件选择控制位
    Uint16 SHDWAMODE:1;         // 4      计数比较A寄存器操作模式控制位
    Uint16 rsvd1:1;             // 5      预留
    Uint16 SHDWBMODE:1;         // 6      计数比较B寄存器操作模式控制位
    Uint16 rsvd2:1;             // 7      预留
    Uint16 SHDWAFULL:1;         // 8      CMPA影子寄存器满标志位
    Uint16 SHDWBFULL:1;         // 9      CMPB影子寄存器满标志位
    Uint16 rsvd3:6;             // 15:10  预留
};

union CMPCTL_REG {
   Uint16                all;
   struct CMPCTL_BITS    bit;
};

//----------------------------------------------------
/* 动作控制寄存器（AQCTLx）位定义  */
struct AQCTL_BITS {             // 位描述
    Uint16 ZRO:2;               // 1:0    当时基计数器的值=0时动作
    Uint16 PRD:2;               // 3:2    当时基计数器的值=周期寄存器的值时动作
    Uint16 CAU:2;               // 5:4    当向上计数时，当时基计数器的值=CMPA寄存器的值时动作
    Uint16 CAD:2;               // 7:6    当向下计数时，当时基计数器的值=CMPA寄存器的值时动作
    Uint16 CBU:2;               // 9:8    当向上计数时，当时基计数器的值=CMPB寄存器的值时动作
    Uint16 CBD:2;               // 11:10  当向下计数时，当时基计数器的值=CMPB寄存器的值时动作
    Uint16 rsvd:4;              // 15:12  预留
};

union AQCTL_REG {
   Uint16                all;
   struct AQCTL_BITS     bit;
};

//----------------------------------------------------
/* 软件控制动作寄存器（AQSFRC）位定义  */
struct AQSFRC_BITS {            // 位描述
    Uint16 ACTSFA:2;            // 1:0    当一次性软件强制A输出被调用时的动作
    Uint16 OTSFA:1;             // 2      一次性软件强制ePWMxA输出
    Uint16 ACTSFB:2;            // 4:3    当一次性软件强制B输出被调用时的动作
    Uint16 OTSFB:1;             // 5      一次性软件强制ePWMxB输出
    Uint16 RLDCSF:2;            // 7:6    AQCSF有效寄存器装载影子寄存器的条件
    Uint16 rsvd1:8;             // 15:8   预留
};

union AQSFRC_REG {
    Uint16                 all;
    struct AQSFRC_BITS     bit;
};

//----------------------------------------------------
/* 软件连续控制动作寄存器（AQCSFRC）位定义 */
struct AQCSFRC_BITS {           // 位描述
    Uint16 CSFA:2;              // 1:0    软件连续控制A输出
    Uint16 CSFB:2;              // 3:2    软件连续控制B输出
    Uint16 rsvd1:12;            // 15:4   预留
};

union AQCSFRC_REG {
    Uint16                  all;
    struct AQCSFRC_BITS     bit;
};

//----------------------------------------------------
/* 死区控制寄存器（DBCTL）位定义 */
struct DBCTL_BITS {             // 位描述
    Uint16 OUT_MODE:2;          // 1:0    死区模块输出控制位
    Uint16 POLSEL:2;            // 3:2    极性选择控制位
    Uint16 IN_MODE:2;           // 5:4    死区模块输入控制位
    Uint16 rsvd1:10;            // 15:4   预留
};

union DBCTL_REG {
   Uint16                  all;
   struct DBCTL_BITS       bit;
};

//----------------------------------------------------
/* 错误联防选择寄存器（TZSEL）位定义  */
struct TZSEL_BITS {             // 位描述
    Uint16  CBC1:1;             // 0      CBC错误联防1选择（TZ1）周期触发联防控制位
    Uint16  CBC2:1;             // 1      CBC错误联防2选择（TZ2）周期触发联防控制位
    Uint16  CBC3:1;             // 2      CBC错误联防3选择（TZ3）周期触发联防控制位
    Uint16  CBC4:1;             // 3      CBC错误联防4选择（TZ4）周期触发联防控制位
    Uint16  CBC5:1;             // 4      CBC错误联防5选择（TZ5）周期触发联防控制位
    Uint16  CBC6:1;             // 5      CBC错误联防6选择（TZ6）周期触发联防控制位
    Uint16  rsvd1:2;            // 7:6    预留
    Uint16  OSHT1:1;            // 8      One-shot错误1联防选择（TZ1）单次触发联防
    Uint16  OSHT2:1;            // 9      One-shot错误2联防选择（TZ2）单次触发联防
    Uint16  OSHT3:1;            // 10     One-shot错误3联防选择（TZ3）单次触发联防
    Uint16  OSHT4:1;            // 11     One-shot错误4联防选择（TZ4）单次触发联防
    Uint16  OSHT5:1;            // 12     One-shot错误5联防选择（TZ5）单次触发联防
    Uint16  OSHT6:1;            // 13     One-shot错误6联防选择（TZ5）单次触发联防
    Uint16  rsvd2:2;            // 15:14  预留
};

union TZSEL_REG {
    Uint16                  all;
    struct TZSEL_BITS       bit;
};

//----------------------------------------------------
/* 错误联防控制寄存器（TZCTL）位定义 */
struct TZCTL_BITS {             // 位描述
    Uint16 TZA:2;               // 1:0    当错误发生时，TZ1~TZ6控制EPWMxA的输出状态控制位
    Uint16 TZB:2;               // 3:2    当错误发生时，TZ1~TZ6控制EPWMxB的输出状态控制位
    Uint16 rsvd:12;             // 15:4   预留
};

union TZCTL_REG {
    Uint16                  all;
    struct TZCTL_BITS       bit;
};

//----------------------------------------------------
/* 错误联防中断使能寄存器（TZEINT）位定义 */
struct TZEINT_BITS {            // 位描述
    Uint16  rsvd1:1;            // 0      预留
    Uint16  CBC:1;              // 1      CBC中断使能位
    Uint16  OST:1;              // 2      One-Shot中断使能位
    Uint16  rsvd2:13;           // 15:3   预留
};   

union TZEINT_REG {
   Uint16                  all;
   struct TZEINT_BITS      bit;
};

//----------------------------------------------------
/* 错误联防中断标志寄存器（TZFLG）位定义 */
struct TZFLG_BITS {             // 位描述
    Uint16  INT:1;              // 0      全局中断状态位
    Uint16  CBC:1;              // 1      CBC中断状态位
    Uint16  OST:1;              // 2      One-Shot中断状态位
    Uint16  rsvd2:13;           // 15:3   预留
};

union TZFLG_REG {
    Uint16                  all;
    struct TZFLG_BITS       bit;
};

//----------------------------------------------------
/* 错误联防中断标志清除寄存器（TZCLR）位定义 */
struct TZCLR_BITS {             // 位描述
    Uint16  INT:1;              // 0      ePWM全局中断标志清除控制位
    Uint16  CBC:1;              // 1      CBC中断标志清除控制位
    Uint16  OST:1;              // 2      One-Shot中断标志清除控制位
    Uint16  rsvd2:13;           // 15:3   预留
};

union TZCLR_REG {
    Uint16                  all;
    struct TZCLR_BITS       bit;
};

//----------------------------------------------------
/* 错误联防中断强制寄存器（TZFRC）位定义 */
struct TZFRC_BITS {             // 位描述
    Uint16  rsvd1:1;            // 0      预留
    Uint16  CBC:1;              // 1      软件强制周期（CBC）中断
    Uint16  OST:1;              // 2      软件强制单次（One-Shot）中断
    Uint16  rsvd2:13;           // 15:3   预留
};

union TZFRC_REG {
    Uint16                  all;
    struct TZFRC_BITS       bit;
};

//----------------------------------------------------
/* 事件触发选择寄存器（ETSEL）位定义 */
struct ETSEL_BITS {             // 位描述
    Uint16  INTSEL:3;           // 2:0    ePWM中断时刻选择控制位
    Uint16  INTEN:1;            // 3      ePWM中断使能控制位
    Uint16  rsvd1:4;            // 7:4    预留
    Uint16  SOCASEL:3;          // 10:8   ePWMxSOCA信号产生条件控制位
    Uint16  SOCAEN:1;           // 11     ePWMxSOCA信号产生使能控制位
    Uint16  SOCBSEL:3;          // 14:12  ePWMxSOCB信号产生条件控制位
    Uint16  SOCBEN:1;           // 15     ePWMxSOCB信号产生使能控制位
};

union ETSEL_REG {
    Uint16                  all;
    struct ETSEL_BITS       bit;
};

//----------------------------------------------------
/* 事件触发分频寄存器（ETPS）位定义 */
struct ETPS_BITS {              // 位描述
    Uint16  INTPRD:2;           // 1:0    EPWMx中断产生时刻控制位
    Uint16  INTCNT:2;           // 3:2    EPWMx中断产生次数计数位
    Uint16  rsvd1:4;            // 7:4    预留
    Uint16  SOCAPRD:2;          // 9:8    ePWMxSOCA信号产生条件控制位
    Uint16  SOCACNT:2;          // 11:10  ePWMxSOCA信号产生次数计数位
    Uint16  SOCBPRD:2;          // 13:12  ePWMxSOCB信号产生条件控制位
    Uint16  SOCBCNT:2;          // 15:14  ePWMxSOCB信号产生次数计数位
};

union ETPS_REG {
    Uint16                  all;
    struct ETPS_BITS        bit;
};

//----------------------------------------------------
/* 事件触发标志寄存器（ETPS）位定义 */
struct ETFLG_BITS {             // 位描述
    Uint16  INT:1;              // 0	EPWMx中断标志位
    Uint16  rsvd1:1;            // 1	预留
    Uint16  SOCA:1;             // 2	ePWMxSOCA信号产生标志
    Uint16  SOCB:1;             // 3	ePWMxSOCB信号产生标志
    Uint16  rsvd2:12;           // 15:4	预留
};

union ETFLG_REG {
   Uint16                   all;
   struct ETFLG_BITS        bit;
};

//----------------------------------------------------
/* 事件触发标志清除寄存器（ETCLR）位定义 */
struct ETCLR_BITS {             // 位描述
    Uint16  INT:1;              // 0	EPWMx中断标志清除位
    Uint16  rsvd1:1;            // 1	预留
    Uint16  SOCA:1;             // 2	ePWMxSOCA信号产生标志清除位
    Uint16  SOCB:1;             // 3	ePWMxSOCB信号产生标志清除位
    Uint16  rsvd2:12;           // 15:4	预留
};

union ETCLR_REG {
   Uint16                   all;
   struct ETCLR_BITS        bit;
};

//----------------------------------------------------
/* 强制事件触发寄存器（ETFRC）位定义 */
struct ETFRC_BITS {             // 位描述
    Uint16  INT:1;              // 0	EPWMx中断强制控制位
    Uint16  rsvd1:1;            // 1	预留
    Uint16  SOCA:1;             // 2	ePWMxSOCA强制控制位
    Uint16  SOCB:1;             // 3	ePWMxSOCB强制控制位
    Uint16  rsvd2:12;           // 15:4	预留
};

union ETFRC_REG {
    Uint16                  all;
    struct ETFRC_BITS        bit;
};
//----------------------------------------------------
/* 斩波控制寄存器（PCCTL）位定义 */
struct PCCTL_BITS {             // 位描述
    Uint16  CHPEN:1;            // 0      PWM斩波使能
    Uint16  OSHTWTH:4;          // 4:1    第一个脉冲宽度控制位
    Uint16  CHPFREQ:3;          // 7:5    斩波时钟频率分频系数设置位
    Uint16  CHPDUTY:3;          // 10:8   斩波时钟占空比设置位
    Uint16  rsvd1:5;            // 15:11  预留
};

union PCCTL_REG {
    Uint16                  all;
    struct PCCTL_BITS       bit;
};

//----------------------------------------------------
/* 高精度PWM控制寄存器（HRCNFG）位定义 */
struct HRCNFG_BITS {       	    // 位描述
    Uint16  EDGMODE:2;     	    // 1:0            边沿模式选择位
    Uint16  CTLMODE:1;     	    // 2                  控制模式选择位
    Uint16  HRLOAD:1;      	    // 3                  影子寄存器加载到主寄存器的时刻选择控制位
    Uint16  rsvd1:12;      	    // 15:4   预留
};

union HRCNFG_REG {
   Uint16                  	all;
   struct HRCNFG_BITS       bit;
};

//----------------------------------------------------
/* 高精度PWM相位扩展控制寄存器（TBPHS_HRPWM）位定义 */
struct TBPHS_HRPWM_REG {   	    // 位描述
    Uint16  TBPHSHR;     	    // 15:0   HRPWM相位扩展控制位
    Uint16  TBPHS;              // 31:16  相位偏置控制位
};

union TBPHS_HRPWM_GROUP {
   Uint32                  all;
   struct TBPHS_HRPWM_REG  half;
};

//----------------------------------------------------
/* 高精度PWM比较扩展控制寄存器（CMPA_HRPWM）位定义 */
struct CMPA_HRPWM_REG {   	    // 位描述
    Uint16  CMPAHR;     	    // 15:0   HRPWM比较扩展控制位
    Uint16  CMPA;               // 31:16  CMPA
};

union CMPA_HRPWM_GROUP {
   Uint32                 all;
   struct CMPA_HRPWM_REG  half;
};

/**************************************/
/* ePWM控制和状态寄存器文件 */
/**************************************/
struct EPWM_REGS {
   union  TBCTL_REG           TBCTL;   // 时基控制寄存器
   union  TBSTS_REG           TBSTS;   // 时基状态寄存器
   union  TBPHS_HRPWM_GROUP   TBPHS;   // 高精度相位寄存器联合体
   Uint16                     TBCTR;   // 时基计数器寄存器
   Uint16                     TBPRD;   // 时基周期寄存器
   Uint16                     rsvd1;   // 
   union  CMPCTL_REG          CMPCTL;  // 比较控制寄存器
   union  CMPA_HRPWM_GROUP    CMPA;    // 高精度比较寄存器A联合体
   Uint16                     CMPB;    // 比较寄存器B
   union  AQCTL_REG           AQCTLA;  // 动作控制寄存器A
   union  AQCTL_REG           AQCTLB;  // 动作控制寄存器B
   union  AQSFRC_REG          AQSFRC;  // 软件强制动作控制寄存器
   union  AQCSFRC_REG         AQCSFRC; // 软件连续强制动作控制寄存器
   union  DBCTL_REG           DBCTL;   // 死区控制寄存器
   Uint16                     DBRED;   // 死区上升沿延时控制寄存器
   Uint16                     DBFED;   // 死区下降沿延时控制寄存器
   union  TZSEL_REG           TZSEL;   // 错误联防选择寄存器
   Uint16                     rsvd2;   
   union  TZCTL_REG           TZCTL;   // 错误联防控制寄存器
   union  TZEINT_REG          TZEINT;  // 错误联防中断使能控制寄存器
   union  TZFLG_REG           TZFLG;   // 错误联防中断标志寄存器
   union  TZCLR_REG           TZCLR;   // 错误联防中断标志清除寄存器
   union  TZFRC_REG    	      TZFRC;   // 错误联防中断强制寄存器
   union  ETSEL_REG           ETSEL;   // 事件触发选择控制寄存器
   union  ETPS_REG            ETPS;    // 事件触发分频控制寄存器
   union  ETFLG_REG           ETFLG;   // 事件触发标志寄存器
   union  ETCLR_REG           ETCLR;   // 事件触发标志清除寄存器
   union  ETFRC_REG           ETFRC;   // 事件触发强制控制寄存器
   union  PCCTL_REG           PCCTL;   // PWM斩波控制寄存器
   Uint16                     rsvd3;   // 
   union  HRCNFG_REG          HRCNFG;  // 高精度PWM控制寄存器
};


//---------------------------------------------------------------------------
// ePWM外部引用和函数声明:
//
extern volatile struct EPWM_REGS EPwm1Regs;
extern volatile struct EPWM_REGS EPwm2Regs;
extern volatile struct EPWM_REGS EPwm3Regs;
extern volatile struct EPWM_REGS EPwm4Regs;
extern volatile struct EPWM_REGS EPwm5Regs;
extern volatile struct EPWM_REGS EPwm6Regs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_EPWM_H definition

//===========================================================================
// End of file.
//===========================================================================
