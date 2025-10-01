#include "include/motors.h"

#include <stdlib.h>
#include "include/i2c_master.h"

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

    uint8_t x = *(uint8_t *)&value;
    uint8_t data[] = {MOTOR_DEF->buffer_reg, x};

    i2c_master_write(I2C_MOTION_CONTROLLER_ADDRESS, data, 2);
}

int8_t i2c_get_motor(motors_enum_t motor)
{
    const motor_def_t *MOTOR_DEF = &MOTORS_DEFS[motor];

    uint8_t data;
    i2c_master_read_reg(I2C_MOTION_CONTROLLER_ADDRESS, MOTOR_DEF->buffer_reg, &data, 1);

    int8_t value = *(int8_t *)&data;
    return value;
}

void i2c_set_servo_motor(servo_motors_enum_t servo_motor, uint8_t value)
{
    const servo_motor_def_t *SERVO_MOTOR_DEF = &SERVO_MOTORS_DEFS[servo_motor];

    uint8_t data[] = {SERVO_MOTOR_DEF->buffer_reg_and_payload_byte, value};

    i2c_master_write(I2C_MOTION_CONTROLLER_ADDRESS, data, 2);
}

uint8_t i2c_get_servo_motor(servo_motors_enum_t servo_motor)
{
    const servo_motor_def_t *servo_motor_def = &SERVO_MOTORS_DEFS[servo_motor];

    uint8_t value;
    i2c_master_read_reg(I2C_MOTION_CONTROLLER_ADDRESS, servo_motor_def->buffer_reg_and_payload_byte, &value, 1);

    return value;
}
