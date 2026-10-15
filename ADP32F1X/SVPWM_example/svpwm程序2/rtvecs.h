

            .if ( 1 ) ; macro occupies fourteen words in the vector table.
MON_EINTR   mon_eintr_vecs       ; 0E
                                 ; 10
                                 ; 12
                                 ; 14
                                 ; 16
                                 ; 18
                                 ; 1A
            .else ; macro not in vector table.
MON_EINTR_B B     MON_EINTR      ; 0E
HUNG10      B     HUNG10         ; 10
HUNG12      B     HUNG12         ; 12
HUNG14      B     HUNG14         ; 14
HUNG16      B     HUNG16         ; 16
HUNG18      B     HUNG18         ; 18
HUNG1A      B     HUNG1A         ; 1A
            .endif
HUNG1C      B     HUNG1C         ; 1C
HUNG1E      B     HUNG1E         ; 1E
HUNG20      B     HUNG20         ; 20
TRAP        B     TRAP           ; 22
NMI         B     PHANTOM           ; 24
            .if ( 1 ) ; macro occupies eight words in the vector table.
MON_ETRAP   mon_etrap_vecs       ; 26
                                 ; 28
                                 ; 2A
                                 ; 2C
            .else ; macro not in vector table.
MON_ETRAP_B B     MON_ETRAP      ; 26
HUNG28      B     HUNG28         ; 28
HUNG2A      B     HUNG2A         ; 2A
HUNG2C      B     HUNG2C         ; 2C
            .endif
HUNG2E      B     HUNG2E         ; 2E
HUNG30      B     HUNG30         ; 30
HUNG32      B     HUNG32         ; 32
HUNG34      B     HUNG34         ; 34
HUNG36      B     HUNG36         ; 36
HUNG38      B     HUNG38         ; 38
HUNG3A      B     HUNG3A         ; 3A
HUNG3C      B     HUNG3C         ; 3C
HUNG3E      B     HUNG3E         ; 3E

