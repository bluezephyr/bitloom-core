/*
 * Unit tests for the Bit Loom fake_timer.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "unity_fixture.h"
#include "mocks/fake_timer.h"

TEST_GROUP(fake_timer);
TEST_GROUP_RUNNER(fake_timer)
{
    RUN_TEST_CASE(fake_timer, init);
    RUN_TEST_CASE(fake_timer, start);
    RUN_TEST_CASE(fake_timer, stop);
}

TEST_SETUP(fake_timer)
{
    timer_init();
}

TEST_TEAR_DOWN(fake_timer)
{
}

TEST(fake_timer, init)
{
    TEST_ASSERT_EQUAL_INT(TIMER_DISABLED, faketimer_get_state());
}

TEST(fake_timer, start)
{
    timer_start();
    TEST_ASSERT_EQUAL_INT(TIMER_ENABLED, faketimer_get_state());
}

TEST(fake_timer, stop)
{
    timer_start();
    timer_stop();
    TEST_ASSERT_EQUAL_INT(TIMER_DISABLED, faketimer_get_state());
}
