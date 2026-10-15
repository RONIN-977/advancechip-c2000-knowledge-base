################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Library/src/ADP32F03x_CSMPasswords.asm \
../Library/src/ADP32F03x_CodeStartBranch.asm \
../Library/src/ADP32F03x_DBGIER.asm \
../Library/src/ADP32F03x_DisInt.asm \
../Library/src/ADP32F03x_usDelay.asm 

C_SRCS += \
../Library/src/ADP32F03x_Adc.c \
../Library/src/ADP32F03x_Comp.c \
../Library/src/ADP32F03x_CpuTimers.c \
../Library/src/ADP32F03x_DefaultIsr.c \
../Library/src/ADP32F03x_ECan.c \
../Library/src/ADP32F03x_ECap.c \
../Library/src/ADP32F03x_EPwm.c \
../Library/src/ADP32F03x_EQep.c \
../Library/src/ADP32F03x_GlobalVariableDefs.c \
../Library/src/ADP32F03x_Gpio.c \
../Library/src/ADP32F03x_HRCap.c \
../Library/src/ADP32F03x_I2C.c \
../Library/src/ADP32F03x_Lin.c \
../Library/src/ADP32F03x_MemCopy.c \
../Library/src/ADP32F03x_OscComp.c \
../Library/src/ADP32F03x_PieCtrl.c \
../Library/src/ADP32F03x_PieVect.c \
../Library/src/ADP32F03x_Sci.c \
../Library/src/ADP32F03x_Spi.c \
../Library/src/ADP32F03x_SysCtrl.c \
../Library/src/ADP32F03x_TempSensorConv.c 

C_DEPS += \
./Library/src/ADP32F03x_Adc.d \
./Library/src/ADP32F03x_Comp.d \
./Library/src/ADP32F03x_CpuTimers.d \
./Library/src/ADP32F03x_DefaultIsr.d \
./Library/src/ADP32F03x_ECan.d \
./Library/src/ADP32F03x_ECap.d \
./Library/src/ADP32F03x_EPwm.d \
./Library/src/ADP32F03x_EQep.d \
./Library/src/ADP32F03x_GlobalVariableDefs.d \
./Library/src/ADP32F03x_Gpio.d \
./Library/src/ADP32F03x_HRCap.d \
./Library/src/ADP32F03x_I2C.d \
./Library/src/ADP32F03x_Lin.d \
./Library/src/ADP32F03x_MemCopy.d \
./Library/src/ADP32F03x_OscComp.d \
./Library/src/ADP32F03x_PieCtrl.d \
./Library/src/ADP32F03x_PieVect.d \
./Library/src/ADP32F03x_Sci.d \
./Library/src/ADP32F03x_Spi.d \
./Library/src/ADP32F03x_SysCtrl.d \
./Library/src/ADP32F03x_TempSensorConv.d 

OBJS += \
./Library/src/ADP32F03x_Adc.obj \
./Library/src/ADP32F03x_CSMPasswords.obj \
./Library/src/ADP32F03x_CodeStartBranch.obj \
./Library/src/ADP32F03x_Comp.obj \
./Library/src/ADP32F03x_CpuTimers.obj \
./Library/src/ADP32F03x_DBGIER.obj \
./Library/src/ADP32F03x_DefaultIsr.obj \
./Library/src/ADP32F03x_DisInt.obj \
./Library/src/ADP32F03x_ECan.obj \
./Library/src/ADP32F03x_ECap.obj \
./Library/src/ADP32F03x_EPwm.obj \
./Library/src/ADP32F03x_EQep.obj \
./Library/src/ADP32F03x_GlobalVariableDefs.obj \
./Library/src/ADP32F03x_Gpio.obj \
./Library/src/ADP32F03x_HRCap.obj \
./Library/src/ADP32F03x_I2C.obj \
./Library/src/ADP32F03x_Lin.obj \
./Library/src/ADP32F03x_MemCopy.obj \
./Library/src/ADP32F03x_OscComp.obj \
./Library/src/ADP32F03x_PieCtrl.obj \
./Library/src/ADP32F03x_PieVect.obj \
./Library/src/ADP32F03x_Sci.obj \
./Library/src/ADP32F03x_Spi.obj \
./Library/src/ADP32F03x_SysCtrl.obj \
./Library/src/ADP32F03x_TempSensorConv.obj \
./Library/src/ADP32F03x_usDelay.obj 

ASM_DEPS += \
./Library/src/ADP32F03x_CSMPasswords.d \
./Library/src/ADP32F03x_CodeStartBranch.d \
./Library/src/ADP32F03x_DBGIER.d \
./Library/src/ADP32F03x_DisInt.d \
./Library/src/ADP32F03x_usDelay.d 

