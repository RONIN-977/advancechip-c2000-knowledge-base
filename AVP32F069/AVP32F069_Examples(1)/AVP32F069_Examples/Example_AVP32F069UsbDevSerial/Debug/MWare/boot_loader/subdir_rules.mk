################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MWare/boot_loader/F2806x_BL_MemCopy.obj: ../MWare/boot_loader/F2806x_BL_MemCopy.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/F2806x_BL_MemCopy.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/F2806x_CSMPasswords.obj: ../MWare/boot_loader/F2806x_CSMPasswords.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/F2806x_CSMPasswords.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/F2806x_CodeStartBranch.obj: ../MWare/boot_loader/F2806x_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/F2806x_CodeStartBranch.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/bl_check.obj: ../MWare/boot_loader/bl_check.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/bl_check.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/bl_decrypt.obj: ../MWare/boot_loader/bl_decrypt.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/bl_decrypt.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/bl_entrytable.obj: ../MWare/boot_loader/bl_entrytable.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/bl_entrytable.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/bl_usb.obj: ../MWare/boot_loader/bl_usb.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/bl_usb.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

MWare/boot_loader/bl_usbfuncs.obj: ../MWare/boot_loader/bl_usbfuncs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_common/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/AVP32F069_headers/include" --include_path="D:/dsp/AVP32F069_Examples/Example_AVP32F069UsbDevSerial/depreciated" --advice:performance=all --define=DEBUG --define=ccs_c2k -g --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="MWare/boot_loader/bl_usbfuncs.d_raw" --obj_directory="MWare/boot_loader" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


