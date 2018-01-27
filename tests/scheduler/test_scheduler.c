/*
 * Unit tests for the Bit Loom scheduler.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "unity_fixture.h"
#include "scheduler.h"

TEST_GROUP(scheduler);
TEST_GROUP_RUNNER(scheduler)
{
    RUN_TEST_CASE(scheduler, Init);
}

TEST_SETUP(scheduler)
{
}

TEST_TEAR_DOWN(scheduler)
{
}

TEST(scheduler, Init)
{
    TEST_FAIL_MESSAGE("First test case");
}
