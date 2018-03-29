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
 * Local defines
 */
// Set bit 0 in slave address to indicate read
#define I2C_READ_BIT    0x01

/*
 * Current operation state of the i2c master driver.
 */
typedef enum
{
    idle_state,
    start_state,
    write_register_state,
    restart_in_read_state,
    read_data_state,
    write_data_state,
    stop_state,
    error_state
} i2c_operation_state_t;

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
    i2c_operation_state_t state;
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
    self.state = idle_state;
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

    switch (self.state)
    {
        case idle_state:
            state = i2c_idle;
            break;

        case start_state:
        case write_register_state:
        case restart_in_read_state:
        case read_data_state:
        case write_data_state:
        case stop_state:
           state = i2c_busy;
           break;

        case error_state:
           state = i2c_error;
           break;
    }
    return state;
}

void i2c_master_write (uint8_t address, uint8_t* buffer, uint16_t len)
{
    if (self.state == idle_state)
    {
        // Store the input data
        self.slave_address = address;
        self.buffer = buffer;
        self.buffer_len = len;

        // Prepare write operation. Will be started next tick for the driver
        self.state = start_state;
        self.operation = write_op;
        self.handled_bytes = 0;
    }
}

void i2c_master_run (void)
{
    uint8_t bytes_this_tick = 0;
    i2c_result_t result;

    while (bytes_this_tick < I2C_BYTES_PER_TICK)
    {
        switch (self.state)
        {
            case idle_state:
                // Nothing to do
                return;

            case start_state:
                result = i2c_start();
                if (result == i2c_ok)
                {
                    if (self.operation == write_op)
                    {
                        result = i2c_write_byte(self.slave_address);
                        self.state = write_data_state;
                    }
                }
                break;

            case write_data_state:
                result = i2c_write_byte (self.buffer[self.handled_bytes++]);

                if (self.handled_bytes == self.buffer_len)
                {
                    // All bytes written -- stop
                    self.state = stop_state;
                }
                break;

            case stop_state:
                /*
                 * Stop the I2C communication and inform the user that the operation has
                 * finished. Set the I2C module in idle to allow for next operation.
                 */
                self.state = idle_state;
                i2c_stop();
                break;

            case write_register_state:
            case restart_in_read_state:
            case read_data_state:
            case error_state:
                /*
                 * This should not happen
                 */
                break;
        }
        bytes_this_tick++;

        // Stop and return
        if ((result == i2c_arbitration_lost) ||
            (result == i2c_operation_error))
        {
            self.state = error_state;
        }
    }
}
