/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   cpu1bootrom.h
*                         TITLE:  BootROM Definitions. 
*******************************************************************************************/



#ifndef C_BOOTROM_H_
#define C_BOOTROM_H_

#include <stdint.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_sysctl.h"
#include "hw_sci.h"
#include "hw_i2c.h"
#include "sysctl.h"
#include "cpu.h"
#include "flash.h"
#include "hw_dcsm.h"
#include "pin_map.h"
#include "gpio.h"
#include "sci.h"
#include "spi.h"
#include "i2c.h"
#include "dcc.h"
#include "cpu1brom_pbist.h"
#include "hw_memcfg.h"

//
//Start Address of Boot ROM
//
#define ROM_START_ADDRESS               0x3F8000UL

//
//Length of ROM in KB
//
#define ROM_REGION_LENGTH               64UL

#define RAM_GSX_NOT_DONE                0x0U
#define BROM_PLL_CONFIG_ERROR           0xFFFFU
#define BROM_PLL_CONFIG_SUCCESS         0x0U

#define EFUSE_SINGLE_BIT_ERROR          (0x15UL)

//
// Flash Configurations
//
#define CPU1_FLASH_15MHZ_PSLEEP        (0xAAU * 2U)
#define CPU1_FLASH_15MHZ_RWAIT         (0x1U)
#define CPU1_FLASH_DEFAULT_PSLEEP      (0x4D4U * 2U)
#define CPU1_FLASH_115MHZ_RWAIT        (0x6U)
#define CPU1_FLASH_DEFAULT_RWAIT       (0xFU)
#define CPU1_FLASH_15MHZ_TIMEOUT_VALUE (192U)     //Loop ~28cycles (Freq = 15MHz)
                                                  //(Loop timeout value = (360us * freq)/28cycles)


//When the ROM is run with an emulator connected, these four addresses are used
//to emulate OTP configuration.
#define EMU_BOOTPIN_CONFIG  0xD00    //Equivalent to GPREG1
#define EMU_BOOT_GPREG2     0xD02    //Equivalent to GPREG2
#define EMU_BOOTDEF_LOW     0xD04    //Equivalent to GPREG3
#define EMU_BOOTDEF_HIGH    0xD06    //Equivalent to BOOTCTRL

//Emulation boot pin configuration fields. Currently, only EMU_BOOTPIN_CONFIG_KEY
//is used.
#define EMU_BOOTPIN_CONFIG_KEY      ((HWREG(EMU_BOOTPIN_CONFIG) & (uint32_t)0xFF000000UL) >> 24)

#define EMU_BOOTDEF_L(x)            ((HWREG(EMU_BOOTDEF_LOW)  & ((uint32_t)0xFFU << (8U*x))) >> (8U*x))
#define EMU_BOOTDEF_H(x)            ((HWREG(EMU_BOOTDEF_HIGH) & ((uint32_t)0xFFU << (8U*x))) >> (8U*x))

//
// Standalone Boot Defines
//

//
// DCSM OTP Boot Configuration Registers
//
#define Z1_OTP_BOOTPIN_CONFIG           (DCSM_Z1_BASE + (uint32_t)DCSM_O_Z1_GPREG1)
#define Z1_OTP_BOOT_GPREG2              (DCSM_Z1_BASE + (uint32_t)DCSM_O_Z1_GPREG2)
#define Z1_OTP_BOOTDEF_LOW              (DCSM_Z1_BASE + (uint32_t)DCSM_O_Z1_GPREG3)
#define Z1_OTP_BOOTDEF_HIGH             (DCSM_Z1_BASE + (uint32_t)DCSM_O_Z1_GPREG4)
#define Z2_OTP_BOOTPIN_CONFIG           (DCSM_Z2_BASE + (uint32_t)DCSM_O_Z2_GPREG1)
#define Z2_OTP_BOOT_GPREG2              (DCSM_Z2_BASE + (uint32_t)DCSM_O_Z2_GPREG2)
#define Z2_OTP_BOOTDEF_LOW              (DCSM_Z2_BASE + (uint32_t)DCSM_O_Z2_GPREG3)
#define Z2_OTP_BOOTDEF_HIGH             (DCSM_Z2_BASE + (uint32_t)DCSM_O_Z2_GPREG4)

//
//DCSM Z1 CSM Key Mask Values
//

#define DCSM_Z1_CSMKEY0_MASK            0xFFFFFFFFUL
#define DCSM_Z1_CSMKEY1_MASK            0xFFFFFFFFUL
#define DCSM_Z1_CSMKEY2_MASK            0xFFFFFFFFUL
#define DCSM_Z1_CSMKEY3_MASK            0xFFFFFFFFUL

//
//DCSM Z2 CSM Key Mask Values
//

