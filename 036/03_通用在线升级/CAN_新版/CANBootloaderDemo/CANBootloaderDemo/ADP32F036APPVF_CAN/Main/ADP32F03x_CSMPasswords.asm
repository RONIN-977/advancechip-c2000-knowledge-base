;//###########################################################################
;//
;// FILE:	ADP32F03x_CSMPasswords.asm
;//
;// TITLE:	ADP32F03x Code Security Module Passwords.
;// 
;// DESCRIPTION:
;//         CSM password configuration file, setting password values for Flash addresses 0x3F7FF8-0x3F7FFF
;//         Reserved addresses 0x3F7F80-0X3f7ff5 must be programmed to 0x0000
;//
;//###########################################################################

; CSM Password Section - 8x32-bit password words forming 128-bit password
; Default value 0xFFFF does not enable code security, recommended to maintain during development
      .sect "csmpasswds"

      .int	0xFFFF		;PWL0 (LSW of 128-bit password)
      .int	0xFFFF		;PWL1
      .int	0xFFFF		;PWL2
      .int	0xFFFF		;PWL3
      .int	0xFFFF		;PWL4
      .int	0xFFFF		;PWL5
      .int	0xFFFF		;PWL6
      .int	0xFFFF		;PWL7 (MSW of 128-bit password)

;----------------------------------------------------------------------

; CSM Reserved Address Section - 0x3F7F80 to 0X3f7ff5
; Must be programmed to 0x0000 when code security is enabled
        .sect "csm_rsvd"
        .loop (3F7FF5h - 3F7F80h + 1)
              .int 0x0000
        .endloop

;//===========================================================================
;// End of file.
;//===========================================================================
