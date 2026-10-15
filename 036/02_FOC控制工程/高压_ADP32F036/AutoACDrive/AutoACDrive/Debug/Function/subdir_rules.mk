################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Function/%.obj: ../Function/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_20.2.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -O2 --opt_for_speed=2 --include_path="C:/Users/17879/Desktop/CHIP/036/ADP32F036/AutoACDrive/AutoACDrive" --include_path="C:/Users/17879/Desktop/CHIP/036/ADP32F036/AutoACDrive/AutoACDrive/Motor/StaticVModelObs" --include_path="C:/Users/17879/Desktop/CHIP/036/ADP32F036/AutoACDrive/AutoACDrive/HeardFiles/SourceHeader" --include_path="C:/ti/ti-cgt-c2000_20.2.0.LTS/include" --advice:performance=all --define=REFRIGERATOR --define=FIVEPWM_ENABLE --define=POSTIVE_DRIVE_LOGIC --define=VOL_800V_DEMO --define=ENABLE_NEW_STATIC_OBS_1 --define=OPEN_LOOP_START2 --define=SF_INDEPAND_OF_FREQUENCY_N --define=IPD_PERIOD_AUTO_DECT_N --define=INTER_OSC --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Function/$(basename $(<F)).d_raw" --obj_directory="Function" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