#define DCSM_Z2_CSMKEY0_MASK            0xFFFFFFFFUL
#define DCSM_Z2_CSMKEY1_MASK            0xFFFFFFFFUL
#define DCSM_Z2_CSMKEY2_MASK            0xFFFFFFFFUL
#define DCSM_Z2_CSMKEY3_MASK            0xFFFFFFFFUL


/*
Z1-GPREG2[31:24] => VALIDITY_KEY (=0x5A);
Z1-GPREG2[23:8] => RESERVED; no usage defined yet.
Z1-GPREG2[7:6] => 00 - Run PBIST with PLL disabled (10MHz internal oscillator)
                       (includes CRC test on 64KB unsecure ROM)
                  01 - Run PBIST at 115MHz
                  10 - Run PBIST at 57.5MHz
                  11 - Do not run PBIST
Z1-GPREG2[5:4] => ERROR_STS_PIN config; this tells which GPIO pin is supposed to be used as ERROR_PIN and boot ROM configures the mux as such for the said pin.
                0 - GPIO24, MUX Option 13
                1 - GPIO28, MUX Option 13
                2 - GPIO29, MUX Option 13
                3 - ERROR_STS function Disable  (default)

Z1-GPREG2[3:0]  =>  CJTAGNODEID[3:0];
                boot ROM takes this values and programs the lower 4 bits of the CJTAGNODEID register.
*/

#define HWREAD_Z1_OTP_BOOT_GPREG2_KEY            ((HWREG(Z1_OTP_BOOT_GPREG2) & (uint32_t)0xFF000000UL) >> 24U)
#define HWREAD_Z2_OTP_BOOT_GPREG2_KEY            ((HWREG(Z2_OTP_BOOT_GPREG2) & (uint32_t)0xFF000000UL) >> 24U)

#define ERRORSTS_OTP_VALUE_Z1              ((HWREG(Z1_OTP_BOOT_GPREG2) & (uint32_t)0x00000030UL) >> 4U)
#define ERRORSTS_OTP_VALUE_Z2              ((HWREG(Z2_OTP_BOOT_GPREG2) & (uint32_t)0x00000030UL) >> 4U)
#define ERRORSTS_PIN_NUMBER_1              24UL
#define ERRORSTS_PIN_NUMBER_2              28UL
#define ERRORSTS_PIN_NUMBER_3              29UL
#define ERRORSTS_PIN_NUMBER_INVALID        0xFFFFFFFFUL

#define ERRORSTS_PIN_24                    0x0UL
#define ERRORSTS_PIN_28                    0x1UL
#define ERRORSTS_PIN_29                    0x2UL

#define GPREG2_KEY                         0x5AU

#define GPREG2_PBIST_RUN_PLL_BYPASS        0x0UL
#define GPREG2_PBIST_RUN_SYSCLK_115MHZ     0x1UL
#define GPREG2_PBIST_RUN_SYSCLK_57_5MHZ    0x2UL
#define GPREG2_PBIST_DISABLED              0x3UL

#define PBIST_CHECKSUM_SUCCESS             0x0U

//
// Get key to validate Z1 OTP BOOTPIN_CONFIG
//
#define HWREAD_Z1_OTP_BOOTPIN_CONFIG_KEY        ((HWREG(Z1_OTP_BOOTPIN_CONFIG) & (uint32_t)0xFF000000UL) >> 24U)
#define HWREAD_Z2_OTP_BOOTPIN_CONFIG_KEY        ((HWREG(Z2_OTP_BOOTPIN_CONFIG) & (uint32_t)0xFF000000UL) >> 24U)

//
// Standalone macros to extract boot definition from BOOTDEF table at
// specified index
//
#define HWREAD_Z1_OTP_BOOTDEF_L(x)             ((HWREG(Z1_OTP_BOOTDEF_LOW)  & ((uint32_t)0xFFUL << (8UL*(x)))) >> (8UL*(x)))
#define HWREAD_Z1_OTP_BOOTDEF_H(x)             ((HWREG(Z1_OTP_BOOTDEF_HIGH) & ((uint32_t)0xFFUL << (8UL*(x)))) >> (8UL*(x)))
#define HWREAD_Z2_OTP_BOOTDEF_L(x)             ((HWREG(Z2_OTP_BOOTDEF_LOW)  & ((uint32_t)0xFFUL << (8UL*(x)))) >> (8UL*(x)))
#define HWREAD_Z2_OTP_BOOTDEF_H(x)             ((HWREG(Z2_OTP_BOOTDEF_HIGH) & ((uint32_t)0xFFUL << (8UL*(x)))) >> (8UL*(x)))

