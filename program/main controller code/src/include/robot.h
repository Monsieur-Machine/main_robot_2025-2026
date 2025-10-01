#ifndef ROBOT_H
#define ROBOT_H

#include <stdbool.h>
#include "gyro.h"
#include "motion_control.h"

typedef struct robot_t {
    gyro_data_t gyro_data;
    motion_control_data_t motion_control_data;

    bool is_running;
    double delta_time_ms;
} robot_t;

extern robot_t robot;

// Init all robot's components
int robot_init(void);
// Handle inputs and outputs
void robot_handle_inputs_outputs(void);
// Deinit all robot's components
void robot_deinit(void);

#endif // ROBOT_H