//###########################################################################
//
// FILE:   ADP32F03x_Canfd.h
//
// TITLE:   ADP32F03x Device CANFD Register Definitions.

//###########################################################################

#ifndef  ADP32F03X_CANFD_H_
#define  ADP32F03X_CANFD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------- */
/* CANFD Control & Status Registers                     */
/* ----------------------------------------------------*/
struct  RID0_BITS {      // bit  description
   Uint16      ID0:11;     // 0_10  ID effective signal bits 0:10
   Uint16      ID1:5;      //11_15  ID effective signal bits 11:15 for extended frame;reserved for standard frame
};

/* Allow access to the bit fields or entire register */
union RID0_REG {
   Uint16             all;
   struct RID0_BITS  bit;
};

struct  RID1_BITS {
    Uint16      ID:13;      //0_12  ID effective signal bits 16:28 for extended frame
    Uint16      rsvd1:2;    // 13_14    reserved
    Uint16      ESI:1;      //15
};

/* Allow access to the bit fields or entire register */
union RID1_REG {
    Uint16              all;
    struct RID1_BITS    bit;
};

struct  RIDST_BITS {
    Uint16      DLC:4;      //0_3   data length control
    Uint16      BRS:1;      //4     bit rate switch
    Uint16      FDF_EDL:1;  //5     0:CAN2.0;1:CANFD
    Uint16      RTR:1;      //6     remote transmission request; 0:standard mode;1:remote mode
    Uint16      IDE:1;      //7     ID extended;0:standard ID;1:extended ID
    Uint16      rsvd1:4;    //8_11  reserved
    Uint16      TX:1;       //12
    Uint16      KOER:3;     //13_15 type error
};

union RIDST_REG {
    Uint16              all;
    struct RIDST_BITS   bit;
};

struct  RCYCLE_TIME_BITS {
    Uint16      CYCLE_TIMER:16; //0_15  cycle timer register of SOF
};

union RCYCLE_TIME_REG {
    Uint16                  all;
    struct RCYCLE_TIME_BITS  bit;
};

struct  CANFD_RBUF_REGS {
    union RID0_REG      RID0;
    union RID1_REG      RID1;
    union RIDST_REG     RIDST;
    union RCYCLE_TIME_REG    RCYCLE_TIME;
    Uint16  DATA[32];
    Uint16  RTS01;
    Uint16  RTS23;
    Uint16  RTS45;
    Uint16  RTS67;
};

struct  TID0_BITS {      // bit  description
   Uint16      ID0:11;     // 0_10  ID effective signal bits 0:10
   Uint16      ID1:5;      //11_15  ID effective signal bits 11:15 for extended frame;reserved for standard frame
};

/* Allow access to the bit fields or entire register */
union TID0_REG {
   Uint16             all;
   struct TID0_BITS  bit;
};

struct  TID1_BITS {
    Uint16      ID:13;      //0_12  ID effective signal bits 16:28 for extended frame
    Uint16      rsvd1:2;    // 13_14    reserved
    Uint16      ESI:1;      //15
};

/* Allow access to the bit fields or entire register */
union TID1_REG {
    Uint16              all;
    struct TID1_BITS    bit;
};

struct  TIDST_BITS {
    Uint16      DLC:4;      //0_3   data length control
    Uint16      BRS:1;      //4     bit rate switch
    Uint16      FDF_EDL:1;  //5     0:CAN2.0;1:CANFD
    Uint16      RTR:1;      //6     remote transmission request; 0:standard mode;1:remote mode
    Uint16      IDE:1;      //7     ID extended;0:standard ID;1:extended ID
    Uint16      rsvd1:8;    //8_15  reserved
};

union TIDST_REG {
    Uint16              all;
    struct TIDST_BITS   bit;
};

struct  TCYCLE_TIME_BITS {
    Uint16      CYCLE_TIMER:16; //0_15  cycle timer register of SOF
};

union TCYCLE_TIME_REG {
    Uint16                  all;
    struct TCYCLE_TIME_BITS  bit;
};

struct  CANFD_TBUF_REGS {
    union TID0_REG      TID0;
    union TID1_REG      TID1;
    union TIDST_REG     TIDST;
    union TCYCLE_TIME_REG    TCYCLE_TIME;
    Uint16  DATA[32];
    Uint16  TTS01;
    Uint16  TTS23;
    Uint16  TTS45;
    Uint16  TTS67;
};

