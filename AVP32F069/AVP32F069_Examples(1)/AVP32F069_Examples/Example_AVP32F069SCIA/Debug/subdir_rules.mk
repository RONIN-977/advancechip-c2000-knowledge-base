################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 --include_path="C:/Users/17879/Desktop/CHIP/AVP32F069/AVP32F069_Examples(1)/AVP32F069_Examples/Example_AVP32F069SCIA/AVP32F069_common/include" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F069/AVP32F069_Examples(1)/AVP32F069_Examples/Example_AVP32F069SCIA/AVP32F069_headers/include" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


