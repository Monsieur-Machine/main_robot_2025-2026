#include "headers/i2c_buffer.h"

#include "headers/robot.h"
#include "headers/motors.h"

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
