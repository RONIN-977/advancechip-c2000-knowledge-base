################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
headers/source/%.obj: ../headers/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/fsi/fsi_ex2_loopback_clacontrol" --include_path="E:/A024_AVP32F0049_example/examples/fsi/fsi_ex2_loopback_clacontrol/headers/include" --include_path="E:/A024_AVP32F0049_example/examples/fsi/fsi_ex2_loopback_clacontrol/include" --include_path="E:/A024_AVP32F0049_example/examples/fsi/fsi_ex2_loopback_clacontrol/device" --include_path="E:/A024_AVP32F0049_example/examples/fsi/fsi_ex2_loopback_clacontrol/device/driverlib" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="headers/source/$(basename $(<F)).d_raw" --obj_directory="headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


