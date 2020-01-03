#ifndef TIMER_CONFIG_H
#define TIMER_CONFIG_H

/*
 * The timer module must be configured to convey new ticks to the sheduler. The scheduler will
 * call the function like macro TIMER_GET_TICKS() and expect to get the number of ticks since
 * init. The value is of the type Tick_t.
 *
 * The macro can be implemented as a function - typically this is done in the unit tests since
 * it makes it easier. For efficiency, however, the TIMER_GET_TICKS() can also be expanded as
 * a global variable of the Tick_t type. In this way, the variable can be set by the timer
 * interrupt and read directly by the scheduler main function, thus avoiding contex switches.
 */

/*
 * The following must be defined:
 * - Tick_t (unsigned int of appropriate type)
 * - TIMER_GET_TICKS()
 *
 */

#endif  // TIMER_CONFIG_H
