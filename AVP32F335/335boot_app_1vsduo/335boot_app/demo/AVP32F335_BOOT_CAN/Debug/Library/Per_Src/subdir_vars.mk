################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Library/Per_Src/AVP33x_ADC_cal.asm \
../Library/Per_Src/AVP33x_CSMPasswords.asm \
../Library/Per_Src/AVP33x_CodeStartBranch.asm \
../Library/Per_Src/AVP33x_DBGIER.asm \
../Library/Per_Src/AVP33x_DisInt.asm \
../Library/Per_Src/AVP33x_usDelay.asm 

C_SRCS += \
../Library/Per_Src/AVP33x_Adc.c \
../Library/Per_Src/AVP33x_CpuTimers.c \
../Library/Per_Src/AVP33x_DMA.c \
../Library/Per_Src/AVP33x_DefaultIsr.c \
../Library/Per_Src/AVP33x_ECan.c \
../Library/Per_Src/AVP33x_ECap.c \
../Library/Per_Src/AVP33x_EPwm.c \
../Library/Per_Src/AVP33x_EQep.c \
../Library/Per_Src/AVP33x_GlobalVariableDefs.c \
../Library/Per_Src/AVP33x_Gpio.c \
../Library/Per_Src/AVP33x_I2C.c \
../Library/Per_Src/AVP33x_Mcbsp.c \
../Library/Per_Src/AVP33x_MemCopy.c \
../Library/Per_Src/AVP33x_PieCtrl.c \
../Library/Per_Src/AVP33x_PieVect.c \
../Library/Per_Src/AVP33x_Sci.c \
../Library/Per_Src/AVP33x_Spi.c \
../Library/Per_Src/AVP33x_SysCtrl.c \
../Library/Per_Src/AVP33x_Xintf.c 

C_DEPS += \
./Library/Per_Src/AVP33x_Adc.d \
./Library/Per_Src/AVP33x_CpuTimers.d \
./Library/Per_Src/AVP33x_DMA.d \
./Library/Per_Src/AVP33x_DefaultIsr.d \
./Library/Per_Src/AVP33x_ECan.d \
./Library/Per_Src/AVP33x_ECap.d \
./Library/Per_Src/AVP33x_EPwm.d \
./Library/Per_Src/AVP33x_EQep.d \
./Library/Per_Src/AVP33x_GlobalVariableDefs.d \
./Library/Per_Src/AVP33x_Gpio.d \
./Library/Per_Src/AVP33x_I2C.d \
./Library/Per_Src/AVP33x_Mcbsp.d \
./Library/Per_Src/AVP33x_MemCopy.d \
./Library/Per_Src/AVP33x_PieCtrl.d \
./Library/Per_Src/AVP33x_PieVect.d \
./Library/Per_Src/AVP33x_Sci.d \
./Library/Per_Src/AVP33x_Spi.d \
./Library/Per_Src/AVP33x_SysCtrl.d \
./Library/Per_Src/AVP33x_Xintf.d 

OBJS += \
./Library/Per_Src/AVP33x_ADC_cal.obj \
./Library/Per_Src/AVP33x_Adc.obj \
./Library/Per_Src/AVP33x_CSMPasswords.obj \
./Library/Per_Src/AVP33x_CodeStartBranch.obj \
./Library/Per_Src/AVP33x_CpuTimers.obj \
./Library/Per_Src/AVP33x_DBGIER.obj \
./Library/Per_Src/AVP33x_DMA.obj \
./Library/Per_Src/AVP33x_DefaultIsr.obj \
./Library/Per_Src/AVP33x_DisInt.obj \
./Library/Per_Src/AVP33x_ECan.obj \
./Library/Per_Src/AVP33x_ECap.obj \
./Library/Per_Src/AVP33x_EPwm.obj \
./Library/Per_Src/AVP33x_EQep.obj \
./Library/Per_Src/AVP33x_GlobalVariableDefs.obj \
./Library/Per_Src/AVP33x_Gpio.obj \
./Library/Per_Src/AVP33x_I2C.obj \
./Library/Per_Src/AVP33x_Mcbsp.obj \
./Library/Per_Src/AVP33x_MemCopy.obj \
./Library/Per_Src/AVP33x_PieCtrl.obj \
./Library/Per_Src/AVP33x_PieVect.obj \
./Library/Per_Src/AVP33x_Sci.obj \
./Library/Per_Src/AVP33x_Spi.obj \
./Library/Per_Src/AVP33x_SysCtrl.obj \
./Library/Per_Src/AVP33x_Xintf.obj \
./Library/Per_Src/AVP33x_usDelay.obj 

ASM_DEPS += \
./Library/Per_Src/AVP33x_ADC_cal.d \
./Library/Per_Src/AVP33x_CSMPasswords.d \
./Library/Per_Src/AVP33x_CodeStartBranch.d \
./Library/Per_Src/AVP33x_DBGIER.d \
./Library/Per_Src/AVP33x_DisInt.d \
./Library/Per_Src/AVP33x_usDelay.d 

