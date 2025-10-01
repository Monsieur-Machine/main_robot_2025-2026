#include "include/robot.h"

#include <pico/stdlib.h>
#include "include/motors.h"
#include "include/i2c_slave.h"

void robot_init(void)
{
    stdio_init_all();

    init_motors();
    init_servo_motors();
    i2c_slave_init();

    robot.is_running = true;
}

void robot_handle_inputs_outputs(void)
{
    update_motors_from_buffer();
}

void robot_deinit(void)
{
    i2c_slave_deinit();
}