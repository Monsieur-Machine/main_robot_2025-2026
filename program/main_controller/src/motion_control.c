#include "headers/motion_control.h"

#include <stdio.h>
#include <math.h>
#include <hardware/i2c.h>
#include "i2c/headers/i2c_master.h"
#include "headers/motors.h"
#include "headers/robot.h"

#define ANGLE_PER_S 30.0f
#define GAIN_KD 10

#define MSG_LEN sizeof(uint8_t) * 2
#define MSG_DELAY_MS 5.0f

void motion_control_init(void)
{
    robot.motion_control_data.target_speed = 0;
    robot.motion_control_data.target_angle = 0.0f;
}

void motion_control_update(void)
{
    // First we update motion control data from received data
    robot.motion_control_data.target_angle += (float)robot.udp_client.data.hard.inputs.joystick_y / 256.0f * robot.delta_time_ms / 1000.0f * ANGLE_PER_S;
    robot.motion_control_data.target_speed =  robot.udp_client.data.hard.inputs.joystick_x;

    // Go to nearest 90* angle if L button pressed
    if(robot.udp_client.data.hard.inputs.buttons.button_l)
    {
        float angle_offset = fmodf(robot.motion_control_data.target_angle, 90.0f);

        if(fabsf(angle_offset) < 45.0f)
        {
            robot.motion_control_data.target_angle -= angle_offset;
        }
        else
        {
            robot.motion_control_data.target_angle += (90.0f - abs(angle_offset)) * ((angle_offset > 0.0f) - (angle_offset < 0.0f));
        }
    }

    // Wrap angle
    while(robot.motion_control_data.target_angle >  180.0f) robot.motion_control_data.target_angle -= 360.0f;
    while(robot.motion_control_data.target_angle < -180.0f) robot.motion_control_data.target_angle += 360.0f;

    //printf(">target_angle:%f\n", robot.motion_control_data.target_angle);
    //printf(">target_speed:%d\n", robot.motion_control_data.target_speed);

    float target_angle_radian = robot.motion_control_data.target_angle / 180.0f * M_PI;

    float error = robot.motion_control_data.target_angle - robot.gyro_data.x_angle;
    while(error > 180) error -= 360;
    while(error < -180) error += 360;

    float correction = error * GAIN_KD;

    float motor1_speed = (float)robot.motion_control_data.target_speed - correction;
    float motor2_speed = (float)robot.motion_control_data.target_speed + correction;

    if(motor1_speed > 255) motor1_speed = 255;
    if(motor2_speed > 255) motor2_speed = 255;

    if(motor1_speed < -255) motor1_speed = -255;
    if(motor2_speed < -255) motor2_speed = -255;

    // Set dir pins
    motor_set_dir(MOTOR1, (int16_t)motor1_speed);
    motor_set_dir(MOTOR2, (int16_t)motor2_speed);

    static float elapsed_time_ms = 0.0f;
    elapsed_time_ms += robot.delta_time_ms;

    if(elapsed_time_ms >= MSG_DELAY_MS)
    {
        elapsed_time_ms = 0;
        // Send motors speed via I2C
        uint8_t data[5];

        data[0] = 0x00; /// registre

        data[1] = (uint8_t)abs((int)motor1_speed);
        data[2] = (uint8_t)abs((int)motor2_speed);


        uint8_t reg = 0x00;
        
        if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_MOTION_CONTROLLER_ADDRESS, data, 3, false) == PICO_ERROR_GENERIC)
        {
            puts("Motion controller not reachable");
        }
    }
}
