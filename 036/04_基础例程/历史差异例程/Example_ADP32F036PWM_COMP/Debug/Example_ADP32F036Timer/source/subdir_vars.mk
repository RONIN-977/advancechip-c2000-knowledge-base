################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Example_ADP32F036Timer/source/ADP32F03x_CSMPasswords.asm \
../Example_ADP32F036Timer/source/ADP32F03x_CodeStartBranch.asm \
../Example_ADP32F036Timer/source/ADP32F03x_DBGIER.asm \
../Example_ADP32F036Timer/source/ADP32F03x_DisInt.asm \
../Example_ADP32F036Timer/source/ADP32F03x_usDelay.asm 

C_SRCS += \
../Example_ADP32F036Timer/source/ADP32F03x_Adc.c \
../Example_ADP32F036Timer/source/ADP32F03x_Canfd.c \
../Example_ADP32F036Timer/source/ADP32F03x_Comp.c \
../Example_ADP32F036Timer/source/ADP32F03x_CpuTimers.c \
../Example_ADP32F036Timer/source/ADP32F03x_DefaultIsr.c \
../Example_ADP32F036Timer/source/ADP32F03x_ECan.c \
../Example_ADP32F036Timer/source/ADP32F03x_ECap.c \
../Example_ADP32F036Timer/source/ADP32F03x_EPwm.c \
../Example_ADP32F036Timer/source/ADP32F03x_EQep.c \
../Example_ADP32F036Timer/source/ADP32F03x_GlobalVariableDefs.c \
../Example_ADP32F036Timer/source/ADP32F03x_Gpio.c \
../Example_ADP32F036Timer/source/ADP32F03x_HRCap.c \
../Example_ADP32F036Timer/source/ADP32F03x_I2C.c \
../Example_ADP32F036Timer/source/ADP32F03x_Lin.c \
../Example_ADP32F036Timer/source/ADP32F03x_MemCopy.c \
../Example_ADP32F036Timer/source/ADP32F03x_OscComp.c \
../Example_ADP32F036Timer/source/ADP32F03x_PieCtrl.c \
../Example_ADP32F036Timer/source/ADP32F03x_PieVect.c \
../Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedDefaultIsr.c \
../Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedPieVect.c \
../Example_ADP32F036Timer/source/ADP32F03x_Sci.c \
../Example_ADP32F036Timer/source/ADP32F03x_Spi.c \
../Example_ADP32F036Timer/source/ADP32F03x_SysCtrl.c \
../Example_ADP32F036Timer/source/ADP32F03x_TempSensorConv.c 

C_DEPS += \
./Example_ADP32F036Timer/source/ADP32F03x_Adc.d \
./Example_ADP32F036Timer/source/ADP32F03x_Canfd.d \
./Example_ADP32F036Timer/source/ADP32F03x_Comp.d \
./Example_ADP32F036Timer/source/ADP32F03x_CpuTimers.d \
./Example_ADP32F036Timer/source/ADP32F03x_DefaultIsr.d \
./Example_ADP32F036Timer/source/ADP32F03x_ECan.d \
./Example_ADP32F036Timer/source/ADP32F03x_ECap.d \
./Example_ADP32F036Timer/source/ADP32F03x_EPwm.d \
./Example_ADP32F036Timer/source/ADP32F03x_EQep.d \
./Example_ADP32F036Timer/source/ADP32F03x_GlobalVariableDefs.d \
./Example_ADP32F036Timer/source/ADP32F03x_Gpio.d \
./Example_ADP32F036Timer/source/ADP32F03x_HRCap.d \
./Example_ADP32F036Timer/source/ADP32F03x_I2C.d \
./Example_ADP32F036Timer/source/ADP32F03x_Lin.d \
./Example_ADP32F036Timer/source/ADP32F03x_MemCopy.d \
./Example_ADP32F036Timer/source/ADP32F03x_OscComp.d \
./Example_ADP32F036Timer/source/ADP32F03x_PieCtrl.d \
./Example_ADP32F036Timer/source/ADP32F03x_PieVect.d \
./Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedDefaultIsr.d \
./Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedPieVect.d \
./Example_ADP32F036Timer/source/ADP32F03x_Sci.d \
./Example_ADP32F036Timer/source/ADP32F03x_Spi.d \
./Example_ADP32F036Timer/source/ADP32F03x_SysCtrl.d \
./Example_ADP32F036Timer/source/ADP32F03x_TempSensorConv.d 

