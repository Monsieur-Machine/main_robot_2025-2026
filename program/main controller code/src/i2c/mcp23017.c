#include "headers/mcp23017.h"

#include <stdio.h>
#include <assert.h>
#include "headers/i2c_master.h"
#include "headers/robot.h"

int init_mcp23017(void)
{
    // IOCON
    const uint8_t IOCON_REG = 0x0A;
    const uint8_t IOCON_CONFIG = 0x00;

    i2c_master_write_reg(I2C_MCP23017_ADDRESS, IOCON_REG, &IOCON_CONFIG, 1);

    uint8_t iocon_config_verif;
    i2c_master_read_reg(I2C_MCP23017_ADDRESS, IOCON_REG, &(iocon_config_verif), 1);
    printf("mcp23017 config: 0x%x\n", iocon_config_verif);

    if(IOCON_CONFIG != iocon_config_verif)
        return -1;

    const uint8_t IODIR_REG = 0x00;
    const uint8_t IODIR_CONFIG[GPIO_NB_BANKS] = {0x00, 0x00};

    i2c_master_write_reg(I2C_MCP23017_ADDRESS, IODIR_REG, IODIR_CONFIG, GPIO_NB_BANKS);

    uint8_t iodir_config_verif[GPIO_NB_BANKS];
    i2c_master_read_reg(I2C_MCP23017_ADDRESS, IODIR_REG, iodir_config_verif, GPIO_NB_BANKS);
    printf("mcp23017 io dir config: 0x%x, 0x%x\n", iodir_config_verif[0], iodir_config_verif[1]);

    if(IODIR_CONFIG[0] != iodir_config_verif[0] || IODIR_CONFIG[1] != iodir_config_verif[1])
        return -1;

    return 0;
}

void mcp23017_gpio_put(uint gpio_num, bool state)
{
    assert(gpio_num < 16);

    uint8_t gpio_bank = GPIO_BANK0;

    if(gpio_num > 7)
    {
        gpio_bank++;
        gpio_num -= 0x0008;
    }

    robot.mcp23017_data.gpio_state[gpio_bank] = (robot.mcp23017_data.gpio_state[gpio_bank] & ~(0x1 << gpio_num)) | (state << gpio_num);
}

void mcp23017_update(void)
{
    i2c_master_write_reg(I2C_MCP23017_ADDRESS, GPIO_REG, robot.mcp23017_data.gpio_state, GPIO_NB_BANKS);

    uint8_t gpio_state[GPIO_NB_BANKS];
    i2c_master_read_reg(I2C_MCP23017_ADDRESS, GPIO_REG, gpio_state, GPIO_NB_BANKS);
    printf("gpio states: 0x%x, 0x%x\n", gpio_state[0], gpio_state[1]);
}
