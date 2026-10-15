################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
F2806x_CodeStartBranch_ebss_init.obj: ../F2806x_CodeStartBranch_ebss_init.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/depreciated" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/AVP32F069_headers/include" --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="F2806x_CodeStartBranch_ebss_init.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

usb_dev_mouse.obj: ../usb_dev_mouse.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/depreciated" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/AVP32F069_headers/include" --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="usb_dev_mouse.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

usb_mouse_structs.obj: ../usb_mouse_structs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/depreciated" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevMouse/AVP32F069_headers/include" --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="usb_mouse_structs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


