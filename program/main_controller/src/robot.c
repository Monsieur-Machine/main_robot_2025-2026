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

    i2c_master_init();

    mcp23017_init();

    if(gyro_init())
        robot.is_running = false;

    gyro_calibrate();

    motion_control_init();

    if(wifi_operator_init()){
        while(1){
            printf("No Wifi\n");
            robot.is_running = false;
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            sleep_ms(50);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            sleep_ms(50);
        }
    }
        

    if(udp_client_init()){
        while(1){
            printf("No UDP\n");
            robot.is_running = false;
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            sleep_ms(50);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            sleep_ms(50);
        }
    }
        
    // Initialisation ended
    for(uint8_t i = 0, led_state = true; i < 5; i++)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);

        sleep_ms(100);

        led_state = !led_state;
    }
    printf(">robot_running:%d\n", robot.is_running);
}

static inline void update_time(void)
{
    static float last_time_ms = 0.0;
    float start_time_ms = (float)clock() * 1000.0f / (float)CLOCKS_PER_SEC;
    robot.delta_time_ms = start_time_ms - last_time_ms;
    last_time_ms = start_time_ms;

    static float elapsed_time_ms = 0.0f;
    elapsed_time_ms += robot.delta_time_ms;

    static bool led_state = false;
    if(elapsed_time_ms >= 1000.0f)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);

        elapsed_time_ms = 0.0f;
        led_state = !led_state;
    }
}

void robot_handle_inputs_outputs(void)
{
    static bool led_state=false;
    static uint32_t temps_ms_old =0, timer_200_ms=0;
    uint32_t temps_ms;
    update_time();

    cyw43_arch_poll();

    gyro_update();

    motion_control_update();

    mcp23017_update();
    
    temps_ms = to_ms_since_boot(get_absolute_time());
    timer_200_ms += temps_ms - temps_ms_old;
    temps_ms_old = temps_ms;
    if(timer_200_ms > 200){
        timer_200_ms = 0;
        led_state= !led_state;
        uint8_t data[] = {0x02, led_state};
        int ret = i2c_write_blocking(I2C_MASTER_INSTANCE, 0x09, data, 2, false);
    }
}

void robot_deinit(void)
{
    udp_client_deinit();

    cyw43_arch_deinit();

    i2c_master_deinit();
}
