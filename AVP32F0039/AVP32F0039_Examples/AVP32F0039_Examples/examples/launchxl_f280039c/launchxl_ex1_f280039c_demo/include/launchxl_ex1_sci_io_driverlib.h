/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  launchxl_ex1_sci_io_driverlib.h
*                         TITLE: Contains public interface to various functions related
*                                to the serial communications interface (SCI) object
*******************************************************************************************/

#ifndef _SCI_IO_H_
#define _SCI_IO_H_


#ifdef __cplusplus
extern "C" {
#endif

//
// Globals
//

// 
// Function prototypes
//
int SCI_open(const char * path, unsigned flags, int llv_fd);
int SCI_close(int dev_fd);
int SCI_read(int dev_fd, char * buf, unsigned count);
int SCI_write(int dev_fd, const char * buf, unsigned count);
off_t SCI_lseek(int dev_fd, off_t offset, int origin);
int SCI_unlink(const char * path);
int SCI_rename(const char * old_name, const char * new_name);


#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of _SCI_H_ definition

//
// End of File
//

