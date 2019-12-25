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
            schedule_timer_tick();
            schedule_run();
        }
    }
};

/*
 * HELP FUNCTIONS
 */
void ticker_function(void)
{
    schedule_timer_tick();
}

/*
 * MOCKED INTERFACES USING CPPUTEST MOCKING FRAMEWORK
 */
void timer_start(void)
{
    mock().actualCall("timer_start");
}

/*
 * TEST CASES
 */
TEST(scheduler, init_scheduler_no_tasks_overrun)
{
    UNSIGNED_LONGS_EQUAL(0, schedule_get_overrun_tasks());
}

TEST(scheduler, start_scheduler_starts_timer)
{
    mock().expectOneCall("timer_start");
    schedule_start();
}

TEST(scheduler, schedule_no_tasks)
{
    mock().ignoreOtherCalls();
    start_tick_run(1);
    UNSIGNED_LONGS_EQUAL(0, schedule_get_overrun_tasks());
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

TEST(scheduler, schedule_one_task_overrun)
{
    uint8_t task_id;
    mock().ignoreOtherCalls();
    SpyTask_t task = spytask_create_overrun_task(1, 0, ticker_function);
    task_id = schedule_add_task(task.period, task.offset, task.run);
    start_tick_run(1);
    UNSIGNED_LONGS_EQUAL(1u << task_id, schedule_get_overrun_tasks());
}


int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
