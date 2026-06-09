################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/intexc_riscv.S \
F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/portasm.S \
F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/startup_riscv.S 

C_SRCS += \
F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/qemu_board.c 

C_DEPS += \
./galaxy_sdk/bsp/src/qemu_board.d 

OBJS += \
./galaxy_sdk/bsp/src/intexc_riscv.o \
./galaxy_sdk/bsp/src/portasm.o \
./galaxy_sdk/bsp/src/qemu_board.o \
./galaxy_sdk/bsp/src/startup_riscv.o 

S_UPPER_DEPS += \
./galaxy_sdk/bsp/src/intexc_riscv.d \
./galaxy_sdk/bsp/src/portasm.d \
./galaxy_sdk/bsp/src/startup_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
galaxy_sdk/bsp/src/intexc_riscv.o: F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/intexc_riscv.S galaxy_sdk/bsp/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv64-unknown-elf-gcc -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -x assembler-with-cpp -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include\arch\riscv\n309" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\config\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\PrivateInclude" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health\include" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

galaxy_sdk/bsp/src/portasm.o: F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/portasm.S galaxy_sdk/bsp/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv64-unknown-elf-gcc -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -x assembler-with-cpp -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include\arch\riscv\n309" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\config\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\PrivateInclude" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health\include" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

galaxy_sdk/bsp/src/qemu_board.o: F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/qemu_board.c galaxy_sdk/bsp/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv64-unknown-elf-gcc -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include\arch\riscv\n309" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\algorithm" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\tasks" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\config\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\PrivateInclude" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health\include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

galaxy_sdk/bsp/src/startup_riscv.o: F:/data/VeriSilicon/VeriHealthi_QEMU_SDK.202606_preliminary/V3.0-VeriHealthi_QEMU_SDK_v3.7/galaxy_sdk/bsp/src/startup_riscv.S galaxy_sdk/bsp/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv64-unknown-elf-gcc -march=rv32imafc_xxldsp -mabi=ilp32f -mtune=nuclei-300-series -mcmodel=medlow -mno-save-restore -O2 -ffunction-sections -fdata-sections -fno-common -Werror -Wall -g -x assembler-with-cpp -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include\arch\riscv\n309" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\bsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\config\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\drivers\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\external\riscv_dsp\PrivateInclude" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\modules\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\os\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\osal\include" -I"F:\data\VeriSilicon\VeriHealthi_QEMU_SDK.202606_preliminary\V3.0-VeriHealthi_QEMU_SDK_v3.7\galaxy_sdk\prebuilts\bluetooth\health\include" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


