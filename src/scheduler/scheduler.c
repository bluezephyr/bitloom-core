/*
 * BitLoom Scheduler - Simple non-preemptive scheduler for tasks.
 *
 * Copyright (c) 2016-2020. BlueZephyr
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

typedef struct Scheduler_t
{
    Task_t tasks[SCHEDULER_NO_TASKS];
    uint8_t no_of_tasks;
    uint32_t task_error;
    Tick_t current_ticks;
} Scheduler_t;
static Scheduler_t self;

void schedule_init (void)
{
    self.no_of_tasks = 0;
    self.task_error = 0;
    self.current_ticks = 0;
}

uint8_t schedule_add_task (uint8_t period, uint8_t offset, task_run run_function)
{
    if(self.no_of_tasks < SCHEDULER_NO_TASKS)
    {
        self.tasks[self.no_of_tasks].period = period;
        self.tasks[self.no_of_tasks].time = period + offset;
        self.tasks[self.no_of_tasks].run = run_function;
        return self.no_of_tasks++;
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
 * Scheduler main function.  This function waits for a tick and when that happens
 * it calls the run function of the tasks that are scheduled for that tick.
 */
void schedule_run (void)
{
    uint8_t task;
    Tick_t ticks;

    do
    {
        ticks = TIMER_GET_TICKS();
    } while(self.current_ticks == ticks);
    self.current_ticks = ticks;

    for (task=0; task<self.no_of_tasks; task++)
    {
        if (--self.tasks[task].time == 0)
        {
            self.tasks[task].time = self.tasks[task].period;
            self.tasks[task].run();
        }
    }
}

