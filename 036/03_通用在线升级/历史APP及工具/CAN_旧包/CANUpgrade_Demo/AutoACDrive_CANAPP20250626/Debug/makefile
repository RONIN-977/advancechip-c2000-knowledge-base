################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Function/CANcomm.obj" \
"./Function/CommVariable.obj" \
"./Function/Command.obj" \
"./Function/FunctionMain.obj" \
"./Function/Parameter.obj" \
"./Function/Reference.obj" \
"./Function/SPIcomm.obj" \
"./Main/DSP03x_CSMPasswords.obj" \
"./Main/DSP03x_CodeStartBranch.obj" \
"./Main/DSP03x_CpuTimers.obj" \
"./Main/DSP03x_GlobalVariableDefs.obj" \
"./Main/DSP03x_usDelay.obj" \
"./Main/HardwareSetup.obj" \
"./Main/MainSystem.obj" \
"../Motor/obj/PWM.obj" \
"../Motor/obj/SVC.obj" \
"./Motor/src/IPD.obj" \
"./Motor/src/InfoCollect.obj" \
"./Motor/src/InvProtect.obj" \
"./Motor/src/MCCall.obj" \
"./Motor/src/MCExchange.obj" \
"./Motor/src/MCMain.obj" \
"./Motor/src/MCVF.obj" \
"./Motor/src/MCVar.obj" \
"./Motor/src/Tune.obj" \
"./SubProgra/ASMSubPrg.obj" \
"./SubProgra/Record.obj" \
"./SubProgra/SubPrg.obj" \
"../DSP28036Peripheral.cmd" \
"../F28035_BOOTLOAD.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Function/subdir_vars.mk
-include Main/subdir_vars.mk
-include Motor/obj/subdir_vars.mk
-include Motor/src/subdir_vars.mk
-include SubProgra/subdir_vars.mk
-include subdir_rules.mk
-include Function/subdir_rules.mk
-include Main/subdir_rules.mk
-include Motor/obj/subdir_rules.mk
-include Motor/src/subdir_rules.mk
-include SubProgra/subdir_rules.mk
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
AutoACDrive400V036.out 

EXE_OUTPUTS__QUOTED += \
"AutoACDrive400V036.out" 

BIN_OUTPUTS += \
AutoACDrive400V036.hex 

BIN_OUTPUTS__QUOTED += \
"AutoACDrive400V036.hex" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "AutoACDrive400V036.out" secondary-outputs

# Tool invocations
AutoACDrive400V036.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -Ooff --opt_for_speed=2 --advice:performance=all --define=REFRIGERATOR --define=OPEN_LOOP_START_N --define=SF_INDEPAND_OF_FREQUENCY_N --define=IPD_PERIOD_AUTO_DECT_N --define=CUT_DOWN_VER_HARDWARE --define=INTER_OSC_N --define=FREQUENCY_CONTROL_01HZ --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"AutoACDrive400V036.map" --stack_size=0x300 --warn_sections -i"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/lib" -i"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="AutoACDrive400V036_linkInfo.xml" --entry_point=code_start --rom_model -o "AutoACDrive400V036.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

AutoACDrive400V036.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/hex2000" --diag_wrap=off -o "AutoACDrive400V036.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Function\CANcomm.obj" "Function\CommVariable.obj" "Function\Command.obj" "Function\FunctionMain.obj" "Function\Parameter.obj" "Function\Reference.obj" "Function\SPIcomm.obj" "Main\DSP03x_CSMPasswords.obj" "Main\DSP03x_CodeStartBranch.obj" "Main\DSP03x_CpuTimers.obj" "Main\DSP03x_GlobalVariableDefs.obj" "Main\DSP03x_usDelay.obj" "Main\HardwareSetup.obj" "Main\MainSystem.obj" "Motor\src\IPD.obj" "Motor\src\InfoCollect.obj" "Motor\src\InvProtect.obj" "Motor\src\MCCall.obj" "Motor\src\MCExchange.obj" "Motor\src\MCMain.obj" "Motor\src\MCVF.obj" "Motor\src\MCVar.obj" "Motor\src\Tune.obj" "SubProgra\ASMSubPrg.obj" "SubProgra\Record.obj" "SubProgra\SubPrg.obj" 
	-$(RM) "Function\CANcomm.d" "Function\CommVariable.d" "Function\Command.d" "Function\FunctionMain.d" "Function\Parameter.d" "Function\Reference.d" "Function\SPIcomm.d" "Main\DSP03x_CpuTimers.d" "Main\DSP03x_GlobalVariableDefs.d" "Main\HardwareSetup.d" "Main\MainSystem.d" "Motor\src\IPD.d" "Motor\src\InfoCollect.d" "Motor\src\InvProtect.d" "Motor\src\MCCall.d" "Motor\src\MCExchange.d" "Motor\src\MCMain.d" "Motor\src\MCVF.d" "Motor\src\MCVar.d" "Motor\src\Tune.d" "SubProgra\Record.d" "SubProgra\SubPrg.d" 
	-$(RM) "Main\DSP03x_CSMPasswords.d" "Main\DSP03x_CodeStartBranch.d" "Main\DSP03x_usDelay.d" "SubProgra\ASMSubPrg.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

