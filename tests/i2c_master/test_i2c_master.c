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
#include "i2c_master.h"
#include "i2c.h"
#include "i2c_mock.h"

/*
 * Defines for a test I2C device
 */
#define DEVICE_ADDRESS    0x3C
#define WRITE_ADDRESS     0x3C
#define READ_ADDRESS      0x3D
#define WRITE_REGISTER    0x20
#define WRITE_DATA_BYTE_0 0x40

/*
 * Tests to be written:
 * - TBD
 */

TEST_GROUP(i2c_master);
TEST_GROUP_RUNNER(i2c_master)
{
    RUN_TEST_CASE(i2c_master, init);
    RUN_TEST_CASE(i2c_master, run_driver_idle);
    RUN_TEST_CASE(i2c_master, write_request_sets_driver_in_start_state);
    RUN_TEST_CASE(i2c_master, write_single_register_on_device);
}

TEST_SETUP(i2c_master)
{
    i2c_master_init(1);
    i2c_mock_create();
}

TEST_TEAR_DOWN(i2c_master)
{
    i2c_mock_destroy();
}

void check_driver_idle(void)
{
    TEST_ASSERT_EQUAL_INT(i2c_idle, i2c_master_get_state());
}

void run_until_idle(uint16_t max_number_of_loops)
{
    uint16_t i;

    while (i2c_master_get_state() != i2c_idle)
    {
        if (i++ == max_number_of_loops)
        {
            TEST_FAIL_MESSAGE("Number of run loops exceeded");
        }
        i2c_master_run();
    }
}

/*
 * TEST CASES
 */
TEST(i2c_master, init)
{
    check_driver_idle();
}

TEST(i2c_master, run_driver_idle)
{
    i2c_master_run();
    check_driver_idle();
}

TEST(i2c_master, write_request_sets_driver_in_start_state)
{
    i2c_master_write(0, NULL, 0);
    TEST_ASSERT_EQUAL_INT(i2c_busy, i2c_master_get_state());
}

TEST(i2c_master, write_single_register_on_device)
{
    uint8_t buffer[2] = {WRITE_REGISTER, WRITE_DATA_BYTE_0};
    uint16_t buffer_len = 2;

    i2c_mock_expect_start_then_return(i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_ADDRESS, i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_REGISTER, i2c_ok);
    i2c_mock_expect_write_byte_then_return(WRITE_DATA_BYTE_0, i2c_ok);
    i2c_mock_expect_stop();
    i2c_master_write(DEVICE_ADDRESS, buffer, buffer_len);
    run_until_idle(10);
    i2c_mock_verify_complete();
}
