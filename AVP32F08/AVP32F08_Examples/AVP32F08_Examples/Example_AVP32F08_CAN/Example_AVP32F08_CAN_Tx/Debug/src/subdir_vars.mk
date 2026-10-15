################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/AVP32F08_ADC_cal.asm \
../src/AVP32F08_CSMPasswords.asm \
../src/AVP32F08_CodeStartBranch.asm \
../src/AVP32F08_DBGIER.asm \
../src/AVP32F08_DisInt.asm \
../src/AVP32F08_usDelay.asm 

C_SRCS += \
../src/AVP32F08_Adc.c \
../src/AVP32F08_CpuTimers.c \
../src/AVP32F08_DMA.c \
../src/AVP32F08_DefaultIsr.c \
../src/AVP32F08_ECan.c \
../src/AVP32F08_ECap.c \
../src/AVP32F08_EPwm.c \
../src/AVP32F08_EQep.c \
../src/AVP32F08_GlobalVariableDefs.c \
../src/AVP32F08_Gpio.c \
../src/AVP32F08_I2C.c \
../src/AVP32F08_Mcbsp.c \
../src/AVP32F08_MemCopy.c \
../src/AVP32F08_PieCtrl.c \
../src/AVP32F08_PieVect.c \
../src/AVP32F08_Sci.c \
../src/AVP32F08_Spi.c \
../src/AVP32F08_SysCtrl.c \
../src/AVP32F08_Xintf.c 

C_DEPS += \
./src/AVP32F08_Adc.d \
./src/AVP32F08_CpuTimers.d \
./src/AVP32F08_DMA.d \
./src/AVP32F08_DefaultIsr.d \
./src/AVP32F08_ECan.d \
./src/AVP32F08_ECap.d \
./src/AVP32F08_EPwm.d \
./src/AVP32F08_EQep.d \
./src/AVP32F08_GlobalVariableDefs.d \
./src/AVP32F08_Gpio.d \
./src/AVP32F08_I2C.d \
./src/AVP32F08_Mcbsp.d \
./src/AVP32F08_MemCopy.d \
./src/AVP32F08_PieCtrl.d \
./src/AVP32F08_PieVect.d \
./src/AVP32F08_Sci.d \
./src/AVP32F08_Spi.d \
./src/AVP32F08_SysCtrl.d \
./src/AVP32F08_Xintf.d 

OBJS += \
./src/AVP32F08_ADC_cal.obj \
./src/AVP32F08_Adc.obj \
./src/AVP32F08_CSMPasswords.obj \
./src/AVP32F08_CodeStartBranch.obj \
./src/AVP32F08_CpuTimers.obj \
./src/AVP32F08_DBGIER.obj \
./src/AVP32F08_DMA.obj \
./src/AVP32F08_DefaultIsr.obj \
./src/AVP32F08_DisInt.obj \
./src/AVP32F08_ECan.obj \
./src/AVP32F08_ECap.obj \
./src/AVP32F08_EPwm.obj \
./src/AVP32F08_EQep.obj \
./src/AVP32F08_GlobalVariableDefs.obj \
./src/AVP32F08_Gpio.obj \
./src/AVP32F08_I2C.obj \
./src/AVP32F08_Mcbsp.obj \
./src/AVP32F08_MemCopy.obj \
./src/AVP32F08_PieCtrl.obj \
./src/AVP32F08_PieVect.obj \
./src/AVP32F08_Sci.obj \
./src/AVP32F08_Spi.obj \
./src/AVP32F08_SysCtrl.obj \
./src/AVP32F08_Xintf.obj \
./src/AVP32F08_usDelay.obj 

ASM_DEPS += \
./src/AVP32F08_ADC_cal.d \
./src/AVP32F08_CSMPasswords.d \
./src/AVP32F08_CodeStartBranch.d \
./src/AVP32F08_DBGIER.d \
./src/AVP32F08_DisInt.d \
./src/AVP32F08_usDelay.d 

