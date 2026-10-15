################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../AVP32F069_RAM_lnk.cmd 

LIB_SRCS += \
D:/v151/MWare/driverlib/Debug/driverlib.lib \
D:/v151/MWare/lib/usblib.lib 

ASM_SRCS += \
../AVP32F069_CodeStartBranch_ebss_init.asm 

C_SRCS += \
../usb_bulk_structs.c \
../usb_dev_bulk.c 

C_DEPS += \
./usb_bulk_structs.d \
./usb_dev_bulk.d 

OBJS += \
./AVP32F069_CodeStartBranch_ebss_init.obj \
./usb_bulk_structs.obj \
./usb_dev_bulk.obj 

ASM_DEPS += \
./AVP32F069_CodeStartBranch_ebss_init.d 

OBJS__QUOTED += \
"AVP32F069_CodeStartBranch_ebss_init.obj" \
"usb_bulk_structs.obj" \
"usb_dev_bulk.obj" 

C_DEPS__QUOTED += \
"usb_bulk_structs.d" \
"usb_dev_bulk.d" 

ASM_DEPS__QUOTED += \
"AVP32F069_CodeStartBranch_ebss_init.d" 

ASM_SRCS__QUOTED += \
"../AVP32F069_CodeStartBranch_ebss_init.asm" 

C_SRCS__QUOTED += \
"../usb_bulk_structs.c" \
"../usb_dev_bulk.c" 


