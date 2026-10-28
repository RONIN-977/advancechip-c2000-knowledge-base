/******************************************************************************
文件名                     : AVP33x_EPwm_defines.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : ePWM模块用到的宏定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_EPWM_DEFINES_H
#define AVP33x_EPWM_DEFINES_H


#ifdef __cplusplus
extern "C" {
#endif

// 时基控制寄存器相关宏定义
//==========================
// 计数模式控制位
#define	TB_COUNT_UP		0x0     // 时基计数器向上计数模式
#define	TB_COUNT_DOWN	0x1     // 时基计数器向下计数模式
#define	TB_COUNT_UPDOWN	0x2     // 时基计数器向上-下计数模式
#define	TB_FREEZE		0x3     // 时基计数器停止计数（复位后的默认状态）
// 计数寄存器装载相位寄存器使能位
#define	TB_DISABLE		0x0     // 计数寄存器不装载相位寄存器的值
#define	TB_ENABLE		0x1     // 当同步信号到来时，计数寄存器装载相位寄存器的值
// 周期寄存器装载影子寄存器控制位
#define	TB_SHADOW		0x0     // 当计数器的值为0时，周期寄存器将装载影子寄存器的值
#define	TB_IMMEDIATE	0x1     // 不使用影子寄存器，一旦设置周期值直接就被装载到周期寄存器中
// 同步信号输出控制位
#define	TB_SYNC_IN		0x0     // 当ePWMxSYNCI信号出现时，输出同步信号
#define	TB_CTR_ZERO		0x1     // 当时基计数器值=0时，输出同步信号
#define	TB_CTR_CMPB		0x2     // 当时基计数器值=比较值时，输出同步信号
#define	TB_SYNC_DISABLE	0x3     // 禁止输出同步信号
// 时基时钟分频控制位
#define	TB_DIV1			0x0     // 不分频
#define	TB_DIV2			0x1     // 2分频
#define	TB_DIV4			0x2     // 4分频
// 相位方向控制位
#define	TB_DOWN			0x0     // 同步信号到来时，向下计数
#define	TB_UP			0x1     // 同步信号到来时，向上计数

// CMPCTL (Compare Control)
//==========================
// LOADAMODE and LOADBMODE bits
#define	CC_CTR_ZERO		0x0
#define	CC_CTR_PRD		0x1
#define	CC_CTR_ZERO_PRD	0x2
#define	CC_LD_DISABLE	0x3
// SHDWAMODE and SHDWBMODE bits
#define	CC_SHADOW		0x0
#define	CC_IMMEDIATE	0x1

// AQCTLA and AQCTLB (Action Qualifier Control)
//=============================================
// ZRO, PRD, CAU, CAD, CBU, CBD bits
#define	AQ_NO_ACTION	0x0
#define	AQ_CLEAR		0x1
#define	AQ_SET			0x2
#define	AQ_TOGGLE		0x3

// DBCTL (Dead-Band Control)
//==========================
// OUT MODE bits
#define	DB_DISABLE		0x0
#define	DBA_ENABLE		0x1
#define	DBB_ENABLE		0x2
#define	DB_FULL_ENABLE	0x3
// POLSEL bits
#define	DB_ACTV_HI		0x0
#define	DB_ACTV_LOC		0x1
#define	DB_ACTV_HIC		0x2
#define	DB_ACTV_LO		0x3
// IN MODE
#define DBA_ALL         0x0
#define DBB_RED_DBA_FED 0x1
#define DBA_RED_DBB_FED 0x2
#define DBB_ALL         0x3

// CHPCTL (chopper control)
//==========================
// CHPEN bit
#define	CHP_DISABLE		0x0
#define	CHP_ENABLE		0x1
// CHPFREQ bits
#define	CHP_DIV1		0x0
#define	CHP_DIV2		0x1
#define	CHP_DIV3		0x2
#define	CHP_DIV4		0x3
#define	CHP_DIV5		0x4
#define	CHP_DIV6		0x5
#define	CHP_DIV7		0x6
#define	CHP_DIV8		0x7
// CHPDUTY bits
#define	CHP1_8TH		0x0
#define	CHP2_8TH		0x1
#define	CHP3_8TH		0x2
#define	CHP4_8TH		0x3
#define	CHP5_8TH		0x4
#define	CHP6_8TH		0x5
#define	CHP7_8TH		0x6

// TZSEL (Trip Zone Select)
//==========================
// CBCn and OSHTn bits
#define	TZ_DISABLE		0x0
#define	TZ_ENABLE		0x1

// TZCTL (Trip Zone Control)
//==========================
// TZA and TZB bits
#define	TZ_HIZ			0x0
#define	TZ_FORCE_HI		0x1
#define	TZ_FORCE_LO		0x2
#define	TZ_NO_CHANGE	0x3

// ETSEL (Event Trigger Select)
//=============================
#define	ET_CTR_ZERO		0x1
#define	ET_CTR_PRD		0x2
#define	ET_CTRU_CMPA	0x4
#define	ET_CTRD_CMPA	0x5
#define	ET_CTRU_CMPB	0x6
#define	ET_CTRD_CMPB	0x7

// ETPS (Event Trigger Pre-scale)
//===============================
// INTPRD, SOCAPRD, SOCBPRD bits
#define	ET_DISABLE		0x0
#define	ET_1ST			0x1
#define	ET_2ND			0x2
#define	ET_3RD			0x3


//--------------------------------
// HRPWM (High Resolution PWM)
//================================
// HRCNFG
#define	HR_Disable		0x0
#define	HR_REP			0x1
#define	HR_FEP			0x2
#define	HR_BEP			0x3

#define	HR_CMP			0x0
#define	HR_PHS			0x1

#define	HR_CTR_ZERO		0x0
#define	HR_CTR_PRD		0x1


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif   // - end of AVP33x_EPWM_DEFINES_H

//===========================================================================
// End of file.
//===========================================================================
