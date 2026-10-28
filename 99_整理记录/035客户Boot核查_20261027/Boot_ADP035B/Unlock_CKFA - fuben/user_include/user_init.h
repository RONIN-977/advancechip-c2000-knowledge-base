
#ifndef USER_INCLUDE_USER_INIT_H_
#define USER_INCLUDE_USER_INIT_H_

// 用于IO口设置输入或输出
#define  OUTPUT     1
#define  INPUT      0

// 用于启动模式的判断 f_start_mode
#define AUTO_MODE        0
#define RS485_MODE       1
#define STOP_MODE        2

// 允许Boost启动,至CPLD
/*#define  BoostEnCpld_ON()        GpioDataRegs.GPASET.bit.GPIO11 = 1
#define  BoostEnCpld_OFF()       GpioDataRegs.GPACLEAR.bit.GPIO11 = 1
// 允许AC启动,至CPLD
#define  AcEnCpld_ON()           GpioDataRegs.GPASET.bit.GPIO5 = 1
#define  AcEnCpld_OFF()          GpioDataRegs.GPACLEAR.bit.GPIO5 = 1
// AcRelay1,至CPLD
#define  AcRelay_ON()            GpioDataRegs.GPASET.bit.GPIO3 = 1
#define  AcRelay_OFF()           GpioDataRegs.GPACLEAR.bit.GPIO3 = 1

// 允许逆变启动,至2812
#define  AcEn2812_ON()           GpioDataRegs.GPACLEAR.bit.GPIO26 = 1
#define  AcEn2812_OFF()          GpioDataRegs.GPASET.bit.GPIO26 = 1;

// 允许Boost启动,至2812
#define  BoostEn2812_ON()        GpioDataRegs.GPACLEAR.bit.GPIO9 = 1
#define  BoostEn2812_OFF()       GpioDataRegs.GPASET.bit.GPIO9 = 1
*/
// Rs485通讯方向
//#define  RS485RxEn()             GpioDataRegs.GPBSET.bit.GPIO41=1
//#define  RS485TxEn()             GpioDataRegs.GPBCLEAR.bit.GPIO41=1


#define   SDAIN     EALLOW; GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0; EDIS
#define   SDAOUT    EALLOW; GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1; EDIS

#define   SDADATA   GpioDataRegs.GPBDAT.bit.GPIO32

#define  SDA_SET()      GpioDataRegs.GPBSET.bit.GPIO32 = 1
#define  SDA_CLR()      GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1
#define  SCL_SET()      GpioDataRegs.GPBSET.bit.GPIO33 = 1
#define  SCL_CLR()      GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1

#define  SDA_IN()       EALLOW; GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0; EDIS
#define  SDA_OUT()      EALLOW; GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1; EDIS

#define  SDA_DATA       GpioDataRegs.GPBDAT.bit.GPIO32

    //#define GPIO_Test1 GpioDataRegs.GPADAT.bit.GPIO8
    //#define GPIO_Test1_FLASH GpioDataRegs.GPATOGGLE.bit.GPIO8=1
    //#define GPIO_Test1_set0 GpioDataRegs.GPACLEAR.bit.GPIO8=1
    //#define GPIO_Test1_set1 GpioDataRegs.GPASET.bit.GPIO8=1
    //#define GPIO_Test2_set0 GpioDataRegs.GPBCLEAR.bit.GPIO39=1
    //#define GPIO_Test2_set1  GpioDataRegs.GPBSET.bit.GPIO39=1

    //#define LedWork_NegVal()   GpioDataRegs.GPATOGGLE.bit.GPIO4=1;GpioDataRegs.GPATOGGLE.bit.GPIO20=1
    //#define LedWork_ClrVal() GpioDataRegs.GPACLEAR.bit.GPIO4=1
    //#define LedWork_SetVal() GpioDataRegs.GPASET.bit.GPIO4=1

    #define LedWork_NegVal()   GpioDataRegs.GPATOGGLE.bit.GPIO22=1;GpioDataRegs.GPATOGGLE.bit.GPIO20=1
    #define LedWork_ClrVal() GpioDataRegs.GPACLEAR.bit.GPIO22=1
    #define LedWork_SetVal() GpioDataRegs.GPASET.bit.GPIO22=1

   // #define Backup_Relay1_close  GpioDataRegs.GPASET.bit.GPIO5=1   //闭合备用电源1继电器
   //#define Backup_Relay1_open   GpioDataRegs.GPACLEAR.bit.GPIO5=1   //断开备用电源1继电器
    //#define Backup_Relay1_close   EPwm3Regs.CMPB = 1500
    //#define Backup_Relay1_open   EPwm3Regs.CMPB = 0
    //#define Backup_Relay1_close_PWM  EPwm3Regs.CMPB=750

    //#define Backup_Relay2_close  GpioDataRegs.GPASET.bit.GPIO30=1   //闭合备用电源2继电?
    //#define Backup_Relay2_open   GpioDataRegs.GPACLEAR.bit.GPIO30=1   //断开备用电源2继电器

    //#define Grid_Relay3_close  GpioDataRegs.GPBSET.bit.GPIO41=1   //闭合并网3继电器
    //#define Grid_Relay3_open   GpioDataRegs.GPBCLEAR.bit.GPIO41=1   //断开并网3继电器
    //#define Grid_Relay3_flg        GpioDataRegs.GPBDAT.bit.GPIO41       //并网继电器闭合标志位
    //#define AC_Relay4_close  GpioDataRegs.GPASET.bit.GPIO22=1   //闭合交流4继电器
   // #define AC_Relay4_open   GpioDataRegs.GPACLEAR.bit.GPIO22=1   //断开交流4继电器

    //#define DC_Relay1_close  GpioDataRegs.GPBSET.bit.GPIO44=1   //闭合直流1继电器
    //#define DC_Relay1_open   GpioDataRegs.GPBCLEAR.bit.GPIO44=1   //断开直流1继缙?
   // #define DC_Relay2_close  GpioDataRegs.GPASET.bit.GPIO25=1   //闭合直流2痰缙?
   // #define DC_Relay2_open   GpioDataRegs.GPACLEAR.bit.GPIO25=1   //断开直流2继缙?

    //#define EnAC  GpioDataRegs.GPASET.bit.GPIO9=1//高电平使能20190312
    //#define DisableAC GpioDataRegs.GPACLEAR.bit.GPIO9=1 //低电平封锁20190312
    //#define Online_modle GpioDataRegs.GPADAT.bit.GPIO14
   // #define Mppt_modle  GpioDataRegs.GPADAT.bit.GPIO13
   // #define IO_start_Tx GpioDataRegs.GPADAT.bit.GPIO31
    //#define IO_start_Rx GpioDataRegs.GPBDAT.bit.GPIO34

  //#define  Uaco_zero   GpioDataRegs.GPADAT.bit.GPIO21

    //#define AD_offset_IO    GpioDataRegs.GPADAT.bit.GPIO4     //20201111改
   // #define AD_Reset_key    GpioDataRegs.GPADAT.bit.GPIO22     //20190312改


extern void UserGpioInit(void);
extern void UserVariableInit(void);
extern void RunModeJudge(void);

#endif /* USER_INCLUDE_USER_INIT_H_ */
