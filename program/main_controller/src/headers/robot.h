#ifndef ROBOT_H
#define ROBOT_H

#include <stdbool.h>
#include "i2c/headers/gyro.h"
#include "i2c/headers/mcp23017.h"
#include "headers/motion_control.h"
#include "headers/servos.h"
#include "wifi/headers/udp_client.h"

typedef struct robot_t {
    gyro_data_t gyro_data;
    mcp23017_data_t mcp23017_data;

    motion_control_data_t motion_control_data;
    servos_data_t servos_data;

    udp_client_t udp_client;

    bool is_running;
    float delta_time_ms;
} robot_t;

extern robot_t robot;

// Init all robot's components
void robot_init(void);
// Handle inputs and outputs
void robot_update(void);
// Deinit all robot's components
void robot_deinit(void);

#endif // ROBOT_H