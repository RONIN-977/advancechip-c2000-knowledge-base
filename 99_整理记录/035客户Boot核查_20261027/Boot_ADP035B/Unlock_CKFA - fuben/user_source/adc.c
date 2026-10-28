
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File
#include "IQmathLib.h"
#include "user_declaration.h"

int Ug_adc = 0;
int Ug_avr_adc = 0,Ug_avr = 0;
long Ug_sum = 0;
long Ileak_adc = 0;
int ADResult[16] = {0};
int f_zero_cali = 0;
int f_zero_cali_2812 = 0;
int f_zero_cali_28034 = 0;
int f_zero_crct = 0;
int ubus_ave_cnt = 0;

int Ubus1_avr_adc = 0,Ubus2_avr_adc = 0,Ubus_avr_adc = 0;
int Ubus_avr = 0;
int Ubus1_adc = 0,Ubus2_adc = 0,Ubus_adc = 0;
long Ubus1_sum = 0,Ubus2_sum = 0;

int AdcZeroOffset[16] = {0};
int AdcCoefAdjust[16] = {0};

#pragma CODE_SECTION(AD_Deal_Ubus, "ramfuncs");
void AD_Deal_Ubus(void)
{
    AdcRegs.ADCSOCFRC1.all=0x3FFF;
    DELAY_US(25L);
    ADResult[4] = AdcResult.ADCRESULT4;    // 11 B3 Ubus_H 上半母线电压          SOC4
   // ADResult[5] = AdcResult.ADCRESULT5;    // 9  B1 Ubus_L 下半母线电压          SOC5
    Ubus_avr_adc = ADResult[4];//_IQ11mpy((ADResult[4]-AdcZeroOffset[4]),AdcCoefAdjust[4]);
   // Ubus2_adc = _IQ11mpy((ADResult[5]-AdcZeroOffset[5]),AdcCoefAdjust[4]);
   // Ubus_adc = (Ubus1_adc+Ubus2_adc)>>1;
}


