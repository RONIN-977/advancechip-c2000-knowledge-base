################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti-cgt-c2000_20.2.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Function/CommInterface.obj" \
"./Function/FunctionMain.obj" \
"./Function/Parameter.obj" \
"./Function/PreDrive.obj" \
"./Main/ADP32F03x_CSMPasswords.obj" \
"./Main/ADP32F03x_CodeStartBranch.obj" \
"./Main/ADP32F03x_CpuTimers.obj" \
"./Main/ADP32F03x_GlobalVariableDefs.obj" \
"./Main/ADP32F03x_usDelay.obj" \
"./Main/HardwareSetup.obj" \
"./Main/MainSystem.obj" \
"./Motor/src/MotorControl.obj" \
"./SubProgra/ASMFun.obj" \
"./SubProgra/SubPrg.obj" \
"../ADP32F03xPeripheral.cmd" \
"../F28036LINAPP.cmd" \
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
-include Motor/src/subdir_vars.mk
-include SubProgra/subdir_vars.mk
-include subdir_rules.mk
-include Function/subdir_rules.mk
-include Main/subdir_rules.mk
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
ADM32F036_VF.out 

EXE_OUTPUTS__QUOTED += \
"ADM32F036_VF.out" 

BIN_OUTPUTS += \
ADM32F036_VF.hex 

BIN_OUTPUTS__QUOTED += \
"ADM32F036_VF.hex" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ADM32F036_VF.out" secondary-outputs

# Tool invocations
ADM32F036_VF.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -Ooff --opt_for_speed=3 --advice:performance=all --define=SHORT_GND_TEST_N --define=IPD_PERIOD_AUTO_DECT_N --define=LOW_NOISE_PWM_MODE_N --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT_N --define=FREQUENCY_CONTROL_01HZ --define=CUT_DOWN_VER_HARDWARE --define=OPEN_LOOP_START_N --define=FULL_OPEN_LOOP_MODE_N --define=LIN_CONTROL_MODE --define=INTER_OSC11 --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"ADM32F036_VF.map" --stack_size=0x300 --warn_sections -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/lib" -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="ADM32F036_VF_linkInfo.xml" --entry_point=code_start --rom_model -o "ADM32F036_VF.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

ADM32F036_VF.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/hex2000" --memwidth=16 --romwidth=16 --diag_wrap=off -o "ADM32F036_VF.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Function\CommInterface.obj" "Function\FunctionMain.obj" "Function\Parameter.obj" "Function\PreDrive.obj" "Main\ADP32F03x_CSMPasswords.obj" "Main\ADP32F03x_CodeStartBranch.obj" "Main\ADP32F03x_CpuTimers.obj" "Main\ADP32F03x_GlobalVariableDefs.obj" "Main\ADP32F03x_usDelay.obj" "Main\HardwareSetup.obj" "Main\MainSystem.obj" "Motor\src\MotorControl.obj" "SubProgra\ASMFun.obj" "SubProgra\SubPrg.obj" 
	-$(RM) "Function\CommInterface.d" "Function\FunctionMain.d" "Function\Parameter.d" "Function\PreDrive.d" "Main\ADP32F03x_CpuTimers.d" "Main\ADP32F03x_GlobalVariableDefs.d" "Main\HardwareSetup.d" "Main\MainSystem.d" "Motor\src\MotorControl.d" "SubProgra\SubPrg.d" 
	-$(RM) "Main\ADP32F03x_CSMPasswords.d" "Main\ADP32F03x_CodeStartBranch.d" "Main\ADP32F03x_usDelay.d" "SubProgra\ASMFun.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-C:/ti/ccs1271/ccs/utils/tiobj2bin/tiobj2bin ADM32F036_VF.out ADM32F036_VF.bin C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/ofd2000 C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/hex2000 C:/ti/ccs1271/ccs/utils/tiobj2bin/mkhex4bin
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

