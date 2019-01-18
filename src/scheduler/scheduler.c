/*
 * BitLoom Scheduler - Simple non-preemptive scheduler for up to 32 tasks.
 *
 * Copyright (c) 2016-2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "hal/timer.h"
#include "core/scheduler.h"

typedef struct Task_t
{
    uint8_t period;
    uint8_t time;
    task_run run;   // The task's run function
} Task_t;

static volatile Task_t tasks[SCHEDULER_NO_TASKS];
static volatile uint8_t added_tasks = 0;

/*
 * Corresponding bit in the overrun flag is set if a task exceeds the maximum
 * execution time.  The task_error variable holds the id:s of all tasks that
 * have overrun.
 */
static volatile uint32_t overrun;
static volatile uint32_t task_error;


void schedule_init (void)
{
    overrun = 0;
    task_error = 0;
    added_tasks = 0;
}

uint32_t schedule_get_overrun_tasks(void)
{
    return task_error;
}

uint8_t schedule_add_task (uint8_t period, uint8_t offset,
        task_run run_function)
{
    if(added_tasks < SCHEDULER_NO_TASKS)
    {
        tasks[added_tasks].period = period;
        tasks[added_tasks].time = period + offset;
        tasks[added_tasks].run = run_function;
        return added_tasks++;
    }
    else
    {
        return SCHEDULE_INVALID_TASK_ID;
    }
}

void schedule_start (void)
{
    timer_start();
}

/*
 * Callback that updates the task timers.  Typically, this is called once per
 * ms or similar by a timer interrupt.  Each task timer will be decreased by 1.
 * The scheduler's main function will execute the task when the timer reaches 0
 * and reset the task's timer.
 */
void schedule_timer_tick (void)
{
    uint8_t i;

    if (overrun)
    {
        // One of the tasks exceeded its execution time
        task_error = overrun;
    }

    // Decrease the current time value for all tasks
    for (i=0; i<added_tasks; i++)
    {
        if(tasks[i].time > 0)
        {
            tasks[i].time--;
        }
    }
}

void schedule_run (void)
{
    uint8_t task;

    for (task=0; task<added_tasks; task++)
    {
        if (tasks[task].time == 0)
        {
            overrun |= (1 << task);
            tasks[task].time = tasks[task].period;
            tasks[task].run();
            overrun &= ~(1 << task);
        }
    }
}

