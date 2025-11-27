################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/01soft/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg" -I"D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg/Debug" -I"D:/01soft/ti/mspm0_sdk_2_06_00_05/source/third_party/CMSIS/Core/Include" -I"D:/01soft/ti/mspm0_sdk_2_06_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1818790175: ../lab05.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/01soft/ti/ccs1281/ccs/utils/sysconfig_1.21.0/sysconfig_cli.bat" --script "D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg/lab05.syscfg" -o "." -s "D:/01soft/ti/mspm0_sdk_2_06_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1818790175 ../lab05.syscfg
device.opt: build-1818790175
device.cmd.genlibs: build-1818790175
ti_msp_dl_config.c: build-1818790175
ti_msp_dl_config.h: build-1818790175
Event.dot: build-1818790175

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/01soft/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg" -I"D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg/Debug" -I"D:/01soft/ti/mspm0_sdk_2_06_00_05/source/third_party/CMSIS/Core/Include" -I"D:/01soft/ti/mspm0_sdk_2_06_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: D:/01soft/ti/mspm0_sdk_2_06_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/01soft/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg" -I"D:/03documents/2025/02TIproject/RemoteSystemsTempFiles/37520232205086_lab05_syscfg/Debug" -I"D:/01soft/ti/mspm0_sdk_2_06_00_05/source/third_party/CMSIS/Core/Include" -I"D:/01soft/ti/mspm0_sdk_2_06_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


