################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
boot_core.obj: C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common/boot_core.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

boot_io.obj: C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common/boot_io.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device.obj: C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/source/device.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

f280015x_codestartbranch.obj: C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/source/f280015x_codestartbranch.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

sci_port.obj: C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common/sci_port.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

update_service.obj: C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common/update_service.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --tmu_support=tmu0 -O0 --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Bootloader" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Common" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/include" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/driverlib" --include_path="C:/Users/17879/Desktop/CHIP/BOOTLOADER/AVP32F00157Boot+APP_20261009/AVP32F00157/Device/FlashAPI" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=CPU1 --define=_FLASH -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


