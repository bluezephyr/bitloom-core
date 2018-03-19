#ifndef SCHEDULER_CONFIG_H
#define SCHEDULER_CONFIG_H

#include <stdint.h>

/*
 * The number of tasks that will be used in the system.  For each task, memory
 * will be resereved to hold its internal state.  The maximum number of tasks
 * is 32.
 */
#define SCHEDULER_NO_TASKS      32

#endif  // SCHEDULER_CONFIG_H
