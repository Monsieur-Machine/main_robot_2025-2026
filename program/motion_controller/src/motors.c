#include "headers/motors.h"

#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include "headers/robot.h"

#define MOTOR1_PWM_PIN 9
#define MOTOR2_PWM_PIN 8

static inline uint get_motor_pwm_pin(motors_t motor)
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

static inline void motor_set_speed(motors_t motor, uint8_t value)
{
    uint MOTOR_PWM_PIN = get_motor_pwm_pin(motor);
    pwm_set_gpio_level(MOTOR_PWM_PIN, (uint16_t)value);
}

// Init all motors defined in the MOTORS_DEF array
void motors_init(void)
{
    for(motors_t motor = MOTOR1; motor < NB_MOTORS; motor++)
    {
        // Init PWM
        const uint MOTOR_PWM_PIN = get_motor_pwm_pin(motor);

        const uint SLICE_NUM = pwm_gpio_to_slice_num(MOTOR_PWM_PIN);

        gpio_set_function(MOTOR_PWM_PIN, GPIO_FUNC_PWM);

        pwm_set_clkdiv(SLICE_NUM, 255);
        pwm_set_wrap(SLICE_NUM, 256);

        pwm_set_enabled(SLICE_NUM, true);

        motor_set_speed(motor, 0);
    }
}

void motors_update(void)
{
    motor_set_speed(MOTOR1, robot.i2c_buffer.buffer[I2C_BUFFER_MOTOR1_SPEED]);
    motor_set_speed(MOTOR2, robot.i2c_buffer.buffer[I2C_BUFFER_MOTOR2_SPEED]);
}
