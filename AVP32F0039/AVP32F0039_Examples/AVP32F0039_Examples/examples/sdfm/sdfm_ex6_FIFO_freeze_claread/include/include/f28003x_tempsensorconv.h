/******************************* Copyright(C) *********************************************
*
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:   f28003x_tempsensorconv.h
*                         TITLE:  Definitions for Temperature Sensor.
*******************************************************************************************/

#ifndef F28003X_TEMPSENSOR_CONV_H
#define F28003X_TEMPSENSOR_CONV_H

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif                                  /* extern "C" */
void InitTempSensor(float vrefhi_voltage);
int16_t GetTemperatureC(uint16_t sensorSample);
int16_t GetTemperatureK(uint16_t sensorSample);
#endif

//===========================================================================
// End of file.
//===========================================================================
