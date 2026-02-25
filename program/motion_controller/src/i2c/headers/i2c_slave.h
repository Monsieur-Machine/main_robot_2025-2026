#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <hardware/i2c.h>

#define I2C_SLAVE_SDA_PIN 16
#define I2C_SLAVE_SCL_PIN 17
#define I2C_SLAVE_INSTANCE i2c0
#define I2C_SLAVE_ADDRESS 0x09

typedef struct i2c_buffer_t {
    union {
        struct {
            uint8_t motor1_speed;
            uint8_t motor2_speed;
        } hard;

        uint8_t raw[256];
    } buffer;

    uint8_t buffer_reg;
    bool buffer_reg_written;
} i2c_buffer_t;

// Init i2c slave with default parameters
void init_i2c_slave(void);
// Deinit i2c slave
void deinit_i2c_slave(void);

uint8_t get_vitesse_moteur_1(void);
uint8_t get_vitesse_moteur_2(void);

extern int nb_messages;

#endif // I2C_SLAVE_H