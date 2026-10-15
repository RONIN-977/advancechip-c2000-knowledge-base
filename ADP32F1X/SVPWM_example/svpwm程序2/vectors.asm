
; Abstract: 
;   This file defines the reset Expecting condition code as operand for the TMS320C2407.
;
;--------------------------------------------------------------------------

        .ref _c_int0, _nim,_pwm_int2,PHANTOM
	
        .sect     "vectors"
RESET B            _c_int0             ;
int1   B           PHANTOM             ;02h INT1
int2   B           _pwm_int2 ;04h INT2
int3   B           PHANTOM            ;06h INT3
int4   B           PHANTOM            ;08h INT4
int5   B           PHANTOM            ;0Ah INT5
int6   B           PHANTOM            ;0Ch INT6
RESERVED   B       PHANTOM            ;0Eh reserved
SW_INT8   B        PHANTOM            ;10h INT8  (software)
SW_INT9   B        PHANTOM            ;12h INT9  (software)
SW_INT10  B        PHANTOM           ;14h INT10 (software)
SW_INT11  B        PHANTOM           ;16h INT11 (software)
SW_INT12  B        PHANTOM           ;18h INT12 (software)
SW_INT13  B        PHANTOM           ;1Ah INT13 (software)
SW_INT14  B        PHANTOM          ;1Ch INT14 (software)
SW_INT15 B         PHANTOM          ;1Eh INT15 (software)
SW_INT16  B        PHANTOM          ;20h INT16 (software)
TRAP      B        PHANTOM           ;22h TRAP
NMI       B        NMI               ;24h NMI
EMU_TRAP  B        PHANTOM           ;26h reserved
SW_INT20 B         PHANTOM           ;28h INT20 (software)
SW_INT21  B        PHANTOM          ;2Ah INT21 (software)
SW_INT22  B        PHANTOM           ;2Ch INT22 (software)
SW_INT23  B        PHANTOM           ;2Eh INT23 (software)
SW_INT24  B        PHANTOM           ;30h INT24 (software)
SW_INT25  B        PHANTOM           ;32h INT25 (software)
SW_INT26  B        PHANTOM           ;34h INT26 (software)
SW_INT27  B        PHANTOM           ;36h INT27 (software)
SW_INT28  B        PHANTOM           ;38h INT28 (software)
SW_INT29 B         PHANTOM           ;3Ah INT29 (software)
SW_INT30  B        PHANTOM           ;3Ch INT30 (software)
SW_INT31  B        PHANTOM           ;3Eh INT31 (software)

    .end