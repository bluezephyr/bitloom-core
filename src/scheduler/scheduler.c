/*
 * BitLoom Scheduler - Simple non-preemptive scheduler for up to 32 tasks.
 *
 * Copyright (c) 2016-2019. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <stdbool.h>
#include "hal/timer.h"
#include "core/scheduler.h"

typedef struct Task_t
{
    uint8_t period;
    uint8_t time;
    task_run run;   // The task's run function
} Task_t;

/*
 * Corresponding bit in the overrun flag is set if a task exceeds the maximum
 * execution time.  The task_error variable holds the id:s of all tasks that
 * have overrun.
 */
typedef struct Scheduler_t
{
   Task_t tasks[SCHEDULER_NO_TASKS];
   uint8_t no_of_tasks;
   uint32_t tick;
   uint32_t overrun;
   uint32_t task_error;
} Scheduler_t;

static volatile Scheduler_t self;

void schedule_init (void)
{
    self.no_of_tasks = 0;
    self.tick = 0;
    self.overrun = 0;
    self.task_error = 0;
}

uint32_t schedule_get_overrun_tasks(void)
{
    return self.task_error;
}

uint8_t schedule_add_task (uint8_t period, uint8_t offset,
        task_run run_function)
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
 * Callback that updates the scheduler tick variable.  Typically, this function
 * is called once per ms or similar by a timer interrupt.
 */
void schedule_timer_tick (void)
{
    self.tick++;
}

extern bool schedule_timer_new_tick(void);

/*
 * Scheduler main function.  This function waits for a tick and when that happens
 * it calls the run function of the tasks that are scheduled for that tick.
 */
void schedule_run (void)
{
    uint8_t task;

#ifndef UNIT_TEST
    while(!self.tick);
#else
    while(schedule_timer_new_tick());
#endif
    self.tick = 0;

    for (task=0; task<self.no_of_tasks; task++)
    {
        if (--self.tasks[task].time == 0)
        {
            self.overrun |= (1u << task);
            self.tasks[task].time = self.tasks[task].period;
            self.tasks[task].run();
            self.overrun &= ~(1u << task);
        }
    }
}

