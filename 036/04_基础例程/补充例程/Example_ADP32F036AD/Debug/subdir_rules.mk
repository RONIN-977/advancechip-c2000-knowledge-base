################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AD.obj: ../AD.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" --silicon_version=28 -g --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD" --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD/include" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="AD.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Delay.obj: ../Delay.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" --silicon_version=28 -g --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD" --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD/include" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="Delay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Example_ADP32F036AD.obj: ../Example_ADP32F036AD.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" --silicon_version=28 -g --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD" --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD/include" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="Example_ADP32F036AD.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

printf_uart.obj: ../printf_uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" --silicon_version=28 -g --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD" --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD/include" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="printf_uart.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

timer0_base.obj: ../timer0_base.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" --silicon_version=28 -g --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD" --include_path="D:/dsp/ADP32F036_Examples/Example_ADP32F036AD/include" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 --preproc_with_compile --preproc_dependency="timer0_base.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


