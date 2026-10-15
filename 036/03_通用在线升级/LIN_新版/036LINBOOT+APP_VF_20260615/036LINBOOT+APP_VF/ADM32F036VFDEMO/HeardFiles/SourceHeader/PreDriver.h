
#ifndef _PRE_DRIVER_H_
#define _PRE_DRIVER_H_

#include "MotorInclude.h"

#define  PRE_DRIVER_ADDR        0x6C
#define  I2CDELAY               20000

#define  STATE_REG_ADDR         0x00
#define  VDS_GDF_STATE_ADDR     0x08    // 1 << 3
#define  CONTROL_REG_ADDR       0x10    // 2
#define  TDEAD_WD_REG_ADDR      0x18    // 3
#define  VDS12_CONTROL_ADDR     0x20    // 4
#define  CONFIG_REG_ADDR        0x28    // 5
#define  VDS3_GDF3_REG_ADDR     0x30    // 6
#define  VDS3_CONTROL_ADDR      0x38    // 7

#ifndef  F036_DEVICE_I2C

#define  SDA_H           GpioDataRegs.GPASET.bit.GPIO28 = 1
#define  SDA_L           GpioDataRegs.GPACLEAR.bit.GPIO28 = 1

#define  SCL_H           GpioDataRegs.GPASET.bit.GPIO29 = 1
#define  SCL_L           GpioDataRegs.GPACLEAR.bit.GPIO29 = 1


#define  SDA_IN          {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO28 = 0; EDIS;}
#define  SDA_OUT         {EALLOW; GpioCtrlRegs.GPADIR.bit.GPIO28 = 1; EDIS;}

#define  SDA_DATA        GpioDataRegs.GPADAT.bit.GPIO28

#define  START_DELAY     90
#define  STOP_DELAY      90

//420K  H_LEVEL_DELAY = 12 L_LEVEL_DELAY = 14
//180K  H_LEVEL_DELAY = 45 L_LEVEL_DELAY = 45
#define  H_LEVEL_DELAY   45
#define  L_LEVEL_DELAY   45

#endif

struct PER_DVR_ERR_BITS
{
    Uint16   OTW:1;
    Uint16   OTSD:1;
    Uint16   VCP_UVFL:1;
    Uint16   VM_UVFL:1;
    Uint16   OCP:1;
    Uint16   GDF:1;
    Uint16   WDFLT:1;
    Uint16   FAULT:1;
    Uint16   rsvd1:8;
};
struct PER_DVR_VDSGDF_BITS
{

    Uint16   L1_VDS:1;
    Uint16   H1_VDS:1;
    Uint16   L2_VDS:1;
    Uint16   H2_VDS:1;
    Uint16   L1_GDF:1;
    Uint16   H1_GDF:1;
    Uint16   L2_GDF:1;
    Uint16   H2_GDF:1;
    Uint16   rsvd1:8;
};
struct PER_DVR_VDSGDFNEXT_BITS
{
    Uint16   L3_VDS:1;
    Uint16   H3_VDS:1;
    Uint16   L3_GDF:1;
    Uint16   H3_GDF:1;
    Uint16   VDS_L0:1;
    Uint16   VDS_L1:1;
    Uint16   VDS_L2:1;
    Uint16   VDS_L3:1;
    Uint16   rsvd1:8;
};

union PER_DVR_ERR_REG  {
    Uint16                     all;
    struct PER_DVR_ERR_BITS    bit;
};

union PER_DVR_VDSGDF_REG  {
    Uint16                     all;
    struct PER_DVR_VDSGDF_BITS    bit;
};

union PER_DVR_VDSGDFNEXT_REG  {
    Uint16                     all;
    struct PER_DVR_VDSGDFNEXT_BITS    bit;
};

struct PER_DVR_REGS
{
    union PER_DVR_ERR_REG ErrFlag;
    union PER_DVR_VDSGDF_REG VDSGDF12Status;
    union PER_DVR_VDSGDFNEXT_REG VDSGDF3Status;
};

extern struct PER_DVR_REGS PreDriverRegs;

extern void InitI2C(void);
extern void InitI2CIO(void);
#ifdef F036_DEVICE_I2C
extern Uint16 PreDriverWriteData(Uint16 Addr, Uint16 Data);
extern Uint16 PreDriverReadData(Uint16 Addr, Uint16* Data);
#else
extern void PreDriverWriteData(Uint16 Addr, Uint16 Data);
extern Uint16 PreDriverReadData(Uint16 Addr);
extern void Init_I2C_Read(Uint16 addr, Uint16 cmd);
#endif
extern void PreDriverInit(void);
extern void GetPerDriverState(void);
extern void DriverClaerFault(void);


