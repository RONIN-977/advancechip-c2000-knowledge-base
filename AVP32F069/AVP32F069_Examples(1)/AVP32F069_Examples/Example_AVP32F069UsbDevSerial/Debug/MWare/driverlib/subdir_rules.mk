################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MWare/driverlib/interrupt.obj: ../MWare/driverlib/interrupt.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/driverlib/interrupt.d_raw" --obj_directory="MWare/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/driverlib/sysctl.obj: ../MWare/driverlib/sysctl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/driverlib/sysctl.d_raw" --obj_directory="MWare/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/driverlib/systick.obj: ../MWare/driverlib/systick.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/driverlib/systick.d_raw" --obj_directory="MWare/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/driverlib/uart.obj: ../MWare/driverlib/uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/driverlib/uart.d_raw" --obj_directory="MWare/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/driverlib/usb.obj: ../MWare/driverlib/usb.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/driverlib/usb.d_raw" --obj_directory="MWare/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/driverlib/usb_hal.obj: ../MWare/driverlib/usb_hal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/driverlib/usb_hal.d_raw" --obj_directory="MWare/driverlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


