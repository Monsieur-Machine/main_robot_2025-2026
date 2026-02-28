#include "headers/servos.h"

#include "headers/robot.h"

void servos_init(void)
{
    for(size_t servo = SERVO1; servo < NB_SERVOS; servo++)
        robot.servos_data.servos_pos[servo] = 0;
}

inline void servo_set_angle(servos_t servo, uint16_t pos)
{
    robot.servos_data.servos_pos[servo] = pos;
}
