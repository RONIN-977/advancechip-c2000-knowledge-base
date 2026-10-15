################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/AVP32F08_ADC_cal.obj: ../src/AVP32F08_ADC_cal.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_ADC_cal.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_Adc.obj: ../src/AVP32F08_Adc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_Adc.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_CSMPasswords.obj: ../src/AVP32F08_CSMPasswords.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_CSMPasswords.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_CodeStartBranch.obj: ../src/AVP32F08_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_CodeStartBranch.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_CpuTimers.obj: ../src/AVP32F08_CpuTimers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_CpuTimers.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_DBGIER.obj: ../src/AVP32F08_DBGIER.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_DBGIER.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_DMA.obj: ../src/AVP32F08_DMA.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_DMA.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_DefaultIsr.obj: ../src/AVP32F08_DefaultIsr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_DefaultIsr.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_DisInt.obj: ../src/AVP32F08_DisInt.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_DisInt.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_ECan.obj: ../src/AVP32F08_ECan.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_ECan.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_ECap.obj: ../src/AVP32F08_ECap.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_ECap.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_EPwm.obj: ../src/AVP32F08_EPwm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_EPwm.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_EQep.obj: ../src/AVP32F08_EQep.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_EQep.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_GlobalVariableDefs.obj: ../src/AVP32F08_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_GlobalVariableDefs.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_Gpio.obj: ../src/AVP32F08_Gpio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_Gpio.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_I2C.obj: ../src/AVP32F08_I2C.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_I2C.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_Mcbsp.obj: ../src/AVP32F08_Mcbsp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_Mcbsp.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_MemCopy.obj: ../src/AVP32F08_MemCopy.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_MemCopy.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_PieCtrl.obj: ../src/AVP32F08_PieCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_PieCtrl.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_PieVect.obj: ../src/AVP32F08_PieVect.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_PieVect.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_Sci.obj: ../src/AVP32F08_Sci.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_Sci.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_Spi.obj: ../src/AVP32F08_Spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_Spi.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_SysCtrl.obj: ../src/AVP32F08_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_SysCtrl.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_Xintf.obj: ../src/AVP32F08_Xintf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_Xintf.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/AVP32F08_usDelay.obj: ../src/AVP32F08_usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" --include_path="D:/dsp/AVP32F08_Examples/Example_AVP32F08_CAN/Example_AVP32F08_CAN_Back2Back/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="src/AVP32F08_usDelay.d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


