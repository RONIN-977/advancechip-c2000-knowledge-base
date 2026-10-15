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
../F2806x_CodeStartBranch_ebss_init.asm 

C_SRCS += \
../usb_dev_mouse.c \
../usb_mouse_structs.c 

C_DEPS += \
./usb_dev_mouse.d \
./usb_mouse_structs.d 

OBJS += \
./F2806x_CodeStartBranch_ebss_init.obj \
./usb_dev_mouse.obj \
./usb_mouse_structs.obj 

ASM_DEPS += \
./F2806x_CodeStartBranch_ebss_init.d 

OBJS__QUOTED += \
"F2806x_CodeStartBranch_ebss_init.obj" \
"usb_dev_mouse.obj" \
"usb_mouse_structs.obj" 

C_DEPS__QUOTED += \
"usb_dev_mouse.d" \
"usb_mouse_structs.d" 

ASM_DEPS__QUOTED += \
"F2806x_CodeStartBranch_ebss_init.d" 

ASM_SRCS__QUOTED += \
"../F2806x_CodeStartBranch_ebss_init.asm" 

C_SRCS__QUOTED += \
"../usb_dev_mouse.c" \
"../usb_mouse_structs.c" 


