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

#ifndef GESTURE_ALGORITHM_H
#define GESTURE_ALGORITHM_H

#include <stdint.h>

#define SAMPLE_RATE   50
#define WINDOW_SIZE   50
#define RAW_CHANNELS  6
#define CHANNELS      9

typedef enum {
    GESTURE_CLENCH = 0,
    GESTURE_PINCH,
    GESTURE_UP,
    GESTURE_DOWN,
    GESTURE_OTHERS
} GestureResult_t;

typedef struct {
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t debug_data;
} ImuData_t;

extern const float SCALER_MEANS[9];
extern const float SCALER_SCALES[9];
extern const float AUTO_PINCH_ENERGY_THRESH;

typedef void (*GestureResultCallback)(int gesture_result);

void gesture_algorithm_init(void);
void gesture_register_callback(GestureResultCallback cb);
int gesture_algorithm_process(const ImuData_t* window_buffer, float* out_probs);

#endif /* GESTURE_ALGORITHM_H */