OBJS += \
./Example_ADP32F036Timer/source/ADP32F03x_Adc.obj \
./Example_ADP32F036Timer/source/ADP32F03x_CSMPasswords.obj \
./Example_ADP32F036Timer/source/ADP32F03x_Canfd.obj \
./Example_ADP32F036Timer/source/ADP32F03x_CodeStartBranch.obj \
./Example_ADP32F036Timer/source/ADP32F03x_Comp.obj \
./Example_ADP32F036Timer/source/ADP32F03x_CpuTimers.obj \
./Example_ADP32F036Timer/source/ADP32F03x_DBGIER.obj \
./Example_ADP32F036Timer/source/ADP32F03x_DefaultIsr.obj \
./Example_ADP32F036Timer/source/ADP32F03x_DisInt.obj \
./Example_ADP32F036Timer/source/ADP32F03x_ECan.obj \
./Example_ADP32F036Timer/source/ADP32F03x_ECap.obj \
./Example_ADP32F036Timer/source/ADP32F03x_EPwm.obj \
./Example_ADP32F036Timer/source/ADP32F03x_EQep.obj \
./Example_ADP32F036Timer/source/ADP32F03x_GlobalVariableDefs.obj \
./Example_ADP32F036Timer/source/ADP32F03x_Gpio.obj \
./Example_ADP32F036Timer/source/ADP32F03x_HRCap.obj \
./Example_ADP32F036Timer/source/ADP32F03x_I2C.obj \
./Example_ADP32F036Timer/source/ADP32F03x_Lin.obj \
./Example_ADP32F036Timer/source/ADP32F03x_MemCopy.obj \
./Example_ADP32F036Timer/source/ADP32F03x_OscComp.obj \
./Example_ADP32F036Timer/source/ADP32F03x_PieCtrl.obj \
./Example_ADP32F036Timer/source/ADP32F03x_PieVect.obj \
./Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedDefaultIsr.obj \
./Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedPieVect.obj \
./Example_ADP32F036Timer/source/ADP32F03x_Sci.obj \
./Example_ADP32F036Timer/source/ADP32F03x_Spi.obj \
./Example_ADP32F036Timer/source/ADP32F03x_SysCtrl.obj \
./Example_ADP32F036Timer/source/ADP32F03x_TempSensorConv.obj \
./Example_ADP32F036Timer/source/ADP32F03x_usDelay.obj 

ASM_DEPS += \
./Example_ADP32F036Timer/source/ADP32F03x_CSMPasswords.d \
./Example_ADP32F036Timer/source/ADP32F03x_CodeStartBranch.d \
./Example_ADP32F036Timer/source/ADP32F03x_DBGIER.d \
./Example_ADP32F036Timer/source/ADP32F03x_DisInt.d \
./Example_ADP32F036Timer/source/ADP32F03x_usDelay.d 

OBJS__QUOTED += \
"Example_ADP32F036Timer\source\ADP32F03x_Adc.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_CSMPasswords.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_Canfd.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_CodeStartBranch.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_Comp.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_CpuTimers.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_DBGIER.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_DefaultIsr.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_DisInt.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_ECan.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_ECap.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_EPwm.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_EQep.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_GlobalVariableDefs.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_Gpio.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_HRCap.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_I2C.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_Lin.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_MemCopy.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_OscComp.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_PieCtrl.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_PieVect.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_SWPrioritizedDefaultIsr.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_SWPrioritizedPieVect.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_Sci.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_Spi.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_SysCtrl.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_TempSensorConv.obj" \
"Example_ADP32F036Timer\source\ADP32F03x_usDelay.obj" 

