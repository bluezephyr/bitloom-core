/*
 * Implementation of the i2c mock module for the unit tests.
 * The module implements a mock task that can be controlled by the test cases
 * to verify that the i2c master driver calls the correct i2c operations and
 * that they are called in the correct order.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include "i2c_mock.h"
#include "unity_fixture.h"

#define MAX_EXPECTATIONS 100

typedef enum
{
    start,
    restart,
    stop,
    write_byte,
    read_byte_ack,
    read_byte_nack
} operation_t;

typedef struct
{
    operation_t operation;
    uint8_t data;
    uint8_t read_byte;
    i2c_result_t result;
} action_t;


static action_t* m_expectations;
static uint16_t m_stored_expectations;
static uint16_t m_used_expectations;
static action_t m_current_action;

/*
 * Local function prototypes.
 */
static void store_expectation(operation_t op, i2c_result_t result, uint8_t data, uint8_t read_byte);
static void store_current_action(operation_t op, uint8_t data, uint8_t read_byte);
static void fail_when(int condition, const char* message);
static int operation_is_not_expected(void);
static int data_is_not_expected(void);

/*
 * Functions to control the mock.
 */
void i2c_mock_create(void)
{
    m_expectations = calloc(MAX_EXPECTATIONS, sizeof(action_t));
    m_stored_expectations = 0;
    m_used_expectations = 0;
    m_current_action = (action_t) {0};
}

void i2c_mock_destroy(void)
{
    if (m_expectations)
    {
        free(m_expectations);
    }
    m_expectations = NULL;
}

void i2c_mock_verify_complete(void)
{
}

void i2c_mock_expect_start_then_return(i2c_result_t result)
{
    store_expectation(start, result, 0, 0);
}

void i2c_mock_expect_restart_then_return(i2c_result_t result)
{
    store_expectation(restart, result, 0, 0);
}

void i2c_mock_expect_stop(void)
{
    store_expectation(stop, i2c_ok, 0, 0);
}

void i2c_mock_expect_write_byte_then_return(uint8_t byte, i2c_result_t result)
{
    store_expectation(write_byte, result, byte, 0);
}

static void store_expectation(operation_t op, i2c_result_t result, uint8_t data, uint8_t read_byte)
{
    action_t expectation;

    expectation.operation = op;
    expectation.data = data;
    expectation.result = result;
    expectation.read_byte = read_byte;

    m_expectations[m_stored_expectations] = expectation;
    m_stored_expectations++;
}

/*
 * Implementation of the I2C HAL for the mock.
 */
i2c_result_t i2c_start (void)
{
    store_current_action(start, 0, 0);
    fail_when(operation_is_not_expected(), "Unexpected operation 'start'");
    m_used_expectations++;
    return m_expectations[m_used_expectations].result;
}

i2c_result_t i2c_restart (void)
{
    store_current_action(restart, 0, 0);
    fail_when(operation_is_not_expected(), "Unexpected operation 'restart'");
    m_used_expectations++;
    return m_expectations[m_used_expectations].result;
}

void i2c_stop (void)
{
    store_current_action(stop, 0, 0);
    fail_when(operation_is_not_expected(), "Unexpected operation 'stop'");
    m_used_expectations++;
}

i2c_result_t i2c_write_byte (uint8_t byte)
{
    store_current_action(write_byte, byte, 0);
    fail_when(operation_is_not_expected(), "Unexpected operation 'write_byte'");
    fail_when(data_is_not_expected(), "Unexpected data");
    m_used_expectations++;
    return m_expectations[m_used_expectations].result;
}

i2c_result_t i2c_read_byte (uint8_t* byte, uint8_t send_ack)
{
}

static void store_current_action(operation_t op, uint8_t data, uint8_t read_byte)
{
    m_current_action.operation = op;
    m_current_action.data = data;
    m_current_action.read_byte = read_byte;
}

static void fail_when(int condition, const char* message)
{
    if (condition)
    {
        TEST_FAIL_MESSAGE(message);
    }
}

static int operation_is_not_expected(void)
{
    return (m_current_action.operation != m_expectations[m_used_expectations].operation);
}

static int data_is_not_expected(void)
{
    return (m_current_action.data != m_expectations[m_used_expectations].data);
}
