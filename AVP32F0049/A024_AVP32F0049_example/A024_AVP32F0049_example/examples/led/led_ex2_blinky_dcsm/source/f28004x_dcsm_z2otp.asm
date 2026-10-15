;/******************************* Copyright(C) *********************************************
;*                           AVP32F0049_Example_V1.01
;*                           湖南进芯电子科技有限公司
;*                         http://www.advancechip.com/
;*
;*                         FILE:   f28004x_dcsm_z2otp.asm
;*                         TITLE:  Dual Code Security Module Zone 2 OTP
;*******************************************************************************************/

      .sect "b0_dcsm_otp_z2_linkpointer"
	  .retain
;;    .long 0x1FFFFFFF     ;B0_Z2OTP_LINKPOINTER1
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B0_Z2OTP_LINKPOINTER2
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B0_Z2OTP_LINKPOINTER3
;;    .long 0xFFFFFFFF     ;Reserved

      .sect "b0_dcsm_otp_z2_gpreg"
	  .retain
;;    .long 0xFFFFFFFF     ;Z2OTP_BOOTPIN_CONFIG
;;    .long 0xFFFFFFFF     ;Z2OTP_GPREG2

      .sect "b0_dcsm_otp_z2_pswdlock"
	  .retain
;;    .long 0xbf7fffff     ;Z2-PSWDLOCK
;;    .long 0x77ffffff     ;Reserved

      .sect "b0_dcsm_otp_z2_crclock"
	  .retain
;;    .long 0x0fffffff     ;Z2-CRCLOCK
;;    .long 0x37ffffff     ;Reserved

      .sect "b0_dcsm_otp_z2_bootctrl"
	  .retain
;;    .long 0xFFFFFFFF     ;Z2OTP_GPREG3
;;    .long 0xFFFFFFFF     ;Z2OTP_BOOTCTRL

      .sect "b0_dcsm_zsel_z2"
	  .retain
;;    .long 0xFFFFFFFF     ;B0_Z2OTP_EXEONLYRAM
;;    .long 0xFFFFFFFF     ;B0_Z2OTP_EXEONLYSECT
;;    .long 0xFFFFFFFF     ;B0_Z2OTP_GRABRAM
;;    .long 0xFFFFFFFF     ;B0_Z2OTP_GRABSECT

;;    .long 0xFFFFFFFF     ;B0_Z2OTP_CSMPSWD0 (LSW of 128-bit password)
;;    .long 0xe3ffffff     ;B0_Z2OTP_CSMPSWD1 // this value is for ZSB0
;;    .long 0xFFFFFFFF     ;B0_Z2OTP_CSMPSWD2
;;    .long 0xFFFFFFFF     ;B0_Z2OTP_CSMPSWD3 (MSW of 128-bit password)

      .sect "b1_dcsm_otp_z2_linkpointer"
	  .retain
;;    .long 0x1FFFFFFF     ;B1_Z2OTP_LINKPOINTER1
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B1_Z2OTP_LINKPOINTER2
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0x1FFFFFFF     ;B1_Z2OTP_LINKPOINTER3
;;    .long 0xFFFFFFFF     ;Reserved

      .sect "b1_dcsm_zsel_z2"
	  .retain
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;B1_Z2OTP_EXEONLYSECT
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;B1_Z2OTP_GRABSECT

;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;Reserved
;;    .long 0xFFFFFFFF     ;Reserved

;;----------------------------------------------------------------------

;; For code security operation,after development has completed, prior to
;; production, all other zone select block locations should be programmed
;; to 0x0000 for maximum security.
;; If the first zone select block at offset 0x10 is used, the section
;; "dcsm_rsvd_z2" can be used to program these locations to 0x0000.
;; This code is commented out for development.

;;        .sect "dcsm_rsvd_z2"
;;        .retain
;;        .loop (1e0h)
;;              .int 0x0000
;;        .endloop


;;#############################################################################
;; End of file
;;#############################################################################
