/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   hw_wd.h
*                         TITLE:  Definitions for the WWD registers.
*******************************************************************************************/

#ifndef HW_WD_H
#define HW_WD_H

//*************************************************************************************************
//
// The following are defines for the WWD register offsets
//
//*************************************************************************************************
#define WWD_O_SCSR     0x22U   // System Control & Status Register
#define WWD_O_WDCNTR   0x23U   // Watchdog Counter Register
#define WWD_O_WDKEY    0x25U   // Watchdog Reset Key Register
#define WWD_O_WDCR     0x29U   // Watchdog Control Register
#define WWD_O_WDWCR    0x2AU   // Watchdog Windowed Control Register


//*************************************************************************************************
//
// The following are defines for the bit fields in the SCSR register
//
//*************************************************************************************************
#define WWD_SCSR_WDOVERRIDE   0x1U   // WD Override for WDDIS bit
#define WWD_SCSR_WDENINT      0x2U   // WD Interrupt Enable
#define WWD_SCSR_WDINTS       0x4U   // WD Interrupt Status
//*************************************************************************************************
//
// The following are defines for the bit fields in the WDCNTR register
//
//*************************************************************************************************
#define WWD_WDCNTR_WDCNTR_S   0U
#define WWD_WDCNTR_WDCNTR_M   0xFFU   // WD Counter
//*************************************************************************************************
//
// The following are defines for the bit fields in the WDKEY register
//
//*************************************************************************************************
#define WWD_WDKEY_WDKEY_S   0U
#define WWD_WDKEY_WDKEY_M   0xFFU   // WD KEY
//*************************************************************************************************
//
// The following are defines for the bit fields in the WDCR register
//
//*************************************************************************************************
#define WWD_WDCR_WDPS_S          0U
#define WWD_WDCR_WDPS_M          0x7U     // WD Clock Prescalar
#define WWD_WDCR_WDCHK_S         3U
#define WWD_WDCR_WDCHK_M         0x38U    // WD Check Bits
#define WWD_WDCR_WDDIS           0x40U    // WD Disable
#define WWD_WDCR_WDFLG           0x80U    // WD Reset Status Flag
#define WWD_WDCR_WDPRECLKDIV_S   8U
#define WWD_WDCR_WDPRECLKDIV_M   0xF00U   // WD Pre Clock Divider
//*************************************************************************************************
//
// The following are defines for the bit fields in the WDWCR register
//
//*************************************************************************************************
#define WWD_WDWCR_MIN_S      0U
#define WWD_WDWCR_MIN_M      0xFFU    // WD Min Threshold setting for Windowed Watchdog
                                      // functionality
#define WWD_WDWCR_FIRSTKEY   0x100U   // First Key Detect Flag


#endif
