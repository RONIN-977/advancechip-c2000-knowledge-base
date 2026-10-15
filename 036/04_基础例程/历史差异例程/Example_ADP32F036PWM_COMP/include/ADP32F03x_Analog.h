//###########################################################################
//
// FILE:   ADP32F03x_Analog.h
//
// TITLE: ADP32F03x Device Analog trim Register Definitions.
//
//###########################################################################

#ifndef ADP32F03X_ANALOG_H_
#define ADP32F03X_ANALOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------- */
/* Analog trim & flash status Registers                     */
/* ----------------------------------------------------*/

struct  PIN_MUX_FLASH_BITS {
    Uint16      ecan_gpio_mux:4;        //0_3   ecan pins select
    Uint16      canfd_gpio_mux:4;       //4_7   canfd pins select
    Uint16      rsvd1:6;                //8_13  reserved
    Uint16      flash_RCELL:1;          //14    flash IP RECELL pins control,must be set to 0
    Uint16      trim_succ:1;            //15    finish flash trim flag,read only
};

union PIN_MUX_FLASH_REGS {
    Uint16                      all;
    struct PIN_MUX_FLASH_BITS   bit;
};

struct  TSPG1TRIM_BITS {
    Uint16      PGA1_OFFTRIM:8;     //0_7   PGA1 offset trim
    Uint16      TSENSOR_OFFTRIM:8;  //8_15  TENSOR offset trim
};

union TSPG1TRIM_REGS {
    Uint16              all;
    struct TSPG1TRIM_BITS  bit;
};

struct  OPPGTRIM_BITS {
    Uint16      PGA3_VOFTRIM:3;     //0_2   PGA3 offset trim(analog)
    Uint16      PGA2_VOFTRIM:3;     //3_5   PGA2 offset trim(analog)
    Uint16      PGA1_VOFTRIM:3;     //6_8   PGA1 offset trim(analog)
    Uint16      PGA_IB_TRIM:2;      //9_10  PGA current trim
    Uint16      OPA_VOFTRIM:3;      //11_13 OPA offset trim(analog)
    Uint16      OPA_IB_TRIM:2;      //14_15 OPA current trim
};

union OPPGTRIM_REGS {
    Uint16                  all;
    struct OPPGTRIM_BITS    bit;
};

struct  BGTRIM_BITS {
    Uint16      LDO12_CORE_TRIM:3;      //0_2   1.2V VDD LDO trim
    Uint16      LDO12_CLK_TRIM:3;       //3_5   1.2V VDDCLK LDO trim
    Uint16      LDO30_TRIM:6;           //6_11  3.0V VDDA LDO trim
    Uint16      BG_TEMP_TRIM:4;         //12_15 BG temperature trim
};

union BGTRIM_REGS {
    Uint16              all;
    struct BGTRIM_BITS  bit;
};

struct  PG23TRIM_BITS {
    Uint16      PGA3_OFFTRIM:8;     //0_7   PGA3 offset trim
    Uint16      PGA2_OFFTRIM:8;     //8_15  PGA2 offset trim
};

union PG23TRIM_REGS {
    Uint16                  all;
    struct PG23TRIM_BITS    bit;
};

struct  ANEN_BITS {
    Uint16      PGA3_GAIN:3;        //0_2   PGA3 gain select
    Uint16      rsvd1:1;            //3     reserved
    Uint16      PGA2_GAIN:3;        //4_6   PGA2 gain select
    Uint16      rsvd2:1;            //7     reserved
    Uint16      PGA1_GAIN:3;        //8_10  PGA1 gain select
    Uint16      rsvd3:1;            //11    reserved
    Uint16      PGA3EN:1;           //12    PGA3 enable
    Uint16      PGA2EN:1;           //13    PGA2 enable
    Uint16      PGA1EN:1;           //14    PGA1 enable
    Uint16      OPAEN:1;            //15    OPA enable
};

union ANEN_REGS {
    Uint16              all;
    struct ANEN_BITS    bit;
};

struct  ANALOG_REGS {
    union PIN_MUX_FLASH_REGS    PIN_MUX_FLASH; // 0xA0£¬GPIO29¡¢GPIO28Òý½Å PINMUXÎª TX¡¢RX/ 0x50£¬GPIO32¡¢GPIO0 ½Å PINMUX Îª TX¡¢RX
    union TSPG1TRIM_REGS        TSPA1TRIM;
    union OPPGTRIM_REGS         OPPGTRIM;
    union BGTRIM_REGS           BGTRIM;
    union PG23TRIM_REGS         PG23TRIM;
    union ANEN_REGS             ANEN;
};

extern volatile struct ANALOG_REGS AnalogRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* HAL_INC_ADP32F03X_ANALOG_H_ */
