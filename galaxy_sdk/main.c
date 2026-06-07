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

#define IMU_TIMER_PERIOD_MS 20

static ImuDevice *g_imu_dev = NULL;
static OsalSemaphore *g_imu_data_sem = NULL;

static void imu_data_ready_isr(void)
{
    if (g_imu_data_sem != NULL) {
        osal_sem_post_isr(g_imu_data_sem);
    }
}

static void imu_timer_callback(OsalTimer *timer, void *param)
{
    (void)timer;
    (void)param;
    imu_data_ready_isr();
}

static void task_imu_interrupt(void *param)
{
    (void)param;

    g_imu_dev = hal_imu_get_device(IMU_DEV_ID_0);
    if (g_imu_dev == NULL) {
        uart_printf("[IMU] hal_imu_get_device failed\r\n");
        osal_delete_task(NULL);
        return;
    }

    /* Step 1: Power on */
    int ret = hal_imu_enable_power(g_imu_dev, true);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] enable power failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 2: Initialize IMU device driver */
    ret = hal_imu_init(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_init failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 3: Set default range and bandwidth */
    ret = hal_imu_set_sensor_default_cfg(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_sensor_default_cfg failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /*
     * Step 4: Set ODR to 50Hz (REQUIRED by simulator before normal mode).
     *   range = 2  -> ±2G  (accelerometer)
     *   range = 250 -> ±250 dps (gyroscope)
     *   bwp   = 2  -> normal mode bandwidth
     *   odr   = 50 -> 50 Hz output data rate
     */
    ret = hal_imu_set_accel_cfg(g_imu_dev, 2, 2, 50, IMU_SENSOR_ODR);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_accel_cfg(ODR) failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    ret = hal_imu_set_gyro_cfg(g_imu_dev, 250, 2, 50, IMU_SENSOR_ODR);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_gyro_cfg(ODR) failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 5: Switch to normal mode (now ODR is configured, this will succeed) */
    ret = hal_imu_set_work_mode(g_imu_dev, IMU_ACCEL_GYRO, IMU_SEN_MODE_NORMAL);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_work_mode failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 6: Configure FIFO */
    ret = hal_imu_set_fifo_wm(g_imu_dev, FIFO_WATERMARK_LEVEL);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_fifo_wm failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    ret = hal_imu_set_fifo_cfg(g_imu_dev, IMU_FIFO_GYRO | IMU_FIFO_ACCEL, true);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_fifo_cfg failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    ret = hal_imu_flush_fifo(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_flush_fifo failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 7: Configure interrupt */
    ret = hal_imu_cfg_interrupt(g_imu_dev, true, IMU_ACC_GYRO_FIFO_WATERMARK_INTERRUPT, NULL);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_cfg_interrupt failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    ret = hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_enable_interrupt failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    /* Step 8: Start a 20ms software timer to simulate hardware data-ready interrupt */
    OsalTimer *imu_timer = NULL;
    ret = osal_timer_create(&imu_timer, "imu_timer", IMU_TIMER_PERIOD_MS, true,
                            imu_timer_callback, NULL);
    if (ret != OSAL_SUCCESS) {
        uart_printf("[IMU] osal_timer_create failed: %d\r\n", ret);
        osal_delete_task(NULL);
        return;
    }

    ret = osal_timer_start(imu_timer, OSAL_WAIT_FOREVER);
    if (ret != OSAL_SUCCESS) {
        uart_printf("[IMU] osal_timer_start failed: %d\r\n", ret);
        osal_timer_delete(imu_timer, OSAL_WAIT_FOREVER);
        osal_delete_task(NULL);
        return;
    }

    uart_printf("[IMU] Interrupt mode initialized (period=%dms), waiting for data...\r\n",
                IMU_TIMER_PERIOD_MS);

    /* Step 9: Task-level interrupt-driven loop */
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

        hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
    }
}

static void task_init_app(void *param)
{
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
