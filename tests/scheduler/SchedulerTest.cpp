/*
 * Unit tests for the Bit Loom scheduler.
 *
 * Copyright (c) 2018-2020. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
    #include "core/scheduler.h"
    #include "mocks/spy_task.h"
    #include "hal/timer.h"
}

/*
 * Tests to be written:
 * - Many tasks. Tick and schedule_run -> no action
 * - Many tasks. Tick and schedule_run -> tasks run
 */

TEST_GROUP(scheduler)
{
    void setup() override
    {
        timer_init();
        schedule_init();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }

    void add_no_of_tasks(uint8_t tasks)
    {
        uint8_t i;

        for (i = 0; i < tasks; i++)
        {
            UNSIGNED_LONGS_EQUAL(i, schedule_add_task(1, 1, NULL));
        }
    }

    void start_tick_run(uint32_t no_of_times)
    {
        uint32_t i;

        schedule_start();

        for (i = 0; i < no_of_times; i++)
        {
            schedule_run();
        }
    }
};


/*
 * TEST CASES
 */
TEST(scheduler, start_scheduler_starts_timer)
{
    mock().expectOneCall("timer_start");
    schedule_start();
}

TEST(scheduler, schedule_no_tasks)
{
    mock().ignoreOtherCalls();
    start_tick_run(1);
}

TEST(scheduler, add_one_task)
{
    UNSIGNED_LONGS_EQUAL(0, schedule_add_task(1, 1, NULL));
}

TEST(scheduler, add_max_no_of_tasks)
{
    add_no_of_tasks(SCHEDULER_NO_TASKS);
}

TEST(scheduler, add_too_many_tasks_returns_error)
{
    add_no_of_tasks(SCHEDULER_NO_TASKS);
    UNSIGNED_LONGS_EQUAL(SCHEDULE_INVALID_TASK_ID, schedule_add_task(1, 1, NULL));
}

TEST(scheduler, schedule_one_task_not_scheduled)
{
    mock().ignoreOtherCalls();
    SpyTask_t task = spytask_create_counter_task(2, 0);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(1);
    LONGS_EQUAL(0, spytask_get_no_of_runs());
}

TEST(scheduler, schedule_one_task_scheduled_once)
{
    mock().ignoreOtherCalls();
    SpyTask_t task = spytask_create_counter_task(2, 0);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(2);
    LONGS_EQUAL(1, spytask_get_no_of_runs());
}

/*
 * [ - R - R - R ] - period 2, offset 0
 */
TEST(scheduler, schedule_one_task_scheduled_many)
{
    mock().ignoreOtherCalls();
    SpyTask_t task = spytask_create_counter_task(2, 0);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(6);
    LONGS_EQUAL(3, spytask_get_no_of_runs());
}

/*
 * [ - - R - R - R ] - period 2, offset 1
 */
TEST(scheduler, schedule_one_task_scheduled_many_offset_one)
{
    mock().ignoreOtherCalls();
    SpyTask_t task = spytask_create_counter_task(2, 1);
    schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(6);
    LONGS_EQUAL(2, spytask_get_no_of_runs());
}


/********************************************************************
 * TEST RUNNER
 ********************************************************************/
int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
