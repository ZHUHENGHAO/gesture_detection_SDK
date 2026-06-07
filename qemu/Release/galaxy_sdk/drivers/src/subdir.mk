################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/drivers/src/hal_imu.c \
F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/drivers/src/hal_rtc.c 

C_DEPS += \
./galaxy_sdk/drivers/src/hal_imu.d \
./galaxy_sdk/drivers/src/hal_rtc.d 

OBJS += \
./galaxy_sdk/drivers/src/hal_imu.o \
./galaxy_sdk/drivers/src/hal_rtc.o 


# Each subdirectory must supply rules for building sources it contributes
galaxy_sdk/drivers/src/hal_imu.o: F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/drivers/src/hal_imu.c galaxy_sdk/drivers/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include\arch\riscv\n309" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\config\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\PrivateInclude" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

galaxy_sdk/drivers/src/hal_rtc.o: F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/drivers/src/hal_rtc.c galaxy_sdk/drivers/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include\arch\riscv\n309" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\config\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\PrivateInclude" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


