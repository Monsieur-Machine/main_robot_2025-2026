/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "headers/i2c_slave.h"

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <headers/robot.h>

void i2c_slave_buffer_handler(i2c_inst_t *i2c, i2c_slave_event_t event)
{
    switch(event)
    {
      case I2C_SLAVE_RECEIVE: // master has written some data
        if(!robot.i2c_buffer.buffer_reg_written)
        {
            // writes always start with the memory address
            robot.i2c_buffer.buffer_reg = i2c_read_byte_raw(I2C_SLAVE_INSTANCE);
            robot.i2c_buffer.buffer_reg_written = true;
        }
        else
        {
            // save into memory
            robot.i2c_buffer.buffer[robot.i2c_buffer.buffer_reg] = i2c_read_byte_raw(I2C_SLAVE_INSTANCE);
            robot.i2c_buffer.buffer_reg++;
        }
        break;

      case I2C_SLAVE_REQUEST: // master is requesting data
        // load from memory
        i2c_write_byte_raw(I2C_SLAVE_INSTANCE, robot.i2c_buffer.buffer[robot.i2c_buffer.buffer_reg]);
        robot.i2c_buffer.buffer_reg++;
        break;

      case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        robot.i2c_buffer.buffer_reg_written = false;
        break;

      default:
        break;
    }
}

void init_i2c_slave(void)
{
    // Init GPIO pins
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);

    i2c_init(I2C_SLAVE_INSTANCE, 0);
    // New SDK method to init i2c slave
    i2c_slave_init(I2C_SLAVE_INSTANCE, I2C_SLAVE_ADDRESS, &i2c_slave_buffer_handler);
}

void deinit_i2c_slave(void)
{
    // Reset GPIO pins
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_NULL);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_NULL);

    // New SDK method to reset i2c slave
    i2c_slave_deinit(I2C_SLAVE_INSTANCE);
}