################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
SubProgra/%.obj: ../SubProgra/%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -O2 --opt_for_speed=3 --include_path="D:/036/demoUpdataV1/20260722/or/send/ADM32F036A3DEMO_20260720" --include_path="D:/036/demoUpdataV1/20260722/or/send/ADM32F036A3DEMO_20260720/HeardFiles/SourceHeader" --include_path="D:/036/demoUpdataV1/20260722/or/send/ADM32F036A3DEMO_20260720/HeardFiles/ADP32F03xHeader" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=SHORT_GND_TEST_N --define=LIN_UPDATE --define=EXTINT_PWMGET_n --define=IPD_PERIOD_AUTO_DECT_n --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT_n --define=FREQUENCY_CONTROL_01HZ --define=POSTIVE_DRIVE_LOGIC --define=OPEN_LOOP_START --define=FULL_OPEN_LOOP_MODE_n --define=LIN_CONTROL_MODE --define=INTER_OSC11 --define="LARGE_MODEL" --define=SPIN_CONTROL_WITH_BEMF --define=OV_LIMIT_WITH_ID_n -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="SubProgra/$(basename $(<F)).d_raw" --obj_directory="SubProgra" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SubProgra/%.obj: ../SubProgra/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -O2 --opt_for_speed=3 --include_path="D:/036/demoUpdataV1/20260722/or/send/ADM32F036A3DEMO_20260720" --include_path="D:/036/demoUpdataV1/20260722/or/send/ADM32F036A3DEMO_20260720/HeardFiles/SourceHeader" --include_path="D:/036/demoUpdataV1/20260722/or/send/ADM32F036A3DEMO_20260720/HeardFiles/ADP32F03xHeader" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=SHORT_GND_TEST_N --define=LIN_UPDATE --define=EXTINT_PWMGET_n --define=IPD_PERIOD_AUTO_DECT_n --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT_n --define=FREQUENCY_CONTROL_01HZ --define=POSTIVE_DRIVE_LOGIC --define=OPEN_LOOP_START --define=FULL_OPEN_LOOP_MODE_n --define=LIN_CONTROL_MODE --define=INTER_OSC11 --define="LARGE_MODEL" --define=SPIN_CONTROL_WITH_BEMF --define=OV_LIMIT_WITH_ID_n -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="SubProgra/$(basename $(<F)).d_raw" --obj_directory="SubProgra" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


