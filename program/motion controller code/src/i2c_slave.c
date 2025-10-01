/*
 * Copyright (c) 2021 Valentin Milea <valentin.milea@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "include/i2c_slave.h"

#include <pico/stdlib.h>
#include <hardware/irq.h>
#include "include/i2c_buffer.h"

static bool transfer_in_progress;

static inline void finish_transfer(void)
{
    if(transfer_in_progress)
    {
        i2c_slave_buffer_handler(I2C_SLAVE_FINISH);
        transfer_in_progress = false;
    }
}

static void __not_in_flash_func(i2c_slave_irq_handler)(void)
{
    i2c_hw_t *hw = i2c_get_hw(I2C_SLAVE_INSTANCE);

    uint32_t intr_stat = hw->intr_stat;

    if(intr_stat == 0)
    {
        return;
    }

    if(intr_stat & I2C_IC_INTR_STAT_R_TX_ABRT_BITS)
    {
        hw->clr_tx_abrt;
        finish_transfer();
    }

    if(intr_stat & I2C_IC_INTR_STAT_R_START_DET_BITS)
    {
        hw->clr_start_det;
        finish_transfer();
    }

    if(intr_stat & I2C_IC_INTR_STAT_R_STOP_DET_BITS)
    {
        hw->clr_stop_det;
        finish_transfer();
    }

    if(intr_stat & I2C_IC_INTR_STAT_R_RX_FULL_BITS)
    {
        transfer_in_progress = true;
        i2c_slave_buffer_handler(I2C_SLAVE_RECEIVE);
    }

    if(intr_stat & I2C_IC_INTR_STAT_R_RD_REQ_BITS)
    {
        hw->clr_rd_req;
        transfer_in_progress = true;
        i2c_slave_buffer_handler(I2C_SLAVE_REQUEST);
    }
}

void i2c_slave_init(void)
{
    // Init GPIO pins
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);

    // Note: The I2C slave does clock stretching implicitly after a RD_REQ, while the Tx FIFO is empty.
    // There is also an option to enable clock stretching while the Rx FIFO is full, but we leave it
    // disabled since the Rx FIFO should never fill up (unless i2c_slave.handler() is way too slow).
    i2c_set_slave_mode(I2C_SLAVE_INSTANCE, true, I2C_SLAVE_ADDRESS);

    i2c_hw_t *hw = i2c_get_hw(I2C_SLAVE_INSTANCE);
    // unmask necessary interrupts
    hw->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS | I2C_IC_INTR_MASK_M_RD_REQ_BITS | I2C_IC_RAW_INTR_STAT_TX_ABRT_BITS | I2C_IC_INTR_MASK_M_STOP_DET_BITS | I2C_IC_INTR_MASK_M_START_DET_BITS;

    // enable interrupt for current core
    uint num = I2C0_IRQ + i2c_get_index(I2C_SLAVE_INSTANCE);
    irq_set_exclusive_handler(num, i2c_slave_irq_handler);
    irq_set_enabled(num, true);
}

void i2c_slave_deinit(void)
{
    uint num = I2C0_IRQ + i2c_get_index(I2C_SLAVE_INSTANCE);
    irq_set_enabled(num, false);
    irq_remove_handler(num, i2c_slave_irq_handler);

    i2c_set_slave_mode(I2C_SLAVE_INSTANCE, false, 0);

    transfer_in_progress = false;

    i2c_hw_t *hw = i2c_get_hw(I2C_SLAVE_INSTANCE);
    hw->intr_mask = I2C_IC_INTR_MASK_RESET;
}
