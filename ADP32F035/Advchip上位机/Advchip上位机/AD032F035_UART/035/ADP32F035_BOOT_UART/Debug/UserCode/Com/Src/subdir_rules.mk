################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
UserCode/Com/Src/%.obj: ../UserCode/Com/Src/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_BOOT_UART" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_BOOT_UART/Library/Per_Inc" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_BOOT_UART/UserCode/App/Inc" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_BOOT_UART/UserCode/Com/Inc" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_BOOT_UART/UserCode/HardWare/Inc" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="UserCode/Com/Src/$(basename $(<F)).d_raw" --obj_directory="UserCode/Com/Src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


