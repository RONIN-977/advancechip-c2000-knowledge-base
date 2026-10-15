################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../source/AVP32F069_CSMPasswords.asm \
../source/AVP32F069_CodeStartBranch.asm \
../source/AVP32F069_DBGIER.asm \
../source/AVP32F069_DisInt.asm \
../source/AVP32F069_usDelay.asm 

C_SRCS += \
../source/AVP32F069_Adc.c \
../source/AVP32F069_Comp.c \
../source/AVP32F069_CpuTimers.c \
../source/AVP32F069_DefaultIsr.c \
../source/AVP32F069_Dma.c \
../source/AVP32F069_ECan.c \
../source/AVP32F069_ECap.c \
../source/AVP32F069_EPwm.c \
../source/AVP32F069_EQep.c \
../source/AVP32F069_GlobalVariableDefs.c \
../source/AVP32F069_Gpio.c \
../source/AVP32F069_HRCap.c \
../source/AVP32F069_I2C.c \
../source/AVP32F069_Mcbsp.c \
../source/AVP32F069_OscComp.c \
../source/AVP32F069_PieCtrl.c \
../source/AVP32F069_PieVect.c \
../source/AVP32F069_Sci.c \
../source/AVP32F069_Spi.c \
../source/AVP32F069_SysCtrl.c \
../source/AVP32F069_TempSensorConv.c 

C_DEPS += \
./source/AVP32F069_Adc.d \
./source/AVP32F069_Comp.d \
./source/AVP32F069_CpuTimers.d \
./source/AVP32F069_DefaultIsr.d \
./source/AVP32F069_Dma.d \
./source/AVP32F069_ECan.d \
./source/AVP32F069_ECap.d \
./source/AVP32F069_EPwm.d \
./source/AVP32F069_EQep.d \
./source/AVP32F069_GlobalVariableDefs.d \
./source/AVP32F069_Gpio.d \
./source/AVP32F069_HRCap.d \
./source/AVP32F069_I2C.d \
./source/AVP32F069_Mcbsp.d \
./source/AVP32F069_OscComp.d \
./source/AVP32F069_PieCtrl.d \
./source/AVP32F069_PieVect.d \
./source/AVP32F069_Sci.d \
./source/AVP32F069_Spi.d \
./source/AVP32F069_SysCtrl.d \
./source/AVP32F069_TempSensorConv.d 

OBJS += \
./source/AVP32F069_Adc.obj \
./source/AVP32F069_CSMPasswords.obj \
./source/AVP32F069_CodeStartBranch.obj \
./source/AVP32F069_Comp.obj \
./source/AVP32F069_CpuTimers.obj \
./source/AVP32F069_DBGIER.obj \
./source/AVP32F069_DefaultIsr.obj \
./source/AVP32F069_DisInt.obj \
./source/AVP32F069_Dma.obj \
./source/AVP32F069_ECan.obj \
./source/AVP32F069_ECap.obj \
./source/AVP32F069_EPwm.obj \
./source/AVP32F069_EQep.obj \
./source/AVP32F069_GlobalVariableDefs.obj \
./source/AVP32F069_Gpio.obj \
./source/AVP32F069_HRCap.obj \
./source/AVP32F069_I2C.obj \
./source/AVP32F069_Mcbsp.obj \
./source/AVP32F069_OscComp.obj \
./source/AVP32F069_PieCtrl.obj \
./source/AVP32F069_PieVect.obj \
./source/AVP32F069_Sci.obj \
./source/AVP32F069_Spi.obj \
./source/AVP32F069_SysCtrl.obj \
./source/AVP32F069_TempSensorConv.obj \
./source/AVP32F069_usDelay.obj 

ASM_DEPS += \
./source/AVP32F069_CSMPasswords.d \
./source/AVP32F069_CodeStartBranch.d \
./source/AVP32F069_DBGIER.d \
./source/AVP32F069_DisInt.d \
./source/AVP32F069_usDelay.d 

