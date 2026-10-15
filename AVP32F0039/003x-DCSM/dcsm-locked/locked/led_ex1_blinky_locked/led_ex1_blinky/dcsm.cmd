/* this linker command file is to be included if user wants to use the DCSM feature on the device
 * DCSM  means Dual Zone Code Security Module.
 * This linker command file works as an addendum ot the already existing Flash/RAM linker command file
 * that the project has.
 * The sections in the *_ZoneSelectBlock.asm source file is linked as per the commands given in the file
 * NOTE - please note fill=0xFFFF, this helps if users include this file in the project by mistake and
 * doesn't provide the needed proper *_ZoneSelectBlock.asm sources .
 * Please refer to the Blinky DCSM example in the controlsuite examples for proper usage of this.
 *
 * Once users are confident that they want to program the passwords in OTP, the DSECT section type can be removed.
 *
*/

MEMORY
{
PAGE 0 :  /* Program Memory */

   /* Z1 OTP.  Z1 password locations / Flash and RAM partitioning */
   DCSM_OTP_Z1_LINKPOINTER     : origin = 0x78000, length = 0x00006
   DCSM_OTP_Z1_JLM_ENABLE      : origin = 0x78006, length = 0x00002
   DCSM_OTP_Z1_GPREG           : origin = 0x78008, length = 0x00008
   DCSM_OTP_Z1_PSWDLOCK        : origin = 0x78010, length = 0x00002
   DCSM_OTP_Z1_CRCLOCK         : origin = 0x78012, length = 0x00002
   DCSM_OTP_Z1_JTAG_PSWDH      : origin = 0x78014, length = 0x00004
   DCSM_OTP_Z1_CMAC_KEY        : origin = 0x78018, length = 0x00008

   /* DCSM Z1 Zone Select Contents (!!Movable!!) */
   DCSM_ZSEL_Z1_P0             : origin = 0x78080, length = 0x000020

   /* Z2 OTP.  Z2 password locations / Flash and RAM partitioning */
   DCSM_OTP_Z2_LINKPOINTER     : origin = 0x78200, length = 0x00006
   DCSM_OTP_Z2_RSVD            : origin = 0x78206, length = 0x00002
   DCSM_OTP_Z2_GPREG           : origin = 0x78208, length = 0x00008
   DCSM_OTP_Z2_PSWDLOCK        : origin = 0x78210, length = 0x00002
   DCSM_OTP_Z2_CRCLOCK         : origin = 0x78212, length = 0x00002
   DCSM_OTP_Z2_RSVD1           : origin = 0x78214, length = 0x0000C
   
   /* DCSM Z2 Zone Select Contents (!!Movable!!) */
   DCSM_ZSEL_Z2_P0             : origin = 0x78220, length = 0x000020

}

SECTIONS
{
   dcsm_otp_z1_linkpointer 	: > DCSM_OTP_Z1_LINKPOINTER		PAGE = 0
   dcsm_zsel_z1				: > DCSM_ZSEL_Z1_P0				PAGE = 0


}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
