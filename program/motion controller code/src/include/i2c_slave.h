/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <hardware/i2c.h>

#define I2C_SLAVE_SDA_PIN 21
#define I2C_SLAVE_SCL_PIN 20
#define I2C_SLAVE_INSTANCE i2c0
#define I2C_SLAVE_ADDRESS 0x09

typedef enum i2c_slave_event_t {
    I2C_SLAVE_RECEIVE,  // < Data from master is available for reading. Slave must read from Rx FIFO.
    I2C_SLAVE_REQUEST,  // < Master is requesting data. Slave must write into Tx FIFO.
    I2C_SLAVE_FINISH,   // < Master has sent a Stop or Restart signal. Slave may prepare for the next transfer.
} i2c_slave_event_t;

static inline uint8_t i2c_slave_read_byte(void)
{
    i2c_hw_t *hw = i2c_get_hw(I2C_SLAVE_INSTANCE);
    assert(hw->status & I2C_IC_STATUS_RFNE_BITS); // Rx FIFO must not be empty
    return (uint8_t)hw->data_cmd;
}

static inline void i2c_slave_write_byte(uint8_t value)
{
    i2c_hw_t *hw = i2c_get_hw(I2C_SLAVE_INSTANCE);
    assert(hw->status & I2C_IC_STATUS_TFNF_BITS); // Tx FIFO must not be full
    hw->data_cmd = value;
}

// Init I2C with default parameters
void i2c_slave_init(void);

// Deinit I2C with default parameters
void i2c_slave_deinit(void);

#endif // I2C_SLAVE_H