################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Library/src/AVP33x_ADC_cal.asm \
../Library/src/AVP33x_CSMPasswords.asm \
../Library/src/AVP33x_CodeStartBranch.asm \
../Library/src/AVP33x_DBGIER.asm \
../Library/src/AVP33x_DisInt.asm \
../Library/src/AVP33x_usDelay.asm 

C_SRCS += \
../Library/src/AVP33x_Adc.c \
../Library/src/AVP33x_CpuTimers.c \
../Library/src/AVP33x_DMA.c \
../Library/src/AVP33x_DefaultIsr.c \
../Library/src/AVP33x_ECan.c \
../Library/src/AVP33x_ECap.c \
../Library/src/AVP33x_EPwm.c \
../Library/src/AVP33x_EQep.c \
../Library/src/AVP33x_GlobalVariableDefs.c \
../Library/src/AVP33x_Gpio.c \
../Library/src/AVP33x_I2C.c \
../Library/src/AVP33x_Mcbsp.c \
../Library/src/AVP33x_MemCopy.c \
../Library/src/AVP33x_PieCtrl.c \
../Library/src/AVP33x_PieVect.c \
../Library/src/AVP33x_Sci.c \
../Library/src/AVP33x_Spi.c \
../Library/src/AVP33x_SysCtrl.c \
../Library/src/AVP33x_Xintf.c 

C_DEPS += \
./Library/src/AVP33x_Adc.d \
./Library/src/AVP33x_CpuTimers.d \
./Library/src/AVP33x_DMA.d \
./Library/src/AVP33x_DefaultIsr.d \
./Library/src/AVP33x_ECan.d \
./Library/src/AVP33x_ECap.d \
./Library/src/AVP33x_EPwm.d \
./Library/src/AVP33x_EQep.d \
./Library/src/AVP33x_GlobalVariableDefs.d \
./Library/src/AVP33x_Gpio.d \
./Library/src/AVP33x_I2C.d \
./Library/src/AVP33x_Mcbsp.d \
./Library/src/AVP33x_MemCopy.d \
./Library/src/AVP33x_PieCtrl.d \
./Library/src/AVP33x_PieVect.d \
./Library/src/AVP33x_Sci.d \
./Library/src/AVP33x_Spi.d \
./Library/src/AVP33x_SysCtrl.d \
./Library/src/AVP33x_Xintf.d 

OBJS += \
./Library/src/AVP33x_ADC_cal.obj \
./Library/src/AVP33x_Adc.obj \
./Library/src/AVP33x_CSMPasswords.obj \
./Library/src/AVP33x_CodeStartBranch.obj \
./Library/src/AVP33x_CpuTimers.obj \
./Library/src/AVP33x_DBGIER.obj \
./Library/src/AVP33x_DMA.obj \
./Library/src/AVP33x_DefaultIsr.obj \
./Library/src/AVP33x_DisInt.obj \
./Library/src/AVP33x_ECan.obj \
./Library/src/AVP33x_ECap.obj \
./Library/src/AVP33x_EPwm.obj \
./Library/src/AVP33x_EQep.obj \
./Library/src/AVP33x_GlobalVariableDefs.obj \
./Library/src/AVP33x_Gpio.obj \
./Library/src/AVP33x_I2C.obj \
./Library/src/AVP33x_Mcbsp.obj \
./Library/src/AVP33x_MemCopy.obj \
./Library/src/AVP33x_PieCtrl.obj \
./Library/src/AVP33x_PieVect.obj \
./Library/src/AVP33x_Sci.obj \
./Library/src/AVP33x_Spi.obj \
./Library/src/AVP33x_SysCtrl.obj \
./Library/src/AVP33x_Xintf.obj \
./Library/src/AVP33x_usDelay.obj 

ASM_DEPS += \
./Library/src/AVP33x_ADC_cal.d \
./Library/src/AVP33x_CSMPasswords.d \
./Library/src/AVP33x_CodeStartBranch.d \
./Library/src/AVP33x_DBGIER.d \
./Library/src/AVP33x_DisInt.d \
./Library/src/AVP33x_usDelay.d 

