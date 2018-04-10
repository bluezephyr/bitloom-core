/*
 * Simple I2C master driver.
 *
 * This module implements a simple I2C master driver.  Note that interrupts
 * are not used, the communication is done using busy wait.
 *
 * Copyright (c) 2016-2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "i2c_master.h"
#include "i2c_master_config.h"
#include "i2c.h"

/*
 * Set bit 0 in slave address to indicate read
 */
#define I2C_READ_BIT    0x01

/*
 * ACK and NACK for read operations
 */
#define I2C_ACK         0x01
#define I2C_NACK        0x00

/*
 * Current operation step of the i2c master driver.
 */
typedef enum
{
    i2c_step_idle,
    i2c_step_start,
    i2c_step_write_register,
    i2c_step_restart_in_read,
    i2c_step_read_data,
    i2c_step_write_data,
    i2c_step_stop,
    i2c_step_error
} i2c_operation_step_t;

/*
 * Type of operation that the application has requested.
 */
typedef enum
{
    read_op,
    read_register_op,
    write_op
} i2c_operation_t;

/*
 * I2C master class (singleton)
 */
typedef struct
{
    i2c_operation_step_t step;
    i2c_operation_t operation;
    uint8_t slave_address;
    uint8_t read_register;
    uint16_t handled_bytes;
    uint8_t* buffer;
    uint16_t buffer_len;
} i2c_master_t;

/*
 * I2C Master object
 */
static i2c_master_t self;

void i2c_master_init (uint8_t taskid)
{
    self.step = i2c_step_idle;
    self.operation = write_op;
    self.slave_address = 0;
    self.read_register = 0;
    self.handled_bytes = 0;
    self.buffer = (uint8_t*)0;
    self.buffer_len = 0;
}

i2c_master_state_t i2c_master_get_state (void)
{
    i2c_master_state_t state = i2c_error;

    switch (self.step)
    {
        case i2c_step_idle:
            state = i2c_idle;
            break;

        case i2c_step_start:
        case i2c_step_write_register:
        case i2c_step_restart_in_read:
        case i2c_step_read_data:
        case i2c_step_write_data:
        case i2c_step_stop:
           state = i2c_busy;
           break;

        case i2c_step_error:
           state = i2c_error;
           break;
    }
    return state;
}

void i2c_master_write (uint8_t address, uint8_t* buffer, uint16_t len)
{
    if (i2c_master_get_state() != i2c_busy)
    {
        // Store the input data
        self.slave_address = address;
        self.buffer = buffer;
        self.buffer_len = len;

        // Prepare write operation. Will be started next tick for the driver
        self.step = i2c_step_start;
        self.operation = write_op;
        self.handled_bytes = 0;
    }
}

void i2c_master_read_register (uint8_t address, uint8_t read_register,
                               uint8_t* buffer, uint16_t len)
{
    if (i2c_master_get_state() != i2c_busy)
    {
        // Store the input data
        self.slave_address = address;
        self.read_register = read_register;
        self.buffer = buffer;
        self.buffer_len = len;

        // Prepare read register operation. Will be started next tick
        self.step = i2c_step_start;
        self.operation = read_register_op;
        self.handled_bytes = 0;
    }
}

void i2c_master_run (void)
{
    uint8_t bytes_this_tick = 0;
    i2c_result_t result;

    if (self.step == i2c_step_error)
    {
        return;
    }

    while (bytes_this_tick < I2C_BYTES_PER_TICK)
    {
        switch (self.step)
        {
            case i2c_step_idle:
                // Nothing to do
                return;

            case i2c_step_start:
                result = i2c_start();
                if (result == i2c_ok)
                {
                    if (self.operation == write_op)
                    {
                        result = i2c_write_byte(self.slave_address);
                        self.step = i2c_step_write_data;
                    }
                    else if (self.operation == read_register_op)
                    {
                        result = i2c_write_byte(self.slave_address);
                        self.step = i2c_step_write_register;
                    }
                    if (result == i2c_nack_received)
                    {
                        // Slave not available
                        self.step = i2c_step_error;
                    }
                }
                break;

            case i2c_step_write_data:
                result = i2c_write_byte (self.buffer[self.handled_bytes++]);
                if (result == i2c_nack_received)
                {
                    // Slave reports error
                    self.step = i2c_step_error;
                }
                else if (self.handled_bytes == self.buffer_len)
                {
                    // All bytes written -- stop
                    self.step = i2c_step_stop;
                }
                break;

            case i2c_step_write_register:
                result = i2c_write_byte (self.read_register);
                if (result == i2c_nack_received)
                {
                    // Slave reports error
                    self.step = i2c_step_error;
                }
                else
                {
                    self.step = i2c_step_restart_in_read;
                }
                break;

            case i2c_step_restart_in_read:
                result = i2c_restart();
                if (result == i2c_ok)
                {
                    result = i2c_write_byte(self.slave_address | I2C_READ_BIT);
                    self.step = i2c_step_read_data;
                }
                break;

            case i2c_step_read_data:
                self.buffer[self.handled_bytes++] = i2c_read_byte(I2C_NACK);
                self.step = i2c_step_stop;
                break;

            case i2c_step_stop:
                /*
                 * Stop the I2C communication and inform the user that the operation has
                 * finished. Set the I2C module in i2c_step_idle to allow for next operation.
                 */
                self.step = i2c_step_idle;
                i2c_stop();
                break;

            case i2c_step_error:
                /*
                 * Errors are caught below
                 */
                break;
        }
        bytes_this_tick++;

        if ((result == i2c_arbitration_lost) ||
            (result == i2c_operation_error))
        {
            self.step = i2c_step_error;
        }

        if (self.step == i2c_step_error)
        {
            i2c_stop();
            return;
        }
    }
}
