/*
 * Simple I2C master driver for BitLoom.
 *
 * This module implements a simple I2C master driver.  The driver implements
 * one task that conforms to the requirements of the BitLoom scheduler.  No
 * interrupts are used, the communication is done using busy wait.  The driver
 * uses the I2C functions provided in the HAL.
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_I2C_MASTER_H
#define BL_I2C_MASTER_H

#include "i2c_master_config.h"

/*
 * Current state of the I2C driver
 *
 * idle:    The driver is available and ready to be used.
 * busy:    The driver is currently performing an operation.
 * error:   The driver has encountered an error and stopped.
 */
typedef enum
{
    i2c_idle,
    i2c_busy,
    i2c_error
} i2c_master_state_t;

/*
 * The init function must be called before any other function in the driver is
 * called.
 */
void i2c_master_init (uint8_t taskid);

/*
 * Returns the status of the driver.  The driver will only accept new read and
 * write commands if it is idle.
 */
i2c_master_state_t i2c_master_get_state (void);

/*
 * The following functions are used to read and write data on the I2C bus.
 * The reading or writing of the data can be scheduled over several ticks, the
 * functions will return immediately and the actual operations on the bus will
 * be done by the run functions (executed by the scheduler).  When the
 * operation has completed, the driver state is set to idle.  It is important
 * that the provided data buffers are not modified by the application until the
 * operation has completed.
 *
 * Note that if the driver will ignore any requests if it is in the busy
 * state.  In that case, no data will be copied.
 */

/*
 * The write function is called by the application to request a sequence of
 * bytes to be written on the bus to the specified device.  The address
 * parameter is the 7 bit I2C address.  The buffer holds the bytes that shall
 * be written and the len denotes the number of bytes in the buffer.
 */
void i2c_master_write (uint8_t address, uint8_t* buffer, uint16_t len);

/*
 * Function to write to a specific register on a device on the I2C bus.
 * The address is the 7 bit I2C address.  The caller specifies the register to
 * be written and provides the data.
 */
void i2c_master_write_register (uint8_t address, uint8_t write_register,
                                uint8_t* buffer, uint16_t len);

/*
 * Function to read the specified number of bytes from a device on the I2C bus.
 * The address is the 7 bit I2C address.  The caller specifies the register to
 * be read and the driver will read the data and store it in the provided
 * buffer.  It is the responsibility of the caller to allocate memory for the
 * buffer.
 */
void i2c_master_read_register (uint8_t address, uint8_t read_register,
                               uint8_t* buffer, uint16_t len);

/*
 * Function used by the BitLoom scheduler to process the read and write
 * commands.  The run function reads and/or writes a few bytes on the bus each
 * tick.  The number of bytes can be configured.  When a read or write
 * operation is requested by the application, the driver is set in the busy
 * state.  The run function must then be called repeatedly until the operation
 * has been finished.  The run function will then put the driver in idle state
 * and it is then ready to process a new read or write command.  If an error
 * occur, the driver is put in the error state.
 */
void i2c_master_run (void);

#endif // BL_I2C_MASTER_H
