################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Library/src/%.obj: ../Library/src/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335sciboot/AVP32F335_Bootloader-scia/AVP32F335_Bootloader-scia/AVP32F335_Bootloader/AVP32F335_APP" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335sciboot/AVP32F335_Bootloader-scia/AVP32F335_Bootloader-scia/AVP32F335_Bootloader/AVP32F335_APP/Library/include" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335sciboot/AVP32F335_Bootloader-scia/AVP32F335_Bootloader-scia/AVP32F335_Bootloader/AVP32F335_APP/Main" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Library/src/$(basename $(<F)).d_raw" --obj_directory="Library/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Library/src/%.obj: ../Library/src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335sciboot/AVP32F335_Bootloader-scia/AVP32F335_Bootloader-scia/AVP32F335_Bootloader/AVP32F335_APP" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335sciboot/AVP32F335_Bootloader-scia/AVP32F335_Bootloader-scia/AVP32F335_Bootloader/AVP32F335_APP/Library/include" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335sciboot/AVP32F335_Bootloader-scia/AVP32F335_Bootloader-scia/AVP32F335_Bootloader/AVP32F335_APP/Main" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Library/src/$(basename $(<F)).d_raw" --obj_directory="Library/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


