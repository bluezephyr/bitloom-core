/*
 * Mock to test the behaviour of the i2c master driver.
 *
 * Copyright (c) 2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_I2C_MOCK_H
#define BL_I2C_MOCK_H

#include "hal/i2c.h"

void i2c_mock_create(void);
void i2c_mock_destroy(void);
void i2c_mock_verify_complete(void);

void i2c_mock_expect_start_then_return(i2c_result_t result);
void i2c_mock_expect_restart_then_return(i2c_result_t result);
void i2c_mock_expect_stop(void);
void i2c_mock_expect_write_byte_then_return(uint8_t byte, i2c_result_t result);
void i2c_mock_expect_read_byte_send_ack_then_return(uint8_t byte);
void i2c_mock_expect_read_byte_send_nack_then_return(uint8_t byte);

#endif // BL_I2C_MOCK_H
