/*
 * Copyright (c) 2026, VeriSilicon Holdings Co., Ltd. All rights reserved
 *
 * 1. Redistributing the source code of this software is only allowed after
 * receiving explicit, written permission from VeriSilicon. The copyright notice,
 * this list of conditions and the the following disclaimer must be retained in all
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

#include <string.h>
#include "app.h"
#include "hal_imu.h"
#include "gesture_algorithm.h"
#include "main.h"
#include "osal.h"
#include "uart_printf.h"
#include "vsd_error.h"

#define WINDOW_SIZE  50
#define SAMPLE_RATE  50
#define CRC_TARGET_BYTES  320000U

extern OsalSemaphore *g_imu_data_sem;
extern ImuDevice *g_imu_dev;

void *g_algo_event_queue = NULL;

static ImuData_t g_ring_buffer[WINDOW_SIZE];
static uint32_t g_sample_count = 0;
static uint32_t g_crc_byte_counter = 0;
static uint8_t g_crc_value = 0x00;

static uint8_t crc8_smbus(uint8_t crc, const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

static void gesture_result_callback(int gesture_result) {
    static const char *names[4] = { "clench", "pinch", "up", "down" };
    if (gesture_result < 0 || gesture_result >= 4) return;

    uint32_t bytes = g_sample_count * sizeof(ImuData_t);
    uint32_t time_ms = bytes * 1000U / 700U;

    uart_printf("%ums, %s\r\n", (unsigned int)time_ms, names[gesture_result]);
}

void imu_task(void *param) {
    (void)param;

    ImuGyroAccelData gyro_accel_data[64];
    uint16_t frame_count = 0;

    while (1) {
        int sem_ret = osal_sem_wait(g_imu_data_sem, OSAL_WAIT_FOREVER);
        if (sem_ret != OSAL_SUCCESS) {
            continue;
        }

        int ret = hal_imu_read_gyro_accel(g_imu_dev, gyro_accel_data,
                                          sizeof(gyro_accel_data) / sizeof(gyro_accel_data[0]),
                                          &frame_count);
        if (ret != VSD_SUCCESS || frame_count == 0) {
            uart_printf("[IMU] read gyro accel failed: ret=%d, frame_count=%u\r\n",
                        ret, (unsigned int)frame_count);
            hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
            continue;
        }

        for (uint16_t i = 0; i < frame_count; i++) {
            ImuData_t sample;
            sample.gyro_x = gyro_accel_data[i].gx;
            sample.gyro_y = gyro_accel_data[i].gy;
            sample.gyro_z = gyro_accel_data[i].gz;
            sample.acc_x  = gyro_accel_data[i].ax;
            sample.acc_y  = gyro_accel_data[i].ay;
            sample.acc_z  = gyro_accel_data[i].az;
            sample.debug_data = 0;

            for (int j = 0; j < WINDOW_SIZE - 1; j++) {
                g_ring_buffer[j] = g_ring_buffer[j + 1];
            }
            g_ring_buffer[WINDOW_SIZE - 1] = sample;
            g_sample_count++;

            if (g_crc_byte_counter < CRC_TARGET_BYTES) {
                g_crc_value = crc8_smbus(g_crc_value, (const uint8_t *)&sample, sizeof(ImuData_t));
                g_crc_byte_counter += sizeof(ImuData_t);

                if (g_crc_byte_counter >= CRC_TARGET_BYTES) {
                    uart_printf("CRC-8/SMBus Checksum for first 320000 Bytes: 0x%02X\r\n", g_crc_value);
                }
            }
        }

        if (g_sample_count >= WINDOW_SIZE && (g_sample_count % 15U) == 0U) {
            osal_send_event(g_algo_event_queue, EVENT_ALGO_PROCESS, NULL, 0);
        }

        hal_imu_enable_interrupt(g_imu_dev, IMU_DATA_PIN, true, imu_data_ready_isr);
    }
}

void algo_task(void *param) {
    (void)param;

    if (g_algo_event_queue == NULL) {
        uart_printf("[ALGO] event queue not created\r\n");
        osal_delete_task(NULL);
        return;
    }

    gesture_algorithm_init();
    gesture_register_callback(gesture_result_callback);

    while (1) {
        void *event_data = NULL;
        int event_id = osal_wait_event(g_algo_event_queue, &event_data, OSAL_WAIT_FOREVER);

        if (event_id == EVENT_ALGO_PROCESS) {
            float out_probs[5] = {0};
            gesture_algorithm_process(g_ring_buffer, out_probs);
        }
    }
}
