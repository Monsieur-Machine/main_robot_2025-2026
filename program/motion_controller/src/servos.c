#include "headers/servos.h"

#include <hardware/pwm.h>
#include <pico/stdlib.h>
#include "headers/robot.h"

#define SERVO1_PWM_PIN 7
#define SERVO2_PWM_PIN 6
#define SERVO3_PWM_PIN 0

static inline uint get_servo_pwm_pin(servos_t servo)
{
    switch(servo)
    {
      case SERVO1:
        return SERVO1_PWM_PIN;
      case SERVO2:
        return SERVO2_PWM_PIN;
      case SERVO3:
        return SERVO3_PWM_PIN;

      default:
        return -1;
    }
}

static inline void servo_set_speed(servos_t servo, uint16_t pos)
{
    const uint SERVO_PWM_PIN = get_servo_pwm_pin(servo);
    pwm_set_gpio_level(SERVO_PWM_PIN, pos);
}

void servos_init(void)
{
    for(servos_t servo = SERVO1; servo < NB_SERVOS; servo++)
    {
        // Init PWM
        const uint SERVO_PWM_PIN = get_servo_pwm_pin(servo);

        const uint SLICE_NUM = pwm_gpio_to_slice_num(SERVO_PWM_PIN);

        gpio_set_function(SERVO_PWM_PIN, GPIO_FUNC_PWM);

        pwm_set_clkdiv(SLICE_NUM, 100);
        pwm_set_wrap(SLICE_NUM, 25000);

        pwm_set_enabled(SLICE_NUM, true);

        servo_set_speed(servo, 0);
    }
}

void servos_update(void)
{
    servo_set_speed(SERVO1, robot.i2c_buffer.buffer[I2C_BUFFER_SERVO1_ANGLE_H] << 8 | robot.i2c_buffer.buffer[I2C_BUFFER_SERVO1_ANGLE_L]);
    servo_set_speed(SERVO2, robot.i2c_buffer.buffer[I2C_BUFFER_SERVO2_ANGLE_H] << 8 | robot.i2c_buffer.buffer[I2C_BUFFER_SERVO2_ANGLE_L]);
    servo_set_speed(SERVO3, robot.i2c_buffer.buffer[I2C_BUFFER_SERVO3_ANGLE_H] << 8 | robot.i2c_buffer.buffer[I2C_BUFFER_SERVO3_ANGLE_L]);
}
