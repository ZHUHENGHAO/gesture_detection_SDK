/*
 * Copyright (c) 2026, VeriSilicon Holdings Co., Ltd. All rights reserved
 *
 * 1. Redistributing the source code of this software is only allowed after
 * receiving explicit, written permission from VeriSilicon. The copyright notice,
 * this list of conditions and the following disclaimer must be retained in all
 * source code distributions.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 */

#include <stddef.h>
#include <stdint.h>
#include "vs_conf.h"
#include "soc_init.h"
#include "soc_sysctl.h"
#include "bsp.h"
#include "uart_printf.h"
#include "board.h"
#include "osal.h"
#include "vpi_error.h"
#include "vsd_error.h"
#include "hal_imu.h"
#include "main.h"

static ImuDevice *g_imu_dev = NULL;
static OsalSemaphore *g_imu_data_sem = NULL;

static void imu_data_ready_isr(void)
{
    /* Signal the task to read FIFO data.
     * Note: per SDK manual 4.3.3, after reading FIFO data the task loop must
     * call hal_imu_enable_interrupt to re-arm the interrupt, otherwise no
     * further interrupt will fire. */
    if (g_imu_data_sem != NULL) {
        osal_sem_post_isr(g_imu_data_sem);
    }
}

static void task_imu_interrupt(void *param)
{
    (void)param;

    /* Step 1: Get IMU device */
    g_imu_dev = hal_imu_get_device(IMU_DEV_ID_0);
    if (g_imu_dev == NULL) {
        uart_printf("[IMU] hal_imu_get_device failed\r\n");
        osal_delete_task(NULL);
        return;
    }

    /* Step 2: Power on */
    int ret = hal_imu_enable_power(g_imu_dev, true);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] enable power failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 3: Initialize IMU device driver */
    ret = hal_imu_init(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_init failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /*
     * Step 4: Set sensor default config.
     * QEMU simulator hardcodes FIFO parsing to expect specific shadow values:
     *   accel BWP=7  -> accel_cfg[7:6]=0xC0 (required by FIFO parser entry check)
     *   accel range=8 -> accel_cfg[3:0]=0x8
     *   gyro  BWP=7  -> gyro_bwp=7
     *   gyro  range=7 -> gyro_range=7
     *   gyro  ODR=5  -> gyro_odr=5 (50Hz)
     *   accel ODR=5  -> accel_odr=5 (50Hz)
     * set_sensor_default_cfg() produces exactly these values.
     * Skipping this step and configuring from scratch will fail at FIFO parse time.
     */
    ret = hal_imu_set_sensor_default_cfg(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_sensor_default_cfg failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /*
     * Step 5: Override accel ODR shadow from 5 → 7.
     * set_sensor_default_cfg() writes accel ODR = 5 at shadow offset 10,
     * but bmi160_set_normal_mode(mode=3) requires accel ODR = 7.
     * hal_imu_set_accel_cfg(..., IMU_SENSOR_ODR) sets shadow offset 10 = 7.
     * Also set gyro ODR explicitly to 7 via hal_imu_set_gyro_cfg,
     * since bmi160_set_normal_mode(mode=3) checks gyro shadow offsets.
     * Do NOT set range or BWP — they are hardcoded by the FIFO parser.
     */
    ret = hal_imu_set_accel_cfg(g_imu_dev, 0, 0, 50, IMU_SENSOR_ODR);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_accel_cfg(ODR) failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    ret = hal_imu_set_gyro_cfg(g_imu_dev, 0, 0, 50, IMU_SENSOR_ODR);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_gyro_cfg(ODR) failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 6: Config interrupt — FIFO watermark interrupt on INT1 pin */
    IMUInterruptPinSetting pin_cfg = {
        .output_en   = 1,
        .output_mode = 0,  /* push-pull */
        .output_type = 0,  /* active low */
        .edge_ctrl   = 0,  /* level trigger */
        .input_en    = 0,
    };

    IMUInterruptSetting irq_cfg = {
        .irq_channel   = IMU_DATA_PIN,
        .irq_type      = IMU_ACC_GYRO_FIFO_WATERMARK_INTERRUPT,
        .irq_pin_settg = pin_cfg,
        .fifo_full_irq_en = 0,
        .fifo_wtm_irq_en  = 1,
    };
    ret = hal_imu_cfg_interrupt(g_imu_dev, true, IMU_ACC_GYRO_FIFO_WATERMARK_INTERRUPT, &irq_cfg);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_cfg_interrupt failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 7: Set FIFO watermark level (bytes) — must match accel+gyro frame size */
    ret = hal_imu_set_fifo_wm(g_imu_dev, 49);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_fifo_wm failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 8: Config FIFO data format — gyro + accel */
    ret = hal_imu_set_fifo_cfg(g_imu_dev, IMU_FIFO_GYRO | IMU_FIFO_ACCEL, true);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_fifo_cfg failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 9: Flush FIFO — discard stale data before entering normal mode */
    ret = hal_imu_flush_fifo(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_flush_fifo failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 10: Switch to normal mode — MUST be last, after all config is done */
    ret = hal_imu_set_work_mode(g_imu_dev, IMU_ACCEL_GYRO, IMU_SEN_MODE_NORMAL);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_work_mode failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 11: Enable data ready interrupt and register callback */
    ret = hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_enable_interrupt failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    uart_printf("[IMU] Data-ready interrupt initialized, waiting for data...\r\n");

    /* Step 12: Task-level interrupt-driven loop */
    ImuGyroAccelData gyro_accel_data[64];
    uint16_t frame_count = 0;

    while (1) {
        int sem_ret = osal_sem_wait(g_imu_data_sem, OSAL_WAIT_FOREVER);
        if (sem_ret != OSAL_SUCCESS) {
            continue;
        }

        ret = hal_imu_read_gyro_accel(g_imu_dev, gyro_accel_data,
                                      sizeof(gyro_accel_data) / sizeof(gyro_accel_data[0]),
                                      &frame_count);
        if (ret == VSD_SUCCESS && frame_count > 0) {
            for (uint16_t i = 0; i < frame_count; i++) {
                uart_printf("[IMU] #%u  gx=%d  gy=%d  gz=%d  ax=%d  ay=%d  az=%d\r\n",
                            (unsigned int)i,
                            (int)gyro_accel_data[i].gx,
                            (int)gyro_accel_data[i].gy,
                            (int)gyro_accel_data[i].gz,
                            (int)gyro_accel_data[i].ax,
                            (int)gyro_accel_data[i].ay,
                            (int)gyro_accel_data[i].az);
            }
        }

        /* Per SDK manual 4.3.3: re-arm the interrupt after reading FIFO data,
         * otherwise no further interrupt will fire for the next data batch. */
        hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
    }
}

static void task_init_app(void *param)
{
    (void)param;

    int ret;
    BoardDevice board_dev;

    ret = board_register(board_get_ops());
    ret = vsd_to_vpi(ret);
    if (ret != VPI_SUCCESS) {
        uart_printf("board register failed %d", ret);
        goto exit;
    }
    ret = board_init((void *)&board_dev);
    ret = vsd_to_vpi(ret);
    if (ret != VPI_SUCCESS) {
        uart_printf("board init failed %d", ret);
        goto exit;
    }
    if (board_dev.name) {
        uart_printf("Board: %s", board_dev.name);
    }

    uart_printf("Hello VeriHealthi!\r\n");

    ret = osal_create_sem(&g_imu_data_sem);
    if (ret != OSAL_SUCCESS) {
        uart_printf("[IMU] osal_create_sem failed: %d\r\n", ret);
        goto exit;
    }

    osal_create_task(task_imu_interrupt, "task_imu_int", 1024, 4, NULL);
exit:
    osal_delete_task(NULL);
}

int main(void)
{
    int ret;

    ret = soc_init();
    ret = vsd_to_vpi(ret);
    if (ret != VPI_SUCCESS) {
        uart_printf("soc init error %d", ret);
        goto exit;
    } else {
        uart_printf("soc init done");
    }
    osal_pre_start_scheduler();
    osal_create_task(task_init_app, "init_app", 512, 1, NULL);
    osal_start_scheduler();
exit:
    goto exit;
}
