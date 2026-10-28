################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/vendor/c2000ware" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/device" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/device/driverlib" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/CPU1_RAM/syscfg" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/CPU1_RAM/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/vendor/c2000ware" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/device" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/device/driverlib" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/CPU1_RAM/syscfg" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/17879/Documents/ChatGPT/进芯替代TI C2000系列芯片信息/Development_Projects/0039_SCI_Autobaud_9600_20261021/sci_ex3_autobaud/CPU1_RAM/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


