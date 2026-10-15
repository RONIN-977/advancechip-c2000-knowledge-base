/******************************* Copyright(C) *********************************************
*                          AVP32F0049_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   clb_ex32_lp589x_api.h
*                         TITLE:  Simple APIs for configuring the LP5891 LED driver.
*******************************************************************************************/

//
// Included Files
//
#include <clb_ex32_system_info.h>

//
// Globals
//
extern uint16_t vsyncDone;

//
// Function Prototypes
//
void setData(uint16_t *data, uint16_t high, uint16_t mid, uint16_t low, uint16_t bias);

void LED_Set_Chip_Index(unsigned int checkResponse);

void LED_Write_RGB_Custom_ALL(uint16_t r_value, uint16_t g_value, uint16_t b_value);
void LED_Write_Black_ALL(void);
void LED_Write_White_ALL(void);
void LED_Write_Red_ALL(void);
void LED_Write_Green_ALL(void);
void LED_Write_Blue_ALL(void);
void LED_Write_Red_Custom_ALL(uint16_t brt_value);
void LED_Write_Green_Custom_ALL(uint16_t brt_value);
void LED_Write_Blue_Custom_ALL(uint16_t brt_value);

void LED_Write_Reg_Broadcast(uint16_t fc_reg, uint16_t data2, uint16_t data1, uint16_t data0, unsigned int checkResponse);

void sendSYNC();
void sendSYNCnoWait();

//
// End of File
//
