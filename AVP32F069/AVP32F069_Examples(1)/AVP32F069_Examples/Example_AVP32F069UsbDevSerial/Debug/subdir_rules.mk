################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AVP32F069_CodeStartBranch_ebss_init.obj: ../AVP32F069_CodeStartBranch_ebss_init.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="AVP32F069_CodeStartBranch_ebss_init.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

usb_dev_serial.obj: ../usb_dev_serial.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="usb_dev_serial.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

usb_serial_structs.obj: ../usb_serial_structs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="usb_serial_structs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


