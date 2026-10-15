################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/%.obj: ../source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" --silicon_version=28 -g --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_ADP32F03xAD" --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_ADP32F03xAD/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

source/%.obj: ../source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" --silicon_version=28 -g --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_ADP32F03xAD" --include_path="C:/Users/Allen1/Desktop/chip_information/AVP32F035/Example_ADP32F03xAD/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