//#define EMU_BOOTDEF0              ((HWREG(EMU_BOOTDEF_LOW) & 0xFF) & 0x1F)
//#define EMU_BOOTDEF0_ALT_OPTIONS  (((HWREG(EMU_BOOTDEF_LOW) & 0xFF) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF1              (((HWREG(EMU_BOOTDEF_LOW) & 0xFF00)>>8) & 0x1F)
//#define EMU_BOOTDEF1_ALT_OPTIONS  ((((HWREG(EMU_BOOTDEF_LOW) & 0xFF00)>>8) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF2              (((HWREG(EMU_BOOTDEF_LOW) & (uint32_t)0xFF0000) >> 16) & 0x1F)
//#define EMU_BOOTDEF2_ALT_OPTIONS  ((((HWREG(EMU_BOOTDEF_LOW) & (uint32_t)0xFF0000) >> 16) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF3              (((HWREG(EMU_BOOTDEF_LOW) & (uint32_t)0xFF000000) >> 24) & 0x1F)
//#define EMU_BOOTDEF3_ALT_OPTIONS  ((((HWREG(EMU_BOOTDEF_LOW) & (uint32_t)0xFF000000) >> 24) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF4              ((HWREG(EMU_BOOTDEF_HIGH) & 0xFF) & 0x1F)
//#define EMU_BOOTDEF4_ALT_OPTIONS  (((HWREG(EMU_BOOTDEF_HIGH) & 0xFF) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF5              (((HWREG(EMU_BOOTDEF_HIGH) & 0xFF00)>>8) & 0x1F)
//#define EMU_BOOTDEF5_ALT_OPTIONS  ((((HWREG(EMU_BOOTDEF_HIGH) & 0xFF00)>>8) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF6              (((HWREG(EMU_BOOTDEF_HIGH) & (uint32_t)0xFF0000) >> 16) & 0x1F)
//#define EMU_BOOTDEF6_ALT_OPTIONS  ((((HWREG(EMU_BOOTDEF_HIGH) & (uint32_t)0xFF0000) >> 16) & 0xE0) >> 5)
//
//#define EMU_BOOTDEF7              (((HWREG(EMU_BOOTDEF_HIGH) & (uint32_t)0xFF000000) >> 24) & 0x1F)
//#define EMU_BOOTDEF7_ALT_OPTIONS  ((((HWREG(EMU_BOOTDEF_HIGH) & (uint32_t)0xFF000000) >> 24) & 0xE0) >> 5)


#define FACTORY_DEFAULT_BMSP0       32      //GPIO32
#define FACTORY_DEFAULT_BMSP1       24      //GPIO24

#define PARALLEL_BOOT           0x00U
#define PARALLEL_BOOT_ALT1      0x20U

#define SCI_BOOT                0x01U   //GPIO29; GPIO28 (CCARD)
#define SCI_BOOT_ALT1           0x21U   //GPIO16; GPIO17
#define SCI_BOOT_ALT2           0x41U   //GPIO8;  GPIO9
#define SCI_BOOT_ALT3           0x61U   //GPIO2;  GPIO3
#define SCI_BOOT_ALT4           0x81U   //GPIO16; GPIO3

#define CAN_BOOT                0x02U   //GPIO4; GPIO5
#define CAN_BOOT_ALT1           0x22U   //GPIO32; GPIO33
#define CAN_BOOT_ALT2           0x42U   //GPIO2; GPIO3
#define CAN_BOOT_ALT3           0x62U   //GPIO13, GPIO12
#define CAN_BOOT_SENDTEST       0x82U   //GPIO4; GPIO5
#define CAN_BOOT_ALT1_SENDTEST  0xA2U   //GPIO32; GPIO33
#define CAN_BOOT_ALT2_SENDTEST  0xC2U   //GPIO2; GPIO3
#define CAN_BOOT_ALT3_SENDTEST  0xE2U   //GPIO13, GPIO12

#define FLASH_BOOT              0x03U   //BANK0 sector 0
#define FLASH_BOOT_ALT1         0x23U   //BANK0 sector 8
#define FLASH_BOOT_ALT2         0x43U   //BANK0 end of sector 15
#define FLASH_BOOT_ALT3         0x63U   //BANK1 sector 0
#define FLASH_BOOT_ALT4         0x83U   //BANK1 end of sector 7
#define FLASH_BOOT_ALT5         0xA3U   //BANK1 end of sector 15
#define FLASH_BOOT_ALT6         0xC3U   //BANK2 sector 0
#define FLASH_BOOT_ALT7         0xE3U   //BANK2 end of sector 15

#define WAIT_BOOT               0x04U   //with WDOG enabled
#define WAIT_BOOT_ALT1          0x24U   //without WDOG enabled

#define RAM_BOOT                0x05U

#define SPI_MASTER_BOOT         0x06U   //GPIO 2,1,3,5
#define SPI_MASTER_BOOT_ALT1    0x26U   //GPIO 16,1,3,0
#define SPI_MASTER_BOOT_ALT2    0x46U   //GPIO 8,10,9,11
#define SPI_MASTER_BOOT_ALT3    0x66U   //GPIO 8,17,9,11

