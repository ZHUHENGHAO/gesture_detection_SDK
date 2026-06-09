#ifndef GESTURE_DETECTION_H
#define GESTURE_DETECTION_H

#include <stdint.h>

#define SAMPLE_RATE   50
#define WINDOW_SIZE   50
#define RAW_CHANNELS  6
#define CHANNELS      9

// 手势类别枚举
typedef enum {
    GESTURE_CLENCH = 0,
    GESTURE_PINCH,
    GESTURE_UP,
    GESTURE_DOWN,
    GESTURE_OTHERS
} GestureResult_t;

// 原始IMU数据结构体体 (严格匹配320000字节校核需求)
typedef struct {
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t debug_data; // 赛事要求的第7个预留通道
} ImuData_t;

// 固化的训练集标准化参数 (基于数据盲测的最佳参数直接映射)
extern const float SCALER_MEANS[9];
extern const float SCALER_SCALES[9];
extern const float AUTO_PINCH_ENERGY_THRESH;

// 算法核心接口声明
void gesture_algorithm_init(void);
int gesture_algorithm_process(const ImuData_t* window_buffer, float* out_probs);
int simulate_embedded_state_machine(int pred_label, float pred_prob, float window_energy);

#endif // GESTURE_DETECTION_H