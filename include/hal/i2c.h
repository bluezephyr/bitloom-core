/*
 * Simple I2C master in the Hardware abstraction layer (HAL) for BitLoom.
 *
 * For more information on the different commands and sequences, consult
 * the I2C specifications.
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_HAL_I2C_H
#define BL_HAL_I2C_H

#include <stdint.h>

/*
 * Possible result from I2C bus operations:
 *
 * ok:
 *     The operation was executed successfully
 * ack_received:
 *     ACK has been received from the slave as a result from the operation
 * nack_received:
 *     NACK has been received from the slave as a result from the operation
 * arbitration lost:
 *     The driver lost its master role on the bus
 * operation error:
 *     Some unexpected error occurred during the operation
 */
typedef enum
{
    i2c_ok,
    i2c_ack_received,
    i2c_nack_received,
    i2c_arbitration_lost,
    i2c_operation_error
} i2c_result_t;

/*
 * Function to initalize the I2C hardware.  This function must be called prior
 * to any other function is called.
 */
void i2c_init (void);

/*
 * Send start command on the I2C bus.  A success will return ok.
 */
i2c_result_t i2c_start (void);

/*
 * Send restart command on the I2C bus.  A success will return ok.
 */
i2c_result_t i2c_restart (void);

/*
 * Send stop command on the I2C bus.
 */
void i2c_stop (void);

/*
 * Write the specified byte on the bus.  Note that the bus must be started
 * first.  A success will return the status response (ACK/NACK) from the slave.
 */
i2c_result_t i2c_write_byte (uint8_t byte);

/*
 * Read one byte from from a device on the bus.  The send_ack parameter is used
 * to indicate whether the master should signal ACK (1) or NACK (0) to the
 * slave.  The value of the read byte is returned.
 */
uint8_t i2c_read_byte (uint8_t send_ack);

#endif // BL_HAL_I2C_H
