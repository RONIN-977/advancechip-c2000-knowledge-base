################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" --silicon_version=28 -g --define=_DEBUG --define=LARGE_MODEL --include_path="C:/Users/Allen1/Desktop/flash/DSP281x_common/include" --include_path="C:/Users/Allen1/Desktop/flash/DSP281x_headers/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --quiet --diag_warning=225 --issue_remarks --large_memory_model --unified_memory --asm_directory="C:/tidcs/c28/DSP281x/v120/DSP281x_examples_ccsv4/flash/Debug" --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" --silicon_version=28 -g --define=_DEBUG --define=LARGE_MODEL --include_path="C:/Users/Allen1/Desktop/flash/DSP281x_common/include" --include_path="C:/Users/Allen1/Desktop/flash/DSP281x_headers/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --quiet --diag_warning=225 --issue_remarks --large_memory_model --unified_memory --asm_directory="C:/tidcs/c28/DSP281x/v120/DSP281x_examples_ccsv4/flash/Debug" --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


