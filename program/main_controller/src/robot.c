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

    mutex_enter_blocking(&wifi_mutex);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    mutex_exit(&wifi_mutex);

    //i2c_master_init();

    //mcp23017_init();

    //gyro_init();

    //gyro_calibrate();

    //init_motion_control();

    init_wifi_operator();
    udp_client_init();

    // Initialisation ended
    for(uint i = 0, led_state = true; i < 5; i++)
    {
        mutex_enter_blocking(&wifi_mutex);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
        mutex_exit(&wifi_mutex);

        sleep_ms(100);

        led_state = !led_state;
    }
}

static inline void update_time(void)
{
    static float last_time = 0.0;
    float start_time = (float)clock() * 1000.0f / (float)CLOCKS_PER_SEC;
    robot.delta_time_ms = start_time - last_time;
    last_time = start_time;

    static float elapsed_time = 0.0f;
    elapsed_time += robot.delta_time_ms;

    static bool led_state = false;
    if(elapsed_time >= 1000.0f)
    {
        elapsed_time = 0.0f;

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

    //gyro_update();

    //i2c_update_motion_control();

    //i2c_update_servo_motors();

    //mcp23017_update();

    tight_loop_contents();
}

void robot_deinit(void)
{
    udp_client_deinit();
    //i2c_master_deinit();
}
