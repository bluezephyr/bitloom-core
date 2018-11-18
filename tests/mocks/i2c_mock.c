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
#include "unity_fixture.h"
#include "i2c_mock.h"


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
    i2c_result_t result;
} action_t;

/*
 * I2C mock class to hold state and expected operations (singleton)
 */
typedef struct
{
    action_t* expectations;
    uint16_t stored_expectations;
    uint16_t used_expectations;
    action_t current_action;
} i2c_mock_t;

/*
 * I2C mock object
 */
static i2c_mock_t self;

/*
 * Local function prototypes.
 */
static void store_expectation(operation_t op, i2c_result_t result, uint8_t data);
static void store_current_action(operation_t op, uint8_t data);
static void fail_when_operation_is_not_expected(void);
static void fail_when(int condition, const char* message);
static int data_is_not_expected(void);
static int too_many_operations(void);
static int not_all_operations_used(void);
static const char* get_string_string(operation_t operation);

/*
 * Strings
 */
static const char* string_operation_start          = "start";
static const char* string_operation_restart        = "restart";
static const char* string_operation_stop           = "stop";
static const char* string_operation_write_byte     = "write_byte";
static const char* string_operation_read_byte_ack  = "read_byte_ack";
static const char* string_operation_read_byte_nack = "read_byte_nack";
static const char* string_operation_unknown        = "unknonw string";
static const char* string_too_many_operations      = "Too many operations";
static const char* string_unexpected_data          = "Unexpected data";
static const char* string_not_all_operations_used  = "Not all operations used";

/*
 * Functions to control the mock.
 */
void i2c_mock_create(void)
{
    self.expectations = calloc(MAX_EXPECTATIONS, sizeof(action_t));
    self.stored_expectations = 0;
    self.used_expectations = 0;
    self.current_action = (action_t) {0};
}

void i2c_mock_destroy(void)
{
    if (self.expectations)
    {
        free(self.expectations);
    }
    self.expectations = NULL;
}

void i2c_mock_verify_complete(void)
{
    fail_when(not_all_operations_used(), string_not_all_operations_used);
}

void i2c_mock_expect_start_then_return(i2c_result_t result)
{
    store_expectation(start, result, 0);
}

void i2c_mock_expect_restart_then_return(i2c_result_t result)
{
    store_expectation(restart, result, 0);
}

void i2c_mock_expect_stop(void)
{
    store_expectation(stop, i2c_ok, 0);
}

void i2c_mock_expect_write_byte_then_return(uint8_t byte, i2c_result_t result)
{
    store_expectation(write_byte, result, byte);
}

void i2c_mock_expect_read_byte_send_ack_then_return(uint8_t byte)
{
    // Note that "result" is not applicable here; use i2c_ok as dummy value
    store_expectation(read_byte_ack, i2c_ok, byte);
}

void i2c_mock_expect_read_byte_send_nack_then_return(uint8_t byte)
{
    // Note that "result" is not applicable here; use i2c_ok as dummy value
    store_expectation(read_byte_nack, i2c_ok, byte);
}

static void store_expectation(operation_t op, i2c_result_t result, uint8_t data)
{
    action_t expectation;

    expectation.operation = op;
    expectation.data = data;
    expectation.result = result;

    self.expectations[self.stored_expectations] = expectation;
    self.stored_expectations++;
}

/*
 * Implementation of the I2C HAL for the mock.
 */
i2c_result_t i2c_start (void)
{
    fail_when(too_many_operations(), string_too_many_operations);
    store_current_action(start, 0);
    fail_when_operation_is_not_expected();
    return self.expectations[self.used_expectations++].result;
}

i2c_result_t i2c_restart (void)
{
    fail_when(too_many_operations(), string_too_many_operations);
    store_current_action(restart, 0);
    fail_when_operation_is_not_expected();
    return self.expectations[self.used_expectations++].result;
}

void i2c_stop (void)
{
    fail_when(too_many_operations(), string_too_many_operations);
    store_current_action(stop, 0);
    fail_when_operation_is_not_expected();
    self.used_expectations++;
}

i2c_result_t i2c_write_byte (uint8_t byte)
{
    fail_when(too_many_operations(), string_too_many_operations);
    store_current_action(write_byte, byte);
    fail_when_operation_is_not_expected();
    fail_when(data_is_not_expected(), string_unexpected_data);
    return self.expectations[self.used_expectations++].result;
}

uint8_t i2c_read_byte (uint8_t send_ack)
{
    operation_t operation;

    if (send_ack)
    {
        operation = read_byte_ack;
    }
    else
    {
        operation = read_byte_nack;
    }
    fail_when(too_many_operations(), string_too_many_operations);
    store_current_action(operation, 0);
    fail_when_operation_is_not_expected();
    return self.expectations[self.used_expectations++].data;
}

static void store_current_action(operation_t op, uint8_t data)
{
    self.current_action.operation = op;
    self.current_action.data = data;
}

static void fail_when(int condition, const char* message)
{
    if (condition)
    {
        TEST_FAIL_MESSAGE(message);
    }
}

static void fail_when_operation_is_not_expected(void)
{
    if (self.current_action.operation != self.expectations[self.used_expectations].operation)
    {
        char message[100];
        const char* expected;
        const char* actual;

        actual = get_string_string(self.current_action.operation);
        expected = get_string_string(self.expectations[self.used_expectations].operation);
        sprintf(message, "Operation failure in step %i. Expected: %s, Actual: %s",
                self.used_expectations+1, expected, actual);

        TEST_FAIL_MESSAGE(message);
    }
}

static const char* get_string_string(operation_t operation)
{
    switch (operation)
    {
        case start:
            return string_operation_start;
        case restart:
            return string_operation_restart;
        case stop:
            return string_operation_stop;
        case write_byte:
            return string_operation_write_byte;
        case read_byte_ack:
            return string_operation_read_byte_ack;
        case read_byte_nack:
            return string_operation_read_byte_nack;
    }
    return string_operation_unknown;
}

static int data_is_not_expected(void)
{
    return (self.current_action.data != self.expectations[self.used_expectations].data);
}

static int too_many_operations(void)
{
    return (self.used_expectations == self.stored_expectations);
}

static int not_all_operations_used(void)
{
    return (self.used_expectations < self.stored_expectations);
}
