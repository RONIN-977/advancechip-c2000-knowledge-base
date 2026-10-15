/*
//###########################################################################
//
// FILE:	ADP32F035_FLASH.cmd
//
// TITLE:	Linker Command File For ADP32F035 Device
//
//###########################################################################
//
   Define the memory block start/length for the F28035
   PAGE 0 will be used to organize program sections
   PAGE 1 will be used to organize data sections

   Notes:
         Memory blocks on ADP32F035 are uniform (ie same
         physical memory) in both PAGE 0 and PAGE 1.
         That is the same memory region should not be
         defined for both PAGE 0 and PAGE 1.
         Doing so will result in corruption of program
         and/or data.

         L0 memory block is mirrored - that is
         it can be accessed in high memory or low memory.
         For simplicity only one instance is used in this
         linker file.

         Contiguous SARAM memory blocks or flash sectors can be
         be combined if required to create a larger memory block.
//
//###########################################################################
*/


MEMORY
{
PAGE 0:    /* Program Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE1 for data allocation */
   RAM_L0      : origin = 0x008000, length = 0x000800     /* on-chip RAM block L0 2K word */
   RAM_L1      : origin = 0x008800, length = 0x000400     /* on-chip RAM block L1 1K word */
   OTP         : origin = 0x3D7800, length = 0x000400     /* on-chip OTP */
   APP_BEGIN   : origin = 0x3E8000, length = 0x000002     /* Part of FLASHH.  Used for app to Flash. */
   APP_FLASH   : origin = 0x3E8002, length = 0x009FFB     /* on-chip FLASHH  for APP  D~H: 0x00A000 - 0x02 - 0x01  : 40k word -> 80KB */
   //FLASHG      : origin = 0x3EA000, length = 0x002000     /* on-chip FLASH */
   //FLASHF      : origin = 0x3EC000, length = 0x002000     /* on-chip FLASH */
   //FLASHE      : origin = 0x3EE000, length = 0x002000     /* on-chip FLASH */
   //FLASHD      : origin = 0x3F0000, length = 0x001FFE     /* on-chip FLASH */
   APP_CRC     : origin = 0x3F1FFD, length = 0x000003     /* FLASHD last byte save app program check flag CRC and app length */
   //FLASHC      : origin = 0x3F2000, length = 0x002000
   EEPROM_FLASH :origin = 0x3F2000, length = 0x002000     /* on-chip FLASH  for EEPROM C: 0x002000  save DTC */
   //FLASHB      : origin = 0x3F4000, length = 0x002000     /* on-chip FLASH  Reserved for boot development (can be 0x002000+0x001F80=0x003F80)  */
   //FLASHA      : origin = 0x3F6000, length = 0x001F80     /* on-chip FLASH  Now boot used FLASHA 0x001F80, Just enough  */
   BOOT_FLASH  : origin = 0x3F4000, length = 0x003F80     /* boot length: 0x003F80 : 16k word -> 32KB  */
   CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
   BOOT_BEGIN  : origin = 0x3F7FF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
   CSM_PWL_P0  : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */

   IQTABLES    : origin = 0x3FE000, length = 0x000B50     /* IQ Math Tables in Boot ROM */
   IQTABLES2   : origin = 0x3FEB50, length = 0x00008C     /* IQ Math Tables in Boot ROM */
   IQTABLES3   : origin = 0x3FEBDC, length = 0x0000AA	  /* IQ Math Tables in Boot ROM */

   ROM         : origin = 0x3FF27C, length = 0x000D44     /* Boot ROM */
   RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
   VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */

PAGE 1 :   /* Data Memory */
           /* Memory (RAM/FLASH/OTP) blocks can be moved to PAGE0 for program allocation */
           /* Registers remain on PAGE1                                                  */
   BOOT_RSVD   : origin = 0x000000, length = 0x000050     /* Part of M0, BOOT rom will use this for stack */
   RAM_M0      : origin = 0x000050, length = 0x0003B0     /* on-chip RAM block M0 1K word */
   RAM_M1      : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 1k word */
   RAML2L3     : origin = 0x008C00, length = 0x0013FE     /* on-chip RAM block L2 L3  : 5118 word */
   //RAML3       : origin = 0x009000, length = 0x001000     /* on-chip RAM block L3 4k word */
   RAM_UPDATA  : origin = 0x009FFE, length = 0x000002     /* Jump back to the boot app to check if this flag needs to be upgraded */
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
   .cinit              : > APP_FLASH      PAGE = 0
   .pinit              : > APP_FLASH      PAGE = 0
   .text               : > APP_FLASH      PAGE = 0
   codestart           : > APP_BEGIN  PAGE = 0
   ramfuncs            : LOAD = APP_FLASH,
                         RUN = RAM_L0,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_END(_RamfuncsLoadEnd)
                         RUN_START(_RamfuncsRunStart),
                         PAGE = 0

    Flash28_API:
	{
	    // Utilize the internal API to reduce the storage space by 4kB.
		-l2803x_FlashAPI_BootROMSymbols.lib(.econst)
		-l2803x_FlashAPI_BootROMSymbols.lib(.text)

		// Using external API will increase the size of the Flash.
		//-lFlash2803x_API_V100.lib(.econst)
		//-lFlash2803x_API_V100.lib(.text)
	}
	LOAD = APP_FLASH,
	RUN = RAM_L0,
	LOAD_START(_Flash28_API_LoadStart),
	LOAD_END(_Flash28_API_LoadEnd),
	RUN_START(_Flash28_API_RunStart),
	PAGE = 0

   ram_app_check       : > RAM_UPDATA, PAGE = 1   /* jump to app check flag */

   csmpasswds          : > CSM_PWL_P0  PAGE = 0, TYPE = NOLOAD
   csm_rsvd            : > CSM_RSVD    PAGE = 0, TYPE = NOLOAD

   /* Allocate uninitalized data sections: */
   .stack              : > RAM_M0      PAGE = 1
   .ebss               : > RAML2L3     PAGE = 1
   .esysmem            : > RAML2L3     PAGE = 1
   .cio                : > RAML2L3     PAGE = 1

   /* Initalized sections go in Flash */
   /* For SDFlash to program these, they must be allocated to page 0 */
   .econst             : > APP_FLASH      PAGE = 0
   .switch             : > APP_FLASH      PAGE = 0

   /* Allocate IQ math areas: */
   IQmath              : > APP_FLASH      PAGE = 0            /* Math Code */
   IQmathTables        : > IQTABLES,       PAGE = 0, TYPE = NOLOAD

  /* Uncomment the section below if calling the IQNexp() or IQexp()
      functions from the IQMath.lib library in order to utilize the
      relevant IQ Math table in Boot ROM (This saves space and Boot ROM
      is 1 wait-state). If this section is not uncommented, IQmathTables2
      will be loaded into other memory (SARAM, Flash, etc.) and will take
      up space, but 0 wait-state is possible.
   */
   /*
   IQmathTables2    : > IQTABLES2, PAGE = 0, TYPE = NOLOAD
   {
              IQmath.lib<IQNexpTable.obj> (IQmathTablesRam)
   }
   */
    /* Uncomment the section below if calling the IQNasin() or IQasin()
       functions from the IQMath.lib library in order to utilize the
       relevant IQ Math table in Boot ROM (This saves space and Boot ROM
       is 1 wait-state). If this section is not uncommented, IQmathTables2
       will be loaded into other memory (SARAM, Flash, etc.) and will take
       up space, but 0 wait-state is possible.
    */
    /*
    IQmathTables3    : > IQTABLES3, PAGE = 0, TYPE = NOLOAD
    {
               IQmath.lib<IQNasinTable.obj> (IQmathTablesRam)
    }
    */

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
