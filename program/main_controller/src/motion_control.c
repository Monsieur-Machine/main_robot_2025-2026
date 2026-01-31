#include "headers/motion_control.h"

#include <stdio.h>
#include <math.h>
#include "i2c/headers/motors.h"
#include "headers/robot.h"

#define GAIN_KD 10

void init_motion_control(void)
{
    robot.motion_control_data.angle = 0;
    robot.motion_control_data.x_axis_speed = 0;
    robot.motion_control_data.y_axis_speed = 0;
}

void i2c_update_motion_control(void)
{
    // Motion control work as follow :
    //  - Motors are rotated on-board at 45*.
    //  - we calculate the error of the targeted angle and the actual angle
    //  - First we estimate the targeted speed on irl board on X and Y axis
    //  - Then we calculate motors speed from targeted speed and the error
    //  - And we put limits because motors speed are send by i2c on 1 byte

    float actual_angle = robot.gyro_data.x_angle - 45.0f;

    float target_angle = (float)robot.motion_control_data.angle - 45.0f;
    float target_angle_radian = target_angle / 180.0f * M_PI;

    float error = target_angle - actual_angle;
    if(error > 180) error -= 360;
    if(error < -180) error += 360;

    float correction = error * GAIN_KD;

    float target_x_axis_speed = cosf(target_angle_radian) * robot.motion_control_data.x_axis_speed +
                                    sinf(target_angle_radian) * robot.motion_control_data.y_axis_speed;
    float target_y_axis_speed = cosf(target_angle_radian) * robot.motion_control_data.y_axis_speed -
                                    sinf(target_angle_radian) * robot.motion_control_data.x_axis_speed;

    int motor1_speed = target_x_axis_speed + (int)correction;
    int motor2_speed = target_x_axis_speed - (int)correction;
    int motor3_speed = target_y_axis_speed + (int)correction;
    int motor4_speed = target_y_axis_speed - (int)correction;

    if(motor1_speed > 127) motor1_speed = 127;
    if(motor2_speed > 127) motor2_speed = 127;
    if(motor3_speed > 127) motor3_speed = 127;
    if(motor4_speed > 127) motor4_speed = 127;

    if(motor1_speed < -128) motor1_speed = -128;
    if(motor2_speed < -128) motor2_speed = -128;
    if(motor3_speed < -128) motor3_speed = -128;
    if(motor4_speed < -128) motor4_speed = -128;

    i2c_set_motor(MOTOR1, motor1_speed);
    i2c_set_motor(MOTOR2, motor2_speed);
    i2c_set_motor(MOTOR3, motor3_speed);
    i2c_set_motor(MOTOR4, motor4_speed);
}

void i2c_update_servo_motors(void)
{
    //for(servo_motors_enum_t actual_servo_motor = SERVO_MOTOR1; actual_servo_motor < NB_SERVO_MOTORS; actual_servo_motor++)
    //    i2c_set_servo_motor(actual_servo_motor, robot.motion_control_data.servo_motors_pos[actual_servo_motor]);
}
