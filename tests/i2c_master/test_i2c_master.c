/*
 * Unit tests for the Bit Loom I2C Master driver.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include "unity_fixture.h"
#include "core/i2c_master.h"
#include "hal/i2c.h"
#include "mocks/i2c_mock.h"
#include "costumes.h"

TEST_GROUP(i2c_master);
TEST_GROUP_RUNNER(i2c_master)
{
    RUN_TEST_CASE(i2c_master, init);
    RUN_TEST_CASE(i2c_master, run_driver_idle);
    RUN_TEST_CASE(i2c_master, write_request_sets_driver_in_busy_state);
    RUN_TEST_CASE(i2c_master, no_device_error);
    RUN_TEST_CASE(i2c_master, write_register_not_available);
    RUN_TEST_CASE(i2c_master, write_single_register_on_device);
    RUN_TEST_CASE(i2c_master, write_not_accepted_in_busy_state);
    RUN_TEST_CASE(i2c_master, successful_write_after_failure);
    RUN_TEST_CASE(i2c_master, read_not_accepted_in_busy_state);
    RUN_TEST_CASE(i2c_master, read_one_byte_from_register);
    RUN_TEST_CASE(i2c_master, read_several_bytes_from_register);
}

/********************************************************************
 * Defines for a test I2C device
 ********************************************************************/
#define TASK_ID           1
#define DEVICE_ADDRESS    0x3C
#define WRITE_ADDRESS     0x3C
#define READ_ADDRESS      0x3D
#define READ_REGISTER     0x25
#define READ_DATA_BYTE_0  0x30
#define READ_DATA_BYTE_1  0x31
#define READ_DATA_BYTE_2  0x32
#define WRITE_REGISTER    0x20
#define WRITE_DATA_BYTE_0 0x40


/********************************************************************
 * Local help function declarations
 ********************************************************************/
static void check_driver_idle(void);
static void check_driver_busy(void);
static void run_number_of_loops(uint16_t number_of_loops);
static void run_until(i2c_master_state_t state, uint16_t max_number_of_loops);
static void start_write_op_and_set_driver_in_busy_state(void);


/********************************************************************
 * TEST CASES
 ********************************************************************/
TEST_SETUP(i2c_master)
{
    costumes_create();
    i2c_master_init(TASK_ID);
    i2c_mock_create();
}

TEST_TEAR_DOWN(i2c_master)
{
    i2c_mock_destroy();
    costumes_destroy();
}

TEST(i2c_master, init)
{
    check_driver_idle();
}

TEST(i2c_master, run_driver_idle)
{
    i2c_master_run();
    check_driver_idle();
}

TEST(i2c_master, write_request_sets_driver_in_busy_state)
{
    i2c_master_write(0, NULL, 0);
    check_driver_busy();
}

TEST(i2c_master, no_device_error)
{
    uint8_t buffer[1] = {WRITE_REGISTER};
    uint16_t buffer_len = 1;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_nack_received);
    i2c_mock_expect_stop();
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_error, 10);
    i2c_mock_verify_complete();
} 

TEST(i2c_master, write_register_not_available)
{
    uint8_t buffer[1] = {WRITE_REGISTER};
    uint16_t buffer_len = 1;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[0], i2c_nack_received);
    i2c_mock_expect_stop();
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_error, 10);
    i2c_mock_verify_complete();
}

TEST(i2c_master, write_single_register_on_device)
{
    uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
    uint16_t buffer_len = 2;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[0], i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[1], i2c_ack_received);
    i2c_mock_expect_stop();
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_idle, 10);
    i2c_mock_verify_complete();
}

TEST(i2c_master, write_not_accepted_in_busy_state)
{
    uint8_t buffer[1];
    uint16_t buffer_len = 1;

    start_write_op_and_set_driver_in_busy_state();

    // The following write should not generate any operations
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_idle, 10);
    i2c_mock_verify_complete();
}

