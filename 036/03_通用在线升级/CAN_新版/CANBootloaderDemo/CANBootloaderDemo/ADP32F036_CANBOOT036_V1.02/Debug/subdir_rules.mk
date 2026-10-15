################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --include_path="C:/Users/17879/Desktop/CHIP/036/CANBootloaderDemo/CANBootloaderDemo/ADP32F036_CANBOOT036_V1.02/HeardFiles/ADP32F03xHeader" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --include_path="C:/Users/17879/Desktop/CHIP/036/CANBootloaderDemo/CANBootloaderDemo/ADP32F036_CANBOOT036_V1.02/HeardFiles/SourceHeader" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --include_path="C:/Users/17879/Desktop/CHIP/036/CANBootloaderDemo/CANBootloaderDemo/ADP32F036_CANBOOT036_V1.02/HeardFiles/ADP32F03xHeader" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --include_path="C:/Users/17879/Desktop/CHIP/036/CANBootloaderDemo/CANBootloaderDemo/ADP32F036_CANBOOT036_V1.02/HeardFiles/SourceHeader" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


