################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
UserCode/HardWare/Src/%.obj: ../UserCode/HardWare/Src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335boot_app_1vsduo/335boot_app/demo/AVP32F335_APP_CAN" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335boot_app_1vsduo/335boot_app/demo/AVP32F335_APP_CAN/Library/Per_Inc" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335boot_app_1vsduo/335boot_app/demo/AVP32F335_APP_CAN/UserCode/App/Inc" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335boot_app_1vsduo/335boot_app/demo/AVP32F335_APP_CAN/UserCode/Com/Inc" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F335/335boot_app_1vsduo/335boot_app/demo/AVP32F335_APP_CAN/UserCode/HardWare/Inc" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="UserCode/HardWare/Src/$(basename $(<F)).d_raw" --obj_directory="UserCode/HardWare/Src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


