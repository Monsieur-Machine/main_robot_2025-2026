#include "include/robot.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <time.h>
#include <pico/mutex.h>
#include "include/i2c_master.h"
#include "include/udp_client.h"
#include "include/wifi_operator.h"

//#include "include/udp_fake_client.h"

auto_init_mutex(wifi_mutex);

int robot_init(void)
{
    stdio_init_all();

    if(cyw43_arch_init())
    {
        return -1;
    }

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    wifi_operator_init();
    udp_client_init();

    i2c_master_init();
    if(init_gyro()) return -1;
    gyro_calibrate();

    init_motion_control();

    robot.is_running = true;

    return 0;
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

    if(elapsed_time >= 1000)
    {
        elapsed_time = 0;

        mutex_enter_blocking(&wifi_mutex);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
        mutex_exit(&wifi_mutex);

        led_state = !led_state;
    }
}

void robot_handle_inputs_outputs(void)
{
    mutex_enter_blocking(&wifi_mutex);
    cyw43_arch_poll();
    mutex_exit(&wifi_mutex);

    update_time();

    gyro_update();

    i2c_update_motion_control();

    i2c_update_servo_motors();

    tight_loop_contents();
}

void robot_deinit(void)
{
    //udp_client_exit();
    i2c_master_deinit();
}