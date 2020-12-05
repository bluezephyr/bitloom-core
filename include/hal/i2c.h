/*
 * Simple I2C master in the Hardware abstraction layer (HAL) for BitLoom.
 *
 * For more information on the different commands and sequences, consult
 * the I2C specifications.
 *
 * Copyright (c) 2016-2021 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_HAL_I2C_H
#define BL_HAL_I2C_H

#include <stdint.h>

/*
 * Function to initialize the I2C hardware.  This function must be called prior
 * to any other function is called.
 */
void i2c_init (void);

/*
 * The following functions are used to read and write data on the I2C bus.
 * The reading or writing of the data will be carried out in the background and
 * typically progressed in interrupt context. The read and write functions will
 * return immediately, though, giving an i2c_request_t as response. If the returned
 * value is "ic_request_ok", the operation will be processed and the result of
 * the operation will eventually be published in the result out-parameter. During
 * the processing of the request, the value of the result parameter is set to
 * "i2c_operation_processing". It is important that the provided data buffers
 * are not modified by the application until the operation has completed.
 *
 * Note that if the driver will ignore any requests if it is in the busy
 * state. In that case, no data will be copied and the result parameter will
 * not be updated.
 */

/*
 * Possible results from the I2C request calls.
 */
enum i2c_request_t
{
    i2c_request_ok,
    i2c_request_busy,
};

/*
 * Possible result from I2C bus operations:
 */
enum i2c_op_result_t
{
    i2c_operation_ok = 0,
    i2c_operation_processing,
    i2c_operation_start_error,
    i2c_operation_repeated_start_error,
    i2c_operation_sla_error,
    i2c_operation_write_error,
    i2c_operation_read_error,
    i2c_operation_bus_error,
    i2c_operation_error
};

/*
 * Send a write command (master transmit) to the slave device on the I2C bus.
 * The write function is called by the application to request a sequence of
 * bytes to be written on the bus to the specified device. The address parameter
 * is the 7 bit I2C address. The buffer holds the bytes that shall be written
 * and the length denotes the number of bytes in the buffer.
 */
enum i2c_request_t
i2c_masterTransmit(uint8_t address, const uint8_t *buffer, uint16_t length, enum i2c_op_result_t *result);

/*
 * Send a write register data command (master transmit) to the slave device
 * on the I2C bus. The write function is called by the application to request
 * a sequence of bytes to be written to the specified register on the specified
 * device. The address parameter is the 7 bit I2C address.The buffer holds the
 * bytes that shall be written and the length denotes the number of bytes in the
 * buffer.
 *
 */
enum i2c_request_t
i2c_masterTransmitRegister(uint8_t address, uint8_t reg, const uint8_t *buffer,
                           uint16_t length, enum i2c_op_result_t *result);

/*
 * Send a read data register (master receive) command to the slave device on the
 * I2C bus. The read function is called by the application to read the specified
 * number of bytes from a device on the I2C bus. The address is the 7 bit I2C address.
 * The caller specifies the register to be read and the driver will read the data
 * and store it in the provided buffer. It is the responsibility of the caller to
 * allocate memory for the buffer.
 */
enum i2c_request_t
i2c_read_register(uint8_t address, uint8_t read_register, uint8_t *buffer,
                  uint16_t length, enum i2c_op_result_t *result);


#endif // BL_HAL_I2C_H