struct  CFG_STAT_BITS {
    Uint16      BUSOFF:1;       //0     1:bus off; 0:bus on
    Uint16      TACTIVE:1;      //1     transmit active status
    Uint16      RACTIVE:1;      //2     receive active status
    Uint16      TSSS:1;         //3     secondary single mode transmission for STB
    Uint16      TPSS:1;         //4     first single mode transmission for PTB
    Uint16      LBMI:1;         //5     1:internal loop back mode enable
    Uint16      LBME:1;         //6     1:external loop back mode enable
    Uint16      RESET:1;        //7     1:reset can control registers
    Uint16      TSA:1;          //8     secondary transmission aborted
    Uint16      TSALL:1;        //9     secondary transmission all frame
    Uint16      TSONE:1;        //10    secondary transmission on
    Uint16      TPA:1;          //11
    Uint16      TPE:1;          //12
    Uint16      STBY:1;         //13    standby mode
    Uint16      LOM:1;          //14    listen mode only
    Uint16      TBSEL:1;        //15    transmission buff select
};

union CFG_STAT_REGS {
    Uint16                  all;
    struct CFG_STAT_BITS    bit;
};

struct  CTRL_BITS {
    Uint16      TSSTAT:2;       //0_1   transmission assistance status
    Uint16      rsvd1:2;        //2_3   reserved
    Uint16      TTTBM:1;        //4     TTCAN  transmission buffer mode
    Uint16      TSMODE:1;       //5     0:FIFO mode
    Uint16      TSNEXT:1;       //6     next secondary transmission buffer
    Uint16      FD_ISO:1;       //7     0: Bosch CANFD mode(not ISO) 1: ISO CANFD mode
    Uint16      RSTAT:2;        //8_9   receive buffer status
    Uint16      rsvd2:1;        //10    reserved
    Uint16      RBALL:1;        //11    receive buffer all frame
    Uint16      RREL:1;         //12    release receive buffer
    Uint16      ROV:1;          //13    receive buffer overflow
    Uint16      ROM:1;          //14    receive buffer overflow mode
    Uint16      SACK:1;         //15    self ask mechanism
};

union CTRL_REGS {
    Uint16              all;
    struct CTRL_BITS    bit;
};

struct  RTINTFE_BITS {
    Uint16      TSFF:1;         //0     interrupt stop flag
    Uint16      EIE:1;          //1     error interrupt flag
    Uint16      TSIE:1;         //2     secondary transmission interrupt flag
    Uint16      TPIE:1;         //3     principal transmission interrupt flag
    Uint16      RAFIE:1;        //4     RB almost full interrupt flag
    Uint16      RFIE:1;         //5     RB full interrupt flag
    Uint16      ROIE:1;         //6     RB overflow interrupt flag
    Uint16      RIE:1;          //7     receive interrupt flag
    Uint16      AIF:1;          //8
    Uint16      EIF:1;          //9     error interrupt enable
    Uint16      TSIF:1;         //10    secondary transmission interrupt enable
    Uint16      TPIF:1;         //11    principal transmission interrupt enable
    Uint16      RAFIF:1;        //12    RB almost full interrupt enable
    Uint16      RFIF:1;         //13    RB full interrupt enable
    Uint16      ROIF:1;         //14    RB overflow interrupt enable
    Uint16      RIF:1;          //15    receive interrupt enable
};

union RTINTFE_REGS {
    Uint16              all;
    struct RTINTFE_BITS bit;
};

struct  LIMIT_EINT_BITS {
    Uint16      BEIF:1;         //0     bus error interrupt flag
    Uint16      BEIE:1;         //1     bus error interrupt enable
    Uint16      ALIF:1;         //2     arbitration lost interrupt flag
    Uint16      ALIE:1;         //3     arbitration lost interrupt enable
    Uint16      EPIF:1;         //4     error passive interrupt flag
    Uint16      EPIE:1;         //5     error passive interrupt enable
    Uint16      EPASS:1;        //6     error mode
    Uint16      EWARN:1;        //7
    Uint16      EWL:4;          //8_11  programmable error warning
    Uint16      AFWL:4;         //12_15 receive buffer almost full warning limit
};

union LIMIT_EINT_REGS {
    Uint16                  all;
    struct LIMIT_EINT_BITS  bits;
};

struct  S_SEG_BITS {
    Uint16      S_Seg_1:8;      //0_7
    Uint16      S_Seg_2:7;      //8_14
    Uint16      rsvd1:1;        //15    reserved
};

