#include "headers/udp_payload.h"

#include "i2c/headers/motors.h"
#include "headers/robot.h"

void __not_in_flash_func(udp_client_message_handler)(uint8_t *payload, uint16_t len, const ip_addr_t *addr, u16_t port)
{
    if(len != 16) return;

    robot.motion_control_data.angle = ((payload[UDP_PAYLOAD_ANGLE_H_BYTE] << 8) | payload[UDP_PAYLOAD_ANGLE_L_BYTE]);

    robot.motion_control_data.x_axis_speed = payload[UDP_PAYLOAD_X_AXIS_SPEED_BYTE];
    robot.motion_control_data.y_axis_speed = payload[UDP_PAYLOAD_Y_AXIS_SPEED_BYTE];

    for(servo_motors_enum_t actual_servo_motor = SERVO_MOTOR1; actual_servo_motor < NB_SERVO_MOTORS; actual_servo_motor++)
    {
        const servo_motor_def_t *SERVO_MOTOR_DEF = &SERVO_MOTORS_DEFS[actual_servo_motor];

        robot.motion_control_data.servo_motors_pos[actual_servo_motor] = payload[SERVO_MOTOR_DEF->buffer_reg_and_payload_byte];
    }
}
