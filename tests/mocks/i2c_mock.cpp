/*
 * Implementation of the i2c mock module for the unit tests.
 * The module implements a mock task that can be controlled by the test cases
 * to verify that the i2c master driver calls the correct i2c operations and
 * that they are called in the correct order.
 *
 * The implementation uses the CppUMock framework
 *
 * Copyright (c) 2020. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <CppUTestExt/MockSupport.h>

extern "C"
{
    // This module mocks the following interface
    #include "hal/i2c.h"
}

i2c_result_t i2c_start (void)
{
    return static_cast<i2c_result_t>(mock().actualCall("i2c_start").returnIntValue());
}

i2c_result_t i2c_restart (void)
{
    return static_cast<i2c_result_t>(mock().actualCall("i2c_restart").returnIntValue());
}

void i2c_stop (void)
{
    mock().actualCall("i2c_stop");
}

uint8_t i2c_read_byte (uint8_t send_ack)
{
    return mock().actualCall("i2c_read_byte").withParameter("send_ack", send_ack).returnUnsignedIntValue();
}

i2c_result_t i2c_write_byte (uint8_t byte)
{
    return static_cast<i2c_result_t>(mock().actualCall("i2c_write_byte").withParameter("byte", byte).returnIntValue());
}