#define I2C_MASTER_BOOT         0x07U   //GPIO32(SDA), GPIO33(SCL)
#define I2C_MASTER_BOOT_ALT1    0x27U   //GPIO0 (SDA), GPIO1 (SCL)
#define I2C_MASTER_BOOT_ALT2    0x47U   //GPIO10(SDA), GPIO8 (SCL)

#define MCAN_BOOT                0x08U   //GPIO4 (Tx); GPIO5 (Rx)
#define MCAN_BOOT_ALT1           0x28U   //GPIO1 (Tx); GPIO0 (Rx)
#define MCAN_BOOT_ALT2           0x48U   //GPIO13 (Tx); GPIO12 (Rx)
#define MCAN_BOOT_SENDTEST       0x68U   //GPIO4 (Tx); GPIO5 (Rx)
#define MCAN_BOOT_ALT1_SENDTEST  0x88U   //GPIO0 (Tx); GPIO1 (Rx)
#define MCAN_BOOT_ALT2_SENDTEST  0xA8U   //GPIO13 (Tx); GPIO12 (Rx)

//
// Secure Flash Boot Options
//
#define SECURE_FLASH_BOOT               0x0AU    //BANK0 sector 0
#define SECURE_FLASH_BOOT_ALT1          0x2AU    //BANK0 sector 8
#define SECURE_FLASH_BOOT_ALT2          0x4AU    //BANK0 end of sector 15
#define SECURE_FLASH_BOOT_ALT3          0x6AU    //BANK1 sector 0
#define SECURE_FLASH_BOOT_ALT4          0x8AU    //BANK1 end of sector 7
#define SECURE_FLASH_BOOT_ALT5          0xAAU    //BANK1 end of sector 15
#define SECURE_FLASH_BOOT_ALT6          0xCAU    //BANK2 sector 0

//
// LFU Flash Boot Options
//
#define LFU_FLASH_BOOT                  0x0BU
#define LFU_FLASH_BOOT_ALT1             0x2BU
#define LFU_FLASH_BOOT_ALT2             0x4BU
#define LFU_FLASH_BOOT_ALT3             0x6BU
#define LFU_FLASH_BOOT_ALT4             0x8BU

//
// Secure LFU Flash Boot Options
//
#define SECURE_LFU_FLASH_BOOT           0x0CU
#define SECURE_LFU_FLASH_BOOT_ALT1      0x2CU
// LFU ALT2 intentionally excluded as CMAC requires entry address + 16kb
#define SECURE_LFU_FLASH_BOOT_ALT3      0x6CU
#define SECURE_LFU_FLASH_BOOT_ALT4      0x8CU

#define BROM_ASYSCTL_O_PMMREFTRIM   0x12U       //Power Management Module Reference Trim Register
#define BROM_ASYSCTL_O_PMMVREGTRIM  0x14U       //Power Management Module VREG Trim Register
#define BROM_ASYSCTL_O_PMMVMONTRIM  0x0EU       //Power Management Module VMON Trim Register
#define BROM_ASYSCTL_O_PMMVMONTRIM2 0x10U       //Power Management Module VMON Trim Register

#define BROM_ASYSCTL_O_OSCREFTRIM   0x06U       // OSC IREF,BGSLOPE,BGVAL TRIM
#define BROM_ASYSCTL_O_OSC1_TRIM    0x00U       // INTOSC1 SLOPE, VALFINE TRIMs
#define BROM_ASYSCTL_O_OSC2_TRIM    0x02U       // INTOSC2 SLOPE, VALFINE TRIMs


//---------------------------------------------------------------------------
// Fixed boot entry points:
//
#define FLASH_ENTRY_POINT               0x00080000UL    //BANK0 sector 0
#define FLASH_ENTRY_POINT_ALT1          0x00088000UL    //BANK0 sector 8
#define FLASH_ENTRY_POINT_ALT2          0x0008FFF0UL    //BANK0 end of sector 15
#define FLASH_ENTRY_POINT_ALT3          0x00090000UL    //BANK1 sector 0
#define FLASH_ENTRY_POINT_ALT4          0x00097FF0UL    //BANK1 end of sector 7
#define FLASH_ENTRY_POINT_ALT5          0x0009FFF0UL    //BANK1 end of sector 15
#define FLASH_ENTRY_POINT_ALT6          0x000A0000UL    //BANK2 sector 0
#define FLASH_ENTRY_POINT_ALT7          0x000AFFF0UL    //BANK2 end of sector 15

#define HWREAD_FLASH_ENTRY_POINT_OVERRIDE_KEY  (HWREGH(0x70257UL))
#define HWREAD_FLASH_ENTRY_POINT_OVERRIDE_ADDR (HWREG(0x7025EUL)) 

