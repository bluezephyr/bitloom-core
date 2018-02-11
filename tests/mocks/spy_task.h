/*
 * Mock task for the scheduler unit tests.
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_SPY_TASK_H
#define BL_SPY_TASK_H

#include "scheduler.h"

typedef struct
{
    uint8_t period;
    uint8_t offset;
    task_run run;
} SpyTask_t;


SpyTask_t spytask_create(uint8_t period, uint8_t offset, task_run run_function);
SpyTask_t spytask_create_counter_task(uint8_t period, uint8_t offset);
SpyTask_t spytask_create_overrun_task(uint8_t period, uint8_t offset, task_run run_function);

uint32_t spytask_get_no_of_runs(void);

#endif // BL_SPY_TASK_H
