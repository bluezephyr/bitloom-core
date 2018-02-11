/*
 * Implementation of the spy task module for the unit tests.
 * The module implements a mock task that can be controlled by the test cases
 * to spy on the scheduler's behaviour.
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "spy_task.h"

static uint32_t m_task_no_of_runs;

void counter_function (void)
{
    m_task_no_of_runs++;
}

SpyTask_t spytask_create(uint8_t period, uint8_t offset, task_run run_function)
{
    SpyTask_t task;

    task.period = period;
    task.offset = offset;
    task.run = run_function;

    return task;
}

SpyTask_t spytask_create_counter_task(uint8_t period, uint8_t offset)
{
    m_task_no_of_runs = 0;
    return spytask_create(period, offset, counter_function);
}

uint32_t spytask_get_no_of_runs(void)
{
    return m_task_no_of_runs;
}

SpyTask_t spytask_create_overrun_task(uint8_t period, uint8_t offset, task_run run_function)
{
    return spytask_create(period, offset, run_function);
}
