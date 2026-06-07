################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include galaxy_sdk/drivers/src/subdir.mk
-include galaxy_sdk/bsp/src/subdir.mk
-include galaxy_sdk/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := qemu
BUILD_ARTIFACT_EXTENSION := out
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
qemu.hex \

SECONDARY_LIST += \
qemu.lst \

SECONDARY_SIZE += \
qemu.siz \


# All Target
all: main-build

# Main-build Target
main-build: qemu.out secondary-outputs

# Tool invocations
qemu.out: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C++ Linker'
	riscv64-unknown-elf-g++ -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -T "F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\n309_iot_qemu.ld" -nostartfiles -Xlinker --gc-sections -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp" -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\lib" -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\lib" -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\lib" -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\lib" -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\lib" -L"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health" -Wl,-Map,"qemu.map" -u _printf_float -Wl,--no-warn-rwx-segments -o "qemu.out" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

qemu.hex: qemu.out makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv64-unknown-elf-objcopy -O ihex "qemu.out"  "qemu.hex"
	@echo 'Finished building: $@'
	@echo ' '

qemu.lst: qemu.out makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	riscv64-unknown-elf-objdump --source --all-headers --demangle --line-numbers --wide "qemu.out" > "qemu.lst"
	@echo 'Finished building: $@'
	@echo ' '

qemu.siz: qemu.out makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv64-unknown-elf-size --format=berkeley "qemu.out"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CCM_DEPS)$(C_UPPER_DEPS)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(CXXM_DEPS)$(C_DEPS)$(CC_DEPS)$(SX_DEPS)$(C++_DEPS)$(OBJS)$(CXX_DEPS)$(SECONDARY_FLASH)$(ASM_DEPS)$(S_UPPER_DEPS)$(C++M_DEPS)$(CPP_DEPS) qemu.out
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
