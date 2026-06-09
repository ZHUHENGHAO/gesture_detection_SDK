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
#include "tasks/app.h"

/* FIFO buffer for IMU data read */
#define IMU_FIFO_BUF_SIZE (255)
static uint8_t s_imu_fifo_buf[IMU_FIFO_BUF_SIZE];

ImuDevice *g_imu_dev = NULL;
OsalSemaphore *g_imu_data_sem = NULL;

void imu_data_ready_isr(void)
{
    if (g_imu_data_sem != NULL) {
        osal_sem_post_isr(g_imu_data_sem);
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

    g_imu_dev = hal_imu_get_device(IMU_DEV_ID_0);
    if (g_imu_dev == NULL) {
        uart_printf("[IMU] hal_imu_get_device failed\r\n");
        goto exit;
    }

    /* Allocate FIFO buffer for IMU device */
    g_imu_dev->fifo_buff = s_imu_fifo_buf;

    ret = hal_imu_enable_power(g_imu_dev, true);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] enable power failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_init(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_init failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_set_sensor_default_cfg(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_sensor_default_cfg failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_set_accel_cfg(g_imu_dev, 0, 0, 50, IMU_SENSOR_ODR);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_accel_cfg(ODR) failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_set_gyro_cfg(g_imu_dev, 0, 0, 50, IMU_SENSOR_ODR);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_gyro_cfg(ODR) failed: %d\r\n", ret);
        goto exit;
    }

    IMUInterruptPinSetting pin_cfg = {
        .output_en   = 1,
        .output_mode = 0,
        .output_type = 0,
        .edge_ctrl   = 0,
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
        goto exit;
    }

    ret = hal_imu_set_fifo_wm(g_imu_dev, 49);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_fifo_wm failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_set_fifo_cfg(g_imu_dev, IMU_FIFO_GYRO | IMU_FIFO_ACCEL, true);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_fifo_cfg failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_flush_fifo(g_imu_dev);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_flush_fifo failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_set_work_mode(g_imu_dev, IMU_ACCEL_GYRO, IMU_SEN_MODE_NORMAL);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_set_work_mode failed: %d\r\n", ret);
        goto exit;
    }

    ret = osal_create_sem(&g_imu_data_sem);
    if (ret != OSAL_SUCCESS) {
        uart_printf("[IMU] osal_create_sem failed: %d\r\n", ret);
        goto exit;
    }

    ret = hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
    if (ret != VSD_SUCCESS) {
        uart_printf("[IMU] hal_imu_enable_interrupt failed: %d\r\n", ret);
        goto exit;
    }

    uart_printf("[IMU] Initialized, data-ready interrupt enabled\r\n");

    /* Create event queue for algo_task before starting tasks to avoid race */
    g_algo_event_queue = osal_create_event_queue(8, sizeof(OsalEventEntry));
    if (g_algo_event_queue == NULL) {
        uart_printf("[APP] create event queue failed\r\n");
        goto exit;
    }

    osal_create_task(imu_task, "imu_task", 1024, 4, NULL);
    osal_create_task(algo_task, "algo_task", 2048, 5, NULL);

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
