/****************************************************************************/
/*  LINKER COMMAND FILE - MEMORY SPECIFICATION for C240/243 		    */
/*  									    */
/****************************************************************************/
 -c                /* ROM autoinitialization model */
 -m main.map       /* Create a map file            */
/* -o PWM.out       /* Output file name             */
 -l rts2xx.lib     /* Runtime-support library      */
/****************************************************************************/
/******** Specify the memory configuration **********************************/
/****************************************************************************/
MEMORY
{
    PAGE 0: VECTS: origin = 0000h, length = 0040h
            PVECS: origin = 0040h, length = 0080h
            PROG : origin = 00c0h, length = 7F40h
            SARAM: origin = 8000h, length = 8000h
           
    PAGE 1: BAREA: origin = 0200h, length = 0200h
            B2:    origin = 0060h, length = 0020h
            SARAM: origin = 0800h, length = 0800h
            EXT:   origin = 8000h, length = 8000h    
}
/*--------------------------------------------------------------------------*/
/* SECTIONS ALLOCATION                                                      */
/*--------------------------------------------------------------------------*/
SECTIONS
{
    .vectors    : { } > VECTS   PAGE 0    
    .pvecs      : { } > PVECS   PAGE 0
    .text       : { } > PROG	PAGE 0    /*Executable code and constants*/
    .cinit      : { } > PROG	PAGE 0    /*Tables for explicitly initialized global and static variables*/
    .switch     : { } > PROG	PAGE 0    /*Tables for implementing switch statements*/
    .const      : { } > BAREA	PAGE 1    /*Global and static const variables that are explicitly */
    .bss        : { } > SARAM	PAGE 1    /*Global and static variables*/
      /*Software stack area*/
       /*Dynamic memory area for malloc functions*/
}
/*  .data       : { } > SARAM	PAGE 1*/






