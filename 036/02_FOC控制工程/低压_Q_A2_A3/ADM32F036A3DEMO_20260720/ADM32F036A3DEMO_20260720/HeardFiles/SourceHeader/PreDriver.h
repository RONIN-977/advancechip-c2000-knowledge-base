#ifndef PRE_DRIVER_H
#define PRE_DRIVER_H

#include "Main.h"
#include "MotorInclude.h"


/*=============================================================================
 * Pre Driver Basic Configuration
 *============================================================================*/
#define PRE_DRIVER_ADDR              0x6C
#define I2CDELAY                     20000


/*=============================================================================
 * Pre Driver Register Address
 *============================================================================*/
#define STATE_REG_ADDR               0x00
#define VDS_GDF_STATE_ADDR           0x08    /* 1 << 3 */
#define CONTROL_REG_ADDR             0x10    /* 2      */
#define TDEAD_WD_REG_ADDR            0x18    /* 3      */
#define VDS12_CONTROL_ADDR           0x20    /* 4      */
#define CONFIG_REG_ADDR              0x28    /* 5      */
#define VDS3_GDF3_REG_ADDR           0x30    /* 6      */
#define VDS3_CONTROL_ADDR            0x38    /* 7      */


#ifndef F036_DEVICE_I2C

/*=============================================================================
 * Software I2C GPIO Control
 *============================================================================*/
#define SDA_H                        GpioDataRegs.GPASET.bit.GPIO28 = 1
#define SDA_L                        GpioDataRegs.GPACLEAR.bit.GPIO28 = 1

#define SCL_H                        GpioDataRegs.GPASET.bit.GPIO29 = 1
#define SCL_L                        GpioDataRegs.GPACLEAR.bit.GPIO29 = 1

#define SDA_IN                       {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO28 = 0; EDIS;}
#define SDA_OUT                      {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO28 = 1; EDIS;}

#define SDA_DATA                     GpioDataRegs.GPADAT.bit.GPIO28

#define START_DELAY                  90
#define STOP_DELAY                   90

/* 420K  H_LEVEL_DELAY = 12  L_LEVEL_DELAY = 14 */
/* 180K  H_LEVEL_DELAY = 45  L_LEVEL_DELAY = 45 */
#define H_LEVEL_DELAY                45
#define L_LEVEL_DELAY                45

#endif


/*=============================================================================
 * Pre Driver Error Status Bit Definition
 *============================================================================*/
struct PER_DVR_ERR_BITS
{
    Uint16 OTW:1;
    Uint16 OTSD:1;
    Uint16 VCP_UVFL:1;
    Uint16 VM_UVFL:1;
    Uint16 OCP:1;
    Uint16 GDF:1;
    Uint16 WDFLT:1;
    Uint16 FAULT:1;
    Uint16 rsvd1:8;
};


/*=============================================================================
 * Pre Driver VDS/GDF 1-2 Status Bit Definition
 *============================================================================*/
struct PER_DVR_VDSGDF_BITS
{
    Uint16 L1_VDS:1;
    Uint16 H1_VDS:1;
    Uint16 L2_VDS:1;
    Uint16 H2_VDS:1;
    Uint16 L1_GDF:1;
    Uint16 H1_GDF:1;
    Uint16 L2_GDF:1;
    Uint16 H2_GDF:1;
    Uint16 rsvd1:8;
};


/*=============================================================================
 * Pre Driver VDS/GDF 3 Status Bit Definition
 *============================================================================*/
struct PER_DVR_VDSGDFNEXT_BITS
{
    Uint16 L3_VDS:1;
    Uint16 H3_VDS:1;
    Uint16 L3_GDF:1;
    Uint16 H3_GDF:1;
    Uint16 VDS_L0:1;
    Uint16 VDS_L1:1;
    Uint16 VDS_L2:1;
    Uint16 VDS_L3:1;
    Uint16 rsvd1:8;
};


/*=============================================================================
 * Pre Driver Register Union Definition
 *============================================================================*/
union PER_DVR_ERR_REG
{
    Uint16                  all;
    struct PER_DVR_ERR_BITS bit;
};


union PER_DVR_VDSGDF_REG
{
    Uint16                     all;
    struct PER_DVR_VDSGDF_BITS bit;
};


union PER_DVR_VDSGDFNEXT_REG
{
    Uint16                         all;
    struct PER_DVR_VDSGDFNEXT_BITS bit;
};


/*=============================================================================
 * Pre Driver Register Structure Definition
 *============================================================================*/
struct PER_DVR_REGS
{
    union PER_DVR_ERR_REG        ErrFlag;
    union PER_DVR_VDSGDF_REG     VDSGDF12Status;
    union PER_DVR_VDSGDFNEXT_REG VDSGDF3Status;
};


/* extern struct PER_DVR_REGS PreDriverRegs; */


/*=============================================================================
 * 0x02 主控制寄存器 / 故障位清除
 *============================================================================*/

