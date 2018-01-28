/*
 * Implementation of the timer module for the unit tests.
 * The module implements two interfaces:
 *
 *  - The timer interface needed by the scheduler
 *  - An interface for the test cases to control the timer
 *
 * Copyright (c) 2018. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "timer.h"

/*
 * Implementation of the timer interface
 */
void timer_init (void)
{
}

void timer_enable_interrupts (void)
{
}

void timer_disable_interrupts (void)
{
}


/*
 * Functions to control the fake timer
 */