//////////////////////////////////////////////////////////////////////////////////////
// 0x02 Main Control Register / Fault Bit Clearing
///////////////////////////////////////////////////////////////////////////////////////
//Lock status definition
#define LOCK_STATE_UNLOCKED  (3 << 3)  // Unlock status value
#define LOCK_STATE_LOCKED    (6 << 3)  // Lock state value
//Fault handling
#define CLEAR_FLT            (1 << 0)  //Clear the fault
#define UNCLEAR_FLT          (0 << 0)  //Not clearing the fault
#define DEFAULT_DROCES_CONFIG (LOCK_STATE_UNLOCKED | UNCLEAR_FLT )

//////////////////////////////////////////////////////////////////////////////////////
// 0x03 Pre-drive Dead Zone Time Configuration
///////////////////////////////////////////////////////////////////////////////////////
//Dead zone time setting
#define TDEAD_960ns             (3 << 6)    //dead-time 960ns
#define TDEAD_480ns             (2 << 6)    //dead-time 480ns
#define TDEAD_240ns             (1 << 6)    //dead-time 240ns
#define TDEAD_120ns             (0 << 6)    //dead-time 120ns
//Watchdog timer enable setting
#define WD_ENABLE               (1 << 5)    //Enable watchdog time
#define WD_DISABLE              (0 << 5)    //Disable watchdog timer
//Watchdog Delay Time Selection
#define WD_TIMEOUT_DELAY_100ms  (3 << 3)    // The watchdog timer has a timeout delay of 100ms
#define WD_TIMEOUT_DELAY_50ms   (2 << 3)    // The watchdog timer has a timeout delay of 50ms
#define WD_TIMEOUT_DELAY_20ms   (1 << 3)    // The watchdog timer has a timeout delay of 20ms
#define WD_TIMEOUT_DELAY_10ms   (0 << 3)    // The watchdog timer has a timeout delay of 10ms
#define RESERVED_BITS (7 << 0)              // Reserved
#define TDEAD_WD_CONFIG (TDEAD_120ns | WD_DISABLE | WD_TIMEOUT_DELAY_10ms | RESERVED_BITS )

//////////////////////////////////////////////////////////////////////////////////////
// 0x04 VDS1/2 Control register
///////////////////////////////////////////////////////////////////////////////////////
//SO Output settings
#define SO_LIM_DEFAULT     (0 << 7)   // Default operation, no specific voltage limit
#define SO_LIM_3_6V        (1 << 7)   // The output voltage is limited to 3.6V.
//VDS_H Definition of Voltage Monitoring
#define VDS_H_0_06V        (0 << 4)   // 0.06V
#define VDS_H_0_145V       (1 << 4)   // 0.145V
#define VDS_H_0_17V        (2 << 4)   // 0.17V
#define VDS_H_0_2V         (3 << 4)   // 0.2V
#define VDS_H_0_12V        (4 << 4)   // 0.12V
#define VDS_H_0_24V        (5 << 4)   // 0.24V
#define VDS_H_0_48V        (6 << 4)   // 0.48V
#define VDS_H_0_96V        (7 << 4)   // 0.96V
// Half-bridge VDS monitoring disable definitions
#define H2_VDS_DISABLE     (1 << 3)   // Disable VDS monitoring on high-side FET of half-bridge 2
#define H2_VDS_ENABLE      (0 << 3)   // Enable VDS monitoring on high-side FET of half-bridge 2
#define L2_VDS_DISABLE     (1 << 2)   // Disable VDS monitoring on low-side FET of half-bridge 2
#define L2_VDS_ENABLE      (0 << 2)   // Enable VDS monitoring on low-side FET of half-bridge 2
#define H1_VDS_DISABLE     (1 << 1)   // Disable VDS monitoring on high-side FET of half-bridge 1
#define H1_VDS_ENABLE      (0 << 1)   // Enable VDS monitoring on high-side FET of half-bridge 1
#define L1_VDS_DISABLE     (1 << 0)   // Disable VDS monitoring on low-side FET of half-bridge 1
#define L1_VDS_ENABLE      (0 << 0)   // Enable VDS monitoring on low-side FET of half-bridge 1
#define VDS12_CONFIG (SO_LIM_DEFAULT | VDS_H_0_145V | H2_VDS_ENABLE | L2_VDS_ENABLE | H1_VDS_ENABLE | L1_VDS_ENABLE )
//#define VDS12_CONFIG (SO_LIM_DEFAULT | VDS_H_0_145V | H2_VDS_DISABLE | L2_VDS_DISABLE | H1_VDS_DISABLE | L1_VDS_DISABLE )

