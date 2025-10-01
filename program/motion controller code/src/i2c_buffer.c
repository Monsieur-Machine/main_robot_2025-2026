#include "include/i2c_buffer.h"

#include "include/robot.h"
#include "include/motors.h"

void __not_in_flash_func(i2c_slave_buffer_handler)(i2c_slave_event_t event)
{
    switch(event)
    {
      case I2C_SLAVE_RECEIVE: // master has written some data
        if(!robot.i2c_buffer.buffer_reg_written)
        {
            // writes always start with the memory address
            robot.i2c_buffer.buffer_reg = i2c_slave_read_byte();
            robot.i2c_buffer.buffer_reg_written = true;
        }
        else
        {
            // save into memory
            robot.i2c_buffer.buffer[robot.i2c_buffer.buffer_reg] = i2c_slave_read_byte();
            robot.i2c_buffer.buffer_reg++;
        }
        break;

      case I2C_SLAVE_REQUEST: // master is requesting data
        // load from memory
        i2c_slave_write_byte(robot.i2c_buffer.buffer[robot.i2c_buffer.buffer_reg]);
        robot.i2c_buffer.buffer_reg++;
        break;

      case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        robot.i2c_buffer.buffer_reg_written = false;
        break;

      default:
        break;
    }
}

void update_motors_from_buffer(void)
{
    for(motors_enum_t actual_motor = MOTOR1; actual_motor < NB_MOTORS; actual_motor++)
    {
        const motor_def_t *motor_def = &MOTORS_DEFS[actual_motor];

        motor_set(actual_motor, robot.i2c_buffer.buffer[motor_def->buffer_reg]);
    }
}

void update_servo_motors_from_buffer(void)
{
    for(servo_motors_enum_t actual_servo_motor = SERVO_MOTOR1; actual_servo_motor < NB_SERVO_MOTORS; actual_servo_motor++)
    {
        const servo_motor_def_t *servo_motor_def = &SERVO_MOTORS_DEFS[actual_servo_motor];

        servo_motor_set(actual_servo_motor, robot.i2c_buffer.buffer[servo_motor_def->buffer_reg]);
    }
}