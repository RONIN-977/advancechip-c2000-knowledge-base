################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Function/%.obj: ../Function/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -Ooff --opt_for_speed=2 --include_path="C:/Users/Allen1/Desktop/AutoACDrive_CANAPP20250718/AutoACDrive_CANAPP20250718" --include_path="C:/Users/Allen1/Desktop/AutoACDrive_CANAPP20250718/AutoACDrive_CANAPP20250718/HeardFiles/SourceHeader" --include_path="C:/Users/Allen1/Desktop/AutoACDrive_CANAPP20250718/AutoACDrive_CANAPP20250718/HeardFiles/280xHeader" --include_path="C:/CCS8.2/ccsv8/tools/compiler/ti-cgt-c2000_18.1.3.LTS/include" --advice:performance=all --define=REFRIGERATOR --define=OPEN_LOOP_START_N --define=SF_INDEPAND_OF_FREQUENCY_N --define=IPD_PERIOD_AUTO_DECT_N --define=CUT_DOWN_VER_HARDWARE --define=INTER_OSC_N --define=FREQUENCY_CONTROL_01HZ --define="LARGE_MODEL" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Function/$(basename $(<F)).d_raw" --obj_directory="Function" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