union S_SEG_REGS {
    Uint16              all;
    struct S_SEG_BITS   bit;
};

struct  S_CFG_BITS {
    Uint16      S_SJW:7;        //0_6   error calcution
    Uint16      rsvd1:1;        //7     reserved
    Uint16      S_PRESC:8;      //8_15
};

union S_CFG_REGS {
    Uint16              all;
    struct S_CFG_BITS   bit;
};

struct  F_SEG_BITS {
    Uint16      F_Seg_1:5;      //0_4
    Uint16      rsvd1:3;        //5_7   reserved
    Uint16      F_Seg_2:4;      //8_11
    Uint16      rsvd2:4;        //12_15 reserved
};

union F_SEG_REGS {
    Uint16              all;
    struct F_SEG_BITS   bit;
};

struct  F_CFG_BITS {
    Uint16      F_SJW:4;        //0_3
    Uint16      rsvd1:4;        //4_7
    Uint16      F_PRESC:8;      //8_15
};

union F_CFG_REGS {
    Uint16              all;
    struct F_CFG_BITS   bit;
};

struct  DELAY_EALCAP_BITS {
    Uint16      ALC:5;          //0_4
    Uint16      KOER:3;         //5_7
    Uint16      SSPOFF:7;       //8_14
    Uint16      TDCEN:1;        //15
};

union DELAY_EALCAP_REGS {
    Uint16                      all;
    struct DELAY_EALCAP_BITS    bit;
};

struct  ECNT_BITS {
    Uint16      RECNT:8;        //0_7   receive error counter
    Uint16      TECNT:8;        //8_15  transfer error counter
};

union ECNT_REGS {
    Uint16              all;
    struct ECNT_BITS    bit;
};

struct  CIA_ACF_CFG_BITS {
    Uint16      ACFADR:4;       //0_3   receive filter address
    Uint16      rsvd1:1;        //4     reserved
    Uint16      SELMASK:1;      //5     select receive mask
    Uint16      rsvd2:2;        //6_7   reserved
    Uint16      TIMEEN:1;       //8     enable timer
    Uint16      TIMEPOS:1;      //9     timer position
    Uint16      rsvd3:6;        //10_15 reserved
};

union CIA_ACF_CFG_REGS {
    Uint16                  all;
    struct CIA_ACF_CFG_BITS bit;
};

struct  ACF_EN_BITS {
    Uint16      AE_0:1;         //0     receive filter enable
    Uint16      AE_1:1;         //1     receive filter enable
    Uint16      AE_2:1;         //2     receive filter enable
    Uint16      AE_3:1;         //3     receive filter enable
    Uint16      AE_4:1;         //4     receive filter enable
    Uint16      AE_5:1;         //5     receive filter enable
    Uint16      AE_6:1;         //6     receive filter enable
    Uint16      AE_7:1;         //7     receive filter enable
    Uint16      AE_8:1;         //8     receive filter enable
    Uint16      AE_9:1;         //9     receive filter enable
    Uint16      AE_10:1;        //10    receive filter enable
    Uint16      AE_11:1;        //11    receive filter enable
    Uint16      AE_12:1;        //12    receive filter enable
    Uint16      AE_13:1;        //13    receive filter enable
    Uint16      AE_14:1;        //14    receive filter enable
    Uint16      AE_15:1;        //15    receive filter enable
};

union ACF_EN_REGS {
    Uint16              all;
    struct ACF_EN_BITS  bit;
};

struct  ACF_0_BITS {
    Uint16      ACODE_AMASK_0:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_1:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_2:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_3:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_4:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_5:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_6:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_7:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_8:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_9:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_10:1;     //10    receive code or mask
    Uint16      ACODE_AMASK_11:1;     //11    receive code or mask
    Uint16      ACODE_AMASK_12:1;     //12    receive code or mask
    Uint16      ACODE_AMASK_13:1;     //13    receive code or mask
    Uint16      ACODE_AMASK_14:1;     //14    receive code or mask
    Uint16      ACODE_AMASK_15:1;     //15    receive code or mask
};

union ACF_0_REGS {
    Uint16              all;
    struct ACF_0_BITS   bit;
};