#define RAM_ENTRY_POINT                 0x000000U    //M0 start address

#define LFU_FLASH_ENTRY_POINT_BANK0          0x00080000UL
#define LFU_FLASH_ENTRY_POINT_BANK1          0x00090000UL
#define LFU_FLASH_ENTRY_POINT_BANK2          0x000A0000UL

#define LFU_FLASH_ENTRY_POINT_ALT1_BANK0     0x00088000UL
#define LFU_FLASH_ENTRY_POINT_ALT1_BANK1     0x00098000UL
#define LFU_FLASH_ENTRY_POINT_ALT1_BANK2     0x000A8000UL

#define LFU_FLASH_ENTRY_POINT_ALT2_BANK0     0x0008FFF0UL
#define LFU_FLASH_ENTRY_POINT_ALT2_BANK1     0x0009FFF0UL
#define LFU_FLASH_ENTRY_POINT_ALT2_BANK2     0x000AFFF0UL

#define LFU_FLASH_ENTRY_POINT_ALT3_BANK0     0x00088000UL
#define LFU_FLASH_ENTRY_POINT_ALT3_BANK1     0x00090000UL
#define LFU_FLASH_ENTRY_POINT_ALT3_BANK2     0x000A0000UL

#define LFU_FLASH_ENTRY_POINT_ALT4_BANK0     0x0008EFF0UL
#define LFU_FLASH_ENTRY_POINT_ALT4_BANK1     0x00097FF0UL
#define LFU_FLASH_ENTRY_POINT_ALT4_BANK2     0x000A7FF0UL

#define ERROR                   1U
#define NO_ERROR                0U

#define BROM_EIGHT_BIT_HEADER           0x08AAU

//---------------------------------------------------------------------------
//
#define HWREAD_TI_OTP_PARTID_L          (HWREG(0x70240))
#define HWREAD_TI_OTP_PARTID_H          (HWREG(0x70242))

#define TI_OTP_ADDR_DC01                (0x70245UL)
#define TI_OTP_ADDR_DC11                (0x70246UL)
#define TI_OTP_ADDR_DC18                (0x70247UL)
#define TI_OTP_ADDR_DC20                (0x70248UL)
#define TI_OTP_ADDR_DC21                (0x70249UL)
#define TI_OTP_ADDR_DC29                (0x7024AUL)

#define TI_OTP_REG_VREGCTL_ENMASK       (HWREGH(0x7028CUL))

#define TI_OTP_CPUROM_DC1_ADDR          (0x7024EUL)
#define TI_OTP_CPUROM_DC2_ADDR          (0x7024FUL)
#define TI_OTP_CPUROM_DC3_ADDR          (0x70250UL)
#define TI_OTP_CPUROM_DC4_ADDR          (0x70251UL)

#define TI_OTP_CLAROM_DC1_ADDR          (0x70252UL)
#define TI_OTP_CLAROM_DC2_ADDR          (0x70253UL)
#define TI_OTP_CLAROM_DC3_ADDR          (0x70254UL)
#define TI_OTP_CLAROM_DC4_ADDR          (0x70255UL)


#define BROM_DCX_ALWAYS_ENABLED         0xFFFFFFFFUL
#define BROM_DCX_ENABLE_HIGH            0xFFFF0000UL
#define BROM_DCX_ENABLE_LOW             0x0000FFFFUL

#define HWREAD_TI_OTP_PKG_TYPE          (HWREGH(0x70244UL))
#define PKG_TYPE_KEY                    0x5AU

#define BROM_ANALOG_SYSCTL_O_VREGCTL    0x006AU
#define VREGCTL_ENMASK_KEY              0x5AU

//bits15:8 is the KEY ; if Value == 0x5A then the remaining bits are valid
//bits 7:2 => reserved
//bits 0:1 if set to b'00 BROM will program 0x01 in VREGCTL.ENMASK
//          - any other value the VREGCTL.ENMASK will be left at reset state.

#define TI_OTP_REG_VREGCTL_ENMASK_VAL   ((TI_OTP_REG_VREGCTL_ENMASK) & 0x03U)

#define TI_OTP_REG_VREGCTL_ENMASK_KEY   (((TI_OTP_REG_VREGCTL_ENMASK) & 0xFF00U) >> 0x8U)


#define TI_OTP_SECDC                    0x703F0

//---------------------------------------------------------------------------
//


typedef uint16_t (*uint16fptr)(void);
extern  uint16fptr GetWordData;

#define HWREAD_TI_OTP_DEVCAL_KEY        (HWREGH(0x70271UL))

#define HWREAD_OTP_VERSION_FOR_BOOT     (HWREGH(0x7028BUL))

