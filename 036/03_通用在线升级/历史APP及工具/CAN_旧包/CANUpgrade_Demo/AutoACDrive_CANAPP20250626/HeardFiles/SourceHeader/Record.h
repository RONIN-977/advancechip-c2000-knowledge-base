#ifndef RECORD_HPP_
#define RECORD_HPP_

#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File

void MoveToNextRow(void);
void ControlAllowRecord(int32 Value, int32 SampleRateInput);
void ChangeSampleRate(int32 SampleRateInput);
void ResetIndex();
void IsRecordThisSample(int32 Data0,int32 Data1,int32 Data2,int32 Data3,int32 Data4,int32 Data5,int32 Data6,int32 Data7,int32 Data8,int32 Data9);
int32 GetResult(int16 Channal, int16 Index);


#endif
