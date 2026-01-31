#include "headers/motors.h"

#include <stdlib.h>
#include "headers/i2c_master.h"
#include "headers/motion_control.h"

const motor_def_t MOTORS_DEFS[] = {
    {0,  4,  5, 0x00},
    {1,  6,  7, 0x01},
    {2,  8,  9, 0x02},
    {3, 10, 11, 0x03},
};

const servo_motor_def_t SERVO_MOTORS_DEFS[] = {
    {12, 0, 25000, 0x04},
    {13, 0, 25000, 0x05},
    {14, 0, 25000, 0x06},
    {15, 0, 25000, 0x07},
};

void i2c_set_motor(motors_enum_t motor, int8_t value)
{
    const motor_def_t *MOTOR_DEF = &MOTORS_DEFS[motor];

    uint8_t buf[] = {MOTOR_DEF->buffer_reg, *(uint8_t *)&value};
    i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_MOTION_CONTROLLER_ADDRESS, buf, 2, false);
}

void i2c_set_servo_motor(servo_motors_enum_t servo_motor, uint8_t value)
{
    const servo_motor_def_t *SERVO_MOTOR_DEF = &SERVO_MOTORS_DEFS[servo_motor];

    uint8_t buf[] = {SERVO_MOTOR_DEF->buffer_reg_and_payload_byte, *(uint8_t *)&value};
    i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_MOTION_CONTROLLER_ADDRESS, buf, 2, false);
}
