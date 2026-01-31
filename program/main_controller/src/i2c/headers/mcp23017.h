#ifndef MPU23017_H
#define MPU23017_H

#include <stdbool.h>
#include <stdint.h>

#define I2C_MCP23017_ADDRESS 0x20

typedef struct mcp23017_data_t {
    uint8_t gpio_state[2];
} mcp23017_data_t;

// Init mpu23017 device with default settings over I2C
void mcp23017_init(void);
// Set GPIO buffer
void mcp23017_gpio_put(unsigned int gpio_num, bool state);
// Set GPIO on mcp23017
void mcp23017_gpio_put_imediate(unsigned int gpio_num, bool state);
// Uppdate mcp23017 GPIOs
void mcp23017_update(void);

#endif // MPU23017_H