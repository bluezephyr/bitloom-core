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
}

TEST_SETUP(i2c_master)
{
}

TEST_TEAR_DOWN(i2c_master)
{
}

void check_driver_idle(void)
{
    TEST_ASSERT_EQUAL_INT(i2c_idle, i2c_master_get_state());
}

/*
 * TEST CASES
 */
TEST(i2c_master, init)
{
    i2c_master_init(1);
    check_driver_idle();
}

TEST(i2c_master, run_driver_idle)
{
    i2c_master_init(1);
    i2c_master_run();
    check_driver_idle();
}

TEST(i2c_master, write_request_sets_driver_in_start_state)
{
    i2c_master_init(1);
    i2c_master_write(0, NULL, 0);
    TEST_ASSERT_EQUAL_INT(i2c_busy, i2c_master_get_state());
}
