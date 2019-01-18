/*
 * Unit tests for the Bit Loom spy task mock.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "unity_fixture.h"
#include "mocks/spy_task.h"

TEST_GROUP(spy_task);
TEST_GROUP_RUNNER(spy_task)
{
    RUN_TEST_CASE(spy_task, create);
    RUN_TEST_CASE(spy_task, counter_function_no_runs);
    RUN_TEST_CASE(spy_task, counter_function_one_run);
    RUN_TEST_CASE(spy_task, counter_function_many_runs);
}

TEST_SETUP(spy_task)
{
}

TEST_TEAR_DOWN(spy_task)
{
}

TEST(spy_task, create)
{
    SpyTask_t task = spytask_create(1, 1, NULL);
    TEST_ASSERT_EQUAL_UINT8(1, task.period);
    TEST_ASSERT_EQUAL_UINT8(1, task.offset);
    TEST_ASSERT_EQUAL_PTR(NULL, task.run);
}

TEST(spy_task, counter_function_no_runs)
{
    SpyTask_t task = spytask_create_counter_task(1, 1);
    task = task;
    TEST_ASSERT_EQUAL_UINT32(0, spytask_get_no_of_runs());
}

TEST(spy_task, counter_function_one_run)
{
    SpyTask_t task = spytask_create_counter_task(1, 1);
    task.run();
    TEST_ASSERT_EQUAL_UINT32(1, spytask_get_no_of_runs());
}

TEST(spy_task, counter_function_many_runs)
{
    uint32_t runs;

    SpyTask_t task = spytask_create_counter_task(1, 1);
    for(runs=0; runs<10; runs++)
    {
        task.run();
    }
    TEST_ASSERT_EQUAL_UINT32(10, spytask_get_no_of_runs());
}
