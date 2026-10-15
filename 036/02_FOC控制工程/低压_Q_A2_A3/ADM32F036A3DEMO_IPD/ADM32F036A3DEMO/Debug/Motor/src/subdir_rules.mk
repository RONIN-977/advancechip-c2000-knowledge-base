################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Motor/src/%.obj: ../Motor/src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -O2 --opt_for_speed=3 --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_IPD/ADM32F036A3DEMO" --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_IPD/ADM32F036A3DEMO/HeardFiles/SourceHeader" --include_path="C:/Users/17879/Desktop/CHIP/036/ADM32F036A3/ADM32F036A3DEMO_IPD/ADM32F036A3DEMO/HeardFiles/ADP32F03xHeader" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --advice:performance=all --define=SHORT_GND_TEST_N --define=EXTINT_PWMGETEN --define=INTER_OSC --define=KE_WIDE_RANGE --define=IPD_PERIOD_AUTO_DECT --define=FREQUENCY_CONTROL_01HZ --define=POSTIVE_DRIVE_LOGIC --define=OPEN_LOOP_START2 --define=FULL_OPEN_LOOP_MODE11 --define=LIN_CONTROL_MODE --define="LARGE_MODEL" --define=SPIN_CONTROL_WITH_BEMF --define=OV_LIMIT_WITH_ID_n -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Motor/src/$(basename $(<F)).d_raw" --obj_directory="Motor/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