TEST(i2c_master, successful_write_after_failure)
{
    uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
    uint16_t buffer_len = 2;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_nack_received);
    i2c_mock_expect_stop();
    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[0], i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[1], i2c_ack_received);
    i2c_mock_expect_stop();

    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_error, 10);
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until(i2c_idle, 10);
    i2c_mock_verify_complete();
}

TEST(i2c_master, read_not_accepted_in_busy_state)
{
    uint8_t read_buffer[1];
    uint16_t buffer_len = 1;

    start_write_op_and_set_driver_in_busy_state();

    // The following read should not generate any operations
    i2c_master_read_register(DEVICE_ADDRESS, READ_REGISTER, read_buffer, buffer_len);
    run_until(i2c_idle, 10);
    i2c_mock_verify_complete();
}

TEST(i2c_master, read_one_byte_from_register)
{
    uint8_t read_buffer[1];
    uint16_t buffer_len = 1;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(READ_REGISTER, i2c_ack_received);
    i2c_mock_expect_restart_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(READ_ADDRESS, i2c_ack_received);
    i2c_mock_expect_read_byte_send_nack_then_return(READ_DATA_BYTE_0);
    i2c_mock_expect_stop();

    i2c_master_read_register(DEVICE_ADDRESS, READ_REGISTER, read_buffer, buffer_len);
    run_until(i2c_idle, 10);
    TEST_ASSERT_EQUAL_UINT8(read_buffer[0], READ_DATA_BYTE_0);
    i2c_mock_verify_complete();
}

TEST(i2c_master, read_several_bytes_from_register)
{
    uint8_t read_buffer[3];
    uint16_t buffer_len = 3;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(READ_REGISTER, i2c_ack_received);
    i2c_mock_expect_restart_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(READ_ADDRESS, i2c_ack_received);
    i2c_mock_expect_read_byte_send_ack_then_return(READ_DATA_BYTE_0);
    i2c_mock_expect_read_byte_send_ack_then_return(READ_DATA_BYTE_1);
    i2c_mock_expect_read_byte_send_nack_then_return(READ_DATA_BYTE_2);
    i2c_mock_expect_stop();

    i2c_master_read_register(DEVICE_ADDRESS, READ_REGISTER, read_buffer, buffer_len);
    run_until(i2c_idle, 10);
    TEST_ASSERT_EQUAL_UINT8(read_buffer[0], READ_DATA_BYTE_0);
    TEST_ASSERT_EQUAL_UINT8(read_buffer[1], READ_DATA_BYTE_1);
    TEST_ASSERT_EQUAL_UINT8(read_buffer[2], READ_DATA_BYTE_2);
    i2c_mock_verify_complete();
}

/********************************************************************
 * Local help function implementation
 ********************************************************************/
static void check_driver_idle(void)
{
    TEST_ASSERT_EQUAL_INT(i2c_idle, i2c_master_get_state());
}

static void check_driver_busy(void)
{
    TEST_ASSERT_EQUAL_INT(i2c_busy, i2c_master_get_state());
}

static void run_number_of_loops(uint16_t number_of_loops)
{
    uint16_t i;
    for(i=0; i<number_of_loops; i++)
    {
        i2c_master_run();
    }
}

static void run_until(i2c_master_state_t state, uint16_t max_number_of_loops)
{
    uint16_t i;
    while (i2c_master_get_state() != state)
    {
        if (i++ == max_number_of_loops)
        {
            TEST_FAIL_MESSAGE("Maximum number of run loops exceeded");
        }
        i2c_master_run();
    }
}

static void start_write_op_and_set_driver_in_busy_state(void)
{
    uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
    uint16_t buffer_len = 2;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[0], i2c_ack_received);
    i2c_mock_expect_write_byte_then_return(buffer[1], i2c_ack_received);
    i2c_mock_expect_stop();
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_number_of_loops(3);
}

