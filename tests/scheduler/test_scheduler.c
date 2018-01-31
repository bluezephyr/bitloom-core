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
#include "fake_timer.h"

TEST_GROUP(scheduler);
TEST_GROUP_RUNNER(scheduler)
{
    RUN_TEST_CASE(scheduler, init);
    RUN_TEST_CASE(scheduler, no_start_no_timer_started);
    RUN_TEST_CASE(scheduler, start_with_no_tasks);
}

TEST_SETUP(scheduler)
{
    schedule_init();
}

TEST_TEAR_DOWN(scheduler)
{
}

TEST(scheduler, init)
{
    TEST_ASSERT_EQUAL_UINT32(0, schedule_get_overrun_tasks());
}

TEST(scheduler, no_start_no_timer_started)
{
    TEST_ASSERT_EQUAL_INT(TIMER_DISABLED, faketimer_get_state());
}

TEST(scheduler, start_with_no_tasks)
{
    schedule_start();
    TEST_ASSERT_EQUAL_INT(TIMER_ENABLED, faketimer_get_state());
}
