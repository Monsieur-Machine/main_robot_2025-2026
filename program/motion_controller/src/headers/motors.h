#ifndef MOTORS_H
#define MOTORS_H

#include <pico/stdlib.h>

// Motors
typedef enum motors_t {
    MOTOR1,
    MOTOR2,

    NB_MOTORS
} motors_t;

#define I2C_BUFFER_MOTOR1_SPEED 1
#define I2C_BUFFER_MOTOR2_SPEED 2

// Init all motors defined in the MOTORS_DEF array
void motors_init(void);
// Update motors from the data in the i2c buffer
void motors_update(void);

#endif // MOTORS_H
