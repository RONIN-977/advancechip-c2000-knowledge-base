################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti-cgt-c2000_20.2.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Library/src/AVP33x_ADC_cal.obj" \
"./Library/src/AVP33x_Adc.obj" \
"./Library/src/AVP33x_CSMPasswords.obj" \
"./Library/src/AVP33x_CodeStartBranch.obj" \
"./Library/src/AVP33x_CpuTimers.obj" \
"./Library/src/AVP33x_DBGIER.obj" \
"./Library/src/AVP33x_DMA.obj" \
"./Library/src/AVP33x_DefaultIsr.obj" \
"./Library/src/AVP33x_DisInt.obj" \
"./Library/src/AVP33x_ECan.obj" \
"./Library/src/AVP33x_ECap.obj" \
"./Library/src/AVP33x_EPwm.obj" \
"./Library/src/AVP33x_EQep.obj" \
"./Library/src/AVP33x_GlobalVariableDefs.obj" \
"./Library/src/AVP33x_Gpio.obj" \
"./Library/src/AVP33x_I2C.obj" \
"./Library/src/AVP33x_Mcbsp.obj" \
"./Library/src/AVP33x_MemCopy.obj" \
"./Library/src/AVP33x_PieCtrl.obj" \
"./Library/src/AVP33x_PieVect.obj" \
"./Library/src/AVP33x_Sci.obj" \
"./Library/src/AVP33x_Spi.obj" \
"./Library/src/AVP33x_SysCtrl.obj" \
"./Library/src/AVP33x_Xintf.obj" \
"./Library/src/AVP33x_usDelay.obj" \
"./Main/FunctionMain.obj" \
"./Main/HardwareSetup.obj" \
"./Main/Main.obj" \
"./Main/RemoteModbus.obj" \
"../Library/cmd/AVP32F335_APP.cmd" \
"../Library/cmd/AVP33x_Headers_nonBIOS.cmd" \
"../Library/lib/Flash28335_API_V210.lib" \
"../Library/lib/SFO_TI_Build.lib" \
"../Library/lib/SFO_TI_Build_V5.lib" \
"../Library/lib/SFO_TI_Build_V5B.lib" \
"../Library/lib/SFO_TI_Build_V5B_fpu.lib" \
"../Library/lib/SFO_TI_Build_V5_fpu.lib" \
"../Library/lib/SFO_TI_Build_fpu.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include Library/cmd/subdir_vars.mk
-include Library/lib/subdir_vars.mk
-include Library/src/subdir_vars.mk
-include Main/subdir_vars.mk
-include Library/cmd/subdir_rules.mk
-include Library/lib/subdir_rules.mk
-include Library/src/subdir_rules.mk
-include Main/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
AVP32F335_APP_sci.out 

EXE_OUTPUTS__QUOTED += \
"AVP32F335_APP_sci.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "AVP32F335_APP_sci.out"

# Tool invocations
AVP32F335_APP_sci.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"AVP32F335_APP_sci.map" --stack_size=0x300 --warn_sections -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/lib" -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="AVP32F335_APP_sci_linkInfo.xml" --rom_model -o "AVP32F335_APP_sci.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "Library\src\AVP33x_ADC_cal.obj" "Library\src\AVP33x_Adc.obj" "Library\src\AVP33x_CSMPasswords.obj" "Library\src\AVP33x_CodeStartBranch.obj" "Library\src\AVP33x_CpuTimers.obj" "Library\src\AVP33x_DBGIER.obj" "Library\src\AVP33x_DMA.obj" "Library\src\AVP33x_DefaultIsr.obj" "Library\src\AVP33x_DisInt.obj" "Library\src\AVP33x_ECan.obj" "Library\src\AVP33x_ECap.obj" "Library\src\AVP33x_EPwm.obj" "Library\src\AVP33x_EQep.obj" "Library\src\AVP33x_GlobalVariableDefs.obj" "Library\src\AVP33x_Gpio.obj" "Library\src\AVP33x_I2C.obj" "Library\src\AVP33x_Mcbsp.obj" "Library\src\AVP33x_MemCopy.obj" "Library\src\AVP33x_PieCtrl.obj" "Library\src\AVP33x_PieVect.obj" "Library\src\AVP33x_Sci.obj" "Library\src\AVP33x_Spi.obj" "Library\src\AVP33x_SysCtrl.obj" "Library\src\AVP33x_Xintf.obj" "Library\src\AVP33x_usDelay.obj" "Main\FunctionMain.obj" "Main\HardwareSetup.obj" "Main\Main.obj" "Main\RemoteModbus.obj" 
	-$(RM) "Library\src\AVP33x_Adc.d" "Library\src\AVP33x_CpuTimers.d" "Library\src\AVP33x_DMA.d" "Library\src\AVP33x_DefaultIsr.d" "Library\src\AVP33x_ECan.d" "Library\src\AVP33x_ECap.d" "Library\src\AVP33x_EPwm.d" "Library\src\AVP33x_EQep.d" "Library\src\AVP33x_GlobalVariableDefs.d" "Library\src\AVP33x_Gpio.d" "Library\src\AVP33x_I2C.d" "Library\src\AVP33x_Mcbsp.d" "Library\src\AVP33x_MemCopy.d" "Library\src\AVP33x_PieCtrl.d" "Library\src\AVP33x_PieVect.d" "Library\src\AVP33x_Sci.d" "Library\src\AVP33x_Spi.d" "Library\src\AVP33x_SysCtrl.d" "Library\src\AVP33x_Xintf.d" "Main\FunctionMain.d" "Main\HardwareSetup.d" "Main\Main.d" "Main\RemoteModbus.d" 
	-$(RM) "Library\src\AVP33x_ADC_cal.d" "Library\src\AVP33x_CSMPasswords.d" "Library\src\AVP33x_CodeStartBranch.d" "Library\src\AVP33x_DBGIER.d" "Library\src\AVP33x_DisInt.d" "Library\src\AVP33x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/ti/ccs1271/ccs/utils/tiobj2bin/tiobj2bin.bat" "AVP32F335_APP_sci.out" "AVP32F335_APP_sci.bin" "C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/ofd2000.exe" "C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/hex2000.exe" "C:/ti/ccs1271/ccs/utils/tiobj2bin/mkhex4bin.exe"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

