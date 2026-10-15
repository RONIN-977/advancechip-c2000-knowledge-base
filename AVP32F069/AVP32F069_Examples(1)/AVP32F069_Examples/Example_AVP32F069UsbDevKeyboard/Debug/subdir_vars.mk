################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../AVP32F069_RAM_lnk.cmd 

LIB_SRCS += \
../driverlib.lib \
../usblib.lib 

ASM_SRCS += \
../AVP32F069_CodeStartBranch_ebss_init.asm 

C_SRCS += \
../usb_dev_keyboard.c \
../usb_keyb_structs.c 

C_DEPS += \
./usb_dev_keyboard.d \
./usb_keyb_structs.d 

OBJS += \
./AVP32F069_CodeStartBranch_ebss_init.obj \
./usb_dev_keyboard.obj \
./usb_keyb_structs.obj 

ASM_DEPS += \
./AVP32F069_CodeStartBranch_ebss_init.d 

OBJS__QUOTED += \
"AVP32F069_CodeStartBranch_ebss_init.obj" \
"usb_dev_keyboard.obj" \
"usb_keyb_structs.obj" 

C_DEPS__QUOTED += \
"usb_dev_keyboard.d" \
"usb_keyb_structs.d" 

ASM_DEPS__QUOTED += \
"AVP32F069_CodeStartBranch_ebss_init.d" 

ASM_SRCS__QUOTED += \
"../AVP32F069_CodeStartBranch_ebss_init.asm" 

C_SRCS__QUOTED += \
"../usb_dev_keyboard.c" \
"../usb_keyb_structs.c" 


