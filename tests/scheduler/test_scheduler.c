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
#include "spy_task.h"

/*
 * Tests to be written:
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
    RUN_TEST_CASE(scheduler, schedule_one_task_not_scheduled);
    RUN_TEST_CASE(scheduler, schedule_one_task_scheduled_once);
    RUN_TEST_CASE(scheduler, schedule_one_task_scheduled_many);
    RUN_TEST_CASE(scheduler, schedule_one_task_scheduled_many_offset_one);
}

TEST_SETUP(scheduler)
{
    schedule_init();
}

TEST_TEAR_DOWN(scheduler)
{
}

void add_no_of_tasks(uint8_t tasks)
{
    uint8_t i;

    for(i=0; i<tasks; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i, schedule_add_task(1, 1, NULL));
    }
}

void start_tick_run(uint32_t no_of_times)
{
    uint32_t i;

    schedule_start();

    for(i=0; i<no_of_times; i++)
    {
        schedule_timer_tick();
        schedule_run();
    }
}


/*
 * TEST CASES
 */
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
    start_tick_run(1);
    TEST_ASSERT_EQUAL_UINT32(0, schedule_get_overrun_tasks());
}

TEST(scheduler, add_one_task)
{
    TEST_ASSERT_EQUAL_UINT8(0, schedule_add_task(1, 1, NULL));
}

TEST(scheduler, add_max_no_of_tasks)
{
    add_no_of_tasks(SCHEDULER_NO_TASKS);
}

TEST(scheduler, add_too_many_tasks)
{
    add_no_of_tasks(SCHEDULER_NO_TASKS);
    TEST_ASSERT_EQUAL_UINT8(SCHEDULE_INVALID_TASK_ID, schedule_add_task(1, 1, NULL));
}

TEST(scheduler, schedule_one_task_not_scheduled)
{
    SpyTask_t task = spytask_create_counter_task(2, 0);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(1);
    TEST_ASSERT_EQUAL_INT(0, spytask_get_no_of_runs());
}

TEST(scheduler, schedule_one_task_scheduled_once)
{
    SpyTask_t task = spytask_create_counter_task(2, 0);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(2);
    TEST_ASSERT_EQUAL_INT(1, spytask_get_no_of_runs());
}

/*
 * [ - R - R - R ] - period 2, offset 0
 */
TEST(scheduler, schedule_one_task_scheduled_many)
{
    SpyTask_t task = spytask_create_counter_task(2, 0);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(6);
    TEST_ASSERT_EQUAL_INT(3, spytask_get_no_of_runs());
}

/*
 * [ - - R - R - R ] - period 2, offset 1
 */
TEST(scheduler, schedule_one_task_scheduled_many_offset_one)
{
    SpyTask_t task = spytask_create_counter_task(2, 1);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(6);
    TEST_ASSERT_EQUAL_INT(2, spytask_get_no_of_runs());
}
