################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Example_ADP32F036Timer/%.obj: ../Example_ADP32F036Timer/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/Users/Allen1/Desktop/chip_information/ADP32F036/example/Example_ADP32F036PWM_COMP" --include_path="C:/Users/Allen1/Desktop/chip_information/ADP32F036/example/Example_ADP32F036PWM_COMP/include" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Example_ADP32F036Timer/$(basename $(<F)).d_raw" --obj_directory="Example_ADP32F036Timer" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


