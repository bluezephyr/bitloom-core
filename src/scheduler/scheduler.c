/*
 * BitLoom Scheduler - Simple non-preemptive scheduler
 *
 * Copyright (c) 2016-2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "timer.h"
#include "scheduler.h"
#include "config.h"

typedef struct Task_t
{
    uint8_t period;
    uint8_t offset;
    uint8_t time;
    task_run run;   // The task's run function
} Task_t;

volatile Task_t tasks[SCHEDULER_NO_TASKS];
volatile uint8_t added_tasks = 0;

// Corresponding bit in the overrun flag is set if a task exceeds the maximum
// execution time. Note! The overrun flag is module internal. The task_error
// flag can be READ by other modules.
volatile uint32_t overrun;
volatile uint32_t task_error;

/*
 * Callback that updates the task timers. Should be called once per ms (using
 * interrupts).  Each task timer will be decreased with 1.  The scheduler's
 * main function will execute the task when the timer reaches 0 and reset the
 * timer.
 */
void TIMER_CALLBACK (void)
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

void schedule_init (void)
{
    timer_init ();
    overrun = 0;
    task_error = 0;
}

uint8_t schedule_add_task (uint8_t period, uint8_t offset,
        task_run run_function)
{
    tasks[added_tasks].period = period;
    tasks[added_tasks].offset = offset;
    tasks[added_tasks].time = period + offset;
    tasks[added_tasks].run = run_function;
    return added_tasks++;
}

void schedule_start (void)
{
    timer_enable_interrupts();
}

inline void schedule_run (void)
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