OBJS__QUOTED += \
"Library\src\AVP33x_ADC_cal.obj" \
"Library\src\AVP33x_Adc.obj" \
"Library\src\AVP33x_CSMPasswords.obj" \
"Library\src\AVP33x_CodeStartBranch.obj" \
"Library\src\AVP33x_CpuTimers.obj" \
"Library\src\AVP33x_DBGIER.obj" \
"Library\src\AVP33x_DMA.obj" \
"Library\src\AVP33x_DefaultIsr.obj" \
"Library\src\AVP33x_DisInt.obj" \
"Library\src\AVP33x_ECan.obj" \
"Library\src\AVP33x_ECap.obj" \
"Library\src\AVP33x_EPwm.obj" \
"Library\src\AVP33x_EQep.obj" \
"Library\src\AVP33x_GlobalVariableDefs.obj" \
"Library\src\AVP33x_Gpio.obj" \
"Library\src\AVP33x_I2C.obj" \
"Library\src\AVP33x_Mcbsp.obj" \
"Library\src\AVP33x_MemCopy.obj" \
"Library\src\AVP33x_PieCtrl.obj" \
"Library\src\AVP33x_PieVect.obj" \
"Library\src\AVP33x_Sci.obj" \
"Library\src\AVP33x_Spi.obj" \
"Library\src\AVP33x_SysCtrl.obj" \
"Library\src\AVP33x_Xintf.obj" \
"Library\src\AVP33x_usDelay.obj" 

C_DEPS__QUOTED += \
"Library\src\AVP33x_Adc.d" \
"Library\src\AVP33x_CpuTimers.d" \
"Library\src\AVP33x_DMA.d" \
"Library\src\AVP33x_DefaultIsr.d" \
"Library\src\AVP33x_ECan.d" \
"Library\src\AVP33x_ECap.d" \
"Library\src\AVP33x_EPwm.d" \
"Library\src\AVP33x_EQep.d" \
"Library\src\AVP33x_GlobalVariableDefs.d" \
"Library\src\AVP33x_Gpio.d" \
"Library\src\AVP33x_I2C.d" \
"Library\src\AVP33x_Mcbsp.d" \
"Library\src\AVP33x_MemCopy.d" \
"Library\src\AVP33x_PieCtrl.d" \
"Library\src\AVP33x_PieVect.d" \
"Library\src\AVP33x_Sci.d" \
"Library\src\AVP33x_Spi.d" \
"Library\src\AVP33x_SysCtrl.d" \
"Library\src\AVP33x_Xintf.d" 

ASM_DEPS__QUOTED += \
"Library\src\AVP33x_ADC_cal.d" \
"Library\src\AVP33x_CSMPasswords.d" \
"Library\src\AVP33x_CodeStartBranch.d" \
"Library\src\AVP33x_DBGIER.d" \
"Library\src\AVP33x_DisInt.d" \
"Library\src\AVP33x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../Library/src/AVP33x_ADC_cal.asm" \
"../Library/src/AVP33x_CSMPasswords.asm" \
"../Library/src/AVP33x_CodeStartBranch.asm" \
"../Library/src/AVP33x_DBGIER.asm" \
"../Library/src/AVP33x_DisInt.asm" \
"../Library/src/AVP33x_usDelay.asm" 

C_SRCS__QUOTED += \
"../Library/src/AVP33x_Adc.c" \
"../Library/src/AVP33x_CpuTimers.c" \
"../Library/src/AVP33x_DMA.c" \
"../Library/src/AVP33x_DefaultIsr.c" \
"../Library/src/AVP33x_ECan.c" \
"../Library/src/AVP33x_ECap.c" \
"../Library/src/AVP33x_EPwm.c" \
"../Library/src/AVP33x_EQep.c" \
"../Library/src/AVP33x_GlobalVariableDefs.c" \
"../Library/src/AVP33x_Gpio.c" \
"../Library/src/AVP33x_I2C.c" \
"../Library/src/AVP33x_Mcbsp.c" \
"../Library/src/AVP33x_MemCopy.c" \
"../Library/src/AVP33x_PieCtrl.c" \
"../Library/src/AVP33x_PieVect.c" \
"../Library/src/AVP33x_Sci.c" \
"../Library/src/AVP33x_Spi.c" \
"../Library/src/AVP33x_SysCtrl.c" \
"../Library/src/AVP33x_Xintf.c" 


