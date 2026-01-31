#ifndef MOTION_CONTROL_H
#define MOTION_CONTROL_H

#include <stdint.h>
#include "i2c/headers/motors.h"

#define I2C_MOTION_CONTROLLER_ADDRESS 0x09

typedef struct motion_control_data_t {
    int16_t angle;
    int8_t x_axis_speed;
    int8_t y_axis_speed;
} motion_control_data_t;

// Init values for motion control
void init_motion_control(void);
// Update motion control buffer from motion control data and gyro data
void i2c_update_motion_control(void);
// Update servo motors from motion control data
void i2c_update_servo_motors(void);

#endif // MOTION_CONTROL_H