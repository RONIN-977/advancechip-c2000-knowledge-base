/******************************************************************************
文件名                     : AVP32F08_Xintf.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP32F08芯片外部总线接口模块寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP32F08_XINTF_H
#define AVP32F08_XINTF_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
/* XINTF时序寄存器（XTIMING）位定义 */
struct XTIMING_BITS {       // 位描述
    Uint16 XWRTRAIL:2;      // 1:0   写访问的跟踪时间中等待周期个数设定位
    Uint16 XWRACTIVE:3;     // 4:2   写访问的有效时间中等待周期个数设定位
    Uint16 XWRLEAD:2;       // 6:5   写访问的建立时间中等待周期个数设定位
    Uint16 XRDTRAIL:2;      // 8:7   读访问的跟踪时间中等待周期个数设定位
    Uint16 XRDACTIVE:3;     // 11:9  读访问的有效时间中等待周期个数设定位
    Uint16 XRDLEAD:2;       // 13:12 读访问的建立时间中等待周期个数设定位
    Uint16 USEREADY:1;      // 14    区域XREADY信号采样方式控制位
    Uint16 READYMODE:1;     // 15    XREADY信号采样方式控制位（0：同步采样；1：异步采样）
    Uint16 XSIZE:2;         // 17:16 XINTF数据总线宽度设定位（必须设为01b或11b，其他值无效）
    Uint16 rsvd1:4;         // 21:18 预留
    Uint16 X2TIMING:1;      // 22    指定XRDLEAD、XRDACTIVE、XRDTRAIL、XWRLEAD、XWRACTIVE、XWRTRAIL的实际值与寄存器中设定值的比值。
    Uint16 rsvd3:9;         // 31:23 预留
};

union XTIMING_REG {
    Uint32               all;
    struct XTIMING_BITS  bit;
};

//----------------------------------------------------
/* XINTF配置寄存器（XINTCNF2）位定义 */
struct XINTCNF2_BITS {      // 位描述
    Uint16 WRBUFF:2;        // 1:0   写缓冲器深度控制位
    Uint16 CLKMODE:1;       // 2     XCLKOUT时钟频率控制位
    Uint16 CLKOFF:1;        // 3     XCLKOUT使能控制位
    Uint16 rsvd1:2;         // 5:4   预留
    Uint16 WLEVEL:2;        // 7:6   写缓冲寄存器中数据个数
    Uint16 rsvd2:1;         // 8     预留
    Uint16 HOLD:1;          // 9     允许外部设备驱动XHOLD输入信号，并允许XHOLDA输出信号
    Uint16 HOLDS:1;         // 10    HOLDn的状态标志位
    Uint16 HOLDAS:1;        // 11    HOLDAn的状态标志位
    Uint16 rsvd3:4;         // 15:12 预留
    Uint16 XTIMCLK:3;       // 18:16 配置XTIMCLK与SYSCLKOUT之间的关系
    Uint16 rsvd4:13;        // 31:19 预留
};

union XINTCNF2_REG {
    Uint32                all;
    struct XINTCNF2_BITS  bit;
};

//----------------------------------------------------
/* XINTF区域切换控制寄存器（XBANK）位定义 */
struct XBANK_BITS {         // 位描述
    Uint16  BANK:3;         // 2:0   指定使用区域切换功能的XINTF存储区域
    Uint16  BCYC:3;         // 5:3   定义在区域切换时插入的延时时间
    Uint16  rsvd:10;        // 15:6  预留
};

union XBANK_REG {
    Uint16             all;
    struct XBANK_BITS  bit;
};

//----------------------------------------------------
/* XINTF复位寄存器（XBANK）位定义 */
struct XRESET_BITS {
    Uint16  XHARDRESET:1;
    Uint16  rsvd1:15;
};

union XRESET_REG {
    Uint16            all;
    struct XRESET_BITS bit;
};

/**************************************/
/*             XINTF寄存器文件                                  */
/**************************************/
struct XINTF_REGS {
    union XTIMING_REG XTIMING0;
    Uint32  rsvd1[5];
    union XTIMING_REG XTIMING6;
    union XTIMING_REG XTIMING7;
    Uint32  rsvd2[2];
    union XINTCNF2_REG XINTCNF2;
    Uint32  rsvd3;
    union XBANK_REG    XBANK;
    Uint16  rsvd4;
    Uint16  XREVISION;
    Uint16  rsvd5[2];
    union XRESET_REG   XRESET;
};

//---------------------------------------------------------------------------
// XINTF外部引用和函数声明:
//
extern volatile struct XINTF_REGS XintfRegs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP32F08_XINTF_H definition

//===========================================================================
// No more.
//===========================================================================
