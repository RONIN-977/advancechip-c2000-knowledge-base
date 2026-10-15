################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Main/%.obj: ../Main/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -Ooff --opt_for_speed=3 --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_old" --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_old/HeardFiles/SourceHeader" --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_old/HeardFiles/280xHeader" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --advice:performance=all --define=SHORT_GND_TEST_N --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT_N --define=FREQUENCY_CONTROL_01HZ --define=CUT_DOWN_VER_HARDWARE --define=OPEN_LOOP_START_N --define=FULL_OPEN_LOOP_MODE_N --define=LIN_CONTROL_MODE --define=INTER_OSC11 --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Main/$(basename $(<F)).d_raw" --obj_directory="Main" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Main/%.obj: ../Main/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -Ooff --opt_for_speed=3 --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_old" --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_old/HeardFiles/SourceHeader" --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_old/HeardFiles/280xHeader" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --advice:performance=all --define=SHORT_GND_TEST_N --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT_N --define=FREQUENCY_CONTROL_01HZ --define=CUT_DOWN_VER_HARDWARE --define=OPEN_LOOP_START_N --define=FULL_OPEN_LOOP_MODE_N --define=LIN_CONTROL_MODE --define=INTER_OSC11 --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Main/$(basename $(<F)).d_raw" --obj_directory="Main" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


