################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./AD.obj" \
"./AIP650_IIC.obj" \
"./Delay.obj" \
"./Example_ADP32F03xAD.obj" \
"./printf_uart.obj" \
"./segscan.obj" \
"./timer0_base.obj" \
"./source/AVP32F069_Adc.obj" \
"./source/AVP32F069_CSMPasswords.obj" \
"./source/AVP32F069_CodeStartBranch.obj" \
"./source/AVP32F069_Comp.obj" \
"./source/AVP32F069_CpuTimers.obj" \
"./source/AVP32F069_DBGIER.obj" \
"./source/AVP32F069_DefaultIsr.obj" \
"./source/AVP32F069_DisInt.obj" \
"./source/AVP32F069_Dma.obj" \
"./source/AVP32F069_ECan.obj" \
"./source/AVP32F069_ECap.obj" \
"./source/AVP32F069_EPwm.obj" \
"./source/AVP32F069_EQep.obj" \
"./source/AVP32F069_GlobalVariableDefs.obj" \
"./source/AVP32F069_Gpio.obj" \
"./source/AVP32F069_HRCap.obj" \
"./source/AVP32F069_I2C.obj" \
"./source/AVP32F069_Mcbsp.obj" \
"./source/AVP32F069_OscComp.obj" \
"./source/AVP32F069_PieCtrl.obj" \
"./source/AVP32F069_PieVect.obj" \
"./source/AVP32F069_Sci.obj" \
"./source/AVP32F069_Spi.obj" \
"./source/AVP32F069_SysCtrl.obj" \
"./source/AVP32F069_TempSensorConv.obj" \
"./source/AVP32F069_usDelay.obj" \
"../cmd/ADP32F035_FLASH.cmd" \
"../cmd/AVP32F069_Headers_nonBIOS.cmd" \
"../lib/2806x_BootROM_API_TABLE_Symbols.lib" \
"../lib/2806x_BootROM_API_TABLE_Symbols_fpu32.lib" \
"../lib/SFO_TI_Build_V6b.lib" \
"../lib/SFO_TI_Build_V6b_FPU.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include cmd/subdir_vars.mk
-include lib/subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include cmd/subdir_rules.mk
-include lib/subdir_rules.mk
-include source/subdir_rules.mk
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
Example_ADP32F03xAD.out 

EXE_OUTPUTS__QUOTED += \
"Example_ADP32F03xAD.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Example_ADP32F03xAD.out"

# Tool invocations
Example_ADP32F03xAD.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" --silicon_version=28 -g --diag_warning=225 --display_error_number --large_memory_model --unified_memory --cla_support=cla0 -z -m"Example_ADP32F03xAD.map" --stack_size=0x300 --heap_size=0x400 --display_error_number --warn_sections -i"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/lib" -i"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --reread_libs --xml_link_info="Example_ADP32F03xAD_linkInfo.xml" --rom_model -o "Example_ADP32F03xAD.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "AD.obj" "AIP650_IIC.obj" "Delay.obj" "Example_ADP32F03xAD.obj" "printf_uart.obj" "segscan.obj" "timer0_base.obj" "source\AVP32F069_Adc.obj" "source\AVP32F069_CSMPasswords.obj" "source\AVP32F069_CodeStartBranch.obj" "source\AVP32F069_Comp.obj" "source\AVP32F069_CpuTimers.obj" "source\AVP32F069_DBGIER.obj" "source\AVP32F069_DefaultIsr.obj" "source\AVP32F069_DisInt.obj" "source\AVP32F069_Dma.obj" "source\AVP32F069_ECan.obj" "source\AVP32F069_ECap.obj" "source\AVP32F069_EPwm.obj" "source\AVP32F069_EQep.obj" "source\AVP32F069_GlobalVariableDefs.obj" "source\AVP32F069_Gpio.obj" "source\AVP32F069_HRCap.obj" "source\AVP32F069_I2C.obj" "source\AVP32F069_Mcbsp.obj" "source\AVP32F069_OscComp.obj" "source\AVP32F069_PieCtrl.obj" "source\AVP32F069_PieVect.obj" "source\AVP32F069_Sci.obj" "source\AVP32F069_Spi.obj" "source\AVP32F069_SysCtrl.obj" "source\AVP32F069_TempSensorConv.obj" "source\AVP32F069_usDelay.obj" 
	-$(RM) "AD.d" "AIP650_IIC.d" "Delay.d" "Example_ADP32F03xAD.d" "printf_uart.d" "segscan.d" "timer0_base.d" "source\AVP32F069_Adc.d" "source\AVP32F069_Comp.d" "source\AVP32F069_CpuTimers.d" "source\AVP32F069_DefaultIsr.d" "source\AVP32F069_Dma.d" "source\AVP32F069_ECan.d" "source\AVP32F069_ECap.d" "source\AVP32F069_EPwm.d" "source\AVP32F069_EQep.d" "source\AVP32F069_GlobalVariableDefs.d" "source\AVP32F069_Gpio.d" "source\AVP32F069_HRCap.d" "source\AVP32F069_I2C.d" "source\AVP32F069_Mcbsp.d" "source\AVP32F069_OscComp.d" "source\AVP32F069_PieCtrl.d" "source\AVP32F069_PieVect.d" "source\AVP32F069_Sci.d" "source\AVP32F069_Spi.d" "source\AVP32F069_SysCtrl.d" "source\AVP32F069_TempSensorConv.d" 
	-$(RM) "source\AVP32F069_CSMPasswords.d" "source\AVP32F069_CodeStartBranch.d" "source\AVP32F069_DBGIER.d" "source\AVP32F069_DisInt.d" "source\AVP32F069_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

