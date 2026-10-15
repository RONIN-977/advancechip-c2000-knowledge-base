################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/%.obj: ../source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" --silicon_version=28 -g --define=INTER_OSC --include_path="C:/Users/17879/Desktop/CHIP/036/Example_ADP32F036AD" --include_path="C:/Users/17879/Desktop/CHIP/036/Example_ADP32F036AD/include" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

source/%.obj: ../source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" --silicon_version=28 -g --define=INTER_OSC --include_path="C:/Users/17879/Desktop/CHIP/036/Example_ADP32F036AD" --include_path="C:/Users/17879/Desktop/CHIP/036/Example_ADP32F036AD/include" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


