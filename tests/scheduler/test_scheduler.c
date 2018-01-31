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

/*
 * Tests to be written:
 * - One task. Tick and schedule_run -> no action
 * - One task. Tick and schedule_run -> task runs
 * - One task. Run manu times - check period
 * - One task. Overrun - report error
 * - Many tasks. Tick and schedule_run -> no action
 * - Many tasks. Tick and schedule_run -> tasks run
 */

TEST_GROUP(scheduler);
TEST_GROUP_RUNNER(scheduler)
{
    RUN_TEST_CASE(scheduler, init);
    RUN_TEST_CASE(scheduler, no_start_no_timer_started);
    RUN_TEST_CASE(scheduler, start_with_no_tasks);
    RUN_TEST_CASE(scheduler, schedule_no_tasks);
    RUN_TEST_CASE(scheduler, add_one_task);
    RUN_TEST_CASE(scheduler, add_max_no_of_tasks);
    RUN_TEST_CASE(scheduler, add_too_many_tasks);
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

TEST(scheduler, schedule_no_tasks)
{
    schedule_start();
    schedule_timer_tick();
    schedule_run();
    TEST_ASSERT_EQUAL_UINT32(0, schedule_get_overrun_tasks());
}

TEST(scheduler, add_one_task)
{
    TEST_ASSERT_EQUAL_UINT8(0, schedule_add_task(1, 1, NULL));
}

TEST(scheduler, add_max_no_of_tasks)
{
    uint8_t i;

    for(i=0; i<SCHEDULER_NO_TASKS; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i, schedule_add_task(1, 1, NULL));
    }
}

TEST(scheduler, add_too_many_tasks)
{
    uint8_t i;

    for(i=0; i<SCHEDULER_NO_TASKS; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i, schedule_add_task(1, 1, NULL));
    }

    TEST_ASSERT_EQUAL_UINT8(SCHEDULE_INVALID_TASK_ID, schedule_add_task(1, 1, NULL));
}

// Not enabled
TEST(scheduler, schedule_one_task_no_run)
{
    schedule_start();
    schedule_timer_tick();
    schedule_run();
    TEST_ASSERT_EQUAL_UINT32(0, schedule_get_overrun_tasks());
}
