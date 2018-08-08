################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
CC1350_LAUNCHXL.obj: ../CC1350_LAUNCHXL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="CC1350_LAUNCHXL.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

RFQueue.obj: ../RFQueue.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="RFQueue.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ccfg.obj: ../ccfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="ccfg.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

config.obj: ../config.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="config.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main_nortos.obj: ../main_nortos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main_nortos.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

menu_nortos.obj: ../menu_nortos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="menu_nortos.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

rfPacketErrorRate_nortos.obj: ../rfPacketErrorRate_nortos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="rfPacketErrorRate_nortos.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

rx_nortos.obj: ../rx_nortos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="rx_nortos.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

tx_nortos.obj: ../tx_nortos.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/natelammers/Downloads/Updated Pins Outs/rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos" --include_path="/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="tx_nortos.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


