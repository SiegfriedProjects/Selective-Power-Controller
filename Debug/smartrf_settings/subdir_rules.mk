################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
smartrf_settings/smartrf_settings.obj: ../smartrf_settings/smartrf_settings.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="smartrf_settings/smartrf_settings.d_raw" --obj_directory="smartrf_settings" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

smartrf_settings/smartrf_settings_ble.obj: ../smartrf_settings/smartrf_settings_ble.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="smartrf_settings/smartrf_settings_ble.d_raw" --obj_directory="smartrf_settings" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

smartrf_settings/smartrf_settings_predefined.obj: ../smartrf_settings/smartrf_settings_predefined.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="smartrf_settings/smartrf_settings_predefined.d_raw" --obj_directory="smartrf_settings" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


