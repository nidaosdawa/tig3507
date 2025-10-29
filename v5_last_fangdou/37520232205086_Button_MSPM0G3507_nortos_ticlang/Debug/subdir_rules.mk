################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/01soft/TI/CSS/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang" -I"D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang/Debug" -I"D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source/third_party/CMSIS/Core/Include" -I"D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-158466079: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/01soft/TI/CSS/ccs/utils/sysconfig_1.21.0/sysconfig_cli.bat" --script "D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang/empty.syscfg" -o "." -s "D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-158466079 ../empty.syscfg
device.opt: build-158466079
device.cmd.genlibs: build-158466079
ti_msp_dl_config.c: build-158466079
ti_msp_dl_config.h: build-158466079
Event.dot: build-158466079

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/01soft/TI/CSS/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang" -I"D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang/Debug" -I"D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source/third_party/CMSIS/Core/Include" -I"D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/01soft/TI/CSS/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang" -I"D:/03project/2025/TIproject/37520232205086_Button_MSPM0G3507_nortos_ticlang/Debug" -I"D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source/third_party/CMSIS/Core/Include" -I"D:/01soft/TI/mspmo_sdk/mspm0_sdk_2_06_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


