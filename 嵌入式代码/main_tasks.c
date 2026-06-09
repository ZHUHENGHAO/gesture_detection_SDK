/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-06-09 21:54:59
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-06-09 21:55:07
 * @FilePath: \微调pinch\main_tasks.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "gesture_detection.h"
#include <stdio.h>

// 模拟FreeRTOS及SDK接口的原生组件声明
extern void vTaskDelay(uint32_t ticks);
extern int SendEventToAlgo(void);
extern uint32_t GetCurrentSystemProcessedBytes(void);

static ImuData_t ram_ring_buffer[WINDOW_SIZE];
static uint32_t global_sample_count = 0;
static uint32_t crc_check_byte_counter = 0;
static uint8_t crc_running_value = 0x00;

// 标准 CRC-8/SMBus 算法实现 (Polynomial: 0x07)
uint8_t calculate_crc8_smbus(uint8_t crc, const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x07;
            else crc <<= 1;
        }
    }
    return crc;
}

// 💡 规范约束：硬件IMU数据就绪中断处理函数 (ISR)
void IMU_DataReady_IRQHandler(void) {
    // 严禁执行 HAL接口调用、OS阻塞、CRC运算及printf
    // 仅通过极其短小的事件通知发送数据就绪信号
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // SDK 推荐的信号量或轻量Event触发机制
    // vpi_send_event_from_isr(xImuEvent, &xHigherPriorityTaskWoken);
}

// 任务1：数据采集处理任务 (低功耗，高频调度)
void imu_task(void* pvParameters) {
    // 调用SDK HAL接口配置为 50Hz
    // IMU_HAL_Init(SAMPLE_RATE_50HZ);
    
    while(1) {
        // 等等来自中断服务程序的信号阻塞
        // xSemaphoreTake(xImuSemaphore, portMAX_DELAY);

        ImuData_t fresh_sample;
        // IMU_HAL_ReadRaw(&fresh_sample);

        // 数据暂时存储到静态RAM环形缓冲区
        for(int i=0; i<WINDOW_SIZE-1; i++) {
            ram_ring_buffer[i] = ram_ring_buffer[i+1];
        }
        ram_ring_buffer[WINDOW_SIZE-1] = fresh_sample;
        global_sample_count++;

        // 💡 赛题硬性校验：前320000Byte数据的流式CRC检查
        if (crc_check_byte_counter < 320000) {
            size_t struct_size = sizeof(ImuData_t); // 包含结构体中所有成员
            crc_running_value = calculate_crc8_smbus(crc_running_value, (const uint8_t*)&fresh_sample, struct_size);
            crc_check_byte_counter += struct_size;

            if (crc_check_byte_counter >= 320000) {
                // 首次达到数据量，打印校验和
                printf("CRC-8/SMBus Checksum for first 320000 Bytes: 0x%02X\n", crc_running_value);
            }
        }

        // 当积攒满一秒(50帧)且步长达到15帧时，触发事件送往算法任务
        if (global_sample_count >= WINDOW_SIZE && (global_sample_count % 15 == 0)) {
            // 使用 SDK的 VPI Event System 发送Event到 algo_task
            // vpi_trigger_event(xAlgoProcessEvent);
        }
    }
}

// 任务2：算法推理与执行管理任务 (高算力调度)
void algo_task(void* pvParameters) {
    gesture_algorithm_init();
    const char* gesture_names[4] = {"clench", "pinch", "up", "down"};
    
    while(1) {
        // 阻塞等待来自 imu_task 的 Event 触发信号
        // vpi_wait_event(xAlgoProcessEvent, portMAX_DELAY);
        
        float out_probs[5] = {0};
        // 算法内部不执行直接打印，通过返回值或回调传递结果
        int result = gesture_algorithm_process(ram_ring_buffer, out_probs);
        
        if (result >= 0 && result < 4) {
            // 计算累计处理的时间戳 (毫秒显示)
            // 毫秒 = 字节数 / 位宽 / 通道数 / 采样率 * 1000
            uint32_t current_data_bytes = global_sample_count * sizeof(ImuData_t);
            uint32_t time_ms = (current_data_bytes / 2 / 7 / SAMPLE_RATE) * 1000;
            
            // 统一在非算法模块处理上层打印输出
            printf("%dms, %s\n", time_ms, gesture_names[result]);
        }
    }
}