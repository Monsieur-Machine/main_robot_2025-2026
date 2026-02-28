#ifndef SERVOS_H
#define SERVOS_H

typedef enum servos_t {
    SERVO1,
    SERVO2,
    SERVO3,

    NB_SERVOS
} servos_t;

#define I2C_BUFFER_SERVO1_ANGLE_H 3
#define I2C_BUFFER_SERVO1_ANGLE_L 4
#define I2C_BUFFER_SERVO2_ANGLE_H 5
#define I2C_BUFFER_SERVO2_ANGLE_L 6
#define I2C_BUFFER_SERVO3_ANGLE_H 7
#define I2C_BUFFER_SERVO3_ANGLE_L 8

void servos_init(void);

void servos_update(void);

#endif // SERVOS_H