//Bits [1:0]    If 01, enable the PLL, otherwise leave it disabled
//Bits [7:2]    PLL divider to use when the PLL is enabled
//Bits [31:24]  If 0x5A, use this configuration word, otherwise use the default settings
#define OTP_BOOT_CONFIGURE_WORD_ADDRESS      0x703EEUL
#define BOOT_CONFIGURE_ENABLE_PLL            0x1U
//Bit 23 is used for enabling watchdog monitoring during boot time
//if the bit is 1 then it means the watchdog monitoring is enabled
#define BOOT_CONFIG_ENABLE_WD_S         23U
#define OTP_BOOT_CONFIG_M               0x000000FCUL
#define OTP_BOOT_CONFIG_S               0x2U

#define BOOTPIN_CONFIG_STANDALONE_KEY   0xA5U
#define BOOTPIN_CONFIG_KEY              0x5AUL

#define BOOTPIN_COFIG_KEY_MASK          0xFF000000UL
#define BOOTPIN_COFIG_KEY_START         24U

#define APLL_MULT_46                    46UL
#define APLL_ODIV_4                     3UL

#define SYSCLK_DIV_1                    0UL
#define SYSCLK_DIV_2                    1UL
#define SYSCLK_DIV_4                    2UL

#define CAN_BOOT_DEFAULT_BIT_TIMING     0x0U
#define CAN_BOOT_USE_XTAL               0x1U
#define MCAN_BOOT_XTAL_FREQ             20U   // 20MHz
#define MCAN_BOOT_MCAN_DIVIDER          0U    // divide by 1


//
// APLL Trim - Analog Subsystem Register Offsets
//
// Refer to hw_asysctl.h

//
// APLL OTP Trim Values
//
#define HWREAD_TI_OTP_APLL_TRIM_KEY            (HWREGH(0x7026CUL))
#define TI_OTP_APLLREF_TRIM_ADDRESS            0x7027EUL
#define TI_OTP_APLL_SYS_TRIM_ADDRESS           0x70280UL

//
//Bits 15-0 of APLLREFTRIM are used for trim values
//
#define APLL_REF_TRIM_MASK                     0xFFFFU

//
//Bits 7-0 of APLLDOTRIM are used for trim values
//
#define APLL_LDO_TRIM_MASK                     0xFFU

//
//Bits 2-0 of APLLOSCRIM are used for trim values
//
#define APLL_OSC_TRIM_MASK                     0x7U

//
// APLL OTP Trim Masks/Shifts
//
#define TI_OTP_APLL_OSC_FREQ_MASK       0x700U
#define TI_OTP_APLL_OSC_FREQ_SHIFT      0x8U
#define TI_OTP_APLL_OSC_VDD_MASK        0xFFU

//
// ADC - TRIM key and pointer
//
#define HWREAD_TI_OTP_ADC_TRIM_KEY      (HWREGH(0x7026EUL))
#define TI_OTP_ADC_TRIM_ADDRESS         0x70282UL

//
// DAC - TRIM key and pointer
//
#define HWREAD_TI_OTP_DAC_TRIM_KEY      (HWREGH(0x7026FUL))
#define TI_OTP_DAC_A_TRIM_ADDRESS       0x70284UL
#define TI_OTP_DAC_B_TRIM_ADDRESS       0x70286UL

//
// Flash - TRIM key and pointer
//
#define HWREAD_TI_OTP_FLASH_TRIM_KEY    (HWREGH(0x70270UL))
#define TI_OTP_FLASH_TRIM_ADDRESS       0x70288UL
#define REFSYSTRIM_MASK                 0xFFFFU
#define REFSYS2TRIM_OTP_MASK            0xFFFF0000UL
#define REFSYS2TRIM_OTP_SHIFT           16U

//
// APLL Lock - Analog Subsystem Register Offset
//
// Refer to hw_asysctl.h

#define APLL_CONFIG_LOCK                0x40009FFFUL

#define OTP_BOOT_ESCAPE_TABLE_END              0x703ECUL

//extern void otp_func_refs();
#define CPU1BROM_TI_OTP_ESCAPE_POINT_15         (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-28U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_14         (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-26U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_13         (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-24U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_12         (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-22U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_11         (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-20U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_10         (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-18U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_9          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-16U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_8          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-14U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_7          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-12U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_6          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-10U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_5          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-8U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_4          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-6U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_3          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-4U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_2          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-2U))
#define CPU1BROM_TI_OTP_ESCAPE_POINT_1          (HWREG((OTP_BOOT_ESCAPE_TABLE_END)-0U))

//
// Boot Selection Defines
//
#define CPU1_ALL_BMSP_DISABLED_MASK     0x00FFFFFFUL
#define CPU1_PIN_CONFIG_MASK            0xFFU
#define CPU1_BMSP_DISABLED              0xFFU

//
// OTP Keys
//
#define TI_OTP_KEY                              0x5A5AU

//
// SIMRESET Key
//
#define SIMRESET_KEY                            0xA5A5UL

