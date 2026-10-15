################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./device/device.obj" \
"./device/f28003x_codestartbranch.obj" \
"./driverlib/adc.obj" \
"./driverlib/aes.obj" \
"./driverlib/asysctl.obj" \
"./driverlib/bgcrc.obj" \
"./driverlib/can.obj" \
"./driverlib/cla.obj" \
"./driverlib/clb.obj" \
"./driverlib/cmpss.obj" \
"./driverlib/cputimer.obj" \
"./driverlib/dac.obj" \
"./driverlib/dcc.obj" \
"./driverlib/dcsm.obj" \
"./driverlib/dma.obj" \
"./driverlib/ecap.obj" \
"./driverlib/epg.obj" \
"./driverlib/epwm.obj" \
"./driverlib/eqep.obj" \
"./driverlib/erad.obj" \
"./driverlib/flash.obj" \
"./driverlib/fsi.obj" \
"./driverlib/gpio.obj" \
"./driverlib/hic.obj" \
"./driverlib/hrcap.obj" \
"./driverlib/hrpwm.obj" \
"./driverlib/i2c.obj" \
"./driverlib/interrupt.obj" \
"./driverlib/lin.obj" \
"./driverlib/mcan.obj" \
"./driverlib/memcfg.obj" \
"./driverlib/pmbus.obj" \
"./driverlib/sci.obj" \
"./driverlib/sdfm.obj" \
"./driverlib/spi.obj" \
"./driverlib/sysctl.obj" \
"./driverlib/version.obj" \
"./driverlib/xbar.obj" \
"./source/board.obj" \
"./source/c2000ware_libraries.obj" \
"./source/dsp_fpu_rfft_fpu32_f28003x.obj" \
"../28003x_dsp_fpu_rfft.cmd" \
"../c28x_fpu_dsp_library_eabi.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include device/subdir_vars.mk
-include driverlib/subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include device/subdir_rules.mk
-include driverlib/subdir_rules.mk
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
dsp_fpu_rfft_fpu32_f28003x.out \

EXE_OUTPUTS__QUOTED += \
"dsp_fpu_rfft_fpu32_f28003x.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "dsp_fpu_rfft_fpu32_f28003x.out"

# Tool invocations
dsp_fpu_rfft_fpu32_f28003x.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc --advice:performance=all --define=RAM -g --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi -z -m"dsp_fpu_rfft_fpu32_f28003x.map" --stack_size=0x200 --warn_sections -i"C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/lib" -i"C:/TI/ccs12.0/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --reread_libs --define=RAM --define=FFT_ALIGN=1024 --diag_wrap=off --display_error_number --xml_link_info="dsp_fpu_rfft_fpu32_f28003x_linkInfo.xml" --rom_model -o "dsp_fpu_rfft_fpu32_f28003x.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "device\device.obj" "device\f28003x_codestartbranch.obj" "driverlib\adc.obj" "driverlib\aes.obj" "driverlib\asysctl.obj" "driverlib\bgcrc.obj" "driverlib\can.obj" "driverlib\cla.obj" "driverlib\clb.obj" "driverlib\cmpss.obj" "driverlib\cputimer.obj" "driverlib\dac.obj" "driverlib\dcc.obj" "driverlib\dcsm.obj" "driverlib\dma.obj" "driverlib\ecap.obj" "driverlib\epg.obj" "driverlib\epwm.obj" "driverlib\eqep.obj" "driverlib\erad.obj" "driverlib\flash.obj" "driverlib\fsi.obj" "driverlib\gpio.obj" "driverlib\hic.obj" "driverlib\hrcap.obj" "driverlib\hrpwm.obj" "driverlib\i2c.obj" "driverlib\interrupt.obj" "driverlib\lin.obj" "driverlib\mcan.obj" "driverlib\memcfg.obj" "driverlib\pmbus.obj" "driverlib\sci.obj" "driverlib\sdfm.obj" "driverlib\spi.obj" "driverlib\sysctl.obj" "driverlib\version.obj" "driverlib\xbar.obj" "source\board.obj" "source\c2000ware_libraries.obj" "source\dsp_fpu_rfft_fpu32_f28003x.obj" 
	-$(RM) "device\device.d" "driverlib\adc.d" "driverlib\aes.d" "driverlib\asysctl.d" "driverlib\bgcrc.d" "driverlib\can.d" "driverlib\cla.d" "driverlib\clb.d" "driverlib\cmpss.d" "driverlib\cputimer.d" "driverlib\dac.d" "driverlib\dcc.d" "driverlib\dcsm.d" "driverlib\dma.d" "driverlib\ecap.d" "driverlib\epg.d" "driverlib\epwm.d" "driverlib\eqep.d" "driverlib\erad.d" "driverlib\flash.d" "driverlib\fsi.d" "driverlib\gpio.d" "driverlib\hic.d" "driverlib\hrcap.d" "driverlib\hrpwm.d" "driverlib\i2c.d" "driverlib\interrupt.d" "driverlib\lin.d" "driverlib\mcan.d" "driverlib\memcfg.d" "driverlib\pmbus.d" "driverlib\sci.d" "driverlib\sdfm.d" "driverlib\spi.d" "driverlib\sysctl.d" "driverlib\version.d" "driverlib\xbar.d" "source\board.d" "source\c2000ware_libraries.d" "source\dsp_fpu_rfft_fpu32_f28003x.d" 
	-$(RM) "device\f28003x_codestartbranch.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

