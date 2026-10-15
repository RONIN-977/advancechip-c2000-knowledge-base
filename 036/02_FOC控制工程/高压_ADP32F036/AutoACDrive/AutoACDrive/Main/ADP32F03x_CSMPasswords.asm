
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

; For code security operation, all addresses between 0x3F7F80 and
; 0X3f7ff5 cannot be used as program code or data.  These locations
; must be programmed to 0x0000 when the code security password locations
; (PWL) are programmed.  If security is not a concern, then these addresses
; can be used for code or data.  

; The section "csm_rsvd" can be used to program these locations to 0x0000.

        .sect "csm_rsvd"
        .loop (3F7FF5h - 3F7F80h + 1)
              .int 0x0000
        .endloop

;//===========================================================================
;// End of file.
;//===========================================================================

      
