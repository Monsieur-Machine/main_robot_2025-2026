/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <hardware/i2c.h>

#define I2C_SLAVE_SDA_PIN 16
#define I2C_SLAVE_SCL_PIN 17
#define I2C_SLAVE_INSTANCE i2c0
#define I2C_SLAVE_ADDRESS 0x09

typedef struct i2c_buffer_t {
    uint8_t buffer[256];
    uint8_t buffer_reg;
    bool buffer_reg_written;
} i2c_buffer_t;

// Init i2c slave with default parameters
void init_i2c_slave(void);
// Deinit i2c slave
void deinit_i2c_slave(void);

#endif // I2C_SLAVE_H