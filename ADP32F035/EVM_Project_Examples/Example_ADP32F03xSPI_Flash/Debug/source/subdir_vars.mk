################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../source/ADP32F03x_CSMPasswords.asm \
../source/ADP32F03x_CodeStartBranch.asm \
../source/ADP32F03x_DBGIER.asm \
../source/ADP32F03x_DisInt.asm \
../source/ADP32F03x_usDelay.asm 

C_SRCS += \
../source/ADP32F03x_Adc.c \
../source/ADP32F03x_Comp.c \
../source/ADP32F03x_CpuTimers.c \
../source/ADP32F03x_DefaultIsr.c \
../source/ADP32F03x_ECan.c \
../source/ADP32F03x_ECap.c \
../source/ADP32F03x_EPwm.c \
../source/ADP32F03x_EQep.c \
../source/ADP32F03x_GlobalVariableDefs.c \
../source/ADP32F03x_Gpio.c \
../source/ADP32F03x_HRCap.c \
../source/ADP32F03x_I2C.c \
../source/ADP32F03x_Lin.c \
../source/ADP32F03x_MemCopy.c \
../source/ADP32F03x_OscComp.c \
../source/ADP32F03x_PieCtrl.c \
../source/ADP32F03x_PieVect.c \
../source/ADP32F03x_Sci.c \
../source/ADP32F03x_Spi.c \
../source/ADP32F03x_SysCtrl.c \
../source/ADP32F03x_TempSensorConv.c 

C_DEPS += \
./source/ADP32F03x_Adc.d \
./source/ADP32F03x_Comp.d \
./source/ADP32F03x_CpuTimers.d \
./source/ADP32F03x_DefaultIsr.d \
./source/ADP32F03x_ECan.d \
./source/ADP32F03x_ECap.d \
./source/ADP32F03x_EPwm.d \
./source/ADP32F03x_EQep.d \
./source/ADP32F03x_GlobalVariableDefs.d \
./source/ADP32F03x_Gpio.d \
./source/ADP32F03x_HRCap.d \
./source/ADP32F03x_I2C.d \
./source/ADP32F03x_Lin.d \
./source/ADP32F03x_MemCopy.d \
./source/ADP32F03x_OscComp.d \
./source/ADP32F03x_PieCtrl.d \
./source/ADP32F03x_PieVect.d \
./source/ADP32F03x_Sci.d \
./source/ADP32F03x_Spi.d \
./source/ADP32F03x_SysCtrl.d \
./source/ADP32F03x_TempSensorConv.d 

OBJS += \
./source/ADP32F03x_Adc.obj \
./source/ADP32F03x_CSMPasswords.obj \
./source/ADP32F03x_CodeStartBranch.obj \
./source/ADP32F03x_Comp.obj \
./source/ADP32F03x_CpuTimers.obj \
./source/ADP32F03x_DBGIER.obj \
./source/ADP32F03x_DefaultIsr.obj \
./source/ADP32F03x_DisInt.obj \
./source/ADP32F03x_ECan.obj \
./source/ADP32F03x_ECap.obj \
./source/ADP32F03x_EPwm.obj \
./source/ADP32F03x_EQep.obj \
./source/ADP32F03x_GlobalVariableDefs.obj \
./source/ADP32F03x_Gpio.obj \
./source/ADP32F03x_HRCap.obj \
./source/ADP32F03x_I2C.obj \
./source/ADP32F03x_Lin.obj \
./source/ADP32F03x_MemCopy.obj \
./source/ADP32F03x_OscComp.obj \
./source/ADP32F03x_PieCtrl.obj \
./source/ADP32F03x_PieVect.obj \
./source/ADP32F03x_Sci.obj \
./source/ADP32F03x_Spi.obj \
./source/ADP32F03x_SysCtrl.obj \
./source/ADP32F03x_TempSensorConv.obj \
./source/ADP32F03x_usDelay.obj 

ASM_DEPS += \
./source/ADP32F03x_CSMPasswords.d \
./source/ADP32F03x_CodeStartBranch.d \
./source/ADP32F03x_DBGIER.d \
./source/ADP32F03x_DisInt.d \
./source/ADP32F03x_usDelay.d 

