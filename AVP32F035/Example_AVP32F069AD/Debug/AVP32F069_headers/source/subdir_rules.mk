################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AVP32F069_headers/source/%.obj: ../AVP32F069_headers/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_AVP32F069AD" --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_AVP32F069AD/AVP32F069_common/include" --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_AVP32F069AD/AVP32F069_headers/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" -g --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="AVP32F069_headers/source/$(basename $(<F)).d_raw" --obj_directory="AVP32F069_headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


