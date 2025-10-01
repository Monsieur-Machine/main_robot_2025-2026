#ifndef MOTORS_H
#define MOTORS_H

#include <pico/stdlib.h>

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
    uint8_t buffer_reg;
} servo_motor_def_t;

extern const servo_motor_def_t SERVO_MOTORS_DEFS[];

// Init all motors defined in the MOTORS_DEF array
void init_motors(void);
// Init all servo motors defined in the SERVO_MOTORS_DEF array
void init_servo_motors(void);
// Set [motor] to 0
void motor_zero(motors_enum_t motor);
// Set [motor] in motor_enum_t at [value] between -127 and 128
void motor_set(motors_enum_t motor, int8_t value);
// Set servo motor to its open pos
void servo_motor_zero(servo_motors_enum_t servo_motor);
// Set servo to its close pos if [close] else open pos
void servo_motor_set(servo_motors_enum_t servo_motor, bool close);

#endif // MOTORS_H
