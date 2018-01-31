/*
 * Simple non-preemptive scheduler for BitLoom.
 *
 * The scheduler uses a timer to get a tick (typically) every ms and this tick
 * is used to schedule tasks.  Each task has a period (in ticks) and an offset.
 * The period and the offset must be set so that only one task is run each
 * tick.  Each task must implement a run function that will be called by the
 * scheduler.  The run function must return before the next tick and failure
 * to do this will generate an error.
 *
 * The scheduler requires a config.h file with the following defines:
 *  * SCHEDULER_NO_TASKS - Number of tasks in the application (max 32)
 *
 * Copyright (c) 2016-2018 BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef BL_SCHEDULER_H
#define BL_SCHEDULER_H

#include "config.h"

/*
 * Prototype for the task run function that is called by the scheduler.
 */
typedef void (*task_run)(void);

/*
 * Init the scheduler.  This function must be called before any other function
 * is used.
 */
void schedule_init (void);

/*
 * This function will return a bit field of all tasks that have overrun.  The
 * corresponding task's id in the returned value will be set if the task has
 * overrun during the execution so far.
 */
uint32_t schedule_get_overrun_tasks(void);

/*
 * Function to add a new task to the scheduler.  The period, offset and the
 * run function must be provided.  The function returns the taskid.
 */
uint8_t schedule_add_task (uint8_t period, uint8_t offset,
                           task_run run_function);

/*
 * Start the scheduler.  The scheduler expects that the timer has been
 * configured and initiated.
 */
void schedule_start (void);

/*
 * Timer tick callback function.  This function shall be called once for every
 * tick (typically every ms).  The function will schedule the tasks that will
 * be run when the schedule_run function is called.  Note that the timer tick
 * function will not execute any code in the tasks.
 */
void schedule_timer_tick (void);

/*
 * Schedule run function.  This function shall be called repeatedly from main.
 * The function will execute the run function for the scheduled task.
 */
void schedule_run (void);

#endif // BL_SCHEDULER_H
