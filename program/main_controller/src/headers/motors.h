#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

typedef enum motors_t {
    MOTOR1,
    MOTOR2,

    NB_MOTORS
} motors_t;

void motor_set_dir(motors_t motor, int16_t value);

#endif // MOTORS_H