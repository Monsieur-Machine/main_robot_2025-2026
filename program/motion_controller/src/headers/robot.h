#ifndef ROBOT_H
#define ROBOT_H

#include "i2c/headers/i2c_slave.h"

typedef struct robot_t {
    i2c_buffer_t i2c_buffer;

    bool is_running;
    double delta_time_ms;
} robot_t;

extern robot_t robot;

// Init all robot's components
void robot_init(void);
// Handle inputs and outputs
void robot_handle_inputs_outputs(void);
// Deinit all robot's components
void robot_deinit(void);

#endif // ROBOT_H