#ifndef MOTORS_H
#define MOTORS_H

#include <pico/stdlib.h>

// Motors
typedef enum motors_enum_t {
    MOTOR1,
    MOTOR2,

    NB_MOTORS
} motors_enum_t;

#define MOTOR1_PWM_PIN 9
#define MOTOR2_PWM_PIN 8

// Init all motors defined in the MOTORS_DEF array
void motors_init(void);
// Set [motor] in motor_enum_t at [value] between -127 and 128
void motor_set_speed(motors_enum_t motor, uint8_t value);
// Update motors from the data in the i2c buffer
void motors_update(void);

#endif // MOTORS_H
