################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti-cgt-c2000_20.2.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Function/CommInterface.obj" \
"./Function/Command.obj" \
"./Function/FunctionMain.obj" \
"./Function/LinLDF.obj" \
"./Function/PWMCommFunc.obj" \
"./Function/PreDrive.obj" \
"./Function/Reference.obj" \
"./Function/SPIcomm.obj" \
"./Function/Parameters/Parameter_LuHua_FAN.obj" \
"./Main/ADP32F03x_CSMPasswords.obj" \
"./Main/ADP32F03x_CodeStartBranch.obj" \
"./Main/ADP32F03x_CpuTimers.obj" \
"./Main/ADP32F03x_GlobalVariableDefs.obj" \
"./Main/ADP32F03x_usDelay.obj" \
"./Main/HardwareSetup.obj" \
"./Main/MainSystem.obj" \
"../Motor/obj/IPD.obj" \
"../Motor/obj/PWM.obj" \
"../Motor/obj/SVC_v1.02_20251227.obj" \
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
"../DSP28035Peripheral.cmd" \
"../F28036.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Function/subdir_vars.mk
-include Function/Parameters/subdir_vars.mk
-include Main/subdir_vars.mk
-include Motor/obj/subdir_vars.mk
-include Motor/src/subdir_vars.mk
-include SubProgra/subdir_vars.mk
-include subdir_rules.mk
-include Function/subdir_rules.mk
-include Function/Parameters/subdir_rules.mk
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
ADM32F036A3DEMO_base.out 

EXE_OUTPUTS__QUOTED += \
"ADM32F036A3DEMO_base.out" 

BIN_OUTPUTS += \
ADM32F036A3DEMO_base.hex 

BIN_OUTPUTS__QUOTED += \
"ADM32F036A3DEMO_base.hex" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ADM32F036A3DEMO_base.out" secondary-outputs

# Tool invocations
ADM32F036A3DEMO_base.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -O2 --opt_for_speed=3 --advice:performance=all --define=SHORT_GND_TEST_N --define=EXTINT_PWMGETEN --define=IPD_PERIOD_AUTO_DECT_n --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT_N --define=FREQUENCY_CONTROL_01HZ --define=POSTIVE_DRIVE_LOGIC --define=OPEN_LOOP_START --define=FULL_OPEN_LOOP_MODE11 --define=LIN_CONTROL_MODE --define=INTER_OSC11 --define="LARGE_MODEL" --define=SPIN_CONTROL_WITH_BEMF --define=OV_LIMIT_WITH_ID_n -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"ADM32F036A3DEMO_base.map" --stack_size=0x300 --warn_sections -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/lib" -i"C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --reread_libs --diag_suppress=10063 --diag_wrap=off --display_error_number --xml_link_info="ADM32F036A3DEMO_base_linkInfo.xml" --entry_point=code_start --rom_model -o "ADM32F036A3DEMO_base.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

ADM32F036A3DEMO_base.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/hex2000" --memwidth=16 --romwidth=16 --diag_wrap=off --intel -o "ADM32F036A3DEMO_base.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Function\CommInterface.obj" "Function\Command.obj" "Function\FunctionMain.obj" "Function\LinLDF.obj" "Function\PWMCommFunc.obj" "Function\PreDrive.obj" "Function\Reference.obj" "Function\SPIcomm.obj" "Function\Parameters\Parameter_LuHua_FAN.obj" "Main\ADP32F03x_CSMPasswords.obj" "Main\ADP32F03x_CodeStartBranch.obj" "Main\ADP32F03x_CpuTimers.obj" "Main\ADP32F03x_GlobalVariableDefs.obj" "Main\ADP32F03x_usDelay.obj" "Main\HardwareSetup.obj" "Main\MainSystem.obj" "Motor\src\InfoCollect.obj" "Motor\src\InvProtect.obj" "Motor\src\MCCall.obj" "Motor\src\MCExchange.obj" "Motor\src\MCMain.obj" "Motor\src\MCVF.obj" "Motor\src\MCVar.obj" "Motor\src\Tune.obj" "SubProgra\ASMSubPrg.obj" "SubProgra\Record.obj" "SubProgra\SubPrg.obj" 
	-$(RM) "Function\CommInterface.d" "Function\Command.d" "Function\FunctionMain.d" "Function\LinLDF.d" "Function\PWMCommFunc.d" "Function\PreDrive.d" "Function\Reference.d" "Function\SPIcomm.d" "Function\Parameters\Parameter_LuHua_FAN.d" "Main\ADP32F03x_CpuTimers.d" "Main\ADP32F03x_GlobalVariableDefs.d" "Main\HardwareSetup.d" "Main\MainSystem.d" "Motor\src\InfoCollect.d" "Motor\src\InvProtect.d" "Motor\src\MCCall.d" "Motor\src\MCExchange.d" "Motor\src\MCMain.d" "Motor\src\MCVF.d" "Motor\src\MCVar.d" "Motor\src\Tune.d" "SubProgra\Record.d" "SubProgra\SubPrg.d" 
	-$(RM) "Main\ADP32F03x_CSMPasswords.d" "Main\ADP32F03x_CodeStartBranch.d" "Main\ADP32F03x_usDelay.d" "SubProgra\ASMSubPrg.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-C:/ti/ccs1271/ccs/utils/tiobj2bin/tiobj2bin ADM32F036A3DEMO_base.out ADM32F036A3DEMO_base.bin C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/ofd2000 C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/hex2000 C:/ti/ccs1271/ccs/utils/tiobj2bin/mkhex4bin
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

