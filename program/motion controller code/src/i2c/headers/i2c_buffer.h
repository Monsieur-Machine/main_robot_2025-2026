#ifndef I2C_BUFFER_H
#define I2C_BUFFER_H

#include <stdint.h>
#include "i2c_slave.h"

typedef struct i2c_buffer_t {
    uint8_t buffer[256];
    uint8_t buffer_reg;
    bool buffer_reg_written;
} i2c_buffer_t;

// I2c slave buffer handler for writing and reading data to the buffer
void __not_in_flash_func(i2c_slave_buffer_handler)(i2c_slave_event_t event);

#endif // I2C_BUFFER_H