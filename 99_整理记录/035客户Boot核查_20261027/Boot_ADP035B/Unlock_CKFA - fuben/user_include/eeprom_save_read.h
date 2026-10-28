
#ifndef USER_INCLUDE_EEPROM_SAVE_READ_H_
#define USER_INCLUDE_EEPROM_SAVE_READ_H_

#define  NULL_E            0x00
#define  SAVE_E            0x01
#define  READ_E            0x02
#define  PRG_UPD_E         0x03
#define  UPD_IN_E          0x04
#define  UPD_OUT_E         0x05
#define  ZERO_E            0x06    //零漂校准命令
#define  COEF_E            0x07    //系数校准命令
#define  COEF_RES_E        0x08    //系数校准命令

//EEPROM存储地址 0000 0000 0001 0000b
#define  ADDR_CKFA_FLAG    5001      //进入远程升级标志
#define  ADDR_CKFA_IN      5003      //已经开始升级标志
#define  ADDR_ZERO_CALI    0x40      //零漂校准系数
#define  ADDR_COEF_ADJ     0x60      //系数校准地址


extern int f_i2cw_stop;
extern int timer_i2c;

extern Uint16 eeprom_save_cmd;
extern Uint16 eeprom_read_cmd;

extern Uint16 eeprom_len;
extern Uint16 eeprom_start_addr;
extern Uint16 DateSaveBuf[128];
extern Uint16 DateReadBuf[128];

extern void E2promParmSave(Uint16 start_addr,Uint16 *save_buf,Uint16 save_length);
extern void E2promParmRead(Uint16 start_addr,Uint16 *read_buf,Uint16 read_length);
extern void E2promInitRead(void);
extern void PageWrite(unsigned int addr,unsigned int *data,unsigned int length);
extern void PageRead(unsigned int addr,unsigned int *recv,unsigned int length);
extern void E2promDelay(void);
#endif /* USER_INCLUDE_EEPROM_SAVE_READ_H_ */
