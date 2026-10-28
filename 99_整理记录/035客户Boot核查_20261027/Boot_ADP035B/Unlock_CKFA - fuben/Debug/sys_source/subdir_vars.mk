################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../sys_source/DSP2803x_CSMPasswords.asm \
../sys_source/DSP2803x_CodeStartBranch.asm \
../sys_source/DSP2803x_DBGIER.asm \
../sys_source/DSP2803x_DisInt.asm \
../sys_source/DSP2803x_usDelay.asm \
../sys_source/Example_Flash2803x_CsmKeys.asm 

C_SRCS += \
../sys_source/DSP2803x_Adc.c \
../sys_source/DSP2803x_Comp.c \
../sys_source/DSP2803x_CpuTimers.c \
../sys_source/DSP2803x_DefaultIsr.c \
../sys_source/DSP2803x_ECan.c \
../sys_source/DSP2803x_ECap.c \
../sys_source/DSP2803x_EPwm.c \
../sys_source/DSP2803x_EQep.c \
../sys_source/DSP2803x_GlobalVariableDefs.c \
../sys_source/DSP2803x_Gpio.c \
../sys_source/DSP2803x_HRCap.c \
../sys_source/DSP2803x_I2C.c \
../sys_source/DSP2803x_Lin.c \
../sys_source/DSP2803x_MemCopy.c \
../sys_source/DSP2803x_OscComp.c \
../sys_source/DSP2803x_PieCtrl.c \
../sys_source/DSP2803x_PieVect.c \
../sys_source/DSP2803x_Sci.c \
../sys_source/DSP2803x_Spi.c \
../sys_source/DSP2803x_SysCtrl.c \
../sys_source/DSP2803x_TempSensorConv.c 

C_DEPS += \
./sys_source/DSP2803x_Adc.d \
./sys_source/DSP2803x_Comp.d \
./sys_source/DSP2803x_CpuTimers.d \
./sys_source/DSP2803x_DefaultIsr.d \
./sys_source/DSP2803x_ECan.d \
./sys_source/DSP2803x_ECap.d \
./sys_source/DSP2803x_EPwm.d \
./sys_source/DSP2803x_EQep.d \
./sys_source/DSP2803x_GlobalVariableDefs.d \
./sys_source/DSP2803x_Gpio.d \
./sys_source/DSP2803x_HRCap.d \
./sys_source/DSP2803x_I2C.d \
./sys_source/DSP2803x_Lin.d \
./sys_source/DSP2803x_MemCopy.d \
./sys_source/DSP2803x_OscComp.d \
./sys_source/DSP2803x_PieCtrl.d \
./sys_source/DSP2803x_PieVect.d \
./sys_source/DSP2803x_Sci.d \
./sys_source/DSP2803x_Spi.d \
./sys_source/DSP2803x_SysCtrl.d \
./sys_source/DSP2803x_TempSensorConv.d 

OBJS += \
./sys_source/DSP2803x_Adc.obj \
./sys_source/DSP2803x_CSMPasswords.obj \
./sys_source/DSP2803x_CodeStartBranch.obj \
./sys_source/DSP2803x_Comp.obj \
./sys_source/DSP2803x_CpuTimers.obj \
./sys_source/DSP2803x_DBGIER.obj \
./sys_source/DSP2803x_DefaultIsr.obj \
./sys_source/DSP2803x_DisInt.obj \
./sys_source/DSP2803x_ECan.obj \
./sys_source/DSP2803x_ECap.obj \
./sys_source/DSP2803x_EPwm.obj \
./sys_source/DSP2803x_EQep.obj \
./sys_source/DSP2803x_GlobalVariableDefs.obj \
./sys_source/DSP2803x_Gpio.obj \
./sys_source/DSP2803x_HRCap.obj \
./sys_source/DSP2803x_I2C.obj \
./sys_source/DSP2803x_Lin.obj \
./sys_source/DSP2803x_MemCopy.obj \
./sys_source/DSP2803x_OscComp.obj \
./sys_source/DSP2803x_PieCtrl.obj \
./sys_source/DSP2803x_PieVect.obj \
./sys_source/DSP2803x_Sci.obj \
./sys_source/DSP2803x_Spi.obj \
./sys_source/DSP2803x_SysCtrl.obj \
./sys_source/DSP2803x_TempSensorConv.obj \
./sys_source/DSP2803x_usDelay.obj \
./sys_source/Example_Flash2803x_CsmKeys.obj 

ASM_DEPS += \
./sys_source/DSP2803x_CSMPasswords.d \
./sys_source/DSP2803x_CodeStartBranch.d \
./sys_source/DSP2803x_DBGIER.d \
./sys_source/DSP2803x_DisInt.d \
./sys_source/DSP2803x_usDelay.d \
./sys_source/Example_Flash2803x_CsmKeys.d 

OBJS__QUOTED += \
"sys_source\DSP2803x_Adc.obj" \
"sys_source\DSP2803x_CSMPasswords.obj" \
"sys_source\DSP2803x_CodeStartBranch.obj" \
"sys_source\DSP2803x_Comp.obj" \
"sys_source\DSP2803x_CpuTimers.obj" \
"sys_source\DSP2803x_DBGIER.obj" \
"sys_source\DSP2803x_DefaultIsr.obj" \
"sys_source\DSP2803x_DisInt.obj" \
"sys_source\DSP2803x_ECan.obj" \
"sys_source\DSP2803x_ECap.obj" \
"sys_source\DSP2803x_EPwm.obj" \
"sys_source\DSP2803x_EQep.obj" \
"sys_source\DSP2803x_GlobalVariableDefs.obj" \
"sys_source\DSP2803x_Gpio.obj" \
"sys_source\DSP2803x_HRCap.obj" \
"sys_source\DSP2803x_I2C.obj" \
"sys_source\DSP2803x_Lin.obj" \
"sys_source\DSP2803x_MemCopy.obj" \
"sys_source\DSP2803x_OscComp.obj" \
"sys_source\DSP2803x_PieCtrl.obj" \
"sys_source\DSP2803x_PieVect.obj" \
"sys_source\DSP2803x_Sci.obj" \
"sys_source\DSP2803x_Spi.obj" \
"sys_source\DSP2803x_SysCtrl.obj" \
"sys_source\DSP2803x_TempSensorConv.obj" \
"sys_source\DSP2803x_usDelay.obj" \
"sys_source\Example_Flash2803x_CsmKeys.obj" 

C_DEPS__QUOTED += \
"sys_source\DSP2803x_Adc.d" \
"sys_source\DSP2803x_Comp.d" \
"sys_source\DSP2803x_CpuTimers.d" \
"sys_source\DSP2803x_DefaultIsr.d" \
"sys_source\DSP2803x_ECan.d" \
"sys_source\DSP2803x_ECap.d" \
"sys_source\DSP2803x_EPwm.d" \
"sys_source\DSP2803x_EQep.d" \
"sys_source\DSP2803x_GlobalVariableDefs.d" \
"sys_source\DSP2803x_Gpio.d" \
"sys_source\DSP2803x_HRCap.d" \
"sys_source\DSP2803x_I2C.d" \
"sys_source\DSP2803x_Lin.d" \
"sys_source\DSP2803x_MemCopy.d" \
"sys_source\DSP2803x_OscComp.d" \
"sys_source\DSP2803x_PieCtrl.d" \
"sys_source\DSP2803x_PieVect.d" \
"sys_source\DSP2803x_Sci.d" \
"sys_source\DSP2803x_Spi.d" \
"sys_source\DSP2803x_SysCtrl.d" \
"sys_source\DSP2803x_TempSensorConv.d" 

ASM_DEPS__QUOTED += \
"sys_source\DSP2803x_CSMPasswords.d" \
"sys_source\DSP2803x_CodeStartBranch.d" \
"sys_source\DSP2803x_DBGIER.d" \
"sys_source\DSP2803x_DisInt.d" \
"sys_source\DSP2803x_usDelay.d" \
"sys_source\Example_Flash2803x_CsmKeys.d" 

C_SRCS__QUOTED += \
"../sys_source/DSP2803x_Adc.c" \
"../sys_source/DSP2803x_Comp.c" \
"../sys_source/DSP2803x_CpuTimers.c" \
"../sys_source/DSP2803x_DefaultIsr.c" \
"../sys_source/DSP2803x_ECan.c" \
"../sys_source/DSP2803x_ECap.c" \
"../sys_source/DSP2803x_EPwm.c" \
"../sys_source/DSP2803x_EQep.c" \
"../sys_source/DSP2803x_GlobalVariableDefs.c" \
"../sys_source/DSP2803x_Gpio.c" \
"../sys_source/DSP2803x_HRCap.c" \
"../sys_source/DSP2803x_I2C.c" \
"../sys_source/DSP2803x_Lin.c" \
"../sys_source/DSP2803x_MemCopy.c" \
"../sys_source/DSP2803x_OscComp.c" \
"../sys_source/DSP2803x_PieCtrl.c" \
"../sys_source/DSP2803x_PieVect.c" \
"../sys_source/DSP2803x_Sci.c" \
"../sys_source/DSP2803x_Spi.c" \
"../sys_source/DSP2803x_SysCtrl.c" \
"../sys_source/DSP2803x_TempSensorConv.c" 

ASM_SRCS__QUOTED += \
"../sys_source/DSP2803x_CSMPasswords.asm" \
"../sys_source/DSP2803x_CodeStartBranch.asm" \
"../sys_source/DSP2803x_DBGIER.asm" \
"../sys_source/DSP2803x_DisInt.asm" \
"../sys_source/DSP2803x_usDelay.asm" \
"../sys_source/Example_Flash2803x_CsmKeys.asm" 


