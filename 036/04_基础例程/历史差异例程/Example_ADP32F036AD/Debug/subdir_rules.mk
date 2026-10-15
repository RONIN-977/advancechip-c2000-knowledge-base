################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" --silicon_version=28 -g --include_path="C:/Users/Allen1/Desktop/chip_information/ADP32F036/example/Example_ADP32F036AD" --include_path="C:/Users/Allen1/Desktop/chip_information/ADP32F036/example/Example_ADP32F036AD/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


