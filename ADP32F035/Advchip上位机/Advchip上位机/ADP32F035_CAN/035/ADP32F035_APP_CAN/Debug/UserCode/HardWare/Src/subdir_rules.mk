################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
UserCode/HardWare/Src/%.obj: ../UserCode/HardWare/Src/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_APP_CAN" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_APP_CAN/Library/Per_Inc" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_APP_CAN/UserCode/App/Inc" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_APP_CAN/UserCode/Com/Inc" --include_path="D:/Mr_Wei/AdvanceChip/02_SoftWare/ADP32F035_Routine/ADP32F035_APP_CAN/UserCode/HardWare/Inc" --include_path="D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="UserCode/HardWare/Src/$(basename $(<F)).d_raw" --obj_directory="UserCode/HardWare/Src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


