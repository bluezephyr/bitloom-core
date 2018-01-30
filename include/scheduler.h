/*
 * Simple non-preemptive scheduler for BitLoom.
 *
 * The scheduler uses a timer to get a tick every ms and this tick is used
 * to schedule tasks.  Each task has a period (in ticks) and an offset.
 * The period and the offset must be set so that only one task is run each
 * tick.  The scheduled task must return before the next tick (this is
 * monitored by an overrun flag).  Note that the overrun variable is scheduler
 * internal and must not be read from outside.  If a task overruns, the
 * corresponding bit in the 'task_error' variable is set (i.e., the task id
 * bit).  The 'task_error' variable can be read from other modules.
 *
 * Each task must implement a run function that will be called by the
 * scheduler.
 *
 * The scheduler requires a config.h file with the following defines:
 *  * SCHEDULER_NO_TASKS - Number of tasks in the application (max 32)
 *
 * The scheduler also requires a HAL (Hardware Abstraction Layer) that
 * implements the lower layer functions on the actual target hardware.
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
 * Init the scheduler.
 */
void schedule_init (void);

/*
 * Prototype for the task run function that is called by the scheduler.
 */
typedef void (*task_run)(void);

/*
 * Function to add a new task to the scheduler.  The period, offset and the
 * run function must be provided.  The function returns the taskid.
 */
uint8_t schedule_add_task (uint8_t period, uint8_t offset,
                           task_run run_function);

/*
 * Start the scheduler.
 */
void schedule_start (void);

/*
 * Schedule run function.  This function shall be called repeatedly from main.
 * The function will execute the run function for the scheduled task.
 */
void schedule_run (void);

/*
 * Timer tick callback function.  This function shall be called once for every
 * tick (typically every ms).  The function will schedule the tasks that will
 * be run when the schedule_run function is called.  Note that the timer tick
 * function will not execute any code in the tasks.
 */
void schedule_timer_tick (void);

#endif // BL_SCHEDULER_H
