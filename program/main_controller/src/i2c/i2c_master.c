#include "headers/i2c_master.h"

#include <pico/stdlib.h>
#include <hardware/i2c.h>

void i2c_master_init(void)
{
    gpio_set_function(I2C_MASTER_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_MASTER_SCL_PIN, GPIO_FUNC_I2C);

    i2c_init(I2C_MASTER_INSTANCE, I2C_MASTER_BAUD_RATE);
}

void i2c_master_deinit(void)
{
    i2c_deinit(I2C_MASTER_INSTANCE);
}