struct  ACF_1_BITS {
    Uint16      ACODE_AMASK_0:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_1:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_2:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_3:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_4:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_5:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_6:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_7:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_8:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_9:1;      //0     receive code or mask
    Uint16      ACODE_AMASK_10:1;     //10    receive code or mask
    Uint16      ACODE_AMASK_11:1;     //11    receive code or mask
    Uint16      ACODE_AMASK_12:1;     //12    receive code or mask
    Uint16      rsvd_AIDE_13:1;       //13    receive code or receive mask IDE bit
    Uint16      rsvd_AIDEE_14:1;      //14    receive code or receive mask IDE inspect on
    Uint16      rsvd1:1;              //15    receive code or reserved
};

union ACF_1_REGS {
    Uint16              all;
    struct ACF_1_BITS   bit;
};

struct  TTCFG_TPTR_BITS {
    Uint16      TBPTR:6;        //0_5   pointer to TB message slot
    Uint16      TBF:1;          //6     set TB slot to fill
    Uint16      TBE:1;          //7     set TB slot to empty
    Uint16      TTEN:1;         //8     TTCAN on
    Uint16      T_PRESC:2;      //9_10  TTCAN prescale
    Uint16      TTIF:1;         //11
    Uint16      TTIE:1;         //12
    Uint16      TEIF:1;         //13
    Uint16      WITF:1;         //14
    Uint16      WITE:1;         //15    watch dog trigger interrupt enable
};

union TTCFG_TPTR_REGS {
    Uint16                  all;
    struct TTCFG_TPTR_BITS  bit;
};

struct  REFID_0_BITS {
    Uint16      REF_ID_0:1;     //0
    Uint16      REF_ID_1:1;     //1
    Uint16      REF_ID_2:1;     //2
    Uint16      REF_ID_3:1;     //3
    Uint16      REF_ID_4:1;     //4
    Uint16      REF_ID_5:1;     //5
    Uint16      REF_ID_6:1;     //6
    Uint16      REF_ID_7:1;     //7
    Uint16      REF_ID_8:1;     //8
    Uint16      REF_ID_9:1;     //9
    Uint16      REF_ID_10:1;    //10
    Uint16      REF_ID_11:1;    //11
    Uint16      REF_ID_12:1;    //12
    Uint16      REF_ID_13:1;    //13
    Uint16      REF_ID_14:1;    //14
    Uint16      REF_ID_15:1;    //15
};

union REFID_0_REGS {
    Uint16              all;
    struct REFID_0_BITS bit;
};

struct  REFID_1_BITS {
    Uint16      REF_ID_0:1;     //0
    Uint16      REF_ID_1:1;     //1
    Uint16      REF_ID_2:1;     //2
    Uint16      REF_ID_3:1;     //3
    Uint16      REF_ID_4:1;     //4
    Uint16      REF_ID_5:1;     //5
    Uint16      REF_ID_6:1;     //6
    Uint16      REF_ID_7:1;     //7
    Uint16      REF_ID_8:1;     //8
    Uint16      REF_ID_9:1;     //9
    Uint16      REF_ID_10:1;    //10
    Uint16      REF_ID_11:1;    //11
    Uint16      REF_ID_12:1;    //12
    Uint16      rsvd1:2;        //13_14     reserved
    Uint16      REF_IDE:1;      //15        reference IDE
};

union REFID_1_REGS {
    Uint16              all;
    struct REFID_1_BITS bit;
};

struct  TRIG_CFG_BITS {
    Uint16      TTPTR:6;        //0_5       pointer to TB slot
    Uint16      rsvd1:2;        //6_7       reserved
    Uint16      TTYPE:3;        //8_10      trigger type
    Uint16      rsvd2:1;        //11        reserved
    Uint16      TEW:4;          //12_15
};

union TRIG_CFG_REGS {
    Uint16                  all;
    struct TRIG_CFG_BITS    bit;
};

struct  CANCFG_BITS {
    Uint16      ECC_EN:1;       //0       ecc enable
    Uint16      CAN_CLK_SEL:1;  //1       CAN clock select
    Uint16      CAN_STBY:1;     //2       CAN standby mode
    Uint16      rsvd1:13;       //3_15    reserved
};

union CANCFG_REGS {
    Uint16              all;
    struct CANCFG_BITS bit;
};

struct  TIMERTCR_BITS {
    Uint16      rsvd1:4;        //0_3   reserved
    Uint16      TSS:1;          //4     timer stop status
    Uint16      RELOAD:1;       //5
    Uint16      rsvd2:4;        //6_9   reserved
    Uint16      FREE_SOFT:2;    //10_11
    Uint16      rsvd3:4;        //12_15 reserved
};

