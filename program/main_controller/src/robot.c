#include "headers/robot.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <pico/time.h>
#include <time.h>
#include "i2c/headers/i2c_master.h"
#include "i2c/headers/mcp23017.h"
#include "wifi/headers/udp_client.h"
#include "wifi/headers/wifi_operator.h"

void robot_init(void)
{
    robot.is_running = true;

    stdio_init_all();

    if(cyw43_arch_init_with_country(CYW43_COUNTRY_FRANCE))
        robot.is_running = false;

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);

    // I2C and devices
    i2c_master_init();

    mcp23017_init();

    if(gyro_init())
        robot.is_running = false;

    gyro_calibrate();

    motion_control_init();

    // WiFi
    if(wifi_operator_init())
        robot.is_running = false;

    if(udp_client_init())
        robot.is_running = false;

    // Initialisation ended, loop forever if inititalisation failed
    for(uint i = 0, led_state = false; robot.is_running ? (i < 5) : true; i++)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);

        sleep_ms(100);

        led_state = !led_state;
    }
}

static inline void update_time(void)
{
    static float last_time_ms = 0.0;
    float start_time_ms = (float)clock() * 1000.0f / (float)CLOCKS_PER_SEC;
    robot.delta_time_ms = start_time_ms - last_time_ms;
    last_time_ms = start_time_ms;

    static float elapsed_time_ms = 0.0f;
    elapsed_time_ms += robot.delta_time_ms;

    if(elapsed_time_ms >= 1000.0f)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, robot.motion_control_data.led_state);

        elapsed_time_ms = 0.0f;

        robot.motion_control_data.led_state = !robot.motion_control_data.led_state;
    }
}

void robot_update(void)
{
    update_time();

    cyw43_arch_poll();

    gyro_update();

    motion_control_update();

    motion_control_send();

    mcp23017_update();

    tight_loop_contents();
}

void robot_deinit(void)
{
    udp_client_deinit();

    cyw43_arch_deinit();

    i2c_master_deinit();
}
