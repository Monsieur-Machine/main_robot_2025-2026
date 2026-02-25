/*                      *\
    Copyrights 2025
            Riombotique
\*                      */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*   Code du RPI Pico gérant les different Actionneurs.      *
*   Ce Pico est un esclave piloté par le Pico Principal.    *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <pico/stdlib.h>
#include <stdio.h>
#include "i2c/headers/i2c_slave.h"
#include "headers/motors.h"



i2c_buffer_t i2c_buffer;

int main(void)
{
    stdio_init_all();

    sleep_ms(1000);

    puts("STDIO INIT ALL DONE");

    init_i2c_slave();

    puts("SLAVE INIT DONE");

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    motors_init();

    while(true)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, i2c_buffer.buffer[2]);
        motors_update();
        sleep_ms(10);
    }

    return 0;
}
