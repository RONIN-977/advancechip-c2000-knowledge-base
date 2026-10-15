################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
F2806x_headers/source/AVP32F069_GlobalVariableDefs.obj: ../F2806x_headers/source/AVP32F069_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --float_support=fpu32 --vcu_support=vcu0 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="/packages/ti/xdais" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069GpioToggle/F2806x_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069GpioToggle/F2806x_common/include" --advice:performance=all --define=_DEBUG --define=LARGE_MODEL -g --diag_suppress=10063 --diag_warning=225 --issue_remarks --verbose_diagnostics --quiet --output_all_syms --preproc_with_compile --preproc_dependency="F2806x_headers/source/AVP32F069_GlobalVariableDefs.d_raw" --obj_directory="F2806x_headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


