################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
cmd/AVP335.out: C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version\ Templates_V2.2/AVP32F335_CCS3.3version\ Templates_V2.2/AVP32F335_CCS3.3version\ Templates_V2.2/Ex_ADC/cmd/AVP335.cmd $(GEN_CMDS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml --define=_DEBUG --define=LARGE_MODEL -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/Ex_ADC/cmd/Debug/Ex_ADC.map" --stack_size=800 --warn_sections -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/lib" -i"C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/Ex_ADC/Ex_ADC" -i"C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/Ex_ADC" -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Ex_ADC_linkInfo.xml" --rom_model -o "$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

cmd/AVP33x_Headers_nonBIOS.out: C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version\ Templates_V2.2/AVP32F335_CCS3.3version\ Templates_V2.2/AVP32F335_CCS3.3version\ Templates_V2.2/Ex_ADC/cmd/AVP33x_Headers_nonBIOS.cmd $(GEN_CMDS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml --define=_DEBUG --define=LARGE_MODEL -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/Ex_ADC/cmd/Debug/Ex_ADC.map" --stack_size=800 --warn_sections -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/lib" -i"C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/Ex_ADC/Ex_ADC" -i"C:/Users/17879/Desktop/CHIP/AVP32F335/2026_07_21_AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/AVP32F335_CCS3.3version Templates_V2.2/Ex_ADC" -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Ex_ADC_linkInfo.xml" --rom_model -o "$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


