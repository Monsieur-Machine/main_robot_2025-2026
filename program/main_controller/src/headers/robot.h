#ifndef ROBOT_H
#define ROBOT_H

#include <stdbool.h>
#include "i2c/headers/mcp23017.h"
#include "i2c/headers/gyro.h"
#include "motion_control.h"
#include "wifi/headers/udp_client.h"

typedef struct robot_t {
    udp_client_t udp_client;
    mcp23017_data_t mcp23017_data;
    gyro_data_t gyro_data;
    motion_control_data_t motion_control_data;

    bool is_running;
    float delta_time_ms;
} robot_t;

extern robot_t robot;

// Init all robot's components
void robot_init(void);
// Handle inputs and outputs
void robot_handle_inputs_outputs(void);
// Deinit all robot's components
void robot_deinit(void);

#endif // ROBOT_H