/* 锁状态定义 */
#define LOCK_STATE_UNLOCKED          (3 << 3)    /* 解锁状态值 */
#define LOCK_STATE_LOCKED            (6 << 3)    /* 锁定状态值 */

/* 故障处理 */
#define CLEAR_FLT                    (1 << 0)    /* 清除故障   */
#define UNCLEAR_FLT                  (0 << 0)    /* 不清除故障 */

#define DEFAULT_DROCES_CONFIG        (LOCK_STATE_UNLOCKED | UNCLEAR_FLT)


/*=============================================================================
 * 0x03 预驱死区时间配置
 *============================================================================*/

/* 死区时间设置 */
#define TDEAD_960ns                  (3 << 6)    /* 死区时间960ns */
#define TDEAD_480ns                  (2 << 6)    /* 死区时间480ns */
#define TDEAD_240ns                  (1 << 6)    /* 死区时间240ns */
#define TDEAD_120ns                  (0 << 6)    /* 死区时间120ns */

/* 看门狗时间使能设置 */
#define WD_ENABLE                    (1 << 5)    /* 使能看门狗时间 */
#define WD_DISABLE                   (0 << 5)    /* 禁用看门狗时间 */

/* 看门狗延迟时间选择 */
#define WD_TIMEOUT_DELAY_100ms       (3 << 3)    /* 看门狗超时延迟为100ms */
#define WD_TIMEOUT_DELAY_50ms        (2 << 3)    /* 看门狗超时延迟为50ms  */
#define WD_TIMEOUT_DELAY_20ms        (1 << 3)    /* 看门狗超时延迟为20ms  */
#define WD_TIMEOUT_DELAY_10ms        (0 << 3)    /* 看门狗超时延迟为10ms  */

#define RESERVED_BITS                (7 << 0)    /* 保留位 */

#define TDEAD_WD_CONFIG              (TDEAD_120ns | WD_DISABLE | WD_TIMEOUT_DELAY_10ms | RESERVED_BITS)


/*=============================================================================
 * 0x04 VDS1/2 控制寄存器
 *============================================================================*/

/* SO输出设置 */
#define SO_LIM_DEFAULT               (0 << 7)    /* 默认操作，无特定电压限制 */
#define SO_LIM_3_6V                  (1 << 7)    /* SO输出电压限制为3.6V    */

/* VDS_H监测电压定义 */
#define VDS_H_0_06V                  (0 << 4)    /* 0.06V  */
#define VDS_H_0_145V                 (1 << 4)    /* 0.145V */
#define VDS_H_0_17V                  (2 << 4)    /* 0.17V  */
#define VDS_H_0_2V                   (3 << 4)    /* 0.2V   */
#define VDS_H_0_12V                  (4 << 4)    /* 0.12V  */
#define VDS_H_0_24V                  (5 << 4)    /* 0.24V  */
#define VDS_H_0_48V                  (6 << 4)    /* 0.48V  */
#define VDS_H_0_96V                  (7 << 4)    /* 0.96V  */

/* 半桥VDS监测禁用定义 */
#define H2_VDS_DISABLE               (1 << 3)    /* 禁用半桥2高压侧FET上的VDS监测 */
#define H2_VDS_ENABLE                (0 << 3)    /* 启用半桥2高压侧FET上的VDS监测 */
#define L2_VDS_DISABLE               (1 << 2)    /* 禁用半桥2低压侧FET上的VDS监测 */
#define L2_VDS_ENABLE                (0 << 2)    /* 启用半桥2低压侧FET上的VDS监测 */
#define H1_VDS_DISABLE               (1 << 1)    /* 禁用半桥1高压侧FET上的VDS监测 */
#define H1_VDS_ENABLE                (0 << 1)    /* 启用半桥1高压侧FET上的VDS监测 */
#define L1_VDS_DISABLE               (1 << 0)    /* 禁用半桥1低压侧FET上的VDS监测 */
#define L1_VDS_ENABLE                (0 << 0)    /* 启用半桥1低压侧FET上的VDS监测 */

#define VDS12_CONFIG                 (SO_LIM_DEFAULT | VDS_H_0_96V | H2_VDS_ENABLE | L2_VDS_ENABLE | H1_VDS_ENABLE | L1_VDS_ENABLE)
/* #define VDS12_CONFIG              (SO_LIM_DEFAULT | VDS_H_0_145V | H2_VDS_DISABLE | L2_VDS_DISABLE | H1_VDS_DISABLE | L1_VDS_DISABLE) */

/*
 * VDS_H保护电流 = VDS_H_VOLTAGE / 采样电阻内阻
 * VDS_H保护电流 = 0.145V / 0.0013Ω = 111A
 */


/*=============================================================================
 * 0x05 预驱电流保护设置
 *============================================================================*/

/* 母线过流保护使能位 */
#define OCP_DISABLE                  (1 << 5)    /* 禁止母线过流保护 */
#define OCP_ENABLE                   (0 << 5)    /* 使能母线过流保护 */