//
// PMM Trim - Analog Subsystem Register Offsets
//
// Refer to hw_asysctl.h

// //
// // PMM OTP Trim Values
// //
#define TI_OTP_PMM_LC_TRIM_KEY                  (HWREGH(0x70268))
#define TI_OTP_PMM_REF_TRIM_ADDRESS             0x70272UL

#define TI_OTP_PMM_VMON_TRIM_KEY                (HWREGH(0x70269))
#define TI_OTP_PMM_VMON_TRIM_ADDRESS            0x70274UL

//
//Bits 15-0 are used for PMM REF trims
//
#define PMM_REF_TRIM_MASK                       0xFFFFU

//
//Bits 15-0 are used for PMM VMON trims
//
#define PMM_VMON_TRIM_MASK                      0xFFFFU

//
//Bits 7-0 are used for PMM VREG trim
//
#define PMM_VREG_TRIM_MASK                      0xFFU


//
// PMM VREG Trim Values
//
#define TI_OTP_PMM_VREG_TRIM_KEY                (HWREGH(0x7026A))
#define TI_OTP_PMM_VREG_TRIM_ADDRESS             0x70276UL

//
// Internal Oscillator OTP Trim Values
//
#define HWREAD_TI_OTP_INTOSC_TRIM_KEY           (HWREGH(0x7026B))
#define TI_OTP_OSC_REF_ADDRESS                  0x70278UL
#define TI_OTP_INTOSC1_ADDRESS                  0x7027AUL
#define TI_OTP_INTOSC2_ADDRESS                  0x7027CUL

//
//Bits 15-0 of OSCREFTRIM are reserved for Trims
//
#define OSC_REF_TRIM_MASK                       0xFFFFU

//
//Bits 23-16 and 11-0 of INTOSC1TRIM are reserved for trims
//
#define OSC1_TRIM_MASK                          0x00FF0FFFUL

//
//Bits 23-16 and 11-0 of INTOSC2TRIM are reserved for trims
//
#define OSC2_TRIM_MASK                          0x00FF0FFFUL


#define HWREAD_TI_OTP_GPXPUD_KEY                (HWREGH(0x70256UL))
#define HWREAD_TI_OTP_GPA_PUD_CONFIG            (HWREG(0x7025AUL))
#define HWREAD_TI_OTP_GPB_PUD_CONFIG            (HWREG(0x7025CUL))

//
//Masks for writes to GPA and GPB registers
//to pull up unbonded pins
//
#define GPA_WRITE_MASK                          0xFFFFFFFFUL
#define GPB_WRITE_MASK                          0xFFFFFFFFUL


#define DCSM_O_Zx_EXEONLYRAM        0x0UL
#define DCSM_O_Zx_EXEONLYSECT       0x2UL
#define DCSM_O_Zx_GRABRAM           0x4UL
#define DCSM_O_Zx_GRABSECT          0x6UL

//
// CPU1 Boot ROM Status Bit Fields
//
#define CPU1_BOOTROM_BOOTSTS_BOOT_MASK              0x000000FFUL

//
// CPU1 Boot mode status bit field starts from 0 and ends at 7
// It can take any values in the range 0 to 0xFF
//
#define CPU1_BOOTROM_BOOTSTS_SYSTEM_START_BOOT          0x00000001UL  //Set during the initialization phase of the boot ROM
#define CPU1_BOOTROM_BOOTSTS_IN_FLASH_BOOT              0x00000002UL
#define CPU1_BOOTROM_BOOTSTS_IN_SECURE_FLASH_BOOT       0x00000003UL
#define CPU1_BOOTROM_BOOTSTS_IN_PARALLEL_BOOT           0x00000004UL
#define CPU1_BOOTROM_BOOTSTS_IN_RAM_BOOT                0x00000005UL
#define CPU1_BOOTROM_BOOTSTS_IN_SCI_BOOT                0x00000006UL
#define CPU1_BOOTROM_BOOTSTS_IN_SPI_BOOT                0x00000007UL
#define CPU1_BOOTROM_BOOTSTS_IN_I2C_BOOT                0x00000008UL
#define CPU1_BOOTROM_BOOTSTS_IN_CAN_BOOT                0x00000009UL
#define CPU1_BOOTROM_BOOTSTS_IN_MCAN_BOOT               0x0000000AUL
#define CPU1_BOOTROM_BOOTSTS_IN_WAIT_BOOT               0x0000000BUL
#define CPU1_BOOTROM_BOOTSTS_IN_LFU_FLASH_BOOT          0x0000000CUL
#define CPU1_BOOTROM_BOOTSTS_IN_SECURE_LFU_FLASH_BOOT   0x0000000DUL

