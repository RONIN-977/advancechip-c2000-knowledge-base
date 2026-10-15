################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
driverlib/adc.obj: E:/A024_AVP32F0049_example/driverlib/adc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/asysctl.obj: E:/A024_AVP32F0049_example/driverlib/asysctl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/can.obj: E:/A024_AVP32F0049_example/driverlib/can.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/cla.obj: E:/A024_AVP32F0049_example/driverlib/cla.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/clapromcrc.obj: E:/A024_AVP32F0049_example/driverlib/clapromcrc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/clb.obj: E:/A024_AVP32F0049_example/driverlib/clb.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/cmpss.obj: E:/A024_AVP32F0049_example/driverlib/cmpss.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/cputimer.obj: E:/A024_AVP32F0049_example/driverlib/cputimer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/dac.obj: E:/A024_AVP32F0049_example/driverlib/dac.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/dcc.obj: E:/A024_AVP32F0049_example/driverlib/dcc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/dcsm.obj: E:/A024_AVP32F0049_example/driverlib/dcsm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/dma.obj: E:/A024_AVP32F0049_example/driverlib/dma.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/ecap.obj: E:/A024_AVP32F0049_example/driverlib/ecap.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/epwm.obj: E:/A024_AVP32F0049_example/driverlib/epwm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/eqep.obj: E:/A024_AVP32F0049_example/driverlib/eqep.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/erad.obj: E:/A024_AVP32F0049_example/driverlib/erad.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/flash.obj: E:/A024_AVP32F0049_example/driverlib/flash.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/fsi.obj: E:/A024_AVP32F0049_example/driverlib/fsi.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/gpio.obj: E:/A024_AVP32F0049_example/driverlib/gpio.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/hrcap.obj: E:/A024_AVP32F0049_example/driverlib/hrcap.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/hrpwm.obj: E:/A024_AVP32F0049_example/driverlib/hrpwm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/i2c.obj: E:/A024_AVP32F0049_example/driverlib/i2c.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/interrupt.obj: E:/A024_AVP32F0049_example/driverlib/interrupt.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/lin.obj: E:/A024_AVP32F0049_example/driverlib/lin.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/memcfg.obj: E:/A024_AVP32F0049_example/driverlib/memcfg.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/pga.obj: E:/A024_AVP32F0049_example/driverlib/pga.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/pmbus.obj: E:/A024_AVP32F0049_example/driverlib/pmbus.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/sci.obj: E:/A024_AVP32F0049_example/driverlib/sci.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/sdfm.obj: E:/A024_AVP32F0049_example/driverlib/sdfm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/spi.obj: E:/A024_AVP32F0049_example/driverlib/spi.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/sysctl.obj: E:/A024_AVP32F0049_example/driverlib/sysctl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/version.obj: E:/A024_AVP32F0049_example/driverlib/version.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/xbar.obj: E:/A024_AVP32F0049_example/driverlib/xbar.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/include" --include_path="E:/A024_AVP32F0049_example/examples/adc/adc_ex12_burst_mode_oversampling/device" --include_path="E:/A024_AVP32F0049_example/driverlib" --include_path="E:/A024_AVP32F0049_example/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


