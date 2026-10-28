################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
user_source/Boot_\ file_\ encryption.obj: ../user_source/Boot_\ file_\ encryption.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/c2000/ti-cgt-c2000_16.9.11.LTS/bin/cl2000" -v28 -ml -mt --include_path="D:/Users/PROM12K-E/Unlock_CKFA - fuben" --include_path="C:/ti/c2000/ti-cgt-c2000_16.9.11.LTS/include" --include_path="D:/Users/PROM12K-E/Unlock_CKFA - fuben/sys_include" --include_path="D:/Users/PROM12K-E/Unlock_CKFA - fuben/user_include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="user_source/Boot_ file_ encryption.d_raw" --obj_directory="user_source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

user_source/%.obj: ../user_source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/c2000/ti-cgt-c2000_16.9.11.LTS/bin/cl2000" -v28 -ml -mt --include_path="D:/Users/PROM12K-E/Unlock_CKFA - fuben" --include_path="C:/ti/c2000/ti-cgt-c2000_16.9.11.LTS/include" --include_path="D:/Users/PROM12K-E/Unlock_CKFA - fuben/sys_include" --include_path="D:/Users/PROM12K-E/Unlock_CKFA - fuben/user_include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="user_source/$(basename $(<F)).d_raw" --obj_directory="user_source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


