/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
* Thank you Keuronde !                                                                              *
*   https://git.poivron-robotique.fr/Keuronde/Holonome_2024/src/branch/Demo_2025_03/gyro_L3GD20H.c  *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "headers/gyro.h"

#include <stdio.h>
#include <stdint.h>
#include "headers/i2c_master.h"
#include "headers/robot.h"

// #include <stdio.h>

#define SAMPLE_MIN_ELAPSED_TIME_MS 2.0f
#define DPS_PER_DIGIT 1000.0f / 65535.0f * 2.0f // FS_SEL = 2 // Why * 2 ? Idk it works...

int gyro_init(void)
{
    // Reset gyro
    const uint8_t PWR_MGMT_REG = 0x6B;
    const uint8_t PWR_MGMT_CONFIG = 0b10000000; // Reset bit set to 1

    uint8_t buf[2] = {PWR_MGMT_REG, PWR_MGMT_CONFIG};
    if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, buf, 2, false) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable during initialisation");
        return -1;
    }

    sleep_ms(10);

    // Remove reset bit
    buf[1] = 0x00;
    if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, buf, 2, false) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable during inititalisation");
        return -1;
    }

    sleep_ms(10);

    // Verify device identification
    const uint8_t WHO_AM_I_REG = 0x75;
    buf[0] = WHO_AM_I_REG;
    if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, buf, 1, true) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable during initialisation");
    }
    if(i2c_read_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, buf, 1, false) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable during initialisation");
        return -1;
    }

    if(buf[0] != 0x68)
    {
        puts("Device is not the expected gyro");
        printf("Have : 0x%x\nExpected : 0x68\n", buf[0]);
        return -1;
    }

    // Write gyro config
    const uint8_t GYRO_CONFIG_REG = 0x1B;
    const uint8_t GYRO_CONFIG = 0b00010000; // FS_SEL = 2
    buf[0] = GYRO_CONFIG_REG;
    buf[1] = GYRO_CONFIG;
    if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, buf, 2, false) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable during initialisation");
        return -1;
    }

    // Reset gyro data
    robot.gyro_data.x_offset = 0.0f;
    robot.gyro_data.y_offset = 0.0f;
    robot.gyro_data.z_offset = 0.0f;

    robot.gyro_data.x_angle = 0.0f;
    robot.gyro_data.y_angle = 0.0f;
    robot.gyro_data.z_angle = 0.0f;

    return 0;
}

static inline void __attribute__((always_inline)) gyro_read_raw(int16_t *x, int16_t *y, int16_t *z)
{
    const uint8_t GYRO_XOUT_H_REG = 0x43;

    uint8_t data[6];
    if(i2c_write_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, &GYRO_XOUT_H_REG, 1, true) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable");
    }

    if(i2c_read_blocking(I2C_MASTER_INSTANCE, I2C_GYRO_ADDRESS, data, 6, false) == PICO_ERROR_GENERIC)
    {
        puts("Gyro is not reachable");
    }

    *x = (int16_t)((data[0] << 8) | data[1]);
    *y = (int16_t)((data[2] << 8) | data[3]);
    *z = (int16_t)((data[4] << 8) | data[5]);
}

void gyro_calibrate(void)
{
    const size_t NB_SAMPLES = 2000;

    int16_t x, y, z;
    int32_t x_sum = 0, y_sum = 0, z_sum = 0;

    for(uint i = 0; i < NB_SAMPLES; i++)
    {
        gyro_read_raw(&x, &y, &z);

        x_sum += x;
        y_sum += y;
        z_sum += z;

        sleep_ms(SAMPLE_MIN_ELAPSED_TIME_MS);

        //printf(">cal_x:%d\n", x);
        //printf(">cal_y:%d\n", y);
        //printf(">cal_z:%d\n", z);
    }

    robot.gyro_data.x_offset = (float)x_sum / (float)NB_SAMPLES * DPS_PER_DIGIT;
    robot.gyro_data.y_offset = (float)y_sum / (float)NB_SAMPLES * DPS_PER_DIGIT;
    robot.gyro_data.z_offset = (float)z_sum / (float)NB_SAMPLES * DPS_PER_DIGIT;

    //printf("\nx_cal:%.5f\n", robot.gyro_data.x_offset);
    //printf("\ny_cal:%.5f\n", robot.gyro_data.y_offset);
    //printf("\nz_cal:%.5f\n", robot.gyro_data.z_offset);
}

static inline void __attribute__((always_inline)) gyro_get_dps(float *x_dps, float *y_dps, float *z_dps)
{
    int16_t x, y, z;
    gyro_read_raw(&x, &y, &z);

    *x_dps = x * DPS_PER_DIGIT;
    *y_dps = y * DPS_PER_DIGIT;
    *z_dps = z * DPS_PER_DIGIT;

    //printf(">x_dps:%f\n", *x_dps);
    //printf(">y_dps:%f\n", *y_dps);
    //printf(">z_dps:%f\n", *z_dps);
}

void gyro_update(void)
{
    static float elapsed_since_sample_ms = SAMPLE_MIN_ELAPSED_TIME_MS;

    elapsed_since_sample_ms += robot.delta_time_ms;

    if(elapsed_since_sample_ms >= SAMPLE_MIN_ELAPSED_TIME_MS)
    {
        float x_dps, y_dps, z_dps;
        gyro_get_dps(&x_dps, &y_dps, &z_dps);

        x_dps -= robot.gyro_data.x_offset;
        y_dps -= robot.gyro_data.y_offset;
        z_dps -= robot.gyro_data.z_offset;

        robot.gyro_data.x_angle += x_dps * elapsed_since_sample_ms / 1000.0f;
        robot.gyro_data.y_angle += y_dps * elapsed_since_sample_ms / 1000.0f;
        robot.gyro_data.z_angle += z_dps * elapsed_since_sample_ms / 1000.0f;

        while(robot.gyro_data.x_angle > 180)    robot.gyro_data.x_angle -= 360;
        while(robot.gyro_data.x_angle < -180)   robot.gyro_data.x_angle += 360;
        while(robot.gyro_data.y_angle > 180)    robot.gyro_data.y_angle -= 360;
        while(robot.gyro_data.y_angle < -180)   robot.gyro_data.y_angle += 360;
        while(robot.gyro_data.z_angle > 180)    robot.gyro_data.z_angle -= 360;
        while(robot.gyro_data.z_angle < -180)   robot.gyro_data.z_angle += 360;

        //printf(">gyro_x_angle:%f\n", robot.gyro_data.x_angle);
        //printf(">gyro_y_angle:%f\n", robot.gyro_data.y_angle);
        //printf(">gyro_z_angle:%f\n", robot.gyro_data.z_angle);

        elapsed_since_sample_ms = 0.0;
    }
}