#define CPU1_BOOTROM_RAM_INIT_COMPLETE              0x00000100UL
#define CPU1_BOOTROM_DCSM_INIT_COMPLETE             0x00000200UL
#define CPU1_BOOTROM_POR_MEM_TEST_COMPLETE          0x00000400UL
#define CPU1_BOOTROM_RESC_HANDLED                   0x00000800UL
#define CPU1_BOOTROM_HANDLED_XRSN                   0x00001000UL
#define CPU1_BOOTROM_HANDLED_POR                    0x00002000UL
#define CPU1_BOOTROM_WATCHDOG_SELFTEST_FAIL         0x00004000UL
#define CPU1_BOOTROM_BOOT_COMPLETE                  0x00008000UL

#define CPU1_BOOTROM_GOT_ITRAP                      0x00010000UL
#define CPU1_BOOTROM_GOT_A_PIE_MISMATCH             0x00020000UL
#define CPU1_BOOTROM_GOT_AN_ERAD_NMI                0x00040000UL
#define CPU1_BOOTROM_GOT_A_RL_NMI                   0x00080000UL
#define CPU1_BOOTROM_GOT_A_FLASH_UNCERR_NMI         0x00100000UL
#define CPU1_BOOTROM_GOT_A_RAM_UNCERR_NMI           0x00200000UL
#define CPU1_BOOTROM_GOT_A_MCLK_NMI                 0x00400000UL
#define CPU1_BOOTROM_GOT_A_HWBIST_NMI               0x00800000UL

#define BOOTROM_PLL_ENABLE_SUCCESS                  0x01000000UL
#define CPU1_BOOTROM_DCSM_INIT_INVALID_LP           0x02000000UL
#define CPU1_BOOTROM_DCSM_INIT_LP_ERROR_SET         0x04000000UL
#define CPU1_BOOTROM_FLASH_VERIFICATION_ERROR       0x08000000UL
#define CPU1_BOOTROM_LFU_FLASH_BOOT_ERROR           0x10000000UL
#define CPU1_BOOTROM_EFUSE_SINGLE_BIT_ERROR         0x20000000UL
#define BOOTROM_HANDLED_HWBIST                      0x80000000UL
//
// BootROM System Clock (10MHz)
//
#define BOOTROM_SYSCLK                 10000000UL

#define BROM_FLASH_ALL_START        0x80000UL
#define BROM_FLASH_ALL_END          0xB0000UL

//
//Mask for NMI enable bit (bit 0)
//
#define NMI_ENABLE_MASK             0x1U


//
// Watchdog prescaler and prediv values
//
//
// setting prescale and prediv values to get timeout of 104.8 ms
//
#define WD_PRESCALE_VALUE SYSCTL_WD_PRESCALE_8
#define WD_PREDIV_VALUE   SYSCTL_WD_PREDIV_512
//
// the reset values of prescalar and prediv
//
#define WD_DEF_PRESCALE_VALUE SYSCTL_WD_PRESCALE_1
#define WD_DEF_PREDIV_VALUE   SYSCTL_WD_PREDIV_512
//
// min values of prediv and prescale values so as to ensure maximum speed for watchdog clk
//
#define WD_PRESCALE_VALUE_MIN SYSCTL_WD_PRESCALE_1
#define WD_PREDIV_VALUE_MIN   SYSCTL_WD_PREDIV_2

//Function prototypes
extern void cbrom_configure_flash(void);
extern uint32_t Gather_Bx_Zx_ZSB(uint16_t bank, uint16_t zone, uint32_t *csmkey);

extern interrupt void CPU1BROM_itrapISR(void);
extern interrupt void CPU1BROM_nmiHandler(void);

extern uint32_t GetLongData(void);
extern void CopyData(void);
extern void ReadReservedFn(void);

extern uint32_t I2C_Boot(uint32_t  bootMode);
extern uint32_t SCI_Boot(uint32_t  bootMode);
extern uint32_t SPI_Boot(uint32_t  bootMode);
uint32_t DCAN_Boot(uint32_t bootMode, uint32_t bitTimingRegValue, uint16_t switchToXTAL);
uint32_t MCAN_Boot(uint32_t bootMode, uint32_t nominalBitTiming,
                   uint32_t dataBitTiming, uint16_t deviceSystemFreq,
                   uint16_t mcanDivider, uint16_t clockSelection);
extern uint32_t SPI_Alternate_IO_Boot(void);
extern uint32_t Parallel_Boot(uint32_t  BootMode);
extern uint32_t performLFUBankSelection(uint32_t bootMode);

extern uint32_t CPU1BROM_selectBootMode(void);

extern void CPU1BROM_triggerSysPLLLock(uint32_t multiplier, uint32_t divider);
extern uint16_t CPU1BROM_switchToPLL(void);
extern uint32_t PBIST_PORMemoryTest(void);
extern void CPU1BROM_devcalInit(void);
extern bool CPU1BROM_bootROMTest(void);

#endif //C_BOOTROM_H_
