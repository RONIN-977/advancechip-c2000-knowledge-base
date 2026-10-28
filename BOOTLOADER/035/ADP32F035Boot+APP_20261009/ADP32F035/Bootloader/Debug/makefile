################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./flash_port.obj" \
"./main.obj" \
"./Common/boot_core.obj" \
"./Common/boot_io.obj" \
"./Common/sci_port.obj" \
"./Common/update_service.obj" \
"./Device/source/DSP2803x_CodeStartBranch.obj" \
"./Device/source/DSP2803x_DefaultIsr.obj" \
"./Device/source/DSP2803x_GlobalVariableDefs.obj" \
"./Device/source/DSP2803x_PieCtrl.obj" \
"./Device/source/DSP2803x_PieVect.obj" \
"./Device/source/DSP2803x_SysCtrl.obj" \
"./Device/source/DSP2803x_usDelay.obj" \
"../bootloader.cmd" \
"../Device/DSP2803x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l2803x_FlashAPI_BootROMSymbols.lib \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Common/subdir_vars.mk
-include Device/subdir_vars.mk
-include Device/source/subdir_vars.mk
-include subdir_rules.mk
-include Common/subdir_rules.mk
-include Device/subdir_rules.mk
-include Device/source/subdir_rules.mk
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
ADP32F035_Bootloader.out 

EXE_OUTPUTS__QUOTED += \
"ADP32F035_Bootloader.out" 


# All Target
all: ADP32F035_Bootloader.out

# Tool invocations
ADP32F035_Bootloader.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -O0 -g --c99 --diag_warning=225 --emit_warnings_as_errors --gen_func_subsections=on --abi=coffabi -z -m"ADP32F035_Bootloader.map" --stack_size=0x300 --warn_sections -i"../Device/lib" -i"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/lib" -i"../Common" -i"../../Common" --reread_libs --diag_suppress=10063 --xml_link_info="ADP32F035_Bootloader_linkInfo.xml" --entry_point=code_start --rom_model -o "ADP32F035_Bootloader.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "flash_port.obj" "main.obj" "Common\boot_core.obj" "Common\boot_io.obj" "Common\sci_port.obj" "Common\update_service.obj" "Device\source\DSP2803x_CodeStartBranch.obj" "Device\source\DSP2803x_DefaultIsr.obj" "Device\source\DSP2803x_GlobalVariableDefs.obj" "Device\source\DSP2803x_PieCtrl.obj" "Device\source\DSP2803x_PieVect.obj" "Device\source\DSP2803x_SysCtrl.obj" "Device\source\DSP2803x_usDelay.obj" 
	-$(RM) "flash_port.d" "main.d" "Common\boot_core.d" "Common\boot_io.d" "Common\sci_port.d" "Common\update_service.d" "Device\source\DSP2803x_DefaultIsr.d" "Device\source\DSP2803x_GlobalVariableDefs.d" "Device\source\DSP2803x_PieCtrl.d" "Device\source\DSP2803x_PieVect.d" "Device\source\DSP2803x_SysCtrl.d" 
	-$(RM) "Device\source\DSP2803x_CodeStartBranch.d" "Device\source\DSP2803x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

