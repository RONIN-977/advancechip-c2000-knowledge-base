################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/examples/fsi/fsi_ex8_ext_p2pconnection_tx" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/examples/fsi/fsi_ex8_ext_p2pconnection_tx/device" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/examples/fsi/fsi_ex8_ext_p2pconnection_tx/include" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/driverlib" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/driverlib/inc" --include_path="C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/examples/fsi/fsi_ex8_ext_p2pconnection_tx" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/examples/fsi/fsi_ex8_ext_p2pconnection_tx/device" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/examples/fsi/fsi_ex8_ext_p2pconnection_tx/include" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/driverlib" --include_path="C:/Users/Administrator/Desktop/AVP32F0039_Examples/driverlib/inc" --include_path="C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


