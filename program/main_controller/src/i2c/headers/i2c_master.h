#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <hardware/i2c.h>

#define I2C_MASTER_SDA_PIN 16
#define I2C_MASTER_SCL_PIN 17
#define I2C_MASTER_INSTANCE i2c0
#define I2C_MASTER_BAUD_RATE 200 * 1000

// Init master i2c
void i2c_master_init(void);
// Deinit master i2c
void i2c_master_deinit(void);

#endif // I2C_MASTER_H