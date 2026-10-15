################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
include/%.obj: ../include/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="C:/Users/Administrator/Desktop/examples0049/1sdspi/zkonglicheng/include" --include_path="C:/Users/Administrator/Desktop/examples0049/1sdspi/zkonglicheng/device" --include_path="C:/Users/Administrator/Desktop/examples0049/1sdspi/zkonglicheng/device/driverlib" --include_path="C:/Users/Administrator/Desktop/examples0049/1sdspi/zkonglicheng/device/driverlib/inc" --include_path="C:/Users/Administrator/Desktop/examples0049/1sdspi/zkonglicheng" --include_path="C:/ti/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="include/$(basename $(<F)).d_raw" --obj_directory="include" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


