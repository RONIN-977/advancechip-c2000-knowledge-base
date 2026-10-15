;/******************************* Copyright(C) *********************************************
;*                           AVP32F0049_Example_V1.01
;*                           湖南进芯电子科技有限公司
;*                         http://www.advancechip.com/
;*
;*                         FILE:   f28004x_dcsm_z1otp.asm
;*                         TITLE:  Dual Code Security Module Zone 1 OTP
;*******************************************************************************************/

      .sect "b0_dcsm_otp_z1_linkpointer"
	  .retain
;;    .long 0x1FFFFFFF     ;B0_Z1OTP_LINKPOINTER1
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B0_Z1OTP_LINKPOINTER2
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B0_Z1OTP_LINKPOINTER3
;;    .long 0xFFFFFFFF     ;Reserved

      .sect "b0_dcsm_otp_z1_gpreg"
	  .retain
;;    See the ROM Code and Peripheral Booting chapter of TRM for more details.
;;
;;    Below is a description of the bit fields of Z1OTP_BOOTPIN_CONFIG
;;    used by Boot ROM.
;;
;;    | Key (31-24) | BMSP2 (23-16) | BMSP1 (15-8) | BMSP0 (7-0) |
;;
;;    Below is a description of the bit fields of Z1OTP_GPREG2 used by 
;;    Boot ROM.
;;
;;    | Key (31-24) | RSVD (23-8) |  RSVD (7-6) | ESP (5-4) | RSVD (3-0) |
;;
;;    .long 0xFFFFFFFF     ;Z1OTP_BOOTPIN_CONFIG
;;    .long 0xFFFFFFFF     ;Z1OTP_GPREG2

      .sect "b0_dcsm_otp_z1_pswdlock"
	  .retain
;;    .long 0xfb7fffff     ;Z1-PSWDLOCK
;;    .long 0x7fffffff     ;Reserved

      .sect "b0_dcsm_otp_z1_crclock"
	  .retain
;;    .long 0x4bffffff     ;Z1-CRCLOCK
;;    .long 0x3fffffff     ;Reserved

      .sect "b0_dcsm_otp_z1_bootctrl"
	  .retain
;;    See the ROM Code and Peripheral Booting chapter of TRM for more details.
;;
;;    Below is a description of the bit fields of Z1OTP_BOOTDEF_LOW used by 
;;    Boot ROM.
;;
;;    | BOOT_DEF3(31-24) | BOOT_DEF2(23-16) | BOOT_DEF1(15-8) | BOOT_DEF0(7-0) |
;;
;;    Below is a description of the bit fields of Z1OTP_BOOTDEF_HIGH
;;    used by Boot ROM.
;;
;;    | BOOT_DEF7(31-24) | BOOT_DEF6(23-16) | BOOT_DEF5(15-8) | BOOT_DEF4(7-0) |
;;
;;    .long 0xFFFFFFFF     ;Z1OTP_BOOTDEF_LOW
;;    .long 0xFFFFFFFF     ;Z1OTP_BOOTDEF_HIGH

      .sect "b0_dcsm_zsel_z1"
	  .retain

;;    .long 0xFFFFFFFF     ;B0_Z1OTP_EXEONLYRAM
;;    .long 0xFFFFFFFF     ;B0_Z1OTP_EXEONLYSECT
;;    .long 0xFFFFFFFF     ;B0_Z1OTP_GRABRAM
;;    .long 0xFFFFFFFF     ;B0_Z1OTP_GRABSECT

;;    .long 0xFFFFFFFF     ;B0_Z1OTP_CSMPSWD0 (LSW of 128-bit password)
;;    .long 0x47ffffff     ;B0_Z1OTP_CSMPSWD1 // this value is for ZSB0
;;    .long 0xFFFFFFFF     ;B0_Z1OTP_CSMPSWD2
;;    .long 0xFFFFFFFF     ;B0_Z1OTP_CSMPSWD3 (MSW of 128-bit password)

      .sect "b1_dcsm_otp_z1_linkpointer"
	  .retain
;;    .long 0x1FFFFFFF     ;B1_Z1OTP_LINKPOINTER1
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B1_Z1OTP_LINKPOINTER2
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B1_Z1OTP_LINKPOINTER3
;;    .long 0xFFFFFFFF     ;Reserved

      .sect "b1_dcsm_zsel_z1"
	  .retain
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;B1_Z1OTP_EXEONLYSECT
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;B1_Z1OTP_GRABSECT

;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;Reserved

;;----------------------------------------------------------------------

;; For code security operation,after development has completed, prior to
;; production, all other zone select block locations should be programmed
;; to 0x0000 for maximum security.
;; If the first zone select block at offset 0x10 is used, the section
;; "dcsm_rsvd_z1" can be used to program these locations to 0x0000.
;; This code is commented out for development.

;;        .sect "dcsm_rsvd_z1"
;;        .retain
;;        .loop (1e0h)
;;              .int 0x0000
;;        .endloop


;;#############################################################################
;; End of file
;;#############################################################################
