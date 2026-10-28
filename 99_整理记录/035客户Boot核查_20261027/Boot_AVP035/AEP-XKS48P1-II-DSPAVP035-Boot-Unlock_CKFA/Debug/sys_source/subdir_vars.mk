################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../sys_source/Example_Flash2806x_CsmKeys.asm \
../sys_source/F2806x_CSMPasswords.asm \
../sys_source/F2806x_CodeStartBranch.asm \
../sys_source/F2806x_DBGIER.asm \
../sys_source/F2806x_DisInt.asm \
../sys_source/F2806x_usDelay.asm 

C_SRCS += \
../sys_source/DSP2803x_MemCopy.c \
../sys_source/DSP2803x_SysCtrl.c \
../sys_source/F2806x_Adc.c \
../sys_source/F2806x_Comp.c \
../sys_source/F2806x_CpuTimers.c \
../sys_source/F2806x_DefaultIsr.c \
../sys_source/F2806x_Dma.c \
../sys_source/F2806x_ECan.c \
../sys_source/F2806x_ECap.c \
../sys_source/F2806x_EPwm.c \
../sys_source/F2806x_EQep.c \
../sys_source/F2806x_GlobalVariableDefs.c \
../sys_source/F2806x_Gpio.c \
../sys_source/F2806x_HRCap.c \
../sys_source/F2806x_I2C.c \
../sys_source/F2806x_Mcbsp.c \
../sys_source/F2806x_OscComp.c \
../sys_source/F2806x_PieCtrl.c \
../sys_source/F2806x_PieVect.c \
../sys_source/F2806x_Sci.c \
../sys_source/F2806x_Spi.c \
../sys_source/F2806x_TempSensorConv.c 

C_DEPS += \
./sys_source/DSP2803x_MemCopy.d \
./sys_source/DSP2803x_SysCtrl.d \
./sys_source/F2806x_Adc.d \
./sys_source/F2806x_Comp.d \
./sys_source/F2806x_CpuTimers.d \
./sys_source/F2806x_DefaultIsr.d \
./sys_source/F2806x_Dma.d \
./sys_source/F2806x_ECan.d \
./sys_source/F2806x_ECap.d \
./sys_source/F2806x_EPwm.d \
./sys_source/F2806x_EQep.d \
./sys_source/F2806x_GlobalVariableDefs.d \
./sys_source/F2806x_Gpio.d \
./sys_source/F2806x_HRCap.d \
./sys_source/F2806x_I2C.d \
./sys_source/F2806x_Mcbsp.d \
./sys_source/F2806x_OscComp.d \
./sys_source/F2806x_PieCtrl.d \
./sys_source/F2806x_PieVect.d \
./sys_source/F2806x_Sci.d \
./sys_source/F2806x_Spi.d \
./sys_source/F2806x_TempSensorConv.d 

OBJS += \
./sys_source/DSP2803x_MemCopy.obj \
./sys_source/DSP2803x_SysCtrl.obj \
./sys_source/Example_Flash2806x_CsmKeys.obj \
./sys_source/F2806x_Adc.obj \
./sys_source/F2806x_CSMPasswords.obj \
./sys_source/F2806x_CodeStartBranch.obj \
./sys_source/F2806x_Comp.obj \
./sys_source/F2806x_CpuTimers.obj \
./sys_source/F2806x_DBGIER.obj \
./sys_source/F2806x_DefaultIsr.obj \
./sys_source/F2806x_DisInt.obj \
./sys_source/F2806x_Dma.obj \
./sys_source/F2806x_ECan.obj \
./sys_source/F2806x_ECap.obj \
./sys_source/F2806x_EPwm.obj \
./sys_source/F2806x_EQep.obj \
./sys_source/F2806x_GlobalVariableDefs.obj \
./sys_source/F2806x_Gpio.obj \
./sys_source/F2806x_HRCap.obj \
./sys_source/F2806x_I2C.obj \
./sys_source/F2806x_Mcbsp.obj \
./sys_source/F2806x_OscComp.obj \
./sys_source/F2806x_PieCtrl.obj \
./sys_source/F2806x_PieVect.obj \
./sys_source/F2806x_Sci.obj \
./sys_source/F2806x_Spi.obj \
./sys_source/F2806x_TempSensorConv.obj \
./sys_source/F2806x_usDelay.obj 

ASM_DEPS += \
./sys_source/Example_Flash2806x_CsmKeys.d \
./sys_source/F2806x_CSMPasswords.d \
./sys_source/F2806x_CodeStartBranch.d \
./sys_source/F2806x_DBGIER.d \
./sys_source/F2806x_DisInt.d \
./sys_source/F2806x_usDelay.d 