/* VREF参考电压选择 */
#define VREF_3V                      (0 << 3)    /* 3V   */
#define VREF_2V                      (1 << 3)    /* 2V   */
#define VREF_1_2V                    (2 << 3)    /* 1.2V */
#define VREF_0_4V                    (3 << 3)    /* 0.4V */

/* 分流放大器控制 */
#define SHUNT_AMP_EN                 (1 << 2)    /* 使能分流放大器 */
#define SHUNT_AMP_DIS                (0 << 2)    /* 禁止分流放大器 */

/* 放大器增益选择 */
#define GAIN_2X                      (0 << 0)    /* 2倍  */
#define GAIN_4X                      (1 << 0)    /* 4倍  */
#define GAIN_8X                      (2 << 0)    /* 8倍  */
#define GAIN_16X                     (3 << 0)    /* 16倍 */

#define IBUSOCP_CONFIG               (OCP_ENABLE | VREF_3V | SHUNT_AMP_EN | GAIN_16X)

/*
 * 保护电流 = VREF / (采样电阻 * 放大器增益)
 * 保护电流 = 1.2V / (0.0025Ω * 16) = 30A
 */


/*=============================================================================
 * 0x06 VDS3/GDF3 控制寄存器 [3-0]只读寄存器
 *============================================================================*/

/* 设置低边桥每个FET的VDS(OCP)监测 */
#define VDS_L_0_06V                  (0 << 4)    /* 0.06V */
#define VDS_L_0_145V                 (1 << 4)    /* 0.145V */
#define VDS_L_0_17V                  (2 << 4)    /* 0.17V */
#define VDS_L_0_20V                  (3 << 4)    /* 0.2V */
#define VDS_L_0_22V                  (4 << 4)    /* 0.22V */
#define VDS_L_0_24V                  (5 << 4)    /* 0.24V */
#define VDS_L_0_36V                  (6 << 4)    /* 0.36V */
#define VDS_L_0_48V                  (7 << 4)    /* 0.48V */
#define VDS_L_0_60V                  (8 << 4)    /* 0.60V */
#define VDS_L_0_72V                  (9 << 4)    /* 0.72V */
#define VDS_L_0_84V                  (10 << 4)   /* 0.84V */
#define VDS_L_0_96V                  (11 << 4)   /* 0.96V */
#define VDS_L_1_20V                  (12 << 4)   /* 1.20V */
#define VDS_L_1_44V                  (13 << 4)   /* 1.44V */
#define VDS_L_1_68V                  (14 << 4)   /* 1.68V */
#define VDS_L_1_96V                  (15 << 4)   /* 1.92V */

#define VDS3_GDF3_CONFIG             (VDS_L_1_96V | 0)

/*
 * VDS_L保护电流 = VDS_L / (功率管内阻 + 采样电阻内阻)
 * VDS_L保护电流 = 0.36V / (0.0013Ω + 0.0025Ω) = 97A
 */


/*=============================================================================
 * 0x07 电流保护使能控制
 *============================================================================*/

/* LIN模块TXD引脚显性超时保护功能 */
#define DIS_T_TO_DOM_TXD_ENABLE      (0 << 6)    /* 使能LIN模块的XTD引脚显性超时保护功能 */
#define DIS_T_TO_DOM_TXD_DISABLE     (1 << 6)    /* 禁用LIN模块的XTD引脚显性超时保护功能 */

/* SP和SN之间的电压差监测启用/禁用设置 */
#define DIS_L16_VDS_ENABLE           (0 << 2)    /* 启用SP和SN之间的电压差监测,可用于母线电流的检测 */
#define DIS_L16_VDS_DISABLE          (1 << 2)    /* 禁用SP和SN之间的电压差监测,可用于母线电流的检测 */

/* VDSH3/L3检测启用/禁用设置 */
#define DIS_H3_VDS_ENABLE            (0 << 1)    /* 启用半桥3高压侧FET上的VDS监测 */
#define DIS_H3_VDS_DISABLE           (1 << 1)    /* 禁用半桥3高压侧FET上的VDS监测 */
#define DIS_L3_VDS_ENABLE            (0 << 0)    /* 启用半桥3低压侧FET上的VDS监测 */
#define DIS_L3_VDS_DISABLE           (1 << 0)    /* 禁用半桥3低压侧FET上的VDS监测 */

#define VDS3_SPSN_CONFIG             (DIS_T_TO_DOM_TXD_ENABLE | DIS_L16_VDS_ENABLE | DIS_H3_VDS_ENABLE | DIS_L3_VDS_ENABLE)
/* #define VDS3_SPSN_CONFIG          (DIS_T_TO_DOM_TXD_ENABLE | DIS_L16_VDS_ENABLE | DIS_H3_VDS_DISABLE | DIS_L3_VDS_DISABLE) */


/*=============================================================================
 * External Function Declaration
 *============================================================================*/
void InitI2CIO(void);
void Init_I2C_Read(Uint16 addr, Uint16 cmd);
void PreDriverInit(void);
void GetPerDriverState(void);
void DriverClaerFault(void);


#endif /* PRE_DRIVER_H */
