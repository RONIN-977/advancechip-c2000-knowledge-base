################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Main/%.obj: ../Main/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/Users/17879/Desktop/CHIP/036/036LINBOOT+APP_VF_20260615/036LINBOOT+APP_VF/ADP32F03x_LINBoot036" --include_path="C:/Users/17879/Desktop/CHIP/036/036LINBOOT+APP_VF_20260615/036LINBOOT+APP_VF/ADP32F03x_LINBoot036/Library/include" --include_path="C:/Users/17879/Desktop/CHIP/036/036LINBOOT+APP_VF_20260615/036LINBOOT+APP_VF/ADP32F03x_LINBoot036/Main" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Main/$(basename $(<F)).d_raw" --obj_directory="Main" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


