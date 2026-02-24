#ifndef MOTION_CONTROL_H
#define MOTION_CONTROL_H

#include <stdint.h>

#define I2C_MOTION_CONTROLLER_ADDRESS 0x09

typedef struct motion_control_data_t {
    int16_t target_speed;
    float target_angle;
} motion_control_data_t;

void motion_control_init(void);

void motion_control_update(void);

#endif // MOTION_CONTROL_H