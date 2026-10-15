
#include "Record.h"

int32 Index;
int32 AllowRecord;
int32 AllowRecordControl;
int32 SampleRate;
int32 Sample;
int32 Results[8][128];

void Write0(int32 Data);
void Write1(int32 Data);
void Write2(int32 Data);
void Write3(int32 Data);
void Write4(int32 Data);
void Write5(int32 Data);
void Write6(int32 Data);
void Write7(int32 Data);

void MoveToNextRow(void)
{
	if (AllowRecordControl != 0) {
		Index++;
		Index &= 0x7F;
		if (Index == 0 && AllowRecordControl == 1) {
			AllowRecord = 0;
		}	
	}
}
 
void IsRecordThisSample(int32 Data0,int32 Data1,int32 Data2,int32 Data3,int32 Data4,int32 Data5,int32 Data6,int32 Data7)
{
	if (AllowRecordControl != 0) {
		Sample++;
		if (Sample == SampleRate) {
			Sample = 0;
			Write0(Data0);
			Write1(Data1);
			Write2(Data2);
			Write3(Data3);
			Write4(Data4);
			Write5(Data5);
            Write6(Data6);
            Write7(Data7);
			MoveToNextRow();
		}
	}
}

void ControlAllowRecord(int32 Value, int32 SampleRateInput)
{
	if (AllowRecordControl == 0 && Value != 0) {
		AllowRecordControl = Value;
		AllowRecord = 1;
		SampleRate = SampleRateInput;
		
		// Record first sample.
		Sample = SampleRateInput - 1;
	}
}

void Write0(int32 Data)
{
	if (AllowRecord == 1) {
		Results[0][Index] = Data;
	}
}
void Write1(int32 Data)
{
	if (AllowRecord == 1) {
		Results[1][Index] = Data;
	}
}
void Write2(int32 Data)
{
	if (AllowRecord == 1) {
		Results[2][Index] = Data;
	}
}
void Write3(int32 Data)
{
	if (AllowRecord == 1) {
		Results[3][Index] = Data;
	}
}
void Write4(int32 Data)
{
	if (AllowRecord == 1) {
		Results[4][Index] = Data;
	}
}
void Write5(int32 Data)
{
    if (AllowRecord == 1) {
        Results[5][Index] = Data;
    }
}
void Write6(int32 Data)
{
    if (AllowRecord == 1) {
        Results[6][Index] = Data;
    }
}
void Write7(int32 Data)
{
    if (AllowRecord == 1) {
        Results[7][Index] = Data;
    }
}
void ResetIndex()
{
	Index = 0;
	AllowRecordControl = 0;
}

void ChangeSampleRate(int32 SampleRateInput)
{
	SampleRate = SampleRateInput;
}

int32 GetResult(int16 Channal, int16 Index)
{
	return Results[Channal][Index];
}

