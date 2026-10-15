################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../MWare/boot_loader/F28069_bl.cmd 

ASM_SRCS += \
../MWare/boot_loader/F2806x_CSMPasswords.asm \
../MWare/boot_loader/F2806x_CodeStartBranch.asm \
../MWare/boot_loader/bl_entrytable.asm 

C_SRCS += \
../MWare/boot_loader/F2806x_BL_MemCopy.c \
../MWare/boot_loader/bl_check.c \
../MWare/boot_loader/bl_decrypt.c \
../MWare/boot_loader/bl_usb.c \
../MWare/boot_loader/bl_usbfuncs.c 

C_DEPS += \
./MWare/boot_loader/F2806x_BL_MemCopy.d \
./MWare/boot_loader/bl_check.d \
./MWare/boot_loader/bl_decrypt.d \
./MWare/boot_loader/bl_usb.d \
./MWare/boot_loader/bl_usbfuncs.d 

OBJS += \
./MWare/boot_loader/F2806x_BL_MemCopy.obj \
./MWare/boot_loader/F2806x_CSMPasswords.obj \
./MWare/boot_loader/F2806x_CodeStartBranch.obj \
./MWare/boot_loader/bl_check.obj \
./MWare/boot_loader/bl_decrypt.obj \
./MWare/boot_loader/bl_entrytable.obj \
./MWare/boot_loader/bl_usb.obj \
./MWare/boot_loader/bl_usbfuncs.obj 

ASM_DEPS += \
./MWare/boot_loader/F2806x_CSMPasswords.d \
./MWare/boot_loader/F2806x_CodeStartBranch.d \
./MWare/boot_loader/bl_entrytable.d 

OBJS__QUOTED += \
"MWare\boot_loader\F2806x_BL_MemCopy.obj" \
"MWare\boot_loader\F2806x_CSMPasswords.obj" \
"MWare\boot_loader\F2806x_CodeStartBranch.obj" \
"MWare\boot_loader\bl_check.obj" \
"MWare\boot_loader\bl_decrypt.obj" \
"MWare\boot_loader\bl_entrytable.obj" \
"MWare\boot_loader\bl_usb.obj" \
"MWare\boot_loader\bl_usbfuncs.obj" 

C_DEPS__QUOTED += \
"MWare\boot_loader\F2806x_BL_MemCopy.d" \
"MWare\boot_loader\bl_check.d" \
"MWare\boot_loader\bl_decrypt.d" \
"MWare\boot_loader\bl_usb.d" \
"MWare\boot_loader\bl_usbfuncs.d" 

ASM_DEPS__QUOTED += \
"MWare\boot_loader\F2806x_CSMPasswords.d" \
"MWare\boot_loader\F2806x_CodeStartBranch.d" \
"MWare\boot_loader\bl_entrytable.d" 

C_SRCS__QUOTED += \
"../MWare/boot_loader/F2806x_BL_MemCopy.c" \
"../MWare/boot_loader/bl_check.c" \
"../MWare/boot_loader/bl_decrypt.c" \
"../MWare/boot_loader/bl_usb.c" \
"../MWare/boot_loader/bl_usbfuncs.c" 

ASM_SRCS__QUOTED += \
"../MWare/boot_loader/F2806x_CSMPasswords.asm" \
"../MWare/boot_loader/F2806x_CodeStartBranch.asm" \
"../MWare/boot_loader/bl_entrytable.asm" 


