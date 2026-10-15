################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/driverlib/adc.obj: E:/driverlib/adc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/aes.obj: E:/driverlib/aes.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/asysctl.obj: E:/driverlib/asysctl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/bgcrc.obj: E:/driverlib/bgcrc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/can.obj: E:/driverlib/can.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/cla.obj: E:/driverlib/cla.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/clb.obj: E:/driverlib/clb.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/cmpss.obj: E:/driverlib/cmpss.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/cputimer.obj: E:/driverlib/cputimer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/dac.obj: E:/driverlib/dac.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/dcc.obj: E:/driverlib/dcc.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/dcsm.obj: E:/driverlib/dcsm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/dma.obj: E:/driverlib/dma.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/ecap.obj: E:/driverlib/ecap.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/epg.obj: E:/driverlib/epg.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/epwm.obj: E:/driverlib/epwm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/eqep.obj: E:/driverlib/eqep.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/erad.obj: E:/driverlib/erad.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/flash.obj: E:/driverlib/flash.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/fsi.obj: E:/driverlib/fsi.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/gpio.obj: E:/driverlib/gpio.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/hic.obj: E:/driverlib/hic.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/hrcap.obj: E:/driverlib/hrcap.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/hrpwm.obj: E:/driverlib/hrpwm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/i2c.obj: E:/driverlib/i2c.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/interrupt.obj: E:/driverlib/interrupt.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/lin.obj: E:/driverlib/lin.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/mcan.obj: E:/driverlib/mcan.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/memcfg.obj: E:/driverlib/memcfg.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/pmbus.obj: E:/driverlib/pmbus.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/sci.obj: E:/driverlib/sci.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/sdfm.obj: E:/driverlib/sdfm.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/spi.obj: E:/driverlib/spi.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/sysctl.obj: E:/driverlib/sysctl.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/version.obj: E:/driverlib/version.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/driverlib/xbar.obj: E:/driverlib/xbar.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/include" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x" --include_path="E:/A041_AVP32F0039_example/examples/dsp/dsp_fpu_rfft_fpu32_f28003x/device" --include_path="E:/driverlib" --include_path="E:/driverlib/inc" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/driverlib/$(basename $(<F)).d_raw" --obj_directory="device/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


