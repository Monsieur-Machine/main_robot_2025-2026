#include "headers/mcp23017.h"

#include <stdio.h>
#include <assert.h>
#include "headers/i2c_master.h"
#include "headers/robot.h"

void mcp23017_init(void)
{
    // IODIR
    const uint8_t IODIR_REG = 0x00;
    const uint8_t IODIR_CONFIG[2] = {0x00, 0x00};

    uint8_t buf[] = {IODIR_REG, IODIR_CONFIG[0], IODIR_CONFIG[1]};
    i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_MCP23017_ADDRESS, buf, 3, false);
}

void mcp23017_gpio_put(uint gpio_num, bool state)
{
    assert(gpio_num < 16);

    uint8_t gpio_bank = 0;

    if(gpio_num > 7)
    {
        gpio_bank = 1;
        gpio_num -= 8;
    }

    robot.mcp23017_data.gpio_state[gpio_bank] = (robot.mcp23017_data.gpio_state[gpio_bank] & ~(0x1 << gpio_num)) | (state << gpio_num);
}

inline void mcp23017_update(void)
{
    const uint GPIO_REG = 0x12;

    uint8_t buf[] = {GPIO_REG, robot.mcp23017_data.gpio_state[0], robot.mcp23017_data.gpio_state[1]};
    i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_MCP23017_ADDRESS, buf, 3, false);
}