OBJS__QUOTED += \
"source\ADP32F03x_Adc.obj" \
"source\ADP32F03x_CSMPasswords.obj" \
"source\ADP32F03x_CodeStartBranch.obj" \
"source\ADP32F03x_Comp.obj" \
"source\ADP32F03x_CpuTimers.obj" \
"source\ADP32F03x_DBGIER.obj" \
"source\ADP32F03x_DefaultIsr.obj" \
"source\ADP32F03x_DisInt.obj" \
"source\ADP32F03x_ECan.obj" \
"source\ADP32F03x_ECap.obj" \
"source\ADP32F03x_EPwm.obj" \
"source\ADP32F03x_EQep.obj" \
"source\ADP32F03x_GlobalVariableDefs.obj" \
"source\ADP32F03x_Gpio.obj" \
"source\ADP32F03x_HRCap.obj" \
"source\ADP32F03x_I2C.obj" \
"source\ADP32F03x_Lin.obj" \
"source\ADP32F03x_MemCopy.obj" \
"source\ADP32F03x_OscComp.obj" \
"source\ADP32F03x_PieCtrl.obj" \
"source\ADP32F03x_PieVect.obj" \
"source\ADP32F03x_Sci.obj" \
"source\ADP32F03x_Spi.obj" \
"source\ADP32F03x_SysCtrl.obj" \
"source\ADP32F03x_TempSensorConv.obj" \
"source\ADP32F03x_usDelay.obj" 

C_DEPS__QUOTED += \
"source\ADP32F03x_Adc.d" \
"source\ADP32F03x_Comp.d" \
"source\ADP32F03x_CpuTimers.d" \
"source\ADP32F03x_DefaultIsr.d" \
"source\ADP32F03x_ECan.d" \
"source\ADP32F03x_ECap.d" \
"source\ADP32F03x_EPwm.d" \
"source\ADP32F03x_EQep.d" \
"source\ADP32F03x_GlobalVariableDefs.d" \
"source\ADP32F03x_Gpio.d" \
"source\ADP32F03x_HRCap.d" \
"source\ADP32F03x_I2C.d" \
"source\ADP32F03x_Lin.d" \
"source\ADP32F03x_MemCopy.d" \
"source\ADP32F03x_OscComp.d" \
"source\ADP32F03x_PieCtrl.d" \
"source\ADP32F03x_PieVect.d" \
"source\ADP32F03x_Sci.d" \
"source\ADP32F03x_Spi.d" \
"source\ADP32F03x_SysCtrl.d" \
"source\ADP32F03x_TempSensorConv.d" 

ASM_DEPS__QUOTED += \
"source\ADP32F03x_CSMPasswords.d" \
"source\ADP32F03x_CodeStartBranch.d" \
"source\ADP32F03x_DBGIER.d" \
"source\ADP32F03x_DisInt.d" \
"source\ADP32F03x_usDelay.d" 

C_SRCS__QUOTED += \
"../source/ADP32F03x_Adc.c" \
"../source/ADP32F03x_Comp.c" \
"../source/ADP32F03x_CpuTimers.c" \
"../source/ADP32F03x_DefaultIsr.c" \
"../source/ADP32F03x_ECan.c" \
"../source/ADP32F03x_ECap.c" \
"../source/ADP32F03x_EPwm.c" \
"../source/ADP32F03x_EQep.c" \
"../source/ADP32F03x_GlobalVariableDefs.c" \
"../source/ADP32F03x_Gpio.c" \
"../source/ADP32F03x_HRCap.c" \
"../source/ADP32F03x_I2C.c" \
"../source/ADP32F03x_Lin.c" \
"../source/ADP32F03x_MemCopy.c" \
"../source/ADP32F03x_OscComp.c" \
"../source/ADP32F03x_PieCtrl.c" \
"../source/ADP32F03x_PieVect.c" \
"../source/ADP32F03x_Sci.c" \
"../source/ADP32F03x_Spi.c" \
"../source/ADP32F03x_SysCtrl.c" \
"../source/ADP32F03x_TempSensorConv.c" 

ASM_SRCS__QUOTED += \
"../source/ADP32F03x_CSMPasswords.asm" \
"../source/ADP32F03x_CodeStartBranch.asm" \
"../source/ADP32F03x_DBGIER.asm" \
"../source/ADP32F03x_DisInt.asm" \
"../source/ADP32F03x_usDelay.asm" 


