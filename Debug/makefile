################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./CC1350_LAUNCHXL.obj" \
"./RFQueue.obj" \
"./ccfg.obj" \
"./config.obj" \
"./main_nortos.obj" \
"./menu_nortos.obj" \
"./rfPacketErrorRate_nortos.obj" \
"./rx_nortos.obj" \
"./tx_nortos.obj" \
"./smartrf_settings/smartrf_settings.obj" \
"./smartrf_settings/smartrf_settings_ble.obj" \
"./smartrf_settings/smartrf_settings_predefined.obj" \
"../CC1350_LAUNCHXL_NoRTOS.cmd" \
$(GEN_CMDS__FLAG) \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source/ti/display/lib/display.aem3" \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source/ti/mw/lcd/lib/lcd.aem3" \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source/ti/grlib/lib/grlib.aem3" \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source/ti/drivers/rf/lib/rf_multiMode_cc13x0.aem3" \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source/ti/drivers/lib/drivers_cc13x0.aem3" \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/kernel/nortos/lib/nortos_cc13x0.aem3" \
-l"/Applications/ti/simplelink_cc13x0_sdk_1_60_00_21/source/ti/devices/cc13x0/driverlib/bin/ccs/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include smartrf_settings/subdir_vars.mk
-include subdir_rules.mk
-include smartrf_settings/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.out \

EXE_OUTPUTS__QUOTED += \
"rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.out" \

BIN_OUTPUTS += \
rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.hex \

BIN_OUTPUTS__QUOTED += \
"rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.out"

# Tool invocations
rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --define=BOARD_DISPLAY_USE_LCD=1 --define=DeviceFamily_CC13X0 --define=CCFG_FORCE_VDDR_HH=0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.map" -i"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/lib" -i"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs_linkInfo.xml" --rom_model -o "rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armhex"  -o "rfPacketErrorRate_CC1350_LAUNCHXL_nortos_ccs.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "CC1350_LAUNCHXL.obj" "RFQueue.obj" "ccfg.obj" "config.obj" "main_nortos.obj" "menu_nortos.obj" "rfPacketErrorRate_nortos.obj" "rx_nortos.obj" "tx_nortos.obj" "smartrf_settings/smartrf_settings.obj" "smartrf_settings/smartrf_settings_ble.obj" "smartrf_settings/smartrf_settings_predefined.obj" 
	-$(RM) "CC1350_LAUNCHXL.d" "RFQueue.d" "ccfg.d" "config.d" "main_nortos.d" "menu_nortos.d" "rfPacketErrorRate_nortos.d" "rx_nortos.d" "tx_nortos.d" "smartrf_settings/smartrf_settings.d" "smartrf_settings/smartrf_settings_ble.d" "smartrf_settings/smartrf_settings_predefined.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

