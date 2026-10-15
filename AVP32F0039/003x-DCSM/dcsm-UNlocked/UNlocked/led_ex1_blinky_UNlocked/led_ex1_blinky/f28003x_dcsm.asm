
;----------------------------------------------------------------------
; Zone 1
;----------------------------------------------------------------------
     .sect "dcsm_otp_z1_linkpointer"
      .retain
      .long 0x00003FF0
      .long 0x00003FF0
      .long 0x00003FF0

     .sect "dcsm_otp_z1_jlm_enable"
      .retain
      .long 0xFFFF000F     ;Z1OTP_JLM_ENABLE
;;     .sect "dcsm_otp_z1_jtag_pswdh"
;;      .retain
;;      .long 0x4BFFFFFF     ;Z1OTP_JTAGPSWDH0
;;      .long 0x3FFFFFFF     ;Z1OTP_JTAGPSWDH1
;;
;;     .sect "dcsm_otp_z1_cmac_key"
;;      .retain
;;      .long 0x00000000     ;Z1OTP_CMACKEY0
;;      .long 0x00000000     ;Z1OTP_CMACKEY1
;;      .long 0x00000000     ;Z1OTP_CMACKEY2
;;      .long 0x00000000     ;Z1OTP_CMACKEY3
;;
     .sect "dcsm_otp_z1_pswdlock"
      .retain
      .long 0xFB7FFFFF

     .sect "dcsm_otp_z1_crclock"
      .retain
      .long 0x7FFFFFFF

;;     .sect "dcsm_otp_z1_gpreg"
;;      .retain
;;      .long 0x5AFF1820     ;Z1OTP_GPREG1 = Z1_BOOTPIN
;;      .long 0x5Affffff     ;Z1OTP_GPREG2
;;      .long 0x00000000     ;Z1OTP_GPREG3 = Z1OTP_BOOTDEF_LOW
;;      .long 0xFFFFFFFF     ;Z1OTP_GPREG4 = Z1OTP_BOOTDEF_HIGH
     .sect "dcsm_zsel_z1"
      .retain
      .long 0xFFFFFFFF     ;Z1OTP_CSMPSWD0 (LSW of 128-bit password)
      .long 0x1BFFFFFF     ;Z1OTP_CSMPSWD1
      .long 0xFFFFFFFF     ;Z1OTP_CSMPSWD2
      .long 0xFFFFFFFF     ;Z1OTP_CSMPSWD3 (MSW of 128-bit password)

      .long 0xAAAAAAAA     ;Z1OTP_GRABSECT1
      .long 0xAAAAAAAA     ;Z1OTP_GRABSECT2
      .long 0xAAAAAA00     ;Z1OTP_GRABSECT3
      .long 0x0000AAAA     ;Z1OTP_GRABRAM1
      .long 0xFFFFFFFF     ;Reserved
      .long 0xFFFFFFFF     ;Reserved

      .long 0xFFFFFFFF     ;Z1OTP_EXEONLYSECT1
      .long 0x0000FFFF     ;Z1OTP_EXEONLYSECT2
      .long 0x000000FF     ;Z1OTP_EXEONLYRAM1
      .long 0xFFFFFFFF     ;Reserved
      .long 0xFFFFFFFF     ;Z1OTP_JTAGPSWDL0
      .long 0x7D7FFFFF     ;Z1OTP_JTAGPSWDL1


;----------------------------------------------------------------------

; For code security operation,after development has completed, prior to
; production, all other zone select block locations should be programmed
; to 0x0000 for maximum security.
; If the first zone select block at offset 0x10 is used, the section
; "dcsm_rsvd_z1" can be used to program these locations to 0x0000.
; This code is commented out for development.

;       .sect "dcsm_rsvd_z1"
;        .loop (1e0h)
;              .int 0x0000
;        .endloop