OBJS__QUOTED += \
"source\AVP32F069_Adc.obj" \
"source\AVP32F069_CSMPasswords.obj" \
"source\AVP32F069_CodeStartBranch.obj" \
"source\AVP32F069_Comp.obj" \
"source\AVP32F069_CpuTimers.obj" \
"source\AVP32F069_DBGIER.obj" \
"source\AVP32F069_DefaultIsr.obj" \
"source\AVP32F069_DisInt.obj" \
"source\AVP32F069_Dma.obj" \
"source\AVP32F069_ECan.obj" \
"source\AVP32F069_ECap.obj" \
"source\AVP32F069_EPwm.obj" \
"source\AVP32F069_EQep.obj" \
"source\AVP32F069_GlobalVariableDefs.obj" \
"source\AVP32F069_Gpio.obj" \
"source\AVP32F069_HRCap.obj" \
"source\AVP32F069_I2C.obj" \
"source\AVP32F069_Mcbsp.obj" \
"source\AVP32F069_OscComp.obj" \
"source\AVP32F069_PieCtrl.obj" \
"source\AVP32F069_PieVect.obj" \
"source\AVP32F069_Sci.obj" \
"source\AVP32F069_Spi.obj" \
"source\AVP32F069_SysCtrl.obj" \
"source\AVP32F069_TempSensorConv.obj" \
"source\AVP32F069_usDelay.obj" 

C_DEPS__QUOTED += \
"source\AVP32F069_Adc.d" \
"source\AVP32F069_Comp.d" \
"source\AVP32F069_CpuTimers.d" \
"source\AVP32F069_DefaultIsr.d" \
"source\AVP32F069_Dma.d" \
"source\AVP32F069_ECan.d" \
"source\AVP32F069_ECap.d" \
"source\AVP32F069_EPwm.d" \
"source\AVP32F069_EQep.d" \
"source\AVP32F069_GlobalVariableDefs.d" \
"source\AVP32F069_Gpio.d" \
"source\AVP32F069_HRCap.d" \
"source\AVP32F069_I2C.d" \
"source\AVP32F069_Mcbsp.d" \
"source\AVP32F069_OscComp.d" \
"source\AVP32F069_PieCtrl.d" \
"source\AVP32F069_PieVect.d" \
"source\AVP32F069_Sci.d" \
"source\AVP32F069_Spi.d" \
"source\AVP32F069_SysCtrl.d" \
"source\AVP32F069_TempSensorConv.d" 

ASM_DEPS__QUOTED += \
"source\AVP32F069_CSMPasswords.d" \
"source\AVP32F069_CodeStartBranch.d" \
"source\AVP32F069_DBGIER.d" \
"source\AVP32F069_DisInt.d" \
"source\AVP32F069_usDelay.d" 

C_SRCS__QUOTED += \
"../source/AVP32F069_Adc.c" \
"../source/AVP32F069_Comp.c" \
"../source/AVP32F069_CpuTimers.c" \
"../source/AVP32F069_DefaultIsr.c" \
"../source/AVP32F069_Dma.c" \
"../source/AVP32F069_ECan.c" \
"../source/AVP32F069_ECap.c" \
"../source/AVP32F069_EPwm.c" \
"../source/AVP32F069_EQep.c" \
"../source/AVP32F069_GlobalVariableDefs.c" \
"../source/AVP32F069_Gpio.c" \
"../source/AVP32F069_HRCap.c" \
"../source/AVP32F069_I2C.c" \
"../source/AVP32F069_Mcbsp.c" \
"../source/AVP32F069_OscComp.c" \
"../source/AVP32F069_PieCtrl.c" \
"../source/AVP32F069_PieVect.c" \
"../source/AVP32F069_Sci.c" \
"../source/AVP32F069_Spi.c" \
"../source/AVP32F069_SysCtrl.c" \
"../source/AVP32F069_TempSensorConv.c" 

ASM_SRCS__QUOTED += \
"../source/AVP32F069_CSMPasswords.asm" \
"../source/AVP32F069_CodeStartBranch.asm" \
"../source/AVP32F069_DBGIER.asm" \
"../source/AVP32F069_DisInt.asm" \
"../source/AVP32F069_usDelay.asm" 


