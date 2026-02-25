#include "headers/robot.h"

#include <pico/stdlib.h>
#include <time.h>
#include "headers/motors.h"
#include "i2c/headers/i2c_slave.h"

#include <stdio.h>

void robot_init(void)
{
    robot.is_running = true;

    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    motors_init();

    init_i2c_slave();

    // Initialisation ended
    for(uint i = 0, led_state = false; i < 5; i++)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, led_state);

        sleep_ms(100);

        led_state = !led_state;
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

    if(nb_messages >= 10)
    {
        nb_messages = 0;

        gpio_put(PICO_DEFAULT_LED_PIN, led_state);

        led_state = !led_state;
    }
}

void robot_handle_inputs_outputs(void)
{
    static bool led_state=false;
    update_time();

    motors_update();

    printf(">motor1_speed:%d\n", get_vitesse_moteur_1());
    printf(">motor2_speed:%d\n", get_vitesse_moteur_2());
    sleep_ms(200);
    led_state= !led_state;
    uint8_t data[] = {0x02, led_state};
    int ret = i2c_write_blocking(i2c0, 0x09, data, 2, false);

    //tight_loop_contents();
}

void robot_deinit(void)
{
    deinit_i2c_slave();
}
