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
 * I2C master state variables
 */
static i2c_operation_state_t m_state;
static i2c_operation_t m_operation_type;
static uint8_t m_slave_address;
static uint8_t m_read_register;
static uint16_t m_handled_bytes;
static uint8_t* m_buffer;
static uint16_t m_buffer_len;

void i2c_master_init (uint8_t taskid)
{
    m_state = idle_state;
    m_operation_type = write_op;
    m_slave_address = 0;
    m_read_register = 0;
    m_handled_bytes = 0;
    m_buffer = (uint8_t*)0;
    m_buffer_len = 0;
}

i2c_master_state_t i2c_master_get_state (void)
{
    i2c_master_state_t state = i2c_error;

    switch (m_state)
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
    if (m_state == idle_state)
    {
        // Store the input data
        m_slave_address = address;
        m_buffer = buffer;
        m_buffer_len = len;

        // Prepare write operation. Will be started next tick for the driver
        m_state = start_state;
        m_operation_type = write_op;
        m_handled_bytes = 0;
    }
}

void i2c_master_run (void)
{

}
