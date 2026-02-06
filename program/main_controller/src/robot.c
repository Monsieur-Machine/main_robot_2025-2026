#include "headers/robot.h"

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
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

    //i2c_master_init();

    //mcp23017_init();

    //gyro_init();

    //gyro_calibrate();

    //motion_control_init();

    if(wifi_operator_init())
        robot.is_running = false;

    if(udp_client_init())
        robot.is_running = false;

    // Initialisation ended
    for(uint8_t i = 0, led_state = true; i < 5; i++)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);

        sleep_ms(100);

        led_state = !led_state;
    }
    printf("robot.is_running : %d\n", robot.is_running);
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

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);

        led_state = !led_state;
    }
}

void robot_handle_inputs_outputs(void)
{
    cyw43_arch_poll();

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
    cyw43_arch_deinit();
    i2c_master_deinit();
}
