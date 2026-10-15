/****************************************************************
File Description:
File Version:
Latest Update:

****************************************************************/

MEMORY
{
PAGE 0:    /* Program Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */
   RAMPRG      : origin = 0x008000, length = 0x000A80     /* on-chip RAM block L0, for RamFuncs */
   OTP         : origin = 0x3D7800, length = 0x000400     /* on-chip OTP */
   FLASHA      : origin = 0x3F6000, length = 0x001F7E     /* on-chip FLASH */
   FLASH_ERASE : origin = 0x3F7F7E, length = 0x000002     /* on-chip FLASH */
   CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   BEGIN       : origin = 0x3F7FF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
   CSM_PWL     : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */

   IQTABLES    : origin = 0x3FE000, length = 0x000B50     /* IQ Math Tables in Boot ROM */
   IQTABLES2   : origin = 0x3FEB50, length = 0x00008C     /* IQ Math Tables in Boot ROM */
   IQTABLES3   : origin = 0x3FEBDC, length = 0x0000AA	  /* IQ Math Tables in Boot ROM */

   ROM         : origin = 0x3FF27C, length = 0x000D44     /* Boot ROM */
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */

PAGE 1 :   /* Data Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE0 for program allocation */
           /* Registers remain on PAGE1                                                  */

   RAMM0       : origin = 0x000100, length = 0x000300     /* on-chip RAM block M0 */
   RAMM1       : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   RAML0       : origin = 0x008A80, length = 0x001580     /* on-chip RAM block L0 */
/*   RAML1	   : origin = 0x009000, length = 0x001000*/	  /* on-chip RAM block L1 */
/*   RAMH0	   : origin = 0x00A000, length = 0x002000  */ /* on-chip RAM block H0 */
}

/* Allocate sections to memory blocks.
   Note:
         codestart user defined section in DSP28_CodeStartBranch.asm used to redirect code 
                   execution when booting to flash
         ramfuncs  user defined section to store functions that will be copied from Flash into RAM
*/ 
SECTIONS
{
 
   /* Allocate program areas: */
   /*.InitBoot           : > FLASHA      PAGE = 0*/
   .cinit              : > FLASHA      PAGE = 0
   .pinit              : > FLASHA,     PAGE = 0
   .text               : > FLASHA      PAGE = 0
   .binit              : > FLASHA,     PAGE = 0
   .ovly      : > FLASHA,     PAGE = 0
   codestart           : > BEGIN       PAGE = 0
   flash_erase_in      : > FLASH_ERASE PAGE = 0

   ramfuncs            :
   {
        -lADP32F036_027_Flash_API.lib(.econst)
        -lADP32F036_027_Flash_API.lib(.text)
   }
						 LOAD = FLASHA,
                         RUN = RAMPRG,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_SIZE(_RamfuncsLoadSize),
                         RUN_START(_RamfuncsRunStart),
                         PAGE = 0



   otp_prg      : > OTP         PAGE = 0
   csmpasswds          : > CSM_PWL     PAGE = 0
   csm_rsvd            : > CSM_RSVD    PAGE = 0
   
   /* Allocate uninitalized data sections: */
   .stack              : > RAMM1       PAGE = 1
   .ebss               : > RAML0       PAGE = 1
   .esysmem            : > RAML0       PAGE = 1
   debug_buffer     : > RAML0       PAGE = 1

   /* Initalized sections go in Flash */
   /* For SDFlash to program these, they must be allocated to page 0 */
   .econst             : > FLASHA      PAGE = 0
   .switch             : > FLASHA      PAGE = 0      

   /* Allocate IQ math areas: */
   IQmath              : > FLASHA      PAGE = 0                  /* Math Code */
   IQmathTables        : > IQTABLES,   PAGE = 0, TYPE = NOLOAD

   /* .reset is a standard section used by the compiler.  It contains the */
   /* the address of the start of _c_int00 for C Code.   /*
   /* When using the boot ROM this section and the CPU vector */
   /* table is not needed.  Thus the default type is set here to  */
   /* DSECT  */
   .reset              : > RESET,      PAGE = 0, TYPE = DSECT
   vectors             : > VECTORS     PAGE = 0, TYPE = DSECT
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/

