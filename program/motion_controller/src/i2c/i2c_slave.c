#include "headers/i2c_slave.h"

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>
#include <headers/robot.h>
static struct
{
    uint8_t mem[256];
    uint8_t mem_address;
    bool mem_address_written;
} context;

int nb_messages = 0;

// Our handler is called from the I2C ISR, so it must complete quickly. Blocking calls /
// printing to stdio may interfere with interrupt handling.
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
    case I2C_SLAVE_RECEIVE: // master has written some data
        if (!context.mem_address_written) {
            // writes always start with the memory address
            context.mem_address = i2c_read_byte_raw(i2c);
            context.mem_address_written = true;
        } else {
            // save into memory
            context.mem[context.mem_address] = i2c_read_byte_raw(i2c);
            context.mem_address++;
        }
        break;
    case I2C_SLAVE_REQUEST: // master is requesting data
        // load from memory
        i2c_write_byte_raw(i2c, context.mem[context.mem_address]);
        context.mem_address++;
        break;
    case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
        context.mem_address_written = false;
        nb_messages++;
        break;
    default:
        break;
    }
}

void init_i2c_slave(void)
{
    // Init GPIO pins
    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);

    i2c_init(I2C_SLAVE_INSTANCE, 0);
    // New SDK method to init i2c slave
    i2c_slave_init(I2C_SLAVE_INSTANCE, I2C_SLAVE_ADDRESS, i2c_slave_handler);
}

void deinit_i2c_slave(void)
{
    // Reset GPIO pins
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_NULL);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_NULL);

    // New SDK method to reset i2c slave
    i2c_slave_deinit(I2C_SLAVE_INSTANCE);
}

uint8_t get_vitesse_moteur_1(void){
    return context.mem[0];
}

uint8_t get_vitesse_moteur_2(void){
    return context.mem[1];
}