C_DEPS__QUOTED += \
"Example_ADP32F036Timer\source\ADP32F03x_Adc.d" \
"Example_ADP32F036Timer\source\ADP32F03x_Canfd.d" \
"Example_ADP32F036Timer\source\ADP32F03x_Comp.d" \
"Example_ADP32F036Timer\source\ADP32F03x_CpuTimers.d" \
"Example_ADP32F036Timer\source\ADP32F03x_DefaultIsr.d" \
"Example_ADP32F036Timer\source\ADP32F03x_ECan.d" \
"Example_ADP32F036Timer\source\ADP32F03x_ECap.d" \
"Example_ADP32F036Timer\source\ADP32F03x_EPwm.d" \
"Example_ADP32F036Timer\source\ADP32F03x_EQep.d" \
"Example_ADP32F036Timer\source\ADP32F03x_GlobalVariableDefs.d" \
"Example_ADP32F036Timer\source\ADP32F03x_Gpio.d" \
"Example_ADP32F036Timer\source\ADP32F03x_HRCap.d" \
"Example_ADP32F036Timer\source\ADP32F03x_I2C.d" \
"Example_ADP32F036Timer\source\ADP32F03x_Lin.d" \
"Example_ADP32F036Timer\source\ADP32F03x_MemCopy.d" \
"Example_ADP32F036Timer\source\ADP32F03x_OscComp.d" \
"Example_ADP32F036Timer\source\ADP32F03x_PieCtrl.d" \
"Example_ADP32F036Timer\source\ADP32F03x_PieVect.d" \
"Example_ADP32F036Timer\source\ADP32F03x_SWPrioritizedDefaultIsr.d" \
"Example_ADP32F036Timer\source\ADP32F03x_SWPrioritizedPieVect.d" \
"Example_ADP32F036Timer\source\ADP32F03x_Sci.d" \
"Example_ADP32F036Timer\source\ADP32F03x_Spi.d" \
"Example_ADP32F036Timer\source\ADP32F03x_SysCtrl.d" \
"Example_ADP32F036Timer\source\ADP32F03x_TempSensorConv.d" 

ASM_DEPS__QUOTED += \
"Example_ADP32F036Timer\source\ADP32F03x_CSMPasswords.d" \
"Example_ADP32F036Timer\source\ADP32F03x_CodeStartBranch.d" \
"Example_ADP32F036Timer\source\ADP32F03x_DBGIER.d" \
"Example_ADP32F036Timer\source\ADP32F03x_DisInt.d" \
"Example_ADP32F036Timer\source\ADP32F03x_usDelay.d" 

C_SRCS__QUOTED += \
"../Example_ADP32F036Timer/source/ADP32F03x_Adc.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_Canfd.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_Comp.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_CpuTimers.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_DefaultIsr.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_ECan.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_ECap.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_EPwm.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_EQep.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_GlobalVariableDefs.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_Gpio.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_HRCap.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_I2C.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_Lin.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_MemCopy.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_OscComp.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_PieCtrl.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_PieVect.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedDefaultIsr.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_SWPrioritizedPieVect.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_Sci.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_Spi.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_SysCtrl.c" \
"../Example_ADP32F036Timer/source/ADP32F03x_TempSensorConv.c" 

ASM_SRCS__QUOTED += \
"../Example_ADP32F036Timer/source/ADP32F03x_CSMPasswords.asm" \
"../Example_ADP32F036Timer/source/ADP32F03x_CodeStartBranch.asm" \
"../Example_ADP32F036Timer/source/ADP32F03x_DBGIER.asm" \
"../Example_ADP32F036Timer/source/ADP32F03x_DisInt.asm" \
"../Example_ADP32F036Timer/source/ADP32F03x_usDelay.asm" 