// VDS_H protection current = VDS_H_VOLTAGE / (sampling resistor internal resistance)
// VDS_H protection current = 0.145V/(0.0013O) = 111A
///////////////////////////////////////////////////////////////////////////////////////
// 0x05 Pre-drive current protection setting
///////////////////////////////////////////////////////////////////////////////////////
// Bus overcurrent protection enable bit
#define OCP_DISABLE    (1 << 5)  // Prohibition of busbar overcurrent protection
#define OCP_ENABLE     (0 << 5)  // Enable bus overcurrent protection
// VREF Reference voltage selection
#define VREF_3V        (0 << 3)  // 3V
#define VREF_2V        (1 << 3)  // 2V
#define VREF_1_2V      (2 << 3)  // 1.2V
#define VREF_0_4V      (3 << 3)  // 0.4V
// Splitting amplifier control
#define SHUNT_AMP_EN   (1 << 2)  // Enable the shunt amplifier
#define SHUNT_AMP_DIS  (0 << 2)  // Prohibit splitter amplifier
// Amplifier gain selection
#define GAIN_2X        (0 << 0)  // *2
#define GAIN_4X        (1 << 0)  // *4
#define GAIN_8X        (2 << 0)  // *8
#define GAIN_16X       (3 << 0)  // *16
#define IBUSOCP_CONFIG (OCP_ENABLE | VREF_1_2V | SHUNT_AMP_EN | GAIN_8X)
// Protection current = VREF / (sampling resistor * amplifier gain)
// Protection current = 1.2V/(0.0025¦¸*16) = 30A

///////////////////////////////////////////////////////////////////////////////////////
// 0x06 VDS3/GDF3control register    [3-0]read-only register
///////////////////////////////////////////////////////////////////////////////////////
#define VDS_L_0_06V         (0 << 4)    //0.06V
#define VDS_L_0_145V        (1 << 4)    //0.145V
#define VDS_L_0_17V         (2 << 4)    //0.17V
#define VDS_L_0_20V         (3 << 4)    //0.2V
#define VDS_L_0_22V         (4 << 4)    //0.22V
#define VDS_L_0_24V         (5 << 4)    //0.24V
#define VDS_L_0_36V         (6 << 4)    //0.36V
#define VDS_L_0_48V         (7 << 4)    //0.48V
#define VDS_L_0_60V         (8 << 4)    //0.60V
#define VDS_L_0_72V         (9 << 4)    //0.72V
#define VDS_L_0_84V         (10 << 4)   //0.84V
#define VDS_L_0_96V         (11 << 4)   //0.96V
#define VDS_L_1_20V         (12 << 4)   //1.20V
#define VDS_L_1_44V         (13 << 4)   //1.44V
#define VDS_L_1_68V         (14 << 4)   //1.68V
#define VDS_L_1_96V         (15 << 4)   //1.92V
#define VDS3_GDF3_CONFIG (VDS_L_0_36V | 0)
// VDS_L protection current = VDS_L / (power transistor internal resistance + sampling resistor internal resistance)
//VDS_L protection current = 0.36V/(0.0013¦¸+0.0025¦¸) = 97A

//////////////////////////////////////////////////////////////////////////////////////
// 0x07 Current protection enable control
///////////////////////////////////////////////////////////////////////////////////////
#define DIS_T_TO_DOM_TXD_ENABLE     (0 << 6) // Enable LIN module XTD pin dominant timeout protection
#define DIS_T_TO_DOM_TXD_DISABLE    (1 << 6) // Disable LIN module XTD pin dominant timeout protection
// SP and SN voltage difference monitoring enable/disable settings
#define DIS_L16_VDS_ENABLE          (0 << 2) // Enable voltage difference monitoring between SP and SN, can be used for DC bus current detection
#define DIS_L16_VDS_DISABLE         (1 << 2) // Disable voltage difference monitoring between SP and SN, can be used for DC bus current detection
// VDSH3/L3 detection enable/disable settings
#define DIS_H3_VDS_ENABLE           (0 << 1) // Enable VDS monitoring on high-side FET of half-bridge 3
#define DIS_H3_VDS_DISABLE          (1 << 1) // Disable VDS monitoring on high-side FET of half-bridge 3
#define DIS_L3_VDS_ENABLE           (0 << 0) // Enable VDS monitoring on low-side FET of half-bridge 3
#define DIS_L3_VDS_DISABLE          (1 << 0) // Disable VDS monitoring on low-side FET of half-bridge 3
#define VDS3_SPSN_CONFIG (DIS_T_TO_DOM_TXD_ENABLE | DIS_L16_VDS_ENABLE| DIS_H3_VDS_ENABLE| DIS_L3_VDS_ENABLE)
//#define VDS3_SPSN_CONFIG (DIS_T_TO_DOM_TXD_ENABLE | DIS_L16_VDS_ENABLE| DIS_H3_VDS_DISABLE| DIS_L3_VDS_DISABLE)

#endif /* _PRE_DRIVER_H_ */