OBJS__QUOTED += \
"src\AVP32F08_ADC_cal.obj" \
"src\AVP32F08_Adc.obj" \
"src\AVP32F08_CSMPasswords.obj" \
"src\AVP32F08_CodeStartBranch.obj" \
"src\AVP32F08_CpuTimers.obj" \
"src\AVP32F08_DBGIER.obj" \
"src\AVP32F08_DMA.obj" \
"src\AVP32F08_DefaultIsr.obj" \
"src\AVP32F08_DisInt.obj" \
"src\AVP32F08_ECan.obj" \
"src\AVP32F08_ECap.obj" \
"src\AVP32F08_EPwm.obj" \
"src\AVP32F08_EQep.obj" \
"src\AVP32F08_GlobalVariableDefs.obj" \
"src\AVP32F08_Gpio.obj" \
"src\AVP32F08_I2C.obj" \
"src\AVP32F08_Mcbsp.obj" \
"src\AVP32F08_MemCopy.obj" \
"src\AVP32F08_PieCtrl.obj" \
"src\AVP32F08_PieVect.obj" \
"src\AVP32F08_Sci.obj" \
"src\AVP32F08_Spi.obj" \
"src\AVP32F08_SysCtrl.obj" \
"src\AVP32F08_Xintf.obj" \
"src\AVP32F08_usDelay.obj" 

C_DEPS__QUOTED += \
"src\AVP32F08_Adc.d" \
"src\AVP32F08_CpuTimers.d" \
"src\AVP32F08_DMA.d" \
"src\AVP32F08_DefaultIsr.d" \
"src\AVP32F08_ECan.d" \
"src\AVP32F08_ECap.d" \
"src\AVP32F08_EPwm.d" \
"src\AVP32F08_EQep.d" \
"src\AVP32F08_GlobalVariableDefs.d" \
"src\AVP32F08_Gpio.d" \
"src\AVP32F08_I2C.d" \
"src\AVP32F08_Mcbsp.d" \
"src\AVP32F08_MemCopy.d" \
"src\AVP32F08_PieCtrl.d" \
"src\AVP32F08_PieVect.d" \
"src\AVP32F08_Sci.d" \
"src\AVP32F08_Spi.d" \
"src\AVP32F08_SysCtrl.d" \
"src\AVP32F08_Xintf.d" 

ASM_DEPS__QUOTED += \
"src\AVP32F08_ADC_cal.d" \
"src\AVP32F08_CSMPasswords.d" \
"src\AVP32F08_CodeStartBranch.d" \
"src\AVP32F08_DBGIER.d" \
"src\AVP32F08_DisInt.d" \
"src\AVP32F08_usDelay.d" 

ASM_SRCS__QUOTED += \
"../src/AVP32F08_ADC_cal.asm" \
"../src/AVP32F08_CSMPasswords.asm" \
"../src/AVP32F08_CodeStartBranch.asm" \
"../src/AVP32F08_DBGIER.asm" \
"../src/AVP32F08_DisInt.asm" \
"../src/AVP32F08_usDelay.asm" 

C_SRCS__QUOTED += \
"../src/AVP32F08_Adc.c" \
"../src/AVP32F08_CpuTimers.c" \
"../src/AVP32F08_DMA.c" \
"../src/AVP32F08_DefaultIsr.c" \
"../src/AVP32F08_ECan.c" \
"../src/AVP32F08_ECap.c" \
"../src/AVP32F08_EPwm.c" \
"../src/AVP32F08_EQep.c" \
"../src/AVP32F08_GlobalVariableDefs.c" \
"../src/AVP32F08_Gpio.c" \
"../src/AVP32F08_I2C.c" \
"../src/AVP32F08_Mcbsp.c" \
"../src/AVP32F08_MemCopy.c" \
"../src/AVP32F08_PieCtrl.c" \
"../src/AVP32F08_PieVect.c" \
"../src/AVP32F08_Sci.c" \
"../src/AVP32F08_Spi.c" \
"../src/AVP32F08_SysCtrl.c" \
"../src/AVP32F08_Xintf.c" 


