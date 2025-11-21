################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/01soft/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./I2C_GPIO_25a.o" \
"./OLED13.o" \
"./OLED_BMP.o" \
"./OLED_Font.o" \
"./lab06_adc.o" \
"./startup_mspm0g350x_ticlang.o" \
"./ti_msp_dl_config.o" \
"../mspm0g3507.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-l"D:/03documents/2025/02TIproject/37520232205086_Lab06/iqmath.a" \
-Wl,-l"D:/01soft/ti/mspm0_sdk_2_06_00_05/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
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
37520232205086_Lab06.out 

EXE_OUTPUTS__QUOTED += \
"37520232205086_Lab06.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "37520232205086_Lab06.out"

# Tool invocations
37520232205086_Lab06.out: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"D:/01soft/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -D__MSPM0G3507__ -gdwarf-3 -Wl,-m"37520232205086_Lab06.map" -Wl,-i"D:/03documents/2025/02TIproject/37520232205086_Lab06" -Wl,-i"D:/01soft/ti/mspm0_sdk_2_06_00_05/source" -Wl,-i"D:/03documents/2025/02TIproject/37520232205086_Lab06" -Wl,-i"D:/01soft/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="37520232205086_Lab06_linkInfo.xml" -Wl,--rom_model -o "37520232205086_Lab06.out" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "I2C_GPIO_25a.o" "OLED13.o" "OLED_BMP.o" "OLED_Font.o" "lab06_adc.o" "startup_mspm0g350x_ticlang.o" "ti_msp_dl_config.o" 
	-$(RM) "I2C_GPIO_25a.d" "OLED13.d" "OLED_BMP.d" "OLED_Font.d" "lab06_adc.d" "startup_mspm0g350x_ticlang.d" "ti_msp_dl_config.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

