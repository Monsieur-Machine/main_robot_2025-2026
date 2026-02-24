#include "headers/motors.h"

#include <pico/types.h>
#include "i2c/headers/mcp23017.h"

#define MOTOR1_DIR1_MCP23017_GPIO 12
#define MOTOR1_DIR2_MCP23017_GPIO 13
#define MOTOR2_DIR1_MCP23017_GPIO 14
#define MOTOR2_DIR2_MCP23017_GPIO 15

void motor_set_dir(motors_t motor, int16_t value)
{
    uint gpio_dir1_pin;
    uint gpio_dir2_pin;

    switch(motor)
    {
      case MOTOR1:
        gpio_dir1_pin = MOTOR1_DIR1_MCP23017_GPIO;
        gpio_dir2_pin = MOTOR1_DIR2_MCP23017_GPIO;
        break;

      case MOTOR2:
        gpio_dir1_pin = MOTOR2_DIR1_MCP23017_GPIO;
        gpio_dir2_pin = MOTOR2_DIR2_MCP23017_GPIO;
        break;

    default:
        break;
    }

    if(value > 0)
    {
        mcp23017_gpio_put(gpio_dir1_pin, false);
        mcp23017_gpio_put(gpio_dir2_pin, true);
    }
    else if(value < 0)
    {
        mcp23017_gpio_put(gpio_dir1_pin, true);
        mcp23017_gpio_put(gpio_dir2_pin, false);
    }
    else
    {
        mcp23017_gpio_put(gpio_dir1_pin, false);
        mcp23017_gpio_put(gpio_dir2_pin, false);
    }
}
