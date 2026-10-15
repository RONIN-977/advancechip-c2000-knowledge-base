/******************************************************************************
文件名                     : AVP33x_DevEmu.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片仿真寄存器定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#ifndef AVP33x_DEV_EMU_H
#define AVP33x_DEV_EMU_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// 芯片仿真寄存器位定义:
//
// 芯片配置寄存器位定义
struct DEVICECNF_BITS  {    // 位描述
    Uint16 rsvd1:3;         // 2:0   预留
    Uint16 VMAPS:1;         // 3     VMAP状态
    Uint16 rsvd2:1;         // 4     预留
    Uint16 XRSn:1;          // 5     XRSn信号状态
    Uint16 rsvd3:10;        // 15:6
    Uint16 rsvd4:3;         // 18:16
    Uint16 ENPROT:1;        // 19    使能/禁止管道保护
    Uint16 rsvd5:7;         // 26:20 预留
    Uint16 TRSTN:1;         // 27    TRSTn信号状态
    Uint16 rsvd6:4;         // 31:28 预留
};

union DEVICECNF_REG {
    Uint32                 all;
    struct DEVICECNF_BITS  bit;
};

// 类ID
struct CLASSID_BITS   {     // 位描述
    Uint16 CLASSNO:8;       // 7:0   类编号
    Uint16 PARTTYPE:8;      // 15:8  设备类型
};

union CLASSID_REG {
    Uint16               all;
    struct CLASSID_BITS  bit;
};

struct DEV_EMU_REGS {
    union DEVICECNF_REG DEVICECNF;  // 设备配置
    union CLASSID_REG   CLASSID;    // 类ID
    Uint16              REVID;      // 设备ID
    Uint16              PROTSTART;  // 读写保护启动
    Uint16              PROTRANGE;  // 读写保护范围
    Uint16              rsvd2[202];
};

// 芯片ID
struct PARTID_BITS   {      // 位描述
    Uint16 PARTNO:8;        // 7:0   芯片编号
    Uint16 PARTTYPE:8;      // 15:8  芯片类型
};

union PARTID_REG {
    Uint16               all;
    struct PARTID_BITS   bit;
};

struct PARTID_REGS {
    union PARTID_REG PARTID;// 芯片ID
};


//---------------------------------------------------------------------------
// 设备仿真寄存器外部引用和函数声明:
//
extern volatile struct DEV_EMU_REGS DevEmuRegs;
extern volatile struct PARTID_REGS PartIdRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of AVP33x_DEV_EMU_H definition

//===========================================================================
// End of file.
//===========================================================================
