#include "headers/motors.h"

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <headers/robot.h>

#include <stdio.h>

static inline uint get_motor_pwm_pin(motors_enum_t motor)
{
    switch(motor)
    {
      case MOTOR1:
        return MOTOR1_PWM_PIN;

      case MOTOR2:
        return MOTOR2_PWM_PIN;

      default:
        return -1;
    }
}

// Init all motors defined in the MOTORS_DEF array
void motors_init(void)
{
    for(motors_enum_t motor = MOTOR1; motor < NB_MOTORS; motor++)
    {
        // Init PWM
        const uint MOTOR_PWM_PIN = get_motor_pwm_pin(motor);

        const uint SLICE_NUM = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

        gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);
        pwm_set_wrap(SLICE_NUM, 256);
        pwm_set_enabled(SLICE_NUM, true);

        motor_set_speed(motor, 0);
    }
}

// Set [motor] in motor_enum_t at [value] between 0 and 255
void motor_set_speed(motors_enum_t motor, uint8_t value)
{
    uint MOTOR_PWM_PIN = get_motor_pwm_pin(motor);
    pwm_set_gpio_level(MOTOR_PWM_PIN, (uint16_t)value);
}

void motors_update(void)
{
    motor_set_speed(MOTOR1, robot.i2c_buffer.buffer.hard.motor1_speed);
    motor_set_speed(MOTOR2, robot.i2c_buffer.buffer.hard.motor2_speed);
}