OBJS__QUOTED += \
"Library\Per_Src\AVP33x_ADC_cal.obj" \
"Library\Per_Src\AVP33x_Adc.obj" \
"Library\Per_Src\AVP33x_CSMPasswords.obj" \
"Library\Per_Src\AVP33x_CodeStartBranch.obj" \
"Library\Per_Src\AVP33x_CpuTimers.obj" \
"Library\Per_Src\AVP33x_DBGIER.obj" \
"Library\Per_Src\AVP33x_DMA.obj" \
"Library\Per_Src\AVP33x_DefaultIsr.obj" \
"Library\Per_Src\AVP33x_DisInt.obj" \
"Library\Per_Src\AVP33x_ECan.obj" \
"Library\Per_Src\AVP33x_ECap.obj" \
"Library\Per_Src\AVP33x_EPwm.obj" \
"Library\Per_Src\AVP33x_EQep.obj" \
"Library\Per_Src\AVP33x_GlobalVariableDefs.obj" \
"Library\Per_Src\AVP33x_Gpio.obj" \
"Library\Per_Src\AVP33x_I2C.obj" \
"Library\Per_Src\AVP33x_Mcbsp.obj" \
"Library\Per_Src\AVP33x_MemCopy.obj" \
"Library\Per_Src\AVP33x_PieCtrl.obj" \
"Library\Per_Src\AVP33x_PieVect.obj" \
"Library\Per_Src\AVP33x_Sci.obj" \
"Library\Per_Src\AVP33x_Spi.obj" \
"Library\Per_Src\AVP33x_SysCtrl.obj" \
"Library\Per_Src\AVP33x_Xintf.obj" \
"Library\Per_Src\AVP33x_usDelay.obj" 

C_DEPS__QUOTED += \
"Library\Per_Src\AVP33x_Adc.d" \
"Library\Per_Src\AVP33x_CpuTimers.d" \
"Library\Per_Src\AVP33x_DMA.d" \
"Library\Per_Src\AVP33x_DefaultIsr.d" \
"Library\Per_Src\AVP33x_ECan.d" \
"Library\Per_Src\AVP33x_ECap.d" \
"Library\Per_Src\AVP33x_EPwm.d" \
"Library\Per_Src\AVP33x_EQep.d" \
"Library\Per_Src\AVP33x_GlobalVariableDefs.d" \
"Library\Per_Src\AVP33x_Gpio.d" \
"Library\Per_Src\AVP33x_I2C.d" \
"Library\Per_Src\AVP33x_Mcbsp.d" \
"Library\Per_Src\AVP33x_MemCopy.d" \
"Library\Per_Src\AVP33x_PieCtrl.d" \
"Library\Per_Src\AVP33x_PieVect.d" \
"Library\Per_Src\AVP33x_Sci.d" \
"Library\Per_Src\AVP33x_Spi.d" \
"Library\Per_Src\AVP33x_SysCtrl.d" \
"Library\Per_Src\AVP33x_Xintf.d" 

ASM_DEPS__QUOTED += \
"Library\Per_Src\AVP33x_ADC_cal.d" \
"Library\Per_Src\AVP33x_CSMPasswords.d" \
"Library\Per_Src\AVP33x_CodeStartBranch.d" \
"Library\Per_Src\AVP33x_DBGIER.d" \
"Library\Per_Src\AVP33x_DisInt.d" \
"Library\Per_Src\AVP33x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../Library/Per_Src/AVP33x_ADC_cal.asm" \
"../Library/Per_Src/AVP33x_CSMPasswords.asm" \
"../Library/Per_Src/AVP33x_CodeStartBranch.asm" \
"../Library/Per_Src/AVP33x_DBGIER.asm" \
"../Library/Per_Src/AVP33x_DisInt.asm" \
"../Library/Per_Src/AVP33x_usDelay.asm" 

C_SRCS__QUOTED += \
"../Library/Per_Src/AVP33x_Adc.c" \
"../Library/Per_Src/AVP33x_CpuTimers.c" \
"../Library/Per_Src/AVP33x_DMA.c" \
"../Library/Per_Src/AVP33x_DefaultIsr.c" \
"../Library/Per_Src/AVP33x_ECan.c" \
"../Library/Per_Src/AVP33x_ECap.c" \
"../Library/Per_Src/AVP33x_EPwm.c" \
"../Library/Per_Src/AVP33x_EQep.c" \
"../Library/Per_Src/AVP33x_GlobalVariableDefs.c" \
"../Library/Per_Src/AVP33x_Gpio.c" \
"../Library/Per_Src/AVP33x_I2C.c" \
"../Library/Per_Src/AVP33x_Mcbsp.c" \
"../Library/Per_Src/AVP33x_MemCopy.c" \
"../Library/Per_Src/AVP33x_PieCtrl.c" \
"../Library/Per_Src/AVP33x_PieVect.c" \
"../Library/Per_Src/AVP33x_Sci.c" \
"../Library/Per_Src/AVP33x_Spi.c" \
"../Library/Per_Src/AVP33x_SysCtrl.c" \
"../Library/Per_Src/AVP33x_Xintf.c" 