OBJS__QUOTED += \
"sys_source\DSP2803x_MemCopy.obj" \
"sys_source\DSP2803x_SysCtrl.obj" \
"sys_source\Example_Flash2806x_CsmKeys.obj" \
"sys_source\F2806x_Adc.obj" \
"sys_source\F2806x_CSMPasswords.obj" \
"sys_source\F2806x_CodeStartBranch.obj" \
"sys_source\F2806x_Comp.obj" \
"sys_source\F2806x_CpuTimers.obj" \
"sys_source\F2806x_DBGIER.obj" \
"sys_source\F2806x_DefaultIsr.obj" \
"sys_source\F2806x_DisInt.obj" \
"sys_source\F2806x_Dma.obj" \
"sys_source\F2806x_ECan.obj" \
"sys_source\F2806x_ECap.obj" \
"sys_source\F2806x_EPwm.obj" \
"sys_source\F2806x_EQep.obj" \
"sys_source\F2806x_GlobalVariableDefs.obj" \
"sys_source\F2806x_Gpio.obj" \
"sys_source\F2806x_HRCap.obj" \
"sys_source\F2806x_I2C.obj" \
"sys_source\F2806x_Mcbsp.obj" \
"sys_source\F2806x_OscComp.obj" \
"sys_source\F2806x_PieCtrl.obj" \
"sys_source\F2806x_PieVect.obj" \
"sys_source\F2806x_Sci.obj" \
"sys_source\F2806x_Spi.obj" \
"sys_source\F2806x_TempSensorConv.obj" \
"sys_source\F2806x_usDelay.obj" 

C_DEPS__QUOTED += \
"sys_source\DSP2803x_MemCopy.d" \
"sys_source\DSP2803x_SysCtrl.d" \
"sys_source\F2806x_Adc.d" \
"sys_source\F2806x_Comp.d" \
"sys_source\F2806x_CpuTimers.d" \
"sys_source\F2806x_DefaultIsr.d" \
"sys_source\F2806x_Dma.d" \
"sys_source\F2806x_ECan.d" \
"sys_source\F2806x_ECap.d" \
"sys_source\F2806x_EPwm.d" \
"sys_source\F2806x_EQep.d" \
"sys_source\F2806x_GlobalVariableDefs.d" \
"sys_source\F2806x_Gpio.d" \
"sys_source\F2806x_HRCap.d" \
"sys_source\F2806x_I2C.d" \
"sys_source\F2806x_Mcbsp.d" \
"sys_source\F2806x_OscComp.d" \
"sys_source\F2806x_PieCtrl.d" \
"sys_source\F2806x_PieVect.d" \
"sys_source\F2806x_Sci.d" \
"sys_source\F2806x_Spi.d" \
"sys_source\F2806x_TempSensorConv.d" 

ASM_DEPS__QUOTED += \
"sys_source\Example_Flash2806x_CsmKeys.d" \
"sys_source\F2806x_CSMPasswords.d" \
"sys_source\F2806x_CodeStartBranch.d" \
"sys_source\F2806x_DBGIER.d" \
"sys_source\F2806x_DisInt.d" \
"sys_source\F2806x_usDelay.d" 

C_SRCS__QUOTED += \
"../sys_source/DSP2803x_MemCopy.c" \
"../sys_source/DSP2803x_SysCtrl.c" \
"../sys_source/F2806x_Adc.c" \
"../sys_source/F2806x_Comp.c" \
"../sys_source/F2806x_CpuTimers.c" \
"../sys_source/F2806x_DefaultIsr.c" \
"../sys_source/F2806x_Dma.c" \
"../sys_source/F2806x_ECan.c" \
"../sys_source/F2806x_ECap.c" \
"../sys_source/F2806x_EPwm.c" \
"../sys_source/F2806x_EQep.c" \
"../sys_source/F2806x_GlobalVariableDefs.c" \
"../sys_source/F2806x_Gpio.c" \
"../sys_source/F2806x_HRCap.c" \
"../sys_source/F2806x_I2C.c" \
"../sys_source/F2806x_Mcbsp.c" \
"../sys_source/F2806x_OscComp.c" \
"../sys_source/F2806x_PieCtrl.c" \
"../sys_source/F2806x_PieVect.c" \
"../sys_source/F2806x_Sci.c" \
"../sys_source/F2806x_Spi.c" \
"../sys_source/F2806x_TempSensorConv.c" 

ASM_SRCS__QUOTED += \
"../sys_source/Example_Flash2806x_CsmKeys.asm" \
"../sys_source/F2806x_CSMPasswords.asm" \
"../sys_source/F2806x_CodeStartBranch.asm" \
"../sys_source/F2806x_DBGIER.asm" \
"../sys_source/F2806x_DisInt.asm" \
"../sys_source/F2806x_usDelay.asm" 


