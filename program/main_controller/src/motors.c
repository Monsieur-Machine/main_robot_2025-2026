#include "headers/motors.h"

#include <pico/types.h>
#include "i2c/headers/mcp23017.h"

#define MCP23017_MOTOR1_DIR1_GPIO 12
#define MCP23017_MOTOR1_DIR2_GPIO 13
#define MCP23017_MOTOR2_DIR1_GPIO 14
#define MCP23017_MOTOR2_DIR2_GPIO 15

void motor_set_dir(motors_t motor, int16_t value)
{
    uint gpio_dir1_pin;
    uint gpio_dir2_pin;

    switch(motor)
    {
      case MOTOR1:
        gpio_dir1_pin = MCP23017_MOTOR1_DIR1_GPIO;
        gpio_dir2_pin = MCP23017_MOTOR1_DIR2_GPIO;
        break;

      case MOTOR2:
        gpio_dir1_pin = MCP23017_MOTOR2_DIR1_GPIO;
        gpio_dir2_pin = MCP23017_MOTOR2_DIR2_GPIO;
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
