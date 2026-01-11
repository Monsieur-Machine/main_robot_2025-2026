#include "headers/i2c_master.h"

#include <pico/stdlib.h>
#include <hardware/i2c.h>

void i2c_master_init(void)
{
    gpio_set_function(I2C_MASTER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_MASTER_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_MASTER_SDA_PIN);
    gpio_pull_up(I2C_MASTER_SCL_PIN);

    i2c_init(I2C_MASTER_INSTANCE, I2C_MASTER_BAUD_RATE);
}

void i2c_master_deinit(void)
{
    i2c_deinit(I2C_MASTER_INSTANCE);
}

inline void i2c_master_write(uint8_t address, const uint8_t *src, size_t len)
{
    i2c_write_blocking(I2C_MASTER_INSTANCE, address, src, len, false);
}

inline void i2c_master_write_reg(uint8_t address, uint8_t reg, const uint8_t *src, size_t len)
{
    i2c_write_blocking(I2C_MASTER_INSTANCE, address, &reg, 1, true);
    i2c_write_blocking(I2C_MASTER_INSTANCE, address, src, len, false);
}

inline void i2c_master_read(uint8_t address, uint8_t *dst, size_t len)
{
    i2c_read_blocking(I2C_MASTER_INSTANCE, address, dst, len, false);
}

inline void i2c_master_read_reg(uint8_t address, uint8_t reg, uint8_t *dst, size_t len)
{
    i2c_write_blocking(I2C_MASTER_INSTANCE, address, &reg, 1, true);
    i2c_master_read(address, dst, len);
}