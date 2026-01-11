#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <hardware/i2c.h>

#define I2C_MASTER_SDA_PIN 16
#define I2C_MASTER_SCL_PIN 17
#define I2C_MASTER_INSTANCE i2c0
#define I2C_MASTER_BAUD_RATE 300 * 1000

// Init master i2c
void i2c_master_init(void);
// Deinit master i2c
void i2c_master_deinit(void);
// Send [src] of [len] to [address] and close communication
void i2c_master_write(uint8_t address, const uint8_t *src, size_t len);
// Send [reg] and [src] of [len] to [address] and close communication
void i2c_master_write_reg(uint8_t address, uint8_t reg, const uint8_t *src, size_t len);
// Receive [dst] of [len] from [address] and close communication
void i2c_master_read(uint8_t address, uint8_t *dst, size_t len);
// Send [reg] and receive data in [dst] and close communition
void i2c_master_read_reg(uint8_t address, uint8_t reg, uint8_t *dst, size_t len);

#endif // I2C_MASTER_H