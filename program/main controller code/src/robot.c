#include "headers/robot.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <time.h>
#include <pico/mutex.h>
#include "i2c/headers/i2c_master.h"
#include "i2c/headers/mcp23017.h"
#include "wifi/headers/udp_client.h"
#include "wifi/headers/wifi_operator.h"

auto_init_mutex(wifi_mutex);

void robot_init(void)
{
    robot.is_running = true;

    stdio_init_all();

    if(cyw43_arch_init())
        robot.is_running = false;

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    //wifi_operator_init();
    //udp_client_init();

    i2c_master_init();
/*
    if(init_mcp23017())
        robot.is_running = false;
*/
/*
    if(init_gyro())
        robot.is_running = false;

    gyro_calibrate();
*/
    //init_motion_control();

    // Initialisation ended
    bool led_state = false;
    for(uint i = 0, state = true; i < 5; i++)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
        sleep_ms(100);
        state = !state;
    }
}

static inline void update_time(void)
{
    static bool led_state = false;
    static double last_time = 0.0;
    double start_time = (double)clock() * 1000.0 / (double)CLOCKS_PER_SEC;
    robot.delta_time_ms = start_time - last_time;
    last_time = start_time;

    static double elapsed_time = 0.0;
    elapsed_time += robot.delta_time_ms;

    if(elapsed_time >= 1000.0)
    {
        elapsed_time = 0.0;

        mutex_enter_blocking(&wifi_mutex);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
        mutex_exit(&wifi_mutex);

        led_state = !led_state;
    }
}

void robot_handle_inputs_outputs(void)
{
    //mutex_enter_blocking(&wifi_mutex);
    //cyw43_arch_poll();
    //mutex_exit(&wifi_mutex);

    update_time();

    //gyro_update();

    //i2c_update_motion_control();

    //i2c_update_servo_motors();

    mcp23017_update();

    tight_loop_contents();
}

void robot_deinit(void)
{
    udp_client_exit();
    i2c_master_deinit();
}