union TIMERTCR_REGS {
    Uint16              all;
    struct TIMERTCR_BITS bit;
};

struct  CANFD_REGS {
    struct  CANFD_RBUF_REGS RBUF;
    struct  CANFD_TBUF_REGS TBUF;
    union CFG_STAT_REGS     CFG_STAT;       // 0x50
    union CTRL_REGS         TCTRL;          // 0x51
    union RTINTFE_REGS      RTINTFE;        // 0x52
    union LIMIT_EINT_REGS   LIMIT_EINT;       // 0x53
    union S_SEG_REGS        S_SEG;       // 0x54
    union S_CFG_REGS        S_CFG;       // 0x55
    union F_SEG_REGS        F_SEG;       // 0x56
    union F_CFG_REGS        F_CFG;       // 0x57
    union DELAY_EALCAP_REGS DELAY_EALCAP;       // 0x58
    union ECNT_REGS         ECNT;       // 0x59
    union CIA_ACF_CFG_REGS  CIA_ACF_CFG;       // 0x5a
    union ACF_EN_REGS       ACF_EN;       // 0x5b
    union ACF_0_REGS        ACF_0;       // 0x5c
    union ACF_1_REGS        ACF_1;       // 0x5d
    Uint16                  rsvd1;       // 0x5e
    union TTCFG_TPTR_REGS   TTCFG_TPTR;       // 0x5f
    union REFID_0_REGS      REFID_0;       // 0x60
    union REFID_1_REGS      REFID_1;       // 0x61
    union TRIG_CFG_REGS     TRIG_CFG;       // 0x62
    Uint16                  TT_TRIG;       // 0x63
    Uint16                  TT_WIRIG;       // 0x64
    Uint16                  rsvd2[11];       // 0x65-0x6F
    union CANCFG_REGS       CANCFG;       // 0x70
    Uint16                  rsvd3[15];       // 0x71-0x7F
    Uint16                  TIMERTIM0;       // 0x80
    Uint16                  TIMERTIM1;       // 0x81
    Uint16                  TIMERTIM2;       // 0x82
    Uint16                  TIMERTIM3;       // 0x83
    Uint16                  TIMERPRD0;       // 0x84
    Uint16                  TIMERPRD1;       // 0x85
    Uint16                  TIMERPRD2;       // 0x86
    Uint16                  TIMERPRD3;       // 0x87
    union TIMERTCR_REGS     TIMERTCR;       // 0x88
    Uint16                  TIMERTPR0;       // 0x89
    Uint16                  TIMERTPR1;       // 0x8a
    Uint16                  rsvd4[245];      // 0x8b-0x17F
    Uint16                  TRANCNT0;       // 0x180
    Uint16                  TRANCNT1;       // 0x181
    Uint16                  TRANPRD0;       // 0x182
    Uint16                  TRANPRD1;       // 0x183
    Uint16                  TRANTPR0;       // 0x184
    Uint16                  TRANTPR1;       // 0x185
    Uint16                  rsvd5[378];     // 0x186-0x2FF
};

struct  CANFD_CNTREGS {
    Uint16                  TIMERTIM0;       // 0x80
    Uint16                  TIMERTIM1;       // 0x81
    Uint16                  TIMERTIM2;       // 0x82
    Uint16                  TIMERTIM3;       // 0x83
    Uint16                  TIMERPRD0;       // 0x84
    Uint16                  TIMERPRD1;       // 0x85
    Uint16                  TIMERPRD2;       // 0x86
    Uint16                  TIMERPRD3;       // 0x87
    union TIMERTCR_REGS     TIMERTCR;       // 0x88
    Uint16                  TIMERTPR0;       // 0x89
    Uint16                  TIMERTPR1;       // 0x8a
    Uint16                  rsvd4[245];      // 0x8b-0x17F
    Uint16                  TRANCNT0;       // 0x180
    Uint16                  TRANCNT1;       // 0x181
    Uint16                  TRANPRD0;       // 0x182
    Uint16                  TRANPRD1;       // 0x183
    Uint16                  TRANTPR0;       // 0x184
    Uint16                  TRANTPR1;       // 0x185
    Uint16                  rsvd5[378];     // 0x186-0x2FF
};

extern volatile struct CANFD_REGS      CanfdRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* HAL_INC_ ADP32F03X_CANFD_H_ */

//===========================================================================
// End of file
//===========================================================================
