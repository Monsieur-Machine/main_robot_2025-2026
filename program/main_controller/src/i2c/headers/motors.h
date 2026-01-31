#ifndef MOTORS_H
#define MOTORS_H

#include <pico/types.h>

// Motors
typedef enum motors_enum_t {
    MOTOR1,
    MOTOR2,
    MOTOR3,
    MOTOR4,

    NB_MOTORS
} motors_enum_t;

typedef struct motor_def_t {
    uint pwm_pin;
    uint dir1_pin;
    uint dir2_pin;
    uint8_t buffer_reg;
} motor_def_t;

extern const motor_def_t MOTORS_DEFS[];

// Servo Motors
typedef enum {
    SERVO_MOTOR1,
    SERVO_MOTOR2,
    SERVO_MOTOR3,
    SERVO_MOTOR4,

    NB_SERVO_MOTORS
} servo_motors_enum_t;

typedef struct {
    uint pwm_pin;
    uint open_pos;
    uint close_pos;
    uint8_t buffer_reg_and_payload_byte;
} servo_motor_def_t;

extern const servo_motor_def_t SERVO_MOTORS_DEFS[];

// Send [motor] to [value] through i2c to motion controller
void i2c_set_motor(motors_enum_t motor, int8_t value);
// Set [servo motor] to [value] through i2c
void i2c_set_servo_motor(servo_motors_enum_t servo_motor, uint8_t value);

#endif // MOTORS_H
