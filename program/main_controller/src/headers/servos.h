#ifndef SERVOS_H
#define SERVOS_H

#include <stdint.h>

typedef enum servos_t {
    SERVO1,
    SERVO2,
    SERVO3,

    NB_SERVOS
} servos_t;

typedef struct servos_data_t {
    uint16_t servos_pos[NB_SERVOS];
} servos_data_t;

void servos_init(void);

void servo_set_angle(servos_t servo, uint16_t pos);

#endif // SERVOS_H