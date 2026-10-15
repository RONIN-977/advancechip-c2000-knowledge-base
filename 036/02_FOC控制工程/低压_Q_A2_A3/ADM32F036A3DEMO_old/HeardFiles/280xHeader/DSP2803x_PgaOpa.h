// TI File $Revision: /main/1 $
// Checkin $Date: December 5, 2008   18:04:45 $
//###########################################################################
//
// FILE:   DSP2803x_PgaOpa.h
//
// TITLE:  DSP2803x Device PAG and OPA Register Definitions
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#ifndef DSP2803x_PGA_OPA_H
#define DSP2803x_PGA_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================
//  PGA and OPA Register Bit Definitions
//

struct PAGOPACTL_BITS {          // bit     description
    Uint16   PGA3_GAIN:3;        // 2:0     PGA3 Gain
    Uint16   rsvd1:1;            // 3       reserved
    Uint16   PGA2_GAIN:3;        // 6:4     PGA2 Gain
    Uint16   rsvd2:1;            // 7       reserved
    Uint16   PGA1_GAIN:3;        // 10:8    PGA1 Gain
    Uint16   rsvd3:1;            // 11      reserved
    Uint16   PGA3EN:1;           // 12      PGA3 Enable
    Uint16   PGA2EN:1;           // 13      PGA2 Enable
    Uint16   PGA1EN:1;           // 14      PGA1 Enable
    Uint16   OPAEN:1;            // 15      OPA  Enable
};

union PAGOPACTL_REG  {
    Uint16                         all;
    struct PAGOPACTL_BITS          bit;
};

//===========================================================================
//  PGA and OPA Register Definitions
//

struct PAGOPA_REGS {
    union  PAGOPACTL_REG         PAGOPACTL;
};

//===========================================================================
//  Comparator External References and Function Declarations
//

extern volatile struct PAGOPA_REGS PagOpaRegs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2803x_PGA_OPA_H definition

//===========================================================================
// End of file
//===========================================================================
