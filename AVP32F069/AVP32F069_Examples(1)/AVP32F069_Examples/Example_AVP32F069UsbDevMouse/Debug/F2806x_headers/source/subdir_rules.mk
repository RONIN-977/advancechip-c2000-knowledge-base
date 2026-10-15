################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
F2806x_headers/source/F2806x_GlobalVariableDefs.obj: ../F2806x_headers/source/F2806x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/usb_dev_mouse/depreciated" --include_path="D:/dsp/AVP32F069_Examples/usb_dev_mouse/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/usb_dev_mouse/AVP32F069_headers/include" -g --define=DEBUG --define=ccs_c2k --diag_warning=225 --diag_suppress=10063 --preproc_with_compile --preproc_dependency="F2806x_headers/source/F2806x_GlobalVariableDefs.d_raw" --obj_directory="F2806x_headers/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


