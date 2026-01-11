#ifndef MPU23017_H
#define MPU23017_H

#include <stdbool.h>
#include <stdint.h>

#define I2C_MCP23017_ADDRESS 0x20

#define GPIO_REG 0x12

typedef enum gpio_bank_t {
    GPIO_BANK0,
    GPIO_BANK1,
    GPIO_NB_BANKS
} gpio_bank_t;


typedef struct mcp23017_data_t {
    uint8_t gpio_state[GPIO_NB_BANKS];
} mcp23017_data_t;

// Init mpu23017 device with default settings over I2C
int init_mcp23017(void);
// Set GPIO on mpc23017
void mcp23017_gpio_put(unsigned int gpio_num, bool state);
// Uppdate mcp23017 GPIOs
void mcp23017_update(void);

#endif // MPU23017_H