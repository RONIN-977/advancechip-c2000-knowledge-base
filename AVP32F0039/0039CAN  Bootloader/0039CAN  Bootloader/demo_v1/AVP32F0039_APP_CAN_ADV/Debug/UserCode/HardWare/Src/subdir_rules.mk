################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
UserCode/HardWare/Src/%.obj: ../UserCode/HardWare/Src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/Library/device/FlashAPI/Constants" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/Library/device/FlashAPI" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/Library/device/include" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/Library/driverlib/inc" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/Library/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/UserCode/App/Inc" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/UserCode/Com/Inc" --include_path="C:/Users/17879/Desktop/CHIP/AVP32F003X/0039CAN  Bootloader/0039CAN  Bootloader/demo_v1/AVP32F0039_APP_CAN_ADV/UserCode/HardWare/Inc" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=coffabi --preproc_with_compile --preproc_dependency="UserCode/HardWare/Src/$(basename $(<F)).d_raw" --obj_directory="UserCode/HardWare/Src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