OBJS__QUOTED += \
"Library\src\ADP32F03x_Adc.obj" \
"Library\src\ADP32F03x_CSMPasswords.obj" \
"Library\src\ADP32F03x_CodeStartBranch.obj" \
"Library\src\ADP32F03x_Comp.obj" \
"Library\src\ADP32F03x_CpuTimers.obj" \
"Library\src\ADP32F03x_DBGIER.obj" \
"Library\src\ADP32F03x_DefaultIsr.obj" \
"Library\src\ADP32F03x_DisInt.obj" \
"Library\src\ADP32F03x_ECan.obj" \
"Library\src\ADP32F03x_ECap.obj" \
"Library\src\ADP32F03x_EPwm.obj" \
"Library\src\ADP32F03x_EQep.obj" \
"Library\src\ADP32F03x_GlobalVariableDefs.obj" \
"Library\src\ADP32F03x_Gpio.obj" \
"Library\src\ADP32F03x_HRCap.obj" \
"Library\src\ADP32F03x_I2C.obj" \
"Library\src\ADP32F03x_Lin.obj" \
"Library\src\ADP32F03x_MemCopy.obj" \
"Library\src\ADP32F03x_OscComp.obj" \
"Library\src\ADP32F03x_PieCtrl.obj" \
"Library\src\ADP32F03x_PieVect.obj" \
"Library\src\ADP32F03x_Sci.obj" \
"Library\src\ADP32F03x_Spi.obj" \
"Library\src\ADP32F03x_SysCtrl.obj" \
"Library\src\ADP32F03x_TempSensorConv.obj" \
"Library\src\ADP32F03x_usDelay.obj" 

C_DEPS__QUOTED += \
"Library\src\ADP32F03x_Adc.d" \
"Library\src\ADP32F03x_Comp.d" \
"Library\src\ADP32F03x_CpuTimers.d" \
"Library\src\ADP32F03x_DefaultIsr.d" \
"Library\src\ADP32F03x_ECan.d" \
"Library\src\ADP32F03x_ECap.d" \
"Library\src\ADP32F03x_EPwm.d" \
"Library\src\ADP32F03x_EQep.d" \
"Library\src\ADP32F03x_GlobalVariableDefs.d" \
"Library\src\ADP32F03x_Gpio.d" \
"Library\src\ADP32F03x_HRCap.d" \
"Library\src\ADP32F03x_I2C.d" \
"Library\src\ADP32F03x_Lin.d" \
"Library\src\ADP32F03x_MemCopy.d" \
"Library\src\ADP32F03x_OscComp.d" \
"Library\src\ADP32F03x_PieCtrl.d" \
"Library\src\ADP32F03x_PieVect.d" \
"Library\src\ADP32F03x_Sci.d" \
"Library\src\ADP32F03x_Spi.d" \
"Library\src\ADP32F03x_SysCtrl.d" \
"Library\src\ADP32F03x_TempSensorConv.d" 

ASM_DEPS__QUOTED += \
"Library\src\ADP32F03x_CSMPasswords.d" \
"Library\src\ADP32F03x_CodeStartBranch.d" \
"Library\src\ADP32F03x_DBGIER.d" \
"Library\src\ADP32F03x_DisInt.d" \
"Library\src\ADP32F03x_usDelay.d" 

C_SRCS__QUOTED += \
"../Library/src/ADP32F03x_Adc.c" \
"../Library/src/ADP32F03x_Comp.c" \
"../Library/src/ADP32F03x_CpuTimers.c" \
"../Library/src/ADP32F03x_DefaultIsr.c" \
"../Library/src/ADP32F03x_ECan.c" \
"../Library/src/ADP32F03x_ECap.c" \
"../Library/src/ADP32F03x_EPwm.c" \
"../Library/src/ADP32F03x_EQep.c" \
"../Library/src/ADP32F03x_GlobalVariableDefs.c" \
"../Library/src/ADP32F03x_Gpio.c" \
"../Library/src/ADP32F03x_HRCap.c" \
"../Library/src/ADP32F03x_I2C.c" \
"../Library/src/ADP32F03x_Lin.c" \
"../Library/src/ADP32F03x_MemCopy.c" \
"../Library/src/ADP32F03x_OscComp.c" \
"../Library/src/ADP32F03x_PieCtrl.c" \
"../Library/src/ADP32F03x_PieVect.c" \
"../Library/src/ADP32F03x_Sci.c" \
"../Library/src/ADP32F03x_Spi.c" \
"../Library/src/ADP32F03x_SysCtrl.c" \
"../Library/src/ADP32F03x_TempSensorConv.c" 

ASM_SRCS__QUOTED += \
"../Library/src/ADP32F03x_CSMPasswords.asm" \
"../Library/src/ADP32F03x_CodeStartBranch.asm" \
"../Library/src/ADP32F03x_DBGIER.asm" \
"../Library/src/ADP32F03x_DisInt.asm" \
"../Library/src/ADP32F03x_usDelay.asm" 


