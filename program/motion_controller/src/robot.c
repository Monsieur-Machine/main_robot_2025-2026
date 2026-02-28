#include "headers/robot.h"

#include <pico/stdlib.h>
#include <time.h>
#include "headers/motors.h"
#include "headers/servos.h"
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

    servos_init();

    init_i2c_slave();

    // Initialisation ended, loop forever if initialisation failed
    for(uint i = 0, led_state = false; robot.is_running ? (i < 5) : true; i++)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, led_state);

        sleep_ms(100);

        led_state = !led_state;
    }
}

static inline void update_time(void)
{
    static float last_time = 0.0f;
    float start_time = (float)clock() * 1000.0f / (float)CLOCKS_PER_SEC;
    robot.delta_time_ms = start_time - last_time;
    last_time = start_time;

    gpio_put(PICO_DEFAULT_LED_PIN, robot.i2c_buffer.buffer[0]);
}

void robot_update(void)
{
    update_time();

    motors_update();

    servos_update();

    tight_loop_contents();
}

void robot_deinit(void)
{
    deinit_i2c_slave();
}
