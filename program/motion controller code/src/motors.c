#include "include/motors.h"

#include <pico/stdlib.h>
#include <hardware/pwm.h>

const motor_def_t MOTORS_DEFS[] = {
    {0,  4,  5, 0x00},
    {1,  6,  7, 0x01},
    {2,  9,  8, 0x02},
    {3, 11, 10, 0x03},
};

const servo_motor_def_t SERVO_MOTORS_DEFS[] = {
    {12, 0, 25000, 0x04},
    {13, 0, 25000, 0x05},
    {14, 0, 25000, 0x06},
    {15, 0, 25000, 0x07},
};

// Init all motors defined in the MOTORS_DEF array
void init_motors(void)
{
    for(motors_enum_t actual_motor = MOTOR1; actual_motor < NB_MOTORS; actual_motor++)
    {
        const motor_def_t *motor_def = &MOTORS_DEFS[actual_motor];

        // Init PWM
        const uint SLICE_NUM = pwm_gpio_to_slice_num(motor_def->pwm_pin);

        gpio_set_function(motor_def->pwm_pin, GPIO_FUNC_PWM);
        pwm_set_wrap(SLICE_NUM, 128);
        pwm_set_enabled(SLICE_NUM, true);

        // Init dir pins
        gpio_init(motor_def->dir1_pin);
        gpio_set_dir(motor_def->dir1_pin, GPIO_OUT);

        gpio_init(motor_def->dir2_pin);
        gpio_set_dir(motor_def->dir2_pin, GPIO_OUT);

        motor_zero(actual_motor);
    }
}

// Init all servo motors defined in the SERVO_MOTORS_DEF array
void init_servo_motors(void)
{
    for(servo_motors_enum_t actual_servo_motor = SERVO_MOTOR1; actual_servo_motor < NB_SERVO_MOTORS; actual_servo_motor++)
    {
        const servo_motor_def_t *servo_motor_def = &SERVO_MOTORS_DEFS[actual_servo_motor];

        //  Init PWM   //
        const uint SLICE_NUM = pwm_gpio_to_slice_num(servo_motor_def->pwm_pin);

        gpio_set_function(servo_motor_def->pwm_pin, GPIO_FUNC_PWM);
        pwm_set_wrap(SLICE_NUM, 25000);
        pwm_set_clkdiv(SLICE_NUM, 100);
        pwm_set_enabled(SLICE_NUM, true);

        servo_motor_zero(actual_servo_motor);
    }
}

// Set [motor] to 0
void motor_zero(motors_enum_t motor)
{
    const motor_def_t *motor_def = &MOTORS_DEFS[motor];

    // Set PWM to zero
    pwm_set_gpio_level(motor_def->pwm_pin, 0);

    // Set dir pins to false
    gpio_put(motor_def->dir1_pin, false);
    gpio_put(motor_def->dir2_pin, false);
}

// Set [motor] in motor_enum_t at [value] between -128 and 127 (for this config)
void motor_set(motors_enum_t motor, int8_t value)
{
    const motor_def_t *motor_def = &MOTORS_DEFS[motor];

    if(value < 0)
    {
        gpio_put(motor_def->dir1_pin, true);
        gpio_put(motor_def->dir2_pin, false);

        value = -value;
    }
    else if(value > 0)
    {
        gpio_put(motor_def->dir1_pin, false);
        gpio_put(motor_def->dir2_pin, true);
    }
    else
    {
        gpio_put(motor_def->dir1_pin, false);
        gpio_put(motor_def->dir2_pin, false);
    }

    pwm_set_gpio_level(motor_def->pwm_pin, (uint16_t)value);
}

// Set servo motor to its open pos
void servo_motor_zero(servo_motors_enum_t servo_motor)
{
    const servo_motor_def_t *servo_motor_def = &SERVO_MOTORS_DEFS[servo_motor];

    // Set PWM to zero //
    pwm_set_gpio_level(servo_motor_def->pwm_pin, servo_motor_def->open_pos);
}

// Set servo to its close pos if [close] else open pos
void servo_motor_set(servo_motors_enum_t servo_motor, bool close)
{
    const servo_motor_def_t *servo_motor_def = &SERVO_MOTORS_DEFS[servo_motor];

    // Set PWM to zero //
    pwm_set_gpio_level(servo_motor_def->pwm_pin, close ? servo_motor_def->close_pos : servo_motor_def->open_pos);
}
