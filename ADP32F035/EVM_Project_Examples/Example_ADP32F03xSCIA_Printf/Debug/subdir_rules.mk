################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Delay.obj: ../Delay.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/Examples_ADP32F035_EVM/Examples/Example_ADP32F03xSCIA_Printf" --include_path="C:/Examples_ADP32F035_EVM/Examples/Example_ADP32F03xSCIA_Printf/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Delay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Example_ADP32F03xSCIA_Printf.obj: ../Example_ADP32F03xSCIA_Printf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/Examples_ADP32F035_EVM/Examples/Example_ADP32F03xSCIA_Printf" --include_path="C:/Examples_ADP32F035_EVM/Examples/Example_ADP32F03xSCIA_Printf/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Example_ADP32F03xSCIA_Printf.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

printf_uart.obj: ../printf_uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/Examples_ADP32F035_EVM/Examples/Example_ADP32F03xSCIA_Printf" --include_path="C:/Examples_ADP32F035_EVM/Examples/Example_ADP32F03xSCIA_Printf/include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.4.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="printf_uart.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


