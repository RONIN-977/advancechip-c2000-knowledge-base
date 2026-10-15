/******************************* Copyright(C) *********************************************
*
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  hw_nmi.h
*                         TITLE: Definitions for the NMI registers.
*******************************************************************************************/

#ifndef HW_NMI_H
#define HW_NMI_H

//*************************************************************************************************
//
// The following are defines for the NMI register offsets
//
//*************************************************************************************************
#define NMI_O_CFG      0x0U   // NMI Configuration Register
#define NMI_O_FLG      0x1U   // NMI Flag Register (SYSRsn Clear)
#define NMI_O_FLGCLR   0x2U   // NMI Flag Clear Register
#define NMI_O_FLGFRC   0x3U   // NMI Flag Force Register
#define NMI_O_WDCNT    0x4U   // NMI Watchdog Counter Register
#define NMI_O_WDPRD    0x5U   // NMI Watchdog Period Register
#define NMI_O_SHDFLG   0x6U   // NMI Shadow Flag Register


//*************************************************************************************************
//
// The following are defines for the bit fields in the NMICFG register
//
//*************************************************************************************************
#define NMI_CFG_NMIE   0x1U   // Global NMI Enable

//*************************************************************************************************
//
// The following are defines for the bit fields in the NMIFLG register
//
//*************************************************************************************************
#define NMI_FLG_NMIINT       0x1U      // NMI Interrupt Flag
#define NMI_FLG_CLOCKFAIL    0x2U      // Clock Fail Interrupt Flag
#define NMI_FLG_RAMUNCERR    0x4U      // RAM Uncorrectable Error NMI Flag
#define NMI_FLG_FLUNCERR     0x8U      // Flash Uncorrectable Error NMI Flag
#define NMI_FLG_PIEVECTERR   0x40U     // PIE Vector Fetch Error Flag
#define NMI_FLG_CLBNMI       0x100U    // Configurable Logic Block NMI Flag
#define NMI_FLG_SWERR        0x2000U   // SW Error Force NMI Flag

//*************************************************************************************************
//
// The following are defines for the bit fields in the NMIFLGCLR register
//
//*************************************************************************************************
#define NMI_FLGCLR_NMIINT       0x1U      // NMIINT Flag Clear
#define NMI_FLGCLR_CLOCKFAIL    0x2U      // CLOCKFAIL Flag Clear
#define NMI_FLGCLR_RAMUNCERR    0x4U      // RAMUNCERR Flag Clear
#define NMI_FLGCLR_FLUNCERR     0x8U      // FLUNCERR Flag Clear
#define NMI_FLGCLR_PIEVECTERR   0x40U     // PIEVECTERR Flag Clear
#define NMI_FLGCLR_CLBNMI       0x100U    // CLBNMI Flag Clear
#define NMI_FLGCLR_SWERR        0x2000U   // SWERR Flag Clear

//*************************************************************************************************
//
// The following are defines for the bit fields in the NMIFLGFRC register
//
//*************************************************************************************************
#define NMI_FLGFRC_CLOCKFAIL    0x2U      // CLOCKFAIL Flag Force
#define NMI_FLGFRC_RAMUNCERR    0x4U      // RAMUNCERR Flag Force
#define NMI_FLGFRC_FLUNCERR     0x8U      // FLUNCERR Flag Force
#define NMI_FLGFRC_PIEVECTERR   0x40U     // PIEVECTERR Flag Force
#define NMI_FLGFRC_CLBNMI       0x100U    // CLBNMI Flag Force
#define NMI_FLGFRC_SWERR        0x2000U   // SWERR Flag Force

//*************************************************************************************************
//
// The following are defines for the bit fields in the NMISHDFLG register
//
//*************************************************************************************************
#define NMI_SHDFLG_CLOCKFAIL    0x2U      // Shadow CLOCKFAIL Flag
#define NMI_SHDFLG_RAMUNCERR    0x4U      // Shadow RAMUNCERR Flag
#define NMI_SHDFLG_FLUNCERR     0x8U      // Shadow FLUNCERR Flag
#define NMI_SHDFLG_PIEVECTERR   0x40U     // Shadow PIEVECTERR Flag
#define NMI_SHDFLG_CLBNMI       0x100U    // Shadow CLBNMI flag
#define NMI_SHDFLG_SWERR        0x2000U   // SW Error Force NMI Flag



#endif
