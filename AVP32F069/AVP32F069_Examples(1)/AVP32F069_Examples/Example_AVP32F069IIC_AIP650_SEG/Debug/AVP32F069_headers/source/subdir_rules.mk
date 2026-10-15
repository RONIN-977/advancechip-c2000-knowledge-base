################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AVP32F069_headers/source/AVP32F069_GlobalVariableDefs.obj: ../AVP32F069_headers/source/AVP32F069_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069IIC_AIP650_SEG" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069IIC_AIP650_SEG/AVP32F069_headers/include" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069IIC_AIP650_SEG/AVP32F069_common/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="AVP32F069_headers/source/AVP32F069_GlobalVariableDefs.d_raw" --obj_directory="AVP32F069_headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


