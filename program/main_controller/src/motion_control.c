#include "headers/motion_control.h"

#include <stdio.h>
#include <math.h>
#include <hardware/i2c.h>
#include "i2c/headers/i2c_master.h"
#include "headers/motors.h"
#include "headers/servos.h"
#include "headers/robot.h"

#define ANGLE_PER_S 90.0f
#define GAIN_KP 10

#define MSG_LEN sizeof(uint8_t)                + /*   0x00   */ \
                sizeof(uint8_t)                + /*   LED    */ \
                sizeof(uint8_t)  * NB_MOTORS   + /*  Motors  */ \
                sizeof(uint16_t) * NB_SERVOS     /*  Servos  */

#define MSG_DELAY_MS 2.0f

void motion_control_init(void)
{
    robot.motion_control_data.led_state = false;
    robot.motion_control_data.target_speed = 0;
    robot.motion_control_data.target_angle = 0.0f;
    robot.motion_control_data.motor1_speed = 0;
    robot.motion_control_data.motor2_speed = 0;
}

static inline void motors_control_update(void)
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

    //printf(">target_speed:%d\n", robot.motion_control_data.target_speed);

    float error = robot.motion_control_data.target_angle - robot.gyro_data.x_angle;
    while(error > 180) error -= 360;
    while(error < -180) error += 360;

    float correction = error * GAIN_KP;

    /*printf(">target_angle:%f\n", robot.motion_control_data.target_angle);
    printf(">current_angle:%f\n", robot.gyro_data.x_angle);
    printf(">correction:%f\n", correction);*/

    /*if(correction > 20 && correction < 150){
        correction = 150;
    }
    if(correction < -20 && correction > -150){
        correction = 150;
    }*/

    robot.motion_control_data.motor1_speed = (int16_t)((float)robot.motion_control_data.target_speed - correction);
    robot.motion_control_data.motor2_speed = (int16_t)((float)robot.motion_control_data.target_speed + correction);

    if(robot.motion_control_data.motor1_speed > 255) robot.motion_control_data.motor1_speed = 255;
    if(robot.motion_control_data.motor2_speed > 255) robot.motion_control_data.motor2_speed = 255;

    if(robot.motion_control_data.motor1_speed < -255) robot.motion_control_data.motor1_speed = -255;
    if(robot.motion_control_data.motor2_speed < -255) robot.motion_control_data.motor2_speed = -255;

    // Set dir pins
    motor_set_dir(MOTOR1, robot.motion_control_data.motor1_speed);
    motor_set_dir(MOTOR2, robot.motion_control_data.motor2_speed);
}

static inline void servos_control_update(void)
{
    const uint16_t SERVO1_POSITIONS[5] = {
        500,
        // MORE TO DO
        1950,
    };

    const uint16_t SERVO2_POSITIONS[2] = {
        600,
        2850,
    };

    // XXX DOESN'T WORK ??
    const uint16_t SERVO3_POSITIONS[2] = {
        600,
        2850,
    };
    /*
    // toggle servo 1 pos if button green is pressed
    static float servo1_toggle_elapsed_time = 0.0f;
    servo1_toggle_elapsed_time += robot.delta_time_ms;

    if(robot.udp_client.data.hard.inputs.buttons.button_green && servo1_toggle_elapsed_time >= 200.0f)
    {
        servo1_toggle_elapsed_time = 0.0f;

        servo_set_angle(SERVO1, 90 - robot.servos_data.servos_pos[SERVO1]);
    }
    */
    //servo_set_angle(SERVO3, 400);
}

void steppers_control_update(void)
{
    steppers_set_pos(48);
}

void motion_control_update(void)
{
    motors_control_update();

    servos_control_update();

    steppers_control_update();
}

void motion_control_send(void)
{
    static float elapsed_time_ms = 0.0f;
    elapsed_time_ms += robot.delta_time_ms;

    if(elapsed_time_ms >= MSG_DELAY_MS)
    {
        elapsed_time_ms = 0;
        // Send motors speed via I2C
        uint8_t data[MSG_LEN] = {
            0x00,
            (uint8_t)robot.motion_control_data.led_state,
            (uint8_t)abs(robot.motion_control_data.motor1_speed),
            (uint8_t)abs(robot.motion_control_data.motor2_speed),
            (uint8_t)(robot.servos_data.servos_pos[SERVO1] >> 8) & 0xFF,
            (uint8_t)(robot.servos_data.servos_pos[SERVO1])      & 0xFF,
            (uint8_t)(robot.servos_data.servos_pos[SERVO2] >> 8) & 0xFF,
            (uint8_t)(robot.servos_data.servos_pos[SERVO2])      & 0xFF,
            (uint8_t)(robot.servos_data.servos_pos[SERVO3] >> 8) & 0xFF,
            (uint8_t)(robot.servos_data.servos_pos[SERVO3])      & 0xFF
        };

        if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_MOTION_CONTROLLER_ADDRESS, data, MSG_LEN, false) == PICO_ERROR_GENERIC)
        {
            puts("Motion controller not reachable");
        }
    }
}