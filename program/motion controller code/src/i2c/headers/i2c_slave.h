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

typedef enum i2c_slave_event_t {
    I2C_SLAVE_RECEIVE,  // < Data from master is available for reading. Slave must read from Rx FIFO.
    I2C_SLAVE_REQUEST,  // < Master is requesting data. Slave must write into Tx FIFO.
    I2C_SLAVE_FINISH,   // < Master has sent a Stop or Restart signal. Slave may prepare for the next transfer.
} i2c_slave_event_t;

uint8_t i2c_slave_read_byte(void);

void i2c_slave_write_byte(uint8_t byte);

// Init I2C with default parameters
void i2c_slave_init(void);

// Deinit I2C with default parameters
void i2c_slave_deinit(void);

#endif // I2C_SLAVE_H