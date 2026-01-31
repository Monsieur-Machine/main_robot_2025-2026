#ifndef GYRO_H
#define GYRO_H

#define I2C_GYRO_ADDRESS 0x6b

typedef struct gyro_data_t {
    float x_offset, y_offset, z_offset;
    float x_angle, y_angle, z_angle;
} gyro_data_t;

// Check if gyro has correctly initialised and configure it for simple use
void gyro_init(void);
// Calibrate gyro
void gyro_calibrate(void);
// Update gyro data
void gyro_update(void);

#endif // GYRO